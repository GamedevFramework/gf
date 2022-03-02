/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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

#include <pugixml.hpp>

using namespace std::string_literals;

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void TmxProperties::addStringProperty(std::string name, std::string value) {
    m_props.insert({ std::move(name), std::move(value) });
  }

  void TmxProperties::addIntProperty(std::string name, int value) {
    m_props.insert({ std::move(name), value });
  }

  void TmxProperties::addFloatProperty(std::string name, double value) {
    m_props.insert({ std::move(name), value });
  }

  void TmxProperties::addBoolProperty(std::string name, bool value) {
    m_props.insert({ std::move(name), value });
  }

  void TmxProperties::addColorProperty(std::string name, Color4u value) {
    m_props.insert({ std::move(name), value });
  }

  void TmxProperties::addFileProperty(std::string name, Path value) {
    m_props.insert({ std::move(name), std::move(value) });
  }

  void TmxProperties::addObjectProperty(std::string name, Id value) {
    m_props.insert({ std::move(name), value });
  }

  void TmxProperties::addClassProperty(std::string name, TmxProperties value) {
    m_props.insert({ std::move(name), std::move(value) });
  }

  namespace {

    template<typename T, typename U>
    U findValue(const std::map<std::string, T>& prop, const std::string& name, const U& def) {
      auto it = prop.find(name);

      if (it == prop.end()) {
        return def;
      }

      if (!std::holds_alternative<U>(it->second)) {
        return def;
      }

      return std::get<U>(it->second);
    }

  }

  std::string TmxProperties::getStringProperty(const std::string& name, const std::string& def) const {
    return findValue(m_props, name, def);
  }

  int TmxProperties::getIntProperty(const std::string& name, int def) const {
    return findValue(m_props, name, def);
  }

  double TmxProperties::getFloatProperty(const std::string& name, double def) const {
    return findValue(m_props, name, def);
  }

  bool TmxProperties::getBoolProperty(const std::string& name, bool def) const {
    return findValue(m_props, name, def);
  }

  Color4u TmxProperties::getColorProperty(const std::string& name, const Color4u& def) const {
    return findValue(m_props, name, def);
  }

  Path TmxProperties::getFileProperty(const std::string& name, const Path& def) const {
    return findValue(m_props, name, def);
  }

  Id TmxProperties::getObjectProperty(const std::string& name, Id def) const {
    return findValue(m_props, name, def);
  }

  TmxProperties TmxProperties::getClassProperty(const std::string& name, const TmxProperties& def) const {
    return findValue(m_props, name, def);
  }

  TmxVisitor::~TmxVisitor() = default;

  void TmxVisitor::visitTileLayer([[maybe_unused]] const TmxLayers& map, [[maybe_unused]] const TmxTileLayer& layer) {
  }

  void TmxVisitor::visitObjectLayer([[maybe_unused]] const TmxLayers& map, [[maybe_unused]] const TmxObjectLayer& layer) {
  }

  void TmxVisitor::visitImageLayer([[maybe_unused]] const TmxLayers& map, [[maybe_unused]] const TmxImageLayer& layer) {
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


  const TmxTile *TmxTileset::getTile(int id) const noexcept {
    for (auto& tile : tiles) {
      if (tile.id == id) {
        return std::addressof(tile);
      }
    }

    return nullptr;
  }

  RectI TmxTileset::getSubTexture(int id, Vector2i size) const noexcept {
    int width = (size.width - 2 * margin + spacing) / (tileSize.width + spacing); // number of tiles
    [[maybe_unused]] int height = (size.height - 2 * margin + spacing) / (tileSize.height + spacing); // number of tiles

    int tu = id % width;
    int tv = id / width;
    assert(tv < height);

    int du = margin + tu * spacing + offset.x;
    int dv = margin + tv * spacing + offset.y;
    assert((tu + 1) * tileSize.width + du <= size.width);
    assert((tv + 1) * tileSize.height + dv <= size.height);

    return RectI::fromPositionSize({ tu * tileSize.width + du, tv * tileSize.height + dv }, tileSize);
  }


  const TmxTileset *TmxLayers::getTileSetFromGID(uint32_t gid) const noexcept {
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

      std::string value = attr.as_string();
      assert(!value.empty());

      if (value[0] == '#') {
        value.erase(value.begin());
      }

      Color4u color = def;

      switch (value.size()) {
        case 5:
          value.insert(0, 1, '0');
          [[fallthrough]];
        case 6:
          color.a = 0xFF;
          color.r = (convertHexChar(value[0]) << 4) + convertHexChar(value[1]);
          color.g = (convertHexChar(value[2]) << 4) + convertHexChar(value[3]);
          color.b = (convertHexChar(value[4]) << 4) + convertHexChar(value[5]);
          break;

        case 7:
          value.insert(0, 1, '0');
          [[fallthrough]];
        case 8:
          color.a = (convertHexChar(value[0]) << 4) + convertHexChar(value[1]);
          color.r = (convertHexChar(value[2]) << 4) + convertHexChar(value[3]);
          color.g = (convertHexChar(value[4]) << 4) + convertHexChar(value[5]);
          color.b = (convertHexChar(value[6]) << 4) + convertHexChar(value[7]);
          break;

        default:
          Log::error("Unknown color format: %s\n", value.c_str());
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
      stream.avail_in = static_cast<uInt>(input.size());
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

    TmxCell decodeGID(uint32_t gid) {
      static constexpr uint32_t FlippedHorizontallyFlag = UINT32_C(0x80000000);
      static constexpr uint32_t FlippedVerticallyFlag   = UINT32_C(0x40000000);
      static constexpr uint32_t FlippedDiagonallyFlag   = UINT32_C(0x20000000);
      static constexpr uint32_t RotatedHexagonal120Flag = UINT32_C(0x10000000);

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

      if ((gid & RotatedHexagonal120Flag) != 0) {
        cell.flip.set(Flip::Rotation120);
      }

      // Clear the flags
      cell.gid = gid & ~(FlippedHorizontallyFlag | FlippedVerticallyFlag | FlippedDiagonallyFlag | RotatedHexagonal120Flag);

      return cell;
    }

    TmxProperties parseTmxProperties(const pugi::xml_node node) {
      TmxProperties tmx;

      for (pugi::xml_node properties : node.children("properties")) {
        for (pugi::xml_node property : properties.children("property")) {
          std::string name = required_attribute(property, "name").as_string();
          assert(!name.empty());

          if (property.attribute("type") != nullptr) {
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
            } else if (type == "object") {
              tmx.addObjectProperty(std::move(name), required_attribute(property, "value").as_uint());
            } else if (type == "class") {
              tmx.addClassProperty(std::move(name), parseTmxProperties(property));
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

      tmx.name = node.attribute("name").as_string();
      tmx.opacity = node.attribute("opacity").as_double(1.0);
      tmx.visible = node.attribute("visible").as_bool(true);
      tmx.offset.x = node.attribute("offsetx").as_int(0);
      tmx.offset.y = node.attribute("offsety").as_int(0);
    }

    std::vector<TmxCell> parseCells(const pugi::xml_node node, TmxFormat format) {
      std::vector<TmxCell> cells;

      switch (format) {
        case TmxFormat::Base64:
        case TmxFormat::Base64_Zlib:
        case TmxFormat::Base64_Gzip:
        {
          std::vector<uint8_t> buffer = parseDataBuffer(node, format);

          const std::size_t size = buffer.size();
          assert(size % 4 == 0);

          for (std::size_t i = 0; i < size; i += 4) {
            uint32_t gid = buffer[i] | (buffer[i + 1] << 8) | (buffer[i + 2] << 16) | (buffer[i + 3] << 24);
            cells.push_back(decodeGID(gid));
          }

          break;
        }

        case TmxFormat::Csv:
        {
          const std::string csv = node.child_value();
          std::vector<std::string> items;
          boost::algorithm::split(items, csv, boost::algorithm::is_any_of(","));

          for (auto item : items) {
            auto raw = std::stoul(item);
            assert(raw < UINT32_MAX);
            uint32_t gid = static_cast<uint32_t>(raw);
            cells.push_back(decodeGID(gid));
          }

          break;
        }

        case TmxFormat::Xml:
          for (pugi::xml_node tile : node.children("tile")) {
            uint32_t gid = required_attribute(tile, "gid").as_uint();
            cells.push_back(decodeGID(gid));
          };

          break;
      }

      return cells;
    }

    std::unique_ptr<TmxTileLayer> parseTmxTileLayer(const pugi::xml_node node) {
      assert(node.name() == "layer"s);

      auto tmx = std::make_unique<TmxTileLayer>();
      parseTmxLayer(node, *tmx);

      for (pugi::xml_node data : node.children("data")) {
        auto format = parseDataFormat(data);
        auto range = data.children("chunk");

        if (std::distance(range.begin(), range.end()) > 0) {
          for (pugi::xml_node chunk : range) {
            TmxChunk ck;
            ck.position.x = required_attribute(node, "x").as_int();
            ck.position.y = required_attribute(node, "y").as_int();
            ck.size.width = required_attribute(node, "width").as_int();
            ck.size.height = required_attribute(node, "height").as_int();
            ck.cells = parseCells(chunk, format);
            tmx->chunks.push_back(std::move(ck));
          }
        } else {
          tmx->cells = parseCells(data, format);
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
      tmx->size.width = node.attribute("width").as_int();
      tmx->size.height = node.attribute("height").as_int();

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
      if (node.child("polygon") != nullptr) {
        auto tmx = std::make_unique<TmxPolygon>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Polygon;

        std::string points = required_attribute(node.child("polygon"), "points").as_string();
        tmx->points = parsePoints(points);

        return tmx;
      }

      if (node.child("polyline") != nullptr) {
        auto tmx = std::make_unique<TmxPolyline>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Polyline;

        std::string points = required_attribute(node.child("polyline"), "points").as_string();
        tmx->points = parsePoints(points);

        return tmx;
      }

      if (node.child("text") != nullptr) {
        auto tmx = std::make_unique<TmxText>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Text;

        pugi::xml_node text = node.child("text");

        tmx->fontFamily = text.attribute("fontfamily").as_string("sans-serif");
        tmx->sizeInPixels = text.attribute("pixelsize").as_int(16);
        tmx->wrap = text.attribute("wrap").as_bool(false);
        tmx->color = computeColor(text.attribute("color"), { 0x00, 0x00, 0x00, 0xFF });
        tmx->bold = text.attribute("bold").as_bool(false);
        tmx->italic = text.attribute("italic").as_bool(false);
        tmx->underline = text.attribute("underline").as_bool(false);
        tmx->strikeout = text.attribute("strikeout").as_bool(false);
        tmx->kerning = text.attribute("kerning").as_bool(true);

        tmx->halign = TmxText::HAlign::Left;

        if (text.attribute("halign") != nullptr) {
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

        if (text.attribute("valign") != nullptr) {
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

        return tmx;
      }

      if (node.attribute("gid") != nullptr) {
        uint32_t gid = node.attribute("gid").as_uint();
        TmxCell cell = decodeGID(gid);

        auto tmx = std::make_unique<TmxTileObject>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Tile;
        tmx->gid = cell.gid;
        tmx->flip = cell.flip;

        return tmx;
      }

      if (node.child("point") != nullptr) {
        auto tmx = std::make_unique<TmxPoint>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Point;

        return tmx;
      }

      if (node.child("ellipse") != nullptr) {
        auto tmx = std::make_unique<TmxEllipse>();
        parseTmxObjectCommon(node, *tmx);

        tmx->kind = TmxObject::Ellipse;
        tmx->size.width = node.attribute("width").as_float();
        tmx->size.height = node.attribute("height").as_float();

        return tmx;
      }

      auto tmx = std::make_unique<TmxRectangle>();
      parseTmxObjectCommon(node, *tmx);

      tmx->kind = TmxObject::Rectangle;
      tmx->size.width = required_attribute(node, "width").as_float();
      tmx->size.height = required_attribute(node, "height").as_float();

      return tmx;
    }

    std::unique_ptr<TmxObjectLayer> parseTmxObjectLayer(const pugi::xml_node node) {
      assert(node.name() == "objectgroup"s);

      auto tmx = std::make_unique<TmxObjectLayer>();
      parseTmxLayer(node, *tmx);

      tmx->color = computeColor(node.attribute("color"));
      tmx->drawOrder = TmxDrawOrder::TopDown;

      if (node.attribute("draworder") != nullptr) {
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

      tmx.tileId = required_attribute(node, "tileId").as_int();
      tmx.duration = gf::milliseconds(required_attribute(node, "duration").as_int());

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
      tmx.id = required_attribute(node, "id").as_int();
      tmx.type = node.attribute("type").as_string();

      tmx.probability = node.attribute("probability").as_int(0);

      tmx.image = nullptr;
      pugi::xml_node image = node.child("image");

      if (image != nullptr) {
        tmx.image = parseTmxImage(image, ctx);
      }

      tmx.objects = nullptr;
      pugi::xml_node objects = node.child("objectgroup");

      if (objects != nullptr) {
        tmx.objects = parseTmxObjectLayer(objects);
      }

      tmx.animation = nullptr;
      pugi::xml_node animation = node.child("animation");

      if (animation != nullptr) {
        tmx.animation = parseTmxAnimation(animation);
      }

      return tmx;
    }

    TmxWangColor parseTmxWangColor(const pugi::xml_node node) {
      assert(node.name() == "wangcolor"s);

      TmxWangColor tmx;

      tmx.properties = parseTmxProperties(node);

      tmx.name = required_attribute(node, "name").as_string();
      tmx.color = computeColor(required_attribute(node, "color"));
      tmx.tile = required_attribute(node, "tile").as_int();
      tmx.probability = node.attribute("probability").as_int(0);

      return tmx;
    }

    TmxWangTile parseTmxWangTile(const pugi::xml_node node) {
      assert(node.name() == "wangtile"s);

      TmxWangTile tmx;
      tmx.tileid = required_attribute(node, "tileid").as_int();
      tmx.wangid = { 0, 0, 0, 0, 0, 0, 0, 0 };

      std::string wangid = node.attribute("wangid").as_string();

      if (!wangid.empty()) {
        std::vector<std::string> items;
        boost::algorithm::split(items, wangid, boost::algorithm::is_any_of(","));
        assert(items.size() == 8);
        std::size_t index = 0;

        for (auto item : items) {
          assert(!item.empty());
          tmx.wangid[index++] = std::stoi(item);
        }
      }

      return tmx;
    }

    TmxWangSet parseTmxWangSet(const pugi::xml_node node) {
      assert(node.name() == "wangset"s);

      TmxWangSet tmx;

      tmx.properties = parseTmxProperties(node);

      tmx.name = required_attribute(node, "name").as_string();
      tmx.tile = required_attribute(node, "tile").as_int();

      for (pugi::xml_node color : node.children("wangcolor")) {
        tmx.colors.push_back(parseTmxWangColor(color));
      }

      for (pugi::xml_node tile : node.children("wangtile")) {
        tmx.tiles.push_back(parseTmxWangTile(tile));
      }

      return tmx;
    }

    void parseTmxTilesetFromElement(const pugi::xml_node node, TmxTileset& tmx, TmxParserCtx& ctx) {
      assert(node.name() == "tileset"s);

      tmx.name = node.attribute("name").as_string();

      tmx.tileSize.width = node.attribute("tilewidth").as_int();
      tmx.tileSize.height = node.attribute("tileheight").as_int();

      tmx.spacing = node.attribute("spacing").as_int();
      tmx.margin = node.attribute("margin").as_int();

      tmx.tileCount = node.attribute("tilecount").as_int();
      tmx.columnCount = node.attribute("columns").as_int();

      tmx.offset = { 0, 0 };
      pugi::xml_node offset = node.child("tileoffset");

      if (offset != nullptr) {
        tmx.offset.x = offset.attribute("x").as_int();
        tmx.offset.y = offset.attribute("y").as_int();
      }

      tmx.image = nullptr;
      pugi::xml_node image = node.child("image");

      if (image != nullptr) {
        tmx.image = parseTmxImage(image, ctx);
      }

      for (pugi::xml_node tile : node.children("tile")) {
        tmx.tiles.push_back(parseTmxTile(tile, ctx));
      }

      pugi::xml_node wangsets = node.child("wangsets");

      if (wangsets != nullptr) {
        for (pugi::xml_node wangset : wangsets.children("wangset")) {
          tmx.wangsets.push_back(parseTmxWangSet(wangset));
        }
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

      pugi::xml_node tileset = doc.child("tileset");

      ctx.currentPath = tilesetPath.parent_path();

      if (tileset.attribute("firstgid") != nullptr) {
        Log::warning("Attribute 'firstgid' present in a TSX file: '%s'\n", tilesetPath.string().c_str());
      }

      if (tileset.attribute("source") != nullptr) {
        Log::warning("Attribute 'source' present in a TSX file: '%s'\n", tilesetPath.string().c_str());
      }

      parseTmxTilesetFromElement(tileset, tmx, ctx);
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

      tmx.orientation = CellOrientation::Unknown;
      std::string orientation = required_attribute(node, "orientation").as_string();

      if (orientation == "orthogonal") {
        tmx.orientation = CellOrientation::Orthogonal;
      } else if (orientation == "isometric") {
        tmx.orientation = CellOrientation::Isometric;
      } else if (orientation == "staggered") {
        tmx.orientation = CellOrientation::Staggered;
      } else if (orientation == "hexagonal") {
        tmx.orientation = CellOrientation::Hexagonal;
      } else {
        Log::error("Wrong orientation string: '%s'\n", orientation.c_str());
      }

      tmx.renderOrder = TmxRenderOrder::RightDown; // default value

      if (node.attribute("renderorder") != nullptr) {
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

      tmx.infinite = node.attribute("invisible").as_bool(false);
      tmx.mapSize.width = required_attribute(node, "width").as_int();
      tmx.mapSize.height = required_attribute(node, "height").as_int();

      tmx.tileSize.width = required_attribute(node, "tilewidth").as_int();
      tmx.tileSize.height = required_attribute(node, "tileheight").as_int();

      tmx.hexSideLength = node.attribute("hexsidelength").as_int(0);

      tmx.cellAxis = CellAxis::Y;

      if (node.attribute("staggeraxis") != nullptr) {
        std::string mapCellAxis = node.attribute("staggeraxis").as_string();

        if (mapCellAxis == "x") {
          tmx.cellAxis = CellAxis::X;
        } else if (mapCellAxis == "y") {
          tmx.cellAxis = CellAxis::Y;
        } else {
          Log::error("Wrong stagger axis string: '%s'\n", mapCellAxis.c_str());
        }
      }

      tmx.cellIndex = CellIndex::Odd;

      if (node.attribute("staggerindex") != nullptr) {
        std::string mapCellIndex = node.attribute("staggerindex").as_string();

        if (mapCellIndex == "odd") {
          tmx.cellIndex = CellIndex::Odd;
        } else if (mapCellIndex == "even") {
          tmx.cellIndex = CellIndex::Even;
        } else {
          Log::error("Wrong stagger index string: '%s'\n", mapCellIndex.c_str());
        }
      }

      tmx.backgroundColor = computeColor(node.attribute("backgroundcolor"), Color4u(0xFF, 0xFF, 0xFF, 0xFF));

      tmx.nextObjectId = node.attribute("nextobjectid").as_int(0);

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
    if (!std::filesystem::is_regular_file(filename)) {
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
