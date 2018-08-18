/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <gf/Tmx.h>

#include <cstring>
#include <cassert>
#include <memory>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/split.hpp>

#include <zlib.h>

#include <gf/Log.h>
#include <gf/Unused.h>

#include "vendor/pugixml/src/pugixml.hpp"

namespace fs = boost::filesystem;

using namespace std::string_literals;

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void TmxProperties::addStringProperty(std::string name, std::string value) {
    m_stringProps.insert({ std::move(name), std::move(value) });
  }

  void TmxProperties::addIntProperty(std::string name, int value) {
    m_intProps.insert({ std::move(name), value });
  }

  void TmxProperties::addFloatProperty(std::string name, double value) {
    m_floatProps.insert({ std::move(name), value });
  }

  void TmxProperties::addBoolProperty(std::string name, bool value) {
    m_boolProps.insert({ std::move(name), value });
  }

  void TmxProperties::addColorProperty(std::string name, Color4u value) {
    m_colorProps.insert({ std::move(name), value });
  }

  void TmxProperties::addFileProperty(std::string name, Path value) {
    m_fileProps.insert({ std::move(name), std::move(value) });
  }

  namespace {

    template<typename T>
    T findOr(const std::map<std::string, T>& prop, const std::string& name, const T& def) {
      auto it = prop.find(name);

      if (it != prop.end()) {
        return it->second;
      }

      return def;
    }

  }

  std::string TmxProperties::getStringProperty(const std::string& name, const std::string& def) const {
    return findOr(m_stringProps, name, def);
  }

  int TmxProperties::getIntProperty(const std::string& name, int def) const {
    return findOr(m_intProps, name, def);
  }

  double TmxProperties::getFloatProperty(const std::string& name, double def) const {
    return findOr(m_floatProps, name, def);
  }

  bool TmxProperties::getBoolProperty(const std::string& name, bool def) const {
    return findOr(m_boolProps, name, def);
  }

  Color4u TmxProperties::getColorProperty(const std::string& name, const Color4u& def) const {
    return findOr(m_colorProps, name, def);
  }

  Path TmxProperties::getFileProperty(const std::string& name, const Path& def) const {
    return findOr(m_fileProps, name, def);
  }


  TmxVisitor::~TmxVisitor() = default;

  void TmxVisitor::visitTileLayer(const TmxLayers& map, const TmxTileLayer& layer) {
    gf::unused(map, layer);
  }

  void TmxVisitor::visitObjectLayer(const TmxLayers& map, const TmxObjectLayer& layer) {
    gf::unused(map, layer);
  }

  void TmxVisitor::visitImageLayer(const TmxLayers& map, const TmxImageLayer& layer) {
    gf::unused(map, layer);
  }

  void TmxVisitor::visitGroupLayer(const TmxLayers& map, const TmxGroupLayer& layer) {
    for (auto& sublayer : layer.layers) {
      sublayer->accept(map, *this);
    }
  }

  TmxLayer::~TmxLayer() = default;


  void TmxTileLayer::accept(const TmxLayers& map, TmxVisitor& visitor) const {
    visitor.visitTileLayer(map, *this);
  }

  void TmxObjectLayer::accept(const TmxLayers& map, TmxVisitor& visitor) const {
    visitor.visitObjectLayer(map, *this);
  }

  void TmxImageLayer::accept(const TmxLayers& map, TmxVisitor& visitor) const {
    visitor.visitImageLayer(map, *this);
  }

  void TmxGroupLayer::accept(const TmxLayers& map, TmxVisitor& visitor) const {
    visitor.visitGroupLayer(map, *this);
  }

  TmxObject::~TmxObject() = default;


  const TmxTile *TmxTileset::getTile(unsigned id) const noexcept {
    for (auto& tile : tiles) {
      if (tile.id == id) {
        return std::addressof(tile);
      }
    }

    return nullptr;
  }

  RectU TmxTileset::getSubTexture(unsigned id, Vector2u size) const noexcept {
    unsigned width = (size.width - 2 * margin + spacing) / (tileSize.width + spacing); // number of tiles
    unsigned height = (size.height - 2 * margin + spacing) / (tileSize.height + spacing); // number of tiles

    unsigned tu = id % width;
    unsigned tv = id / width;
    assert(tv < height);

    unsigned du = margin + tu * spacing + offset.x;
    unsigned dv = margin + tv * spacing + offset.y;
    assert((tu + 1) * tileSize.width + du <= size.width);
    assert((tv + 1) * tileSize.height + dv <= size.height);

    return { tu * tileSize.width + du, tv * tileSize.height + dv, tileSize.width, tileSize.height };
  }


  const TmxTileset *TmxLayers::getTileSetFromGID(unsigned gid) const noexcept {
    for (auto it = tilesets.rbegin(); it != tilesets.rend(); ++it) {
      if (it->firstGid <= gid) {
        return std::addressof(*it);
      }
    }

    return nullptr;
  }

  void TmxLayers::visitLayers(TmxVisitor& visitor) const {
    for (auto& layer : layers) {
      layer->accept(*this, visitor);
    }
  }

  /*
   * Parsing of TMX file
   *
   */

  namespace {

    pugi::xml_attribute required_attribute(pugi::xml_node node, const char *name) {
      pugi::xml_attribute attr = node.attribute(name);

      if (!attr) {
        Log::error("Required attribute for node '%s' is missing: %s\n", node.path().c_str(), name);
      }

      return attr;
    }

    uint8_t convertHexChar(char c) {
      if ('0' <= c && c <= '9') {
        return c - '0';
      }

      if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
      }

      if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
      }

      Log::error("Invalid character: '%c' (%x)\n", c, static_cast<int>(c));
      return 0;
    }

    Color4u computeColor(pugi::xml_attribute attr, Color4u def = Color4u(0x00, 0x00, 0x00, 0xFF)) {
      if (!attr) {
        return def;
      }

      const char *value = attr.as_string();
      auto size = std::strlen(value);
      assert(size > 0);

      if (value[0] == '#') {
        ++value;
        --size;
      }

      Color4u color = def;

      switch (size) {
        case 6:
          color.a = 0xFF;
          color.r = (convertHexChar(value[0]) << 4) + convertHexChar(value[1]);
          color.g = (convertHexChar(value[2]) << 4) + convertHexChar(value[3]);
          color.b = (convertHexChar(value[4]) << 4) + convertHexChar(value[5]);
          break;

        case 8:
          color.a = (convertHexChar(value[0]) << 4) + convertHexChar(value[1]);
          color.r = (convertHexChar(value[2]) << 4) + convertHexChar(value[3]);
          color.g = (convertHexChar(value[4]) << 4) + convertHexChar(value[5]);
          color.b = (convertHexChar(value[6]) << 4) + convertHexChar(value[7]);
          break;

        default:
          Log::error("Unknown color format: %s\n", value);
          break;
      }

      return color;
    }

    struct TmxParserCtx {
      Path mapPath;
      Path currentPath;
    };

    /*
     * Layers
     */

    enum class TmxFormat {
      Xml,
      Base64,
      Base64_Zlib,
      Base64_Gzip,
      Csv,
    };

    // http://en.wikibooks.org/wiki/Algorithm_implementation/Miscellaneous/Base64
    std::vector<uint8_t> parseDataBase64(const std::string& input) {
      std::string cleanInput = boost::algorithm::erase_all_copy(input, "\n");
      cleanInput = boost::algorithm::erase_all_copy(cleanInput, " ");

      std::size_t len = cleanInput.size();
      assert(len % 4 == 0);

      size_t padding = 0;
      if (len >= 2) {
        if (input[len - 1] == '=') {
          padding++;
        }

        if (input[len - 2] == '=') {
          padding++;
        }
      }

      // Setup a vector to hold the result
      std::vector<uint8_t> decoded;
      decoded.reserve(((len / 4) * 3) - padding);

      for (std::size_t i = 0; i < len; ) {

        uint32_t tmp = 0; // Holds decoded quanta
        for (int k = 0; k < 4; ++k) {
          tmp <<= 6;

          char c = cleanInput.at(i);

          if (c >= 'A' && c <= 'Z') {
            tmp |= c - 'A';
          } else if (c >= 'a' && c <= 'z') {
            tmp |= c - 'a' + 26;
          } else if (c >= '0' && c <= '9') {
            tmp |= c - '0' + 52;
          } else if (c == '+') {
            tmp |= 0x3E;
          } else if (c == '/') {
            tmp |= 0x3F;
          } else if (c == '=') { //pad
            switch(len - i) {
            case 1: // One pad character
              decoded.push_back((tmp >> 16) & 0x000000FF);
              decoded.push_back((tmp >> 8 ) & 0x000000FF);
              return decoded;
            case 2: // Two pad characters
              decoded.push_back((tmp >> 10) & 0x000000FF);
              return decoded;
            default:
              assert(false);
            }
          } else {
            Log::error("Unknown character: '%c' (%i)\n", c, static_cast<int>(c));
            assert(false);
          }

          ++i;
        }

        decoded.push_back((tmp >> 16) & 0x000000FF);
        decoded.push_back((tmp >> 8 ) & 0x000000FF);
        decoded.push_back((tmp      ) & 0x000000FF);
      }

      return decoded;
    }

    std::vector<uint8_t> parseDataCompressed(const std::vector<uint8_t>& input) {
      std::vector<uint8_t> uncompressed;

      static constexpr uInt TmpLength = 1024;
      uint8_t tmp[TmpLength];

      z_stream stream;
      std::memset(&stream, 0, sizeof(stream));

      stream.next_in = input.data();
      stream.avail_in = input.size();
      stream.zalloc = Z_NULL;
      stream.zfree = Z_NULL;

      inflateInit2(&stream, 15 + 32); // allow to decode gzip and zlib format

      int n = 0;
      for (;;) {
        n++;
        stream.next_out = tmp;
        stream.avail_out = TmpLength;

        int err = inflate(&stream, Z_SYNC_FLUSH);

        if (err == Z_OK) {
          uncompressed.insert(uncompressed.end(), tmp, tmp + TmpLength - stream.avail_out);
        } else if (err == Z_STREAM_END) {
          uncompressed.insert(uncompressed.end(), tmp, tmp + TmpLength - stream.avail_out);
          break;
        } else {
          assert(false);
        }
      }

      assert(stream.avail_in == 0);

      inflateEnd(&stream);

      return uncompressed;
    }

    TmxFormat parseDataFormat(const pugi::xml_node node) {
      assert(node.name() == "data"s);

      std::string encoding = node.attribute("encoding").as_string();

      if (encoding == "csv") {
        return TmxFormat::Csv;
      }

      if (encoding == "base64") {
        std::string compression = node.attribute("compression").as_string();

        if (compression == "zlib") {
          return TmxFormat::Base64_Zlib;
        }

        if (compression == "gzip") {
          return TmxFormat::Base64_Gzip;
        }

        return TmxFormat::Base64;
      }

      return TmxFormat::Xml;
    }

    std::vector<uint8_t> parseDataBuffer(const pugi::xml_node node, TmxFormat format) {
      assert(node.name() == "data"s);

      std::vector<uint8_t> data;

      switch (format) {
        case TmxFormat::Xml:
        case TmxFormat::Csv:
          assert(false);
          break;

        case TmxFormat::Base64:
          data = parseDataBase64(node.child_value());
          break;

        case TmxFormat::Base64_Zlib:
        case TmxFormat::Base64_Gzip:
          data = parseDataCompressed(parseDataBase64(node.child_value()));
          break;
      }

      data.shrink_to_fit();

      return data;
    }

    TmxCell decodeGID(unsigned gid) {
      static constexpr unsigned FlippedHorizontallyFlag = 0x80000000;
      static constexpr unsigned FlippedVerticallyFlag   = 0x40000000;
      static constexpr unsigned FlippedDiagonallyFlag   = 0x20000000;

      TmxCell cell;

      // Read out the flags
      if ((gid & FlippedHorizontallyFlag) != 0) {
        cell.flip.set(Flip::Horizontally);
      }

      if ((gid & FlippedVerticallyFlag) != 0) {
        cell.flip.set(Flip::Vertically);
      }

      if ((gid & FlippedDiagonallyFlag) != 0) {
        cell.flip.set(Flip::Diagonally);
      }

      // Clear the flags
      cell.gid = gid & ~(FlippedHorizontallyFlag | FlippedVerticallyFlag | FlippedDiagonallyFlag);

      return cell;
    }

    TmxProperties parseTmxProperties(const pugi::xml_node node) {
      TmxProperties tmx;

      for (pugi::xml_node properties : node.children("properties")) {
        for (pugi::xml_node property : properties.children("property")) {
          std::string name = required_attribute(property, "name").as_string();
          assert(!name.empty());

          if (property.attribute("type")) {
            std::string type = property.attribute("type").as_string();

            if (type == "string") {
              tmx.addStringProperty(std::move(name), required_attribute(property, "value").as_string());
            } else if (type == "int") {
              tmx.addIntProperty(std::move(name), required_attribute(property, "value").as_int());
            } else if (type == "float") {
              tmx.addFloatProperty(std::move(name), required_attribute(property, "value").as_double());
            } else if (type == "bool") {
              tmx.addBoolProperty(std::move(name), required_attribute(property, "value").as_bool());
            } else if (type == "color") {
              tmx.addColorProperty(std::move(name), computeColor(required_attribute(property, "value")));
            } else if (type == "file") {
              tmx.addFileProperty(std::move(name), required_attribute(property, "value").as_string());
            } else {
              Log::error("Wrong type string: '%s'\n", type.c_str());
            }
          } else {
            tmx.addStringProperty(std::move(name), required_attribute(property, "value").as_string());
          }
        }
      }

      return tmx;
    }


    void parseTmxLayer(const pugi::xml_node node, TmxLayer& tmx) {
      tmx.properties = parseTmxProperties(node);

      tmx.name = required_attribute(node, "name").as_string();
      tmx.opacity = node.attribute("opacity").as_double(1.0);
      tmx.visible = node.attribute("visible").as_bool(true);
      tmx.offset.x = node.attribute("offsetx").as_int(0);
      tmx.offset.y = node.attribute("offsety").as_int(0);
    }


    std::unique_ptr<TmxTileLayer> parseTmxTileLayer(const pugi::xml_node node) {
      assert(node.name() == "layer"s);

      auto tmx = std::make_unique<TmxTileLayer>();
      parseTmxLayer(node, *tmx);

      for (pugi::xml_node data : node.children("data")) {
        auto format = parseDataFormat(data);

        switch (format) {
          case TmxFormat::Base64:
          case TmxFormat::Base64_Zlib:
          case TmxFormat::Base64_Gzip:
          {
            std::vector<uint8_t> buffer = parseDataBuffer(data, format);

            const std::size_t size = buffer.size();
            assert(size % 4 == 0);

            for (std::size_t i = 0; i < size; i += 4) {
              unsigned gid = buffer[i] | (buffer[i + 1] << 8) | (buffer[i + 2] << 16) | (buffer[i + 3] << 24);
              tmx->cells.push_back(decodeGID(gid));
            }

            break;
          }

          case TmxFormat::Csv:
          {
            const std::string csv = data.child_value();
            std::vector<std::string> items;
            boost::algorithm::split(items, csv, boost::algorithm::is_any_of(","));

            for (auto item : items) {
              unsigned gid = std::stoul(item);
              tmx->cells.push_back(decodeGID(gid));
            }

            break;
          }

          case TmxFormat::Xml:
            for (pugi::xml_node tile : data.children("tile")) {
              unsigned gid = required_attribute(tile, "gid").as_uint();
              tmx->cells.push_back(decodeGID(gid));
            };

            break;
        }
      }

      return tmx;
    }

    std::unique_ptr<TmxImage> parseTmxImage(const pugi::xml_node node, const TmxParserCtx& ctx) {
      assert(node.name() == "image"s);

      auto tmx = std::make_unique<TmxImage>();

      tmx->format = node.attribute("format").as_string();
      tmx->source = ctx.currentPath / required_attribute(node, "source").as_string();
      tmx->transparent = computeColor(node.attribute("trans"));
      tmx->size.width = node.attribute("width").as_uint();
      tmx->size.height = node.attribute("height").as_uint();

      // TODO: handle "data"

      return tmx;
    }

    std::unique_ptr<TmxImageLayer> parseTmxImageLayer(const pugi::xml_node node, const TmxParserCtx& ctx) {
      assert(node.name() == "imagelayer"s);

      auto tmx = std::make_unique<TmxImageLayer>();
      parseTmxLayer(node, *tmx);

      for (pugi::xml_node image : node.children("image")) {
        tmx->image = parseTmxImage(image, ctx);
      }

      return tmx;
    }

    std::vector<Vector2f> parsePoints(const std::string& points) {
      std::vector<Vector2f> ret;

      std::vector<std::string> items;
      boost::algorithm::split(items, points, boost::algorithm::is_any_of(" "));

      for (auto item : items) {
        std::vector<std::string> coords;
        boost::algorithm::split(coords, item, boost::algorithm::is_any_of(","));

        assert(coords.size() == 2);
        float x = std::stof(coords[0]);
        float y = std::stof(coords[1]);
        ret.push_back({ x, y });
      }

      return ret;
    }

    void parseTmxObjectCommon(const pugi::xml_node node, TmxObject& tmx) {
      tmx.properties = parseTmxProperties(node);

      tmx.id = node.attribute("id").as_int();
      tmx.name = node.attribute("name").as_string();
      tmx.type = node.attribute("type").as_string();
      tmx.position.x = required_attribute(node, "x").as_float();
      tmx.position.y = required_attribute(node, "y").as_float();
      tmx.rotation = node.attribute("rotation").as_double();
      tmx.visible = node.attribute("visible").as_bool(true);
    }

    std::unique_ptr<TmxObject> parseTmxObject(const pugi::xml_node node) {
      if (node.child("polygon")) {
        auto tmx = std::make_unique<TmxPolygon>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Polygon;

        std::string points = required_attribute(node.child("polygon"), "points").as_string();
        tmx->points = parsePoints(points);

        return std::move(tmx);
      }

      if (node.child("polyline")) {
        auto tmx = std::make_unique<TmxPolyline>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Polyline;

        std::string points = required_attribute(node.child("polyline"), "points").as_string();
        tmx->points = parsePoints(points);

        return std::move(tmx);
      }

      if (node.child("text")) {
        auto tmx = std::make_unique<TmxText>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Text;

        pugi::xml_node text = node.child("text");

        tmx->fontFamily = text.attribute("fontfamily").as_string("sans-serif");
        tmx->sizeInPixels = text.attribute("pixelsize").as_uint(16);
        tmx->wrap = text.attribute("wrap").as_bool(false);
        tmx->color = computeColor(text.attribute("color"), { 0x00, 0x00, 0x00, 0xFF });
        tmx->bold = text.attribute("bold").as_bool(false);
        tmx->italic = text.attribute("italic").as_bool(false);
        tmx->underline = text.attribute("underline").as_bool(false);
        tmx->strikeout = text.attribute("strikeout").as_bool(false);
        tmx->kerning = text.attribute("kerning").as_bool(true);

        tmx->halign = TmxText::HAlign::Left;

        if (text.attribute("halign")) {
          std::string halign = text.attribute("halign").as_string();

          if (halign == "left") {
            tmx->halign = TmxText::HAlign::Left;
          } else if (halign == "center") {
            tmx->halign = TmxText::HAlign::Center;
          } else if (halign == "right") {
            tmx->halign = TmxText::HAlign::Right;
          } else {
            Log::error("Wrong halign string: '%s'\n", halign.c_str());
          }
        }

        tmx->valign = TmxText::VAlign::Top;

        if (text.attribute("valign")) {
          std::string valign = text.attribute("valign").as_string();

          if (valign == "top") {
            tmx->valign = TmxText::VAlign::Top;
          } else if (valign == "center") {
            tmx->valign = TmxText::VAlign::Center;
          } else if (valign == "bottom") {
            tmx->valign = TmxText::VAlign::Bottom;
          } else {
            Log::error("Wrong valign string: '%s'\n", valign.c_str());
          }
        }

        tmx->text = text.child_value();

        return std::move(tmx);
      }

      if (node.attribute("gid")) {
        unsigned gid = node.attribute("gid").as_uint();
        TmxCell cell = decodeGID(gid);

        auto tmx = std::make_unique<TmxTileObject>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Tile;
        tmx->gid = cell.gid;
        tmx->flip = cell.flip;

        return std::move(tmx);
      }

      if (node.child("ellipse")) {
        auto tmx = std::make_unique<TmxEllipse>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Ellipse;
        tmx->size.width = node.attribute("width").as_float();
        tmx->size.height = node.attribute("height").as_float();

        return std::move(tmx);
      }

      auto tmx = std::make_unique<TmxRectangle>();
      parseTmxObjectCommon(node, *tmx);

      tmx->kind = TmxObject::Rectangle;
      tmx->size.width = required_attribute(node, "width").as_float();
      tmx->size.height = required_attribute(node, "height").as_float();

      return std::move(tmx);
    }

    std::unique_ptr<TmxObjectLayer> parseTmxObjectLayer(const pugi::xml_node node) {
      assert(node.name() == "objectgroup"s);

      auto tmx = std::make_unique<TmxObjectLayer>();
      parseTmxLayer(node, *tmx);

      tmx->color = computeColor(node.attribute("color"));
      tmx->drawOrder = TmxDrawOrder::TopDown;

      if (node.attribute("draworder")) {
        std::string drawOrder = node.attribute("draworder").as_string();

        if (drawOrder == "topdown") {
          tmx->drawOrder = TmxDrawOrder::TopDown;
        } else if (drawOrder == "index") {
          tmx->drawOrder = TmxDrawOrder::Index;
        } else {
          Log::error("Wrong draw order string: '%s'\n", drawOrder.c_str());
        }
      }

      for (pugi::xml_node object : node.children("object")) {
        tmx->objects.push_back(parseTmxObject(object));
      }

      return tmx;
    }

    std::unique_ptr<TmxGroupLayer> parseTmxGroupLayer(const pugi::xml_node node, const TmxParserCtx& ctx) {
      assert(node.name() == "group"s);

      auto tmx = std::make_unique<TmxGroupLayer>();
      parseTmxLayer(node, *tmx);

      for (pugi::xml_node layer : node.children()) {
        std::string name = layer.name();

        if (name == "layer") {
          tmx->layers.push_back(parseTmxTileLayer(layer));
        } else if (name == "objectgroup") {
          tmx->layers.push_back(parseTmxObjectLayer(layer));
        } else if (name == "imagelayer") {
          tmx->layers.push_back(parseTmxImageLayer(layer, ctx));
        } else if (name == "group") {
          tmx->layers.push_back(parseTmxGroupLayer(layer, ctx));
        }
      };

      return tmx;
    }

    /*
     * Tilesets
     */

    TmxFrame parseTmxFrame(const pugi::xml_node node) {
      assert(node.name() == "frame"s);

      TmxFrame tmx;

      tmx.tileId = required_attribute(node, "tileId").as_uint();
      tmx.duration = gf::milliseconds(required_attribute(node, "duration").as_uint());

      return tmx;
    }

    std::unique_ptr<TmxAnimation> parseTmxAnimation(const pugi::xml_node node) {
      assert(node.name() == "animation"s);

      auto tmx = std::make_unique<TmxAnimation>();

      for (pugi::xml_node frame : node.children("frame")) {
        tmx->frames.push_back(parseTmxFrame(frame));
      }

      return tmx;
    }

    TmxTile parseTmxTile(const pugi::xml_node node, TmxParserCtx& ctx) {
      assert(node.name() == "tile"s);

      TmxTile tmx;

      tmx.properties = parseTmxProperties(node);
      tmx.id = required_attribute(node, "id").as_uint();
      tmx.type = node.attribute("type").as_string();

      static constexpr unsigned Invalid = static_cast<unsigned>(-1);

      tmx.terrain = { { Invalid, Invalid, Invalid, Invalid } };

      std::string terrain = node.attribute("terrain").as_string();

      if (!terrain.empty()) {
        std::vector<std::string> items;
        boost::algorithm::split(items, terrain, boost::algorithm::is_any_of(","));
        unsigned t = 0;

        for (auto item : items) {
          if (!item.empty()) {
            tmx.terrain[t++] = std::stoul(item);
          }
        }
      }

      tmx.probability = node.attribute("probability").as_uint(100u);

      tmx.image = nullptr;
      pugi::xml_node image = node.child("image");

      if (image) {
        tmx.image = parseTmxImage(image, ctx);
      }

      tmx.objects = nullptr;
      pugi::xml_node objects = node.child("objectgroup");

      if (objects) {
        tmx.objects = parseTmxObjectLayer(objects);
      }

      tmx.animation = nullptr;
      pugi::xml_node animation = node.child("objectgroup");

      if (animation) {
        tmx.animation = parseTmxAnimation(animation);
      }

      return tmx;
    }

    TmxTerrain parseTmxTerrain(const pugi::xml_node node) {
      assert(node.name() == "terrain"s);

      TmxTerrain tmx;

      tmx.properties = parseTmxProperties(node);

      tmx.name = required_attribute(node, "name").as_string();
      tmx.tile = required_attribute(node, "tile").as_uint();

      return tmx;
    }

    void parseTmxTilesetFromElement(const pugi::xml_node node, TmxTileset& tmx, TmxParserCtx& ctx) {
      assert(node.name() == "tileset"s);

      tmx.name = node.attribute("name").as_string();

      tmx.tileSize.width = node.attribute("tilewidth").as_uint();
      tmx.tileSize.height = node.attribute("tileheight").as_uint();

      tmx.spacing = node.attribute("spacing").as_uint();
      tmx.margin = node.attribute("margin").as_uint();

      tmx.tileCount = node.attribute("tilecount").as_uint();
      tmx.columnCount = node.attribute("columns").as_uint();

      tmx.offset = { 0, 0 };
      pugi::xml_node offset = node.child("tileoffset");

      if (offset) {
        tmx.offset.x = offset.attribute("x").as_int();
        tmx.offset.y = offset.attribute("y").as_int();
      }

      tmx.image = nullptr;
      pugi::xml_node image = node.child("image");

      if (image) {
        tmx.image = parseTmxImage(image, ctx);
      }

      pugi::xml_node terrains = node.child("terraintypes");

      if (terrains) {
        for (pugi::xml_node terrain : terrains.children("terrain")) {
          tmx.terrains.push_back(parseTmxTerrain(terrain));
        }
      }

      for (pugi::xml_node tile : node.children("tile")) {
        tmx.tiles.push_back(parseTmxTile(tile, ctx));
      }
    }

    void parseTmxTilesetFromFile(const Path& source, TmxTileset& tmx, TmxParserCtx& ctx) {
      Path tilesetPath = ctx.currentPath / source;

      pugi::xml_document doc;
      pugi::xml_parse_result result = doc.load_file(tilesetPath.string().c_str());

      if (!result) {
        Log::error("Could not load TSX file '%s': %s\n", tilesetPath.string().c_str(), result.description());
        return;
      }

      ctx.currentPath = tilesetPath.parent_path();

      if (doc.attribute("firstgid")) {
        Log::warning("Attribute 'firstgid' present in a TSX file: '%s'\n", tilesetPath.string().c_str());
      }

      if (doc.attribute("source")) {
        Log::warning("Attribute 'source' present in a TSX file: '%s'\n", tilesetPath.string().c_str());
      }

      parseTmxTilesetFromElement(doc, tmx, ctx);
      ctx.currentPath = ctx.mapPath.parent_path();
    }

    TmxTileset parseTmxTileset(const pugi::xml_node node, TmxParserCtx& ctx) {
      TmxTileset tmx;

      tmx.properties = parseTmxProperties(node);

      tmx.firstGid = required_attribute(node, "firstgid").as_uint();

      Path source = node.attribute("source").as_string();

      if (!source.empty()) {
        parseTmxTilesetFromFile(source, tmx, ctx);
      } else {
        parseTmxTilesetFromElement(node, tmx, ctx);
      }

      return tmx;
    }

    /*
     * Map
     */

    bool parseTmxLayers(const pugi::xml_node node, TmxLayers& tmx, TmxParserCtx& ctx) {
      assert(node.name() == "map"s);

      tmx.properties = parseTmxProperties(node);

      tmx.version =  node.attribute("version").as_string("1.0");
      tmx.tiledVersion = node.attribute("tiledversion").as_string("1.0");

      tmx.orientation = TmxOrientation::Unknown;
      std::string orientation = required_attribute(node, "orientation").as_string();

      if (orientation == "orthogonal") {
        tmx.orientation = TmxOrientation::Orthogonal;
      } else if (orientation == "isometric") {
        tmx.orientation = TmxOrientation::Isometric;
      } else if (orientation == "staggered") {
        tmx.orientation = TmxOrientation::Staggered;
      } else if (orientation == "hexagonal") {
        tmx.orientation = TmxOrientation::Hexagonal;
      } else {
        Log::error("Wrong orientation string: '%s'\n", orientation.c_str());
      }

      tmx.renderOrder = TmxRenderOrder::RightDown; // default value

      if (node.attribute("renderorder")) {
        std::string renderOrder = node.attribute("renderorder").as_string();

        if (renderOrder == "right-down") {
          tmx.renderOrder = TmxRenderOrder::RightDown;
        } else if (renderOrder == "right-up") {
          tmx.renderOrder = TmxRenderOrder::RightUp;
        } else if (renderOrder == "left-down") {
          tmx.renderOrder = TmxRenderOrder::LeftDown;
        } else if (renderOrder == "left-up") {
          tmx.renderOrder = TmxRenderOrder::LeftUp;
        } else {
          Log::error("Wrong render order string: '%s'\n", renderOrder.c_str());
        }
      }

      tmx.mapSize.width = required_attribute(node, "width").as_uint();
      tmx.mapSize.height = required_attribute(node, "height").as_uint();

      tmx.tileSize.width = required_attribute(node, "tilewidth").as_uint();
      tmx.tileSize.height = required_attribute(node, "tileheight").as_uint();

      tmx.hexSideLength = node.attribute("hexsidelength").as_uint(0u);

      tmx.staggerAxis = TmxStaggerAxis::Y;

      if (node.attribute("staggeraxis")) {
        std::string staggerAxis = node.attribute("staggeraxis").as_string();

        if (staggerAxis == "x") {
          tmx.staggerAxis = TmxStaggerAxis::X;
        } else if (staggerAxis == "y") {
          tmx.staggerAxis = TmxStaggerAxis::Y;
        } else {
          Log::error("Wrong stagger axis string: '%s'\n", staggerAxis.c_str());
        }
      }

      tmx.staggerIndex = TmxStaggerIndex::Odd;

      if (node.attribute("staggerindex")) {
        std::string staggerIndex = node.attribute("staggerindex").as_string();

        if (staggerIndex == "odd") {
          tmx.staggerIndex = TmxStaggerIndex::Odd;
        } else if (staggerIndex == "even") {
          tmx.staggerIndex = TmxStaggerIndex::Even;
        } else {
          Log::error("Wrong stagger index string: '%s'\n", staggerIndex.c_str());
        }
      }

      tmx.backgroundColor = computeColor(node.attribute("backgroundcolor"), Color4u(0xFF, 0xFF, 0xFF, 0xFF));

      tmx.nextObjectId = node.attribute("nextobjectid").as_uint(0u);

      for (pugi::xml_node tileset : node.children("tileset")) {
        tmx.tilesets.push_back(parseTmxTileset(tileset, ctx));
      }

      for (pugi::xml_node layer : node.children()) {
        std::string name = layer.name();

        if (name == "layer") {
          tmx.layers.push_back(parseTmxTileLayer(layer));
        } else if (name == "objectgroup") {
          tmx.layers.push_back(parseTmxObjectLayer(layer));
        } else if (name == "imagelayer") {
          tmx.layers.push_back(parseTmxImageLayer(layer, ctx));
        } else if (name == "group") {
          tmx.layers.push_back(parseTmxGroupLayer(layer, ctx));
        }
      };

      return true;
    }

  }

  bool TmxLayers::loadFromFile(const Path& filename) {
    if (!fs::is_regular_file(filename)) {
      Log::error("Unknown TMX file: '%s'\n", filename.string().c_str());
      return false;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.string().c_str());

    if (!result) {
      Log::error("Could not load TMX file '%s': %s\n", filename.string().c_str(), result.description());
      return false;
    }

    TmxParserCtx ctx;
    ctx.mapPath = filename;
    ctx.currentPath = filename.parent_path();

    return parseTmxLayers(doc.child("map"), *this, ctx);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
