/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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

#include "vendor/tinyxml2/tinyxml2.h"

namespace fs = boost::filesystem;

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


  TmxVisitor::~TmxVisitor() {

  }

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

  TmxLayer::~TmxLayer() {

  }

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

  TmxObject::~TmxObject() {

  }


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

    template<typename T, typename ... Args>
      std::unique_ptr<T> makeUnique(Args... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    enum class Requirement {
      Optional,
      Mandatory,
    };

    class XMLElementWrapper {
    public:
      XMLElementWrapper(const tinyxml2::XMLElement *elt)
      : m_elt(elt)
      {

      }

      bool is(const char *name) const {
        return !std::strcmp(m_elt->Name(), name);
      }

      bool hasChild(const char *name) const {
        return m_elt->FirstChildElement(name) != nullptr;
      }

      template<typename Func>
      void parseEachElement(Func func) const {
        const tinyxml2::XMLElement *child = m_elt->FirstChildElement();

        while (child != nullptr) {
          func(child);
          child = child->NextSiblingElement();
        }
      }

      template<typename Func>
      void parseManyElements(const char *name, Func func) const {
        const tinyxml2::XMLElement *child = m_elt->FirstChildElement(name);

        while (child != nullptr) {
          func(child);
          child = child->NextSiblingElement(name);
        }
      }

      template<typename Func>
      void parseOneElement(const char *name, Func func) const {
        const tinyxml2::XMLElement *child = m_elt->FirstChildElement(name);

        if (child == nullptr) {
          return;
        }

        func(child);
        child = child->NextSiblingElement(name);

        if (child != nullptr) {
          Log::error("Multiple chidren where a single child was expected for element: %s\n", name);
        }
      }

      bool hasAttribute(const char *name) const {
        return m_elt->Attribute(name) != nullptr;
      }

      unsigned getUIntAttribute(const char *name, Requirement req = Requirement::Mandatory, unsigned val = 0) const {
        int err = m_elt->QueryUnsignedAttribute(name, &val);
        return handleErrorAndReturn(name, val, err, req);
      }

      int getIntAttribute(const char *name, Requirement req = Requirement::Mandatory, int val = 0) const {
        int err = m_elt->QueryIntAttribute(name, &val);
        return handleErrorAndReturn(name, val, err, req);
      }

      double getDoubleAttribute(const char *name, Requirement req = Requirement::Mandatory, double val = 0.) const {
        int err = m_elt->QueryDoubleAttribute(name, &val);
        return handleErrorAndReturn(name, val, err, req);
      }

      bool getBoolAttribute(const char *name, Requirement req = Requirement::Mandatory, bool val = false) const {
        unsigned zeroOrOne = val ? 1 : 0;
        int err = m_elt->QueryUnsignedAttribute(name, &zeroOrOne);
        zeroOrOne = handleErrorAndReturn(name, zeroOrOne, err, req);
        assert(zeroOrOne == 0 || zeroOrOne == 1);
        return zeroOrOne == 1;
      }

      std::string getStringAttribute(const char *name, Requirement req = Requirement::Mandatory, const char *val = "") const {
        const char *attr = m_elt->Attribute(name);
        int err = tinyxml2::XML_SUCCESS;

        if (!attr) {
          attr = val;
          err = tinyxml2::XML_NO_ATTRIBUTE;
        }

        assert(attr);
        return handleErrorAndReturn(name, std::string(attr), err, req);
      }

      Color4u getColorAttribute(const char *name, Requirement req = Requirement::Mandatory, Color4u val = Color4u(0x00, 0x00, 0x00, 0xFF)) const {
        const char *attr = m_elt->Attribute(name);
        int err = tinyxml2::XML_SUCCESS;

        if (!attr) {
          err = tinyxml2::XML_NO_ATTRIBUTE;
          return handleErrorAndReturn(name, val, err, req);
        }

        assert(attr);

        auto size = std::strlen(attr);
        assert(size > 0);

        if (attr[0] == '#') {
          ++attr;
          --size;
        }

        Color4u color = val;

        switch (size) {
          case 6:
            color.a = 0xFF;
            color.r = (convertHexChar(attr[0]) << 4) + convertHexChar(attr[1]);
            color.g = (convertHexChar(attr[2]) << 4) + convertHexChar(attr[3]);
            color.b = (convertHexChar(attr[4]) << 4) + convertHexChar(attr[5]);
            break;

          case 8:
            color.a = (convertHexChar(attr[0]) << 4) + convertHexChar(attr[1]);
            color.r = (convertHexChar(attr[2]) << 4) + convertHexChar(attr[3]);
            color.g = (convertHexChar(attr[4]) << 4) + convertHexChar(attr[5]);
            color.b = (convertHexChar(attr[6]) << 4) + convertHexChar(attr[7]);
            break;

          default:
            Log::error("Unknown color format: %s\n", attr);
            break;
        }

        return color;
      }

      bool isEnumAttribute(const char *name, const char *value) const {
        return m_elt->Attribute(name, value) != nullptr;
      }

      std::string getText() const {
        const char *text = m_elt->GetText();
        return text ? text : "";
      }

    private:
      const tinyxml2::XMLElement * const m_elt;

      template<typename T>
      static T handleErrorAndReturn(const char *name, T val, int err, Requirement req) {
        if (err == tinyxml2::XML_SUCCESS) {
          return val;
        }

        if (err == tinyxml2::XML_NO_ATTRIBUTE) {
          if (req == Requirement::Mandatory) {
            Log::error("Mandatory attribute is missing: %s\n", name);
          }

          return val;
        }

        if (err == tinyxml2::XML_WRONG_ATTRIBUTE_TYPE) {
          Log::error("Wrong attribute type: %s\n", name);
          return val;
        }

        Log::error("Unknown error with attribute: %s\n", name);
        return val;
      }

      static uint8_t convertHexChar(char c) {
        if ('0' <= c && c <= '9') {
          return c - '0';
        }

        if ('a' <= c && c <= 'f') {
          return c - 'a' + 10;
        }

        if (!('A' <= c && c <= 'F')) {
          Log::error("Invalid character: '%c' (%x)\n", c, static_cast<int>(c));
        }

        assert('A' <= c && c <= 'F');
        return c - 'A' + 10;
      };

    };

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

    TmxFormat parseDataFormat(const XMLElementWrapper elt) {
      assert(elt.is("data"));

      if (elt.isEnumAttribute("encoding", "csv")) {
        return TmxFormat::Csv;
      }

      if (elt.isEnumAttribute("encoding", "base64")) {
        if (elt.isEnumAttribute("compression", "zlib")) {
          return TmxFormat::Base64_Zlib;
        }

        if (elt.isEnumAttribute("compression", "gzip")) {
          return TmxFormat::Base64_Gzip;
        }

        return TmxFormat::Base64;
      }

      return TmxFormat::Xml;
    }

    std::vector<uint8_t> parseDataBuffer(const XMLElementWrapper elt, TmxFormat format) {
      assert(elt.is("data"));

      std::vector<uint8_t> data;

      switch (format) {
        case TmxFormat::Xml:
        case TmxFormat::Csv:
          assert(false);
          break;

        case TmxFormat::Base64:
          data = parseDataBase64(elt.getText());
          break;

        case TmxFormat::Base64_Zlib:
        case TmxFormat::Base64_Gzip:
          data = parseDataCompressed(parseDataBase64(elt.getText()));
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
      if ((gid & FlippedDiagonallyFlag) != 0) {
        cell.flip.set(TmxFlip::Horizontally);
      }

      if ((gid & FlippedVerticallyFlag) != 0) {
        cell.flip.set(TmxFlip::Vertically);
      }

      if ((gid & FlippedDiagonallyFlag) != 0) {
        cell.flip.set(TmxFlip::Diagonally);
      }

      // Clear the flags
      cell.gid &= ~(FlippedHorizontallyFlag | FlippedVerticallyFlag | FlippedDiagonallyFlag);

      return cell;
    }

    TmxProperties parseTmxProperties(const XMLElementWrapper elt) {
      TmxProperties tmx;

      elt.parseOneElement("properties", [&tmx](const XMLElementWrapper elt) {
        elt.parseManyElements("property", [&tmx](const XMLElementWrapper elt) {

          std::string name = elt.getStringAttribute("name");
          assert(!name.empty());
          std::string value = elt.getStringAttribute("value");

          if (elt.hasAttribute("type")) {
            if (elt.isEnumAttribute("type", "string")) {
              tmx.addStringProperty(std::move(name), elt.getStringAttribute("value"));
            } else if (elt.isEnumAttribute("type", "int")) {
              tmx.addIntProperty(std::move(name), elt.getIntAttribute("value"));
            } else if (elt.isEnumAttribute("type", "float")) {
              tmx.addFloatProperty(std::move(name), elt.getDoubleAttribute("value"));
            } else if (elt.isEnumAttribute("type", "bool")) {
              std::string value = elt.getStringAttribute("value");
              assert(value == "true" || value == "false");
              tmx.addBoolProperty(std::move(name), value == "true");
            } else if (elt.isEnumAttribute("type", "color")) {
              tmx.addColorProperty(std::move(name), elt.getColorAttribute("value"));
            } else if (elt.isEnumAttribute("type", "file")) {
              tmx.addFileProperty(std::move(name), elt.getStringAttribute("value"));
            } else {
              Log::error("Wrong type string: '%s'\n", elt.getStringAttribute("type").c_str());
            }
          } else {
            tmx.addStringProperty(std::move(name), std::move(value));
          }
        });
      });

      return tmx;
    }


    void parseTmxLayer(const XMLElementWrapper elt, TmxLayer& tmx) {
      tmx.properties = parseTmxProperties(elt);

      tmx.name = elt.getStringAttribute("name");
      tmx.opacity = elt.getDoubleAttribute("opacity", Requirement::Optional, 1.0);
      tmx.visible = elt.getBoolAttribute("visible", Requirement::Optional, true);
      tmx.offset.x = elt.getIntAttribute("offsetx", Requirement::Optional, 0);
      tmx.offset.y = elt.getIntAttribute("offsety", Requirement::Optional, 0);
    }


    std::unique_ptr<TmxTileLayer> parseTmxTileLayer(const XMLElementWrapper elt) {
      assert(elt.is("layer"));

      auto tmx = makeUnique<TmxTileLayer>();
      parseTmxLayer(elt, *tmx);

      elt.parseOneElement("data", [&tmx](const XMLElementWrapper elt) {
        auto format = parseDataFormat(elt);

        switch (format) {
          case TmxFormat::Base64:
          case TmxFormat::Base64_Zlib:
          case TmxFormat::Base64_Gzip:
          {
            std::vector<uint8_t> data = parseDataBuffer(elt, format);

            const std::size_t size = data.size();
            assert(size % 4 == 0);

            for (std::size_t i = 0; i < size; i += 4) {
              unsigned gid = data[i] | (data[i + 1] << 8) | (data[i + 2] << 16) | (data[i + 3] << 24);
              tmx->cells.push_back(decodeGID(gid));
            }
            break;
          }

          case TmxFormat::Csv:
          {
            const std::string csv = elt.getText();
            std::vector<std::string> items;
            boost::algorithm::split(items, csv, boost::algorithm::is_any_of(","));

            for (auto item : items) {
              unsigned gid = std::stoul(item);
              tmx->cells.push_back(decodeGID(gid));
            }

            break;
          }

          case TmxFormat::Xml:
            elt.parseManyElements("tile", [&tmx](const XMLElementWrapper elt) {
              unsigned gid = elt.getUIntAttribute("gid");
              tmx->cells.push_back(decodeGID(gid));
            });

            break;
        }
      });

      return tmx;
    }

    std::unique_ptr<TmxImage> parseTmxImage(const XMLElementWrapper elt, const TmxParserCtx& ctx) {
      assert(elt.is("image"));

      auto tmx = makeUnique<TmxImage>();

      tmx->format = elt.getStringAttribute("format", Requirement::Optional);
      tmx->source = ctx.currentPath / elt.getStringAttribute("source");
      tmx->transparent = elt.getColorAttribute("trans", Requirement::Optional);
      tmx->size.width = elt.getUIntAttribute("width", Requirement::Optional);
      tmx->size.height = elt.getUIntAttribute("height", Requirement::Optional);

#if 0
      elt.parseOneElement("data", [](const XMLElementWrapper elt) {
        assert(false && "Not implemented"); // TODO
      });
#endif

      return tmx;
    }

    std::unique_ptr<TmxImageLayer> parseTmxImageLayer(const XMLElementWrapper elt, const TmxParserCtx& ctx) {
      assert(elt.is("imagelayer"));

      auto tmx = makeUnique<TmxImageLayer>();
      parseTmxLayer(elt, *tmx);

      elt.parseOneElement("image", [&tmx, &ctx](const XMLElementWrapper elt) {
        tmx->image = parseTmxImage(elt, ctx);
      });

      return tmx;
    }

    std::vector<Vector2i> parsePoints(const std::string& points) {
      std::vector<Vector2i> ret;

      std::vector<std::string> items;
      boost::algorithm::split(items, points, boost::algorithm::is_any_of(" "));

      for (auto item : items) {
        std::vector<std::string> coords;
        boost::algorithm::split(coords, item, boost::algorithm::is_any_of(","));

        assert(coords.size() == 2);
        int x = std::stoi(coords[0]);
        int y = std::stoi(coords[1]);
        ret.push_back({ x, y });
      }

      return ret;
    }

    void parseTmxObjectCommon(const XMLElementWrapper elt, TmxObject& tmx) {
      tmx.properties = parseTmxProperties(elt);

      tmx.id = elt.getUIntAttribute("id", Requirement::Optional);
      tmx.name = elt.getStringAttribute("name", Requirement::Optional);
      tmx.type = elt.getStringAttribute("type", Requirement::Optional);
      tmx.position.x = elt.getUIntAttribute("x");
      tmx.position.y = elt.getUIntAttribute("y");
      tmx.rotation = elt.getDoubleAttribute("rotation", Requirement::Optional);
      tmx.visible = elt.getBoolAttribute("visible", Requirement::Optional, true);
    }

    std::unique_ptr<TmxObject> parseTmxObject(const XMLElementWrapper elt) {
      if (elt.hasChild("polygon")) {
        auto tmx = makeUnique<TmxPolygon>();
        parseTmxObjectCommon(elt, *tmx);

        tmx->kind = TmxObject::Polygon;

        elt.parseOneElement("polygon", [&tmx](const XMLElementWrapper elt) {
          std::string points = elt.getStringAttribute("points");
          tmx->points = parsePoints(points);
        });

        return std::move(tmx);
      }

      if (elt.hasChild("polyline")) {
        auto tmx = makeUnique<TmxPolyline>();
        parseTmxObjectCommon(elt, *tmx);

        tmx->kind = TmxObject::Polyline;

        elt.parseOneElement("polyline", [&tmx](const XMLElementWrapper elt) {
          std::string points = elt.getStringAttribute("points");
          tmx->points = parsePoints(points);
        });

        return std::move(tmx);
      }

      if (elt.hasChild("text")) {
        auto tmx = makeUnique<TmxText>();
        parseTmxObjectCommon(elt, *tmx);

        tmx->kind = TmxObject::Text;

        elt.parseOneElement("text", [&tmx](const XMLElementWrapper elt) {
          tmx->fontFamily = elt.getStringAttribute("fontfamily", Requirement::Optional, "sans-serif");
          tmx->sizeInPixels = elt.getUIntAttribute("pixelsize", Requirement::Optional, 16);
          tmx->wrap = elt.getBoolAttribute("wrap", Requirement::Optional, false);
          tmx->color = elt.getColorAttribute("color", Requirement::Optional, { 0x00, 0x00, 0x00, 0xFF });
          tmx->bold = elt.getBoolAttribute("bold", Requirement::Optional, false);
          tmx->italic = elt.getBoolAttribute("italic", Requirement::Optional, false);
          tmx->underline = elt.getBoolAttribute("underline", Requirement::Optional, false);
          tmx->strikeout = elt.getBoolAttribute("strikeout", Requirement::Optional, false);
          tmx->kerning = elt.getBoolAttribute("kerning", Requirement::Optional, true);

          tmx->halign = TmxText::HAlign::Left;

          if (elt.hasAttribute("halign")) {
            if (elt.isEnumAttribute("halign", "left")) {
              tmx->halign = TmxText::HAlign::Left;
            } else if (elt.isEnumAttribute("halign", "center")) {
              tmx->halign = TmxText::HAlign::Center;
            } else if (elt.isEnumAttribute("halign", "right")) {
              tmx->halign = TmxText::HAlign::Right;
            } else {
              Log::error("Wrong halign string: '%s'\n", elt.getStringAttribute("halign").c_str());
            }
          }

          tmx->valign = TmxText::VAlign::Top;

          if (elt.hasAttribute("valign")) {
            if (elt.isEnumAttribute("valign", "left")) {
              tmx->valign = TmxText::VAlign::Top;
            } else if (elt.isEnumAttribute("valign", "center")) {
              tmx->valign = TmxText::VAlign::Center;
            } else if (elt.isEnumAttribute("valign", "right")) {
              tmx->valign = TmxText::VAlign::Bottom;
            } else {
              Log::error("Wrong valign string: '%s'\n", elt.getStringAttribute("valign").c_str());
            }
          }

          tmx->text = elt.getText();
        });

        return std::move(tmx);
      }

      if (elt.hasAttribute("gid")) {
        unsigned gid = elt.getUIntAttribute("gid");
        TmxCell cell = decodeGID(gid);

        auto tmx = makeUnique<TmxTileObject>();
        parseTmxObjectCommon(elt, *tmx);

        tmx->kind = TmxObject::Tile;
        tmx->gid = cell.gid;
        tmx->flip = cell.flip;

        return std::move(tmx);
      }

      if (elt.hasChild("ellipse")) {
        auto tmx = makeUnique<TmxEllipse>();
        parseTmxObjectCommon(elt, *tmx);

        tmx->kind = TmxObject::Ellipse;
        tmx->size.width = elt.getIntAttribute("width");
        tmx->size.height = elt.getIntAttribute("height");

        return std::move(tmx);
      }

      auto tmx = makeUnique<TmxRectangle>();
      parseTmxObjectCommon(elt, *tmx);

      tmx->kind = TmxObject::Rectangle;
      tmx->size.width = elt.getIntAttribute("width");
      tmx->size.height = elt.getIntAttribute("height");

      return std::move(tmx);
    }

    std::unique_ptr<TmxObjectLayer> parseTmxObjectLayer(const XMLElementWrapper elt) {
      assert(elt.is("objectgroup"));

      auto tmx = makeUnique<TmxObjectLayer>();
      parseTmxLayer(elt, *tmx);

      tmx->color = elt.getColorAttribute("color", Requirement::Optional);
      tmx->drawOrder = TmxDrawOrder::TopDown;

      if (elt.hasAttribute("draworder")) {
        if (elt.isEnumAttribute("draworder", "topdown")) {
          tmx->drawOrder = TmxDrawOrder::TopDown;
        } else if (elt.isEnumAttribute("draworder", "index")) {
          tmx->drawOrder = TmxDrawOrder::Index;
        } else {
          Log::error("Wrong draw order string: '%s'\n", elt.getStringAttribute("draworder").c_str());
        }
      }

      elt.parseManyElements("object", [&tmx](const XMLElementWrapper elt) {
        tmx->objects.push_back(parseTmxObject(elt));
      });

      return tmx;
    }

    std::unique_ptr<TmxGroupLayer> parseTmxGroupLayer(const XMLElementWrapper elt, const TmxParserCtx& ctx) {
      assert(elt.is("group"));

      auto tmx = makeUnique<TmxGroupLayer>();
      parseTmxLayer(elt, *tmx);

      elt.parseEachElement([&tmx, &ctx](const XMLElementWrapper elt) {
        if (elt.is("layer")) {
          tmx->layers.push_back(parseTmxTileLayer(elt));
        } else if (elt.is("objectgroup")) {
          tmx->layers.push_back(parseTmxObjectLayer(elt));
        } else if (elt.is("imagelayer")) {
          tmx->layers.push_back(parseTmxImageLayer(elt, ctx));
        } else if (elt.is("group")) {
          tmx->layers.push_back(parseTmxGroupLayer(elt, ctx));
        }
      });

      return tmx;
    }

    /*
     * Tilesets
     */

    TmxFrame parseTmxFrame(const XMLElementWrapper elt) {
      assert(elt.is("frame"));

      TmxFrame tmx;

      tmx.tileId = elt.getUIntAttribute("tileId");
      tmx.duration = gf::milliseconds(elt.getUIntAttribute("duration"));

      return tmx;
    }

    std::unique_ptr<TmxAnimation> parseTmxAnimation(const XMLElementWrapper elt) {
      assert(elt.is("animation"));

      auto tmx = makeUnique<TmxAnimation>();

      elt.parseManyElements("frame", [&tmx](const XMLElementWrapper elt) {
        tmx->frames.push_back(parseTmxFrame(elt));
      });

      return tmx;
    }

    TmxTile parseTmxTile(const XMLElementWrapper elt, TmxParserCtx& ctx) {
      assert(elt.is("tile"));

      TmxTile tmx;

      tmx.properties = parseTmxProperties(elt);
      tmx.id = elt.getUIntAttribute("id");
      tmx.type = elt.getStringAttribute("type", Requirement::Optional);

      static constexpr unsigned Invalid = static_cast<unsigned>(-1);

      tmx.terrain = { { Invalid, Invalid, Invalid, Invalid } };

      std::string terrain = elt.getStringAttribute("terrain", Requirement::Optional);

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

      tmx.probability = elt.getUIntAttribute("probability", Requirement::Optional, 100u);

      tmx.image = nullptr;
      elt.parseOneElement("image", [&tmx, &ctx](const XMLElementWrapper elt) {
        tmx.image = parseTmxImage(elt, ctx);
      });

      tmx.objects = nullptr;

      elt.parseOneElement("objectgroup", [&tmx](const XMLElementWrapper elt) {
        tmx.objects = parseTmxObjectLayer(elt);
      });

      tmx.animation = nullptr;

      elt.parseOneElement("animation", [&tmx](const XMLElementWrapper elt) {
        tmx.animation = parseTmxAnimation(elt);
      });

      return tmx;
    }

    TmxTerrain parseTmxTerrain(const XMLElementWrapper elt) {
      assert(elt.is("terrain"));

      TmxTerrain tmx;

      tmx.properties = parseTmxProperties(elt);

      tmx.name = elt.getStringAttribute("name");
      tmx.tile = elt.getUIntAttribute("tile");

      return tmx;
    }

    void parseTmxTilesetFromElement(const XMLElementWrapper elt, TmxTileset& tmx, TmxParserCtx& ctx) {
      assert(elt.is("tileset"));

      tmx.name = elt.getStringAttribute("name", Requirement::Optional);

      tmx.tileSize.width = elt.getUIntAttribute("tilewidth", Requirement::Optional);
      tmx.tileSize.height = elt.getUIntAttribute("tileheight", Requirement::Optional);

      tmx.spacing = elt.getUIntAttribute("spacing", Requirement::Optional);
      tmx.margin = elt.getUIntAttribute("margin", Requirement::Optional);

      tmx.tileCount = elt.getUIntAttribute("tilecount", Requirement::Optional);
      tmx.columnCount = elt.getUIntAttribute("columns", Requirement::Optional);

      tmx.offset = { 0, 0 };
      elt.parseOneElement("tileoffset", [&tmx](const XMLElementWrapper elt) {
        tmx.offset.x = elt.getIntAttribute("x");
        tmx.offset.y = elt.getIntAttribute("y");
      });

      tmx.image = nullptr;
      elt.parseOneElement("image", [&tmx, &ctx](const XMLElementWrapper elt) {
        tmx.image = parseTmxImage(elt, ctx);
      });

      elt.parseOneElement("terraintypes", [&tmx](const XMLElementWrapper elt) {
        elt.parseManyElements("terrain", [&tmx](const XMLElementWrapper elt) {
          tmx.terrains.push_back(parseTmxTerrain(elt));
        });
      });


      elt.parseManyElements("tile", [&tmx, &ctx](const XMLElementWrapper elt) {
        tmx.tiles.push_back(parseTmxTile(elt, ctx));
      });
    }

    void parseTmxTilesetFromFile(const Path& source, TmxTileset& tmx, TmxParserCtx& ctx) {
      Path tilesetPath = ctx.currentPath / source;

      tinyxml2::XMLDocument doc;
      int err = doc.LoadFile(tilesetPath.string().c_str());

      if (doc.Error()) {
        Log::error("Unable to load a TSX file: '%s'\n", tilesetPath.string().c_str());;
        assert(err != tinyxml2::XML_SUCCESS);
        return;
      }

      assert(err == tinyxml2::XML_SUCCESS);

      ctx.currentPath = tilesetPath.parent_path();

      const tinyxml2::XMLElement *elt = doc.RootElement();

      if (elt->Attribute("firstgid")) {
        Log::warning("Attribute 'firstgid' present in a TSX file: '%s'\n", tilesetPath.string().c_str());
      }

      if (elt->Attribute("source")) {
        Log::warning("Attribute 'source' present in a TSX file: '%s'\n", tilesetPath.string().c_str());
      }

      parseTmxTilesetFromElement(elt, tmx, ctx);
      ctx.currentPath = ctx.mapPath.parent_path();
    }

    TmxTileset parseTmxTileset(const XMLElementWrapper elt, TmxParserCtx& ctx) {
      TmxTileset tmx;

      tmx.properties = parseTmxProperties(elt);

      tmx.firstGid = elt.getUIntAttribute("firstgid");

      Path source = elt.getStringAttribute("source", Requirement::Optional);

      if (!source.empty()) {
        parseTmxTilesetFromFile(source, tmx, ctx);
      } else {
        parseTmxTilesetFromElement(elt, tmx, ctx);
      }

      return tmx;
    }

    /*
     * Map
     */

    bool parseTmxLayers(const XMLElementWrapper elt, TmxLayers& tmx, TmxParserCtx& ctx) {
      assert(elt.is("map"));

      tmx.properties = parseTmxProperties(elt);

      tmx.version =  elt.getStringAttribute("version", Requirement::Optional, "1.0");
      tmx.tiledVersion = elt.getStringAttribute("tiledversion", Requirement::Optional, "1.0");

      tmx.orientation = TmxOrientation::Unknown;

      if (elt.isEnumAttribute("orientation", "orthogonal")) {
        tmx.orientation = TmxOrientation::Orthogonal;
      } else if (elt.isEnumAttribute("orientation", "isometric")) {
        tmx.orientation = TmxOrientation::Isometric;
      } else if (elt.isEnumAttribute("orientation", "staggered")) {
        tmx.orientation = TmxOrientation::Staggered;
      } else if (elt.isEnumAttribute("orientation", "hexagonal")) {
        tmx.orientation = TmxOrientation::Hexagonal;
      } else {
        Log::error("Wrong orientation string: '%s'\n", elt.getStringAttribute("orientation").c_str());
      }

      tmx.renderOrder = TmxRenderOrder::RightDown; // default value

      if (elt.hasAttribute("renderorder")) {
        if (elt.isEnumAttribute("renderorder", "right-down")) {
          tmx.renderOrder = TmxRenderOrder::RightDown;
        } else if (elt.isEnumAttribute("renderorder", "right-up")) {
          tmx.renderOrder = TmxRenderOrder::RightUp;
        } else if (elt.isEnumAttribute("renderorder", "left-down")) {
          tmx.renderOrder = TmxRenderOrder::LeftDown;
        } else if (elt.isEnumAttribute("renderorder", "left-up")) {
          tmx.renderOrder = TmxRenderOrder::LeftUp;
        } else {
          Log::error("Wrong render order string: '%s'\n", elt.getStringAttribute("renderorder").c_str());
        }
      }

      tmx.mapSize.width = elt.getUIntAttribute("width");
      tmx.mapSize.height = elt.getUIntAttribute("height");

      tmx.tileSize.width = elt.getUIntAttribute("tilewidth");
      tmx.tileSize.height = elt.getUIntAttribute("tileheight");

      tmx.hexSideLength = elt.getUIntAttribute("hexsidelength", Requirement::Optional);

      tmx.staggerAxis = TmxStaggerAxis::Y;

      if (elt.hasAttribute("staggeraxis")) {
        if (elt.isEnumAttribute("staggeraxis", "x")) {
          tmx.staggerAxis = TmxStaggerAxis::X;
        } else if (elt.isEnumAttribute("staggeraxis", "y")) {
          tmx.staggerAxis = TmxStaggerAxis::Y;
        } else {
          Log::error("Wrong stagger axis string: '%s'\n", elt.getStringAttribute("staggeraxis").c_str());
        }
      }

      tmx.staggerIndex = TmxStaggerIndex::Odd;

      if (elt.hasAttribute("staggerindex")) {
        if (elt.isEnumAttribute("staggerindex", "odd")) {
          tmx.staggerIndex = TmxStaggerIndex::Odd;
        } else if (elt.isEnumAttribute("staggerindex", "even")) {
          tmx.staggerIndex = TmxStaggerIndex::Even;
        } else {
          Log::error("Wrong stagger index string: '%s'\n", elt.getStringAttribute("staggerindex").c_str());
        }
      }

      tmx.backgroundColor = elt.getColorAttribute("backgroundcolor", Requirement::Optional, Color4u(0xFF, 0xFF, 0xFF, 0xFF));

      tmx.nextObjectId = elt.getUIntAttribute("nextobjectid", Requirement::Optional);

      elt.parseManyElements("tileset", [&tmx, &ctx](const XMLElementWrapper elt) {
        tmx.tilesets.push_back(parseTmxTileset(elt, ctx));
      });

      elt.parseEachElement([&tmx, &ctx](const XMLElementWrapper elt) {
        if (elt.is("layer")) {
          tmx.layers.push_back(parseTmxTileLayer(elt));
        } else if (elt.is("objectgroup")) {
          tmx.layers.push_back(parseTmxObjectLayer(elt));
        } else if (elt.is("imagelayer")) {
          tmx.layers.push_back(parseTmxImageLayer(elt, ctx));
        } else if (elt.is("group")) {
          tmx.layers.push_back(parseTmxGroupLayer(elt, ctx));
        }
      });

      return true;
    }

  }

  bool TmxLayers::loadFromFile(const Path& filename) {
    if (!fs::is_regular_file(filename)) {
      Log::error("Unknown TMX file: '%s'\n", filename.string().c_str());
      return false;
    }

    tinyxml2::XMLDocument doc;
    int err = doc.LoadFile(filename.string().c_str());

    if (doc.Error() || err != tinyxml2::XML_SUCCESS) {
      Log::error("Could not load TMX file: '%s'\n", filename.string().c_str());
      return false;
    }

    assert(err == tinyxml2::XML_SUCCESS);

    TmxParserCtx ctx;
    ctx.mapPath = filename;
    ctx.currentPath = filename.parent_path();

    return parseTmxLayers(doc.RootElement(), *this, ctx);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
