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
#include <gf/ConsoleFont.h>

#include <cassert>
#include <cinttypes>

#include <gf/Image.h>
#include <gf/Log.h>
#include <gf/StringUtils.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  constexpr ConsoleFontFormat PredefinedConsoleFontFormat::Libtcod;
  constexpr ConsoleFontFormat PredefinedConsoleFontFormat::DwarfFortress;

  /*
   * ConsoleFont
   */

  static std::size_t MappingSize = 0x10000;

  ConsoleFont::ConsoleFont()
  : m_mapping(MappingSize, 0x00)
  , m_format({ ConsoleFontFormat::Alpha, ConsoleFontFormat::InRow, ConsoleFontFormat::Custom })
  , m_size(0u, 0u)
  , m_characterSize(0u, 0u)
  {

  }

  ConsoleFont::~ConsoleFont() {

  }

  static unsigned computeIndex(Vector2u position, ConsoleFontFormat::Layout layout, Vector2u size) {
    switch (layout) {
      case ConsoleFontFormat::InColumn:
        return position.x * size.height + position.y;
      case ConsoleFontFormat::InRow:
        return position.y * size.width + position.x;
    }

    assert(false);
    return 0;
  }

  void ConsoleFont::mapCode(char16_t c, Vector2u position) {
    unsigned index = computeIndex(position, m_format.layout, m_size);
    assert(index < 0x100);
    m_mapping[c] = index;
  }

  void ConsoleFont::mapCodeRange(char16_t c, unsigned count, Vector2u position) {
    unsigned index = computeIndex(position, m_format.layout, m_size);

    for (unsigned i = 0; i < count; ++i) {
      assert(index < 0x100);
      m_mapping[c++] = index++;
    }
  }

  void ConsoleFont::mapString(StringRef str, Vector2u position) {
    unsigned index = computeIndex(position, m_format.layout, m_size);

    auto unicodeString = computeUnicodeString(str);

    for (auto codepoint : unicodeString) {
      if (codepoint >= 0x10000) {
        std::uint_least32_t value = static_cast<std::uint_least32_t>(codepoint);
        Log::error("Can not map a codepoint outside the Basic Multilingual Plane (BMP): U+%" PRIXLEAST32, value);
      } else {
        char16_t c = static_cast<char16_t>(codepoint);
        assert(index < 0x100);
        m_mapping[c] = index;
      }

      index++;
    }
  }

  void ConsoleFont::mapElement(ConsoleFontElement element) {
    m_mapping[element.character] = element.index;
  }

  void ConsoleFont::mapElements(ArrayRef<ConsoleFontElement> elements) {
    for (auto element : elements) {
      m_mapping[element.character] = element.index;
    }
  }

  void ConsoleFont::clearMapping() {
    std::fill(m_mapping.begin(), m_mapping.end(), 0x00);
    assert(m_mapping.size() == MappingSize);
  }

  static Vector2u computePosition(uint8_t index, ConsoleFontFormat::Layout layout, Vector2u size) {
    switch (layout) {
      case ConsoleFontFormat::InColumn:
        return { index / size.height, index % size.height };
      case ConsoleFontFormat::InRow:
        return { index % size.width, index / size.width };
    }

    assert(false);
    return { 0u, 0u };
  }

  RectU ConsoleFont::getSubTexture(char16_t c) const {
    uint8_t index = m_mapping[c];
    Vector2u position = computePosition(index, m_format.layout, m_size);
    return RectU(position * m_characterSize, m_characterSize);
  }

  RectF ConsoleFont::getTextureRect(char16_t c) const {
    return getTexture()->computeTextureCoords(getSubTexture(c));
  }

  static constexpr ConsoleFontElement CodePage437Mapping[] = {
    // regular mapping

    { u'\u0000', 0x00 }, { u'\u263A', 0x01 }, { u'\u263B', 0x02 }, { u'\u2665', 0x03 },
    { u'\u2666', 0x04 }, { u'\u2663', 0x05 }, { u'\u2660', 0x06 }, { u'\u2022', 0x07 },
    { u'\u25D8', 0x08 }, { u'\u25CB', 0x09 }, { u'\u25D9', 0x0A }, { u'\u2642', 0x0B },
    { u'\u2640', 0x0C }, { u'\u266A', 0x0D }, { u'\u266B', 0x0E }, { u'\u263C', 0x0F },
    { u'\u25BA', 0x10 }, { u'\u25C4', 0x11 }, { u'\u2195', 0x12 }, { u'\u203C', 0x13 },
    { u'\u00B6', 0x14 }, { u'\u00A7', 0x15 }, { u'\u25AC', 0x16 }, { u'\u21A8', 0x17 },
    { u'\u2191', 0x18 }, { u'\u2193', 0x19 }, { u'\u2192', 0x1A }, { u'\u2190', 0x1B },
    { u'\u221F', 0x1C }, { u'\u2194', 0x1D }, { u'\u25B2', 0x1E }, { u'\u25BC', 0x1F },
    { u'\u0020', 0x20 }, { u'\u0021', 0x21 }, { u'\u0022', 0x22 }, { u'\u0023', 0x23 },
    { u'\u0024', 0x24 }, { u'\u0025', 0x25 }, { u'\u0026', 0x26 }, { u'\u0027', 0x27 },
    { u'\u0028', 0x28 }, { u'\u0029', 0x29 }, { u'\u002A', 0x2A }, { u'\u002B', 0x2B },
    { u'\u002C', 0x2C }, { u'\u002D', 0x2D }, { u'\u002E', 0x2E }, { u'\u002F', 0x2F },
    { u'\u0030', 0x30 }, { u'\u0031', 0x31 }, { u'\u0032', 0x32 }, { u'\u0033', 0x33 },
    { u'\u0034', 0x34 }, { u'\u0035', 0x35 }, { u'\u0036', 0x36 }, { u'\u0037', 0x37 },
    { u'\u0038', 0x38 }, { u'\u0039', 0x39 }, { u'\u003A', 0x3A }, { u'\u003B', 0x3B },
    { u'\u003C', 0x3C }, { u'\u003D', 0x3D }, { u'\u003E', 0x3E }, { u'\u003F', 0x3F },
    { u'\u0040', 0x40 }, { u'\u0041', 0x41 }, { u'\u0042', 0x42 }, { u'\u0043', 0x43 },
    { u'\u0044', 0x44 }, { u'\u0045', 0x45 }, { u'\u0046', 0x46 }, { u'\u0047', 0x47 },
    { u'\u0048', 0x48 }, { u'\u0049', 0x49 }, { u'\u004A', 0x4A }, { u'\u004B', 0x4B },
    { u'\u004C', 0x4C }, { u'\u004D', 0x4D }, { u'\u004E', 0x4E }, { u'\u004F', 0x4F },
    { u'\u0050', 0x50 }, { u'\u0051', 0x51 }, { u'\u0052', 0x52 }, { u'\u0053', 0x53 },
    { u'\u0054', 0x54 }, { u'\u0055', 0x55 }, { u'\u0056', 0x56 }, { u'\u0057', 0x57 },
    { u'\u0058', 0x58 }, { u'\u0059', 0x59 }, { u'\u005A', 0x5A }, { u'\u005B', 0x5B },
    { u'\u005C', 0x5C }, { u'\u005D', 0x5D }, { u'\u005E', 0x5E }, { u'\u005F', 0x5F },
    { u'\u0060', 0x60 }, { u'\u0061', 0x61 }, { u'\u0062', 0x62 }, { u'\u0063', 0x63 },
    { u'\u0064', 0x64 }, { u'\u0065', 0x65 }, { u'\u0066', 0x66 }, { u'\u0067', 0x67 },
    { u'\u0068', 0x68 }, { u'\u0069', 0x69 }, { u'\u006A', 0x6A }, { u'\u006B', 0x6B },
    { u'\u006C', 0x6C }, { u'\u006D', 0x6D }, { u'\u006E', 0x6E }, { u'\u006F', 0x6F },
    { u'\u0070', 0x70 }, { u'\u0071', 0x71 }, { u'\u0072', 0x72 }, { u'\u0073', 0x73 },
    { u'\u0074', 0x74 }, { u'\u0075', 0x75 }, { u'\u0076', 0x76 }, { u'\u0077', 0x77 },
    { u'\u0078', 0x78 }, { u'\u0079', 0x79 }, { u'\u007A', 0x7A }, { u'\u007B', 0x7B },
    { u'\u007C', 0x7C }, { u'\u007D', 0x7D }, { u'\u007E', 0x7E }, { u'\u2302', 0x7F },
    { u'\u00C7', 0x80 }, { u'\u00FC', 0x81 }, { u'\u00E9', 0x82 }, { u'\u00E2', 0x83 },
    { u'\u00E4', 0x84 }, { u'\u00E0', 0x85 }, { u'\u00E5', 0x86 }, { u'\u00E7', 0x87 },
    { u'\u00EA', 0x88 }, { u'\u00EB', 0x89 }, { u'\u00E8', 0x8A }, { u'\u00EF', 0x8B },
    { u'\u00EE', 0x8C }, { u'\u00EC', 0x8D }, { u'\u00C4', 0x8E }, { u'\u00C5', 0x8F },
    { u'\u00C9', 0x90 }, { u'\u00E6', 0x91 }, { u'\u00C6', 0x92 }, { u'\u00F4', 0x93 },
    { u'\u00F6', 0x94 }, { u'\u00F2', 0x95 }, { u'\u00FB', 0x96 }, { u'\u00F9', 0x97 },
    { u'\u00FF', 0x98 }, { u'\u00D6', 0x99 }, { u'\u00DC', 0x9A }, { u'\u00A2', 0x9B },
    { u'\u00A3', 0x9C }, { u'\u00A5', 0x9D }, { u'\u20A7', 0x9E }, { u'\u0192', 0x9F },
    { u'\u00E1', 0xA0 }, { u'\u00ED', 0xA1 }, { u'\u00F3', 0xA2 }, { u'\u00FA', 0xA3 },
    { u'\u00F1', 0xA4 }, { u'\u00D1', 0xA5 }, { u'\u00AA', 0xA6 }, { u'\u00BA', 0xA7 },
    { u'\u00BF', 0xA8 }, { u'\u2310', 0xA9 }, { u'\u00AC', 0xAA }, { u'\u00BD', 0xAB },
    { u'\u00BC', 0xAC }, { u'\u00A1', 0xAD }, { u'\u00AB', 0xAE }, { u'\u00BB', 0xAF },
    { u'\u2591', 0xB0 }, { u'\u2592', 0xB1 }, { u'\u2593', 0xB2 }, { u'\u2502', 0xB3 },
    { u'\u2524', 0xB4 }, { u'\u2561', 0xB5 }, { u'\u2562', 0xB6 }, { u'\u2556', 0xB7 },
    { u'\u2555', 0xB8 }, { u'\u2563', 0xB9 }, { u'\u2551', 0xBA }, { u'\u2557', 0xBB },
    { u'\u255D', 0xBC }, { u'\u255C', 0xBD }, { u'\u255B', 0xBE }, { u'\u2510', 0xBF },
    { u'\u2514', 0xC0 }, { u'\u2534', 0xC1 }, { u'\u252C', 0xC2 }, { u'\u251C', 0xC3 },
    { u'\u2500', 0xC4 }, { u'\u253C', 0xC5 }, { u'\u255E', 0xC6 }, { u'\u255F', 0xC7 },
    { u'\u255A', 0xC8 }, { u'\u2554', 0xC9 }, { u'\u2569', 0xCA }, { u'\u2566', 0xCB },
    { u'\u2560', 0xCC }, { u'\u2550', 0xCD }, { u'\u256C', 0xCE }, { u'\u2567', 0xCF },
    { u'\u2568', 0xD0 }, { u'\u2564', 0xD1 }, { u'\u2565', 0xD2 }, { u'\u2559', 0xD3 },
    { u'\u2558', 0xD4 }, { u'\u2552', 0xD5 }, { u'\u2553', 0xD6 }, { u'\u256B', 0xD7 },
    { u'\u256A', 0xD8 }, { u'\u2518', 0xD9 }, { u'\u250C', 0xDA }, { u'\u2588', 0xDB },
    { u'\u2584', 0xDC }, { u'\u258C', 0xDD }, { u'\u2590', 0xDE }, { u'\u2580', 0xDF },
    { u'\u03B1', 0xE0 }, { u'\u00DF', 0xE1 }, { u'\u0393', 0xE2 }, { u'\u03C0', 0xE3 },
    { u'\u03A3', 0xE4 }, { u'\u03C3', 0xE5 }, { u'\u00B5', 0xE6 }, { u'\u03C4', 0xE7 },
    { u'\u03A6', 0xE8 }, { u'\u0398', 0xE9 }, { u'\u03A9', 0xEA }, { u'\u03B4', 0xEB },
    { u'\u221E', 0xEC }, { u'\u03C6', 0xED }, { u'\u03B5', 0xEE }, { u'\u2229', 0xEF },
    { u'\u2261', 0xF0 }, { u'\u00B1', 0xF1 }, { u'\u2265', 0xF2 }, { u'\u2264', 0xF3 },
    { u'\u2320', 0xF4 }, { u'\u2321', 0xF5 }, { u'\u00F7', 0xF6 }, { u'\u2248', 0xF7 },
    { u'\u00B0', 0xF8 }, { u'\u2219', 0xF9 }, { u'\u00B7', 0xFA }, { u'\u221A', 0xFB },
    { u'\u207F', 0xFC }, { u'\u00B2', 0xFD }, { u'\u25A0', 0xFE }, { u'\u00A0', 0xFF },

    // alternative mapping for some codepoint

    { u'\u03B2', 0xE1 }, { u'\u03A0', 0xE3 }, { u'\u220F', 0xE3 }, { u'\u2211', 0xE4 },
    { u'\u03BC', 0xE6 }, { u'\u2126', 0xEA }, { u'\u03D5', 0xED }, { u'\u2205', 0xED },
    { u'\u2300', 0xED }, { u'\u00D8', 0xED }, { u'\u00F8', 0xED }, { u'\u2208', 0xEE },

  };

  static constexpr ConsoleFontElement ModifiedCodePage437Mapping[] = {
    // remove some mapping in 0xE2-0xE8
    { u'\u0393', 0x00 }, { u'\u03C0', 0x00 }, { u'\u03A3', 0x00 }, { u'\u03C3', 0x00 },
    { u'\u00B5', 0x00 }, { u'\u03C4', 0x00 }, { u'\u03A6', 0x00 }, { u'\u03A0', 0x00 },
    { u'\u220F', 0x00 }, { u'\u2211', 0x00 }, { u'\u03BC', 0x00 },

    //add new mapping
    { u'\u2598', 0xE2 }, { u'\u259D', 0xE3 }, { u'\u2580', 0xE4 }, { u'\u2597', 0xE5 },
    { u'\u259A', 0xE6 }, { u'\u2590', 0xE7 }, { u'\u2596', 0xE8 },
  };

  static constexpr ConsoleFontElement SpecialMapping[] = {
    // first line
    { u'\u0020', 0x00 }, { u'\u0021', 0x01 }, { u'\u0022', 0x02 }, { u'\u0023', 0x03 },
    { u'\u0024', 0x04 }, { u'\u0025', 0x05 }, { u'\u0026', 0x06 }, { u'\u0027', 0x07 },
    { u'\u0028', 0x08 }, { u'\u0029', 0x09 }, { u'\u002A', 0x0A }, { u'\u002B', 0x0B },
    { u'\u002C', 0x0C }, { u'\u002D', 0x0D }, { u'\u002E', 0x0E }, { u'\u002F', 0x0F },
    { u'\u0030', 0x10 }, { u'\u0031', 0x11 }, { u'\u0032', 0x12 }, { u'\u0033', 0x13 },
    { u'\u0034', 0x14 }, { u'\u0035', 0x15 }, { u'\u0036', 0x16 }, { u'\u0037', 0x17 },
    { u'\u0038', 0x18 }, { u'\u0039', 0x19 }, { u'\u003A', 0x1A }, { u'\u003B', 0x1B },
    { u'\u003C', 0x1C }, { u'\u003D', 0x1D }, { u'\u003E', 0x1E }, { u'\u003F', 0x1F },

    // second line
    { u'\u0040', 0x20 }, { u'\u005B', 0x21 }, { u'\u005C', 0x22 }, { u'\u005D', 0x23 },
    { u'\u005E', 0x24 }, { u'\u005F', 0x25 }, { u'\u0060', 0x26 }, { u'\u007B', 0x27 },
    { u'\u007C', 0x28 }, { u'\u007D', 0x29 }, { u'\u007E', 0x2A }, { u'\u2591', 0x2B },
    { u'\u2592', 0x2C }, { u'\u2593', 0x2D }, { u'\u2502', 0x2E }, { u'\u2500', 0x2F },
    { u'\u253C', 0x30 }, { u'\u2524', 0x31 }, { u'\u2534', 0x32 }, { u'\u251C', 0x33 },
    { u'\u252C', 0x34 }, { u'\u2514', 0x35 }, { u'\u250C', 0x36 }, { u'\u2510', 0x37 },
    { u'\u2518', 0x38 }, { u'\u2598', 0x39 }, { u'\u259D', 0x3A }, { u'\u2580', 0x3B },
    { u'\u2597', 0x3C }, { u'\u259A', 0x3D }, { u'\u2590', 0x3E }, { u'\u2596', 0x3F },

    //third line
    { u'\u2191', 0x40 }, { u'\u2193', 0x41 }, { u'\u2190', 0x42 }, { u'\u2192', 0x43 },
    { u'\u25B2', 0x44 }, { u'\u25BC', 0x45 }, { u'\u25C4', 0x46 }, { u'\u25BA', 0x47 },
    { u'\u2195', 0x48 }, { u'\u2194', 0x49 }, { u'\u2610', 0x4A }, { u'\u2611', 0x4B },
    { u'\u26AA', 0x4C }, { u'\u26AB', 0x4D }, { u'\u2551', 0x4E }, { u'\u2550', 0x4F },
    { u'\u256C', 0x50 }, { u'\u2563', 0x51 }, { u'\u2569', 0x52 }, { u'\u2560', 0x53 },
    { u'\u2565', 0x54 }, { u'\u255A', 0x55 }, { u'\u2554', 0x56 }, { u'\u2557', 0x57 },
    { u'\u255D', 0x58 },

    // fourth line
    { u'\u0041', 0x60 }, { u'\u0042', 0x61 }, { u'\u0043', 0x62 }, { u'\u0044', 0x63 },
    { u'\u0045', 0x64 }, { u'\u0046', 0x65 }, { u'\u0047', 0x66 }, { u'\u0048', 0x67 },
    { u'\u0049', 0x68 }, { u'\u004A', 0x69 }, { u'\u004B', 0x6A }, { u'\u004C', 0x6B },
    { u'\u004D', 0x6C }, { u'\u004E', 0x6D }, { u'\u004F', 0x6E }, { u'\u0050', 0x6F },
    { u'\u0051', 0x70 }, { u'\u0052', 0x71 }, { u'\u0053', 0x72 }, { u'\u0054', 0x73 },
    { u'\u0055', 0x74 }, { u'\u0056', 0x75 }, { u'\u0057', 0x76 }, { u'\u0058', 0x77 },
    { u'\u0059', 0x78 }, { u'\u005A', 0x79 },

    // fifth line
    { u'\u0061', 0x80 }, { u'\u0062', 0x81 }, { u'\u0063', 0x82 }, { u'\u0064', 0x83 },
    { u'\u0065', 0x84 }, { u'\u0066', 0x85 }, { u'\u0067', 0x86 }, { u'\u0068', 0x87 },
    { u'\u0069', 0x88 }, { u'\u006A', 0x89 }, { u'\u006B', 0x8A }, { u'\u006C', 0x8B },
    { u'\u006D', 0x8C }, { u'\u006E', 0x8D }, { u'\u006F', 0x8E }, { u'\u0070', 0x8F },
    { u'\u0071', 0x90 }, { u'\u0072', 0x91 }, { u'\u0073', 0x92 }, { u'\u0074', 0x93 },
    { u'\u0075', 0x94 }, { u'\u0076', 0x95 }, { u'\u0077', 0x96 }, { u'\u0078', 0x97 },
    { u'\u0079', 0x98 }, { u'\u007A', 0x99 },

  };

  bool ConsoleFont::setFormatAndComputeSizes(ConsoleFontFormat format, Vector2u size, Vector2u imageSize) {
    m_format = format;

    clearMapping();

    switch (format.mapping) {
      case ConsoleFontFormat::CodePage437:
        mapElements(CodePage437Mapping);
        break;
      case ConsoleFontFormat::ModifiedCodePage437:
        mapElements(CodePage437Mapping);
        mapElements(ModifiedCodePage437Mapping);
        break;
      case ConsoleFontFormat::Special:
        mapElements(SpecialMapping);
        break;
      case ConsoleFontFormat::Custom:
        // nothing to map
        break;
    }

    if (size != Vector2u(0u, 0u)) {
      m_size = size;
    } else {
      switch (format.mapping) {
        case ConsoleFontFormat::CodePage437:
        case ConsoleFontFormat::ModifiedCodePage437:
          m_size = { 16u, 16u };
          break;

        case ConsoleFontFormat::Special:
          m_size = { 32u, 8u };
          break;

        case ConsoleFontFormat::Custom:
          Log::error("Undefined size for a custom font\n");
          m_size = { 0u, 0u };
          return false;
      }
    }

    if (imageSize.width % m_size.width != 0 || imageSize.height % m_size.height != 0) {
      Log::error("Image size (%ux%u) is not a multiple of font layout (%ux%u)\n", imageSize.width, imageSize.height, m_size.width, m_size.height);
      return false;
    }

    m_characterSize = imageSize / m_size;

    return true;
  }

  static const char *getTransparencyString(ConsoleFontFormat::Transparency transparency) {
    switch (transparency) {
      case ConsoleFontFormat::Alpha:
        return "alpha";
      case ConsoleFontFormat::Grayscale:
        return "grayscale";
      case ConsoleFontFormat::ColorKey:
        return "color key";
    }

    assert(false);
    return "?";
  }

  static const char *getLayoutString(ConsoleFontFormat::Layout layout) {
    switch (layout) {
      case ConsoleFontFormat::InRow:
        return "in row";
      case ConsoleFontFormat::InColumn:
        return "in column";
    }

    assert(false);
    return "?";
  }

  static const char *getMappingString(ConsoleFontFormat::Mapping mapping) {
    switch (mapping) {
      case ConsoleFontFormat::CodePage437:
        return "code page 437";
      case ConsoleFontFormat::ModifiedCodePage437:
        return "modified code page 437";
      case ConsoleFontFormat::Special:
        return "special";
      case ConsoleFontFormat::Custom:
        return "custom";
    }

    assert(false);
    return "?";
  }

  void ConsoleFont::logFormat(const Path& filename) const {
    Log::info("Console font '%s': %ux%u with characters %ux%u, %s, %s, %s\n", filename.string().c_str(),
      m_size.width, m_size.height, m_characterSize.width, m_characterSize.height,
      getTransparencyString(m_format.transparency), getLayoutString(m_format.layout), getMappingString(m_format.mapping)
    );
  }

  Vector2u ConsoleFont::getColorKeyPosition() const {
    switch (m_format.mapping) {
      case ConsoleFontFormat::CodePage437:
      case ConsoleFontFormat::ModifiedCodePage437:
        // space character at index 0x20
        return computePosition(0x20, m_format.layout, m_size) * m_characterSize;
      case ConsoleFontFormat::Special:
        // space character at index 0
        return { 0u, 0u };
      case ConsoleFontFormat::Custom:
        // assume the top left pixel is the color key
        return { 0u, 0u };
    }

    assert(false);
    return { 0u, 0u };
  }

  /*
   * BitmapConsoleFont
   */

  const BareTexture *BitmapConsoleFont::getTexture() const {
    return &m_texture;
  }

  bool BitmapConsoleFont::loadFromFile(const Path& filename, ConsoleFontFormat format, Vector2u size) {
    Image image;

    if (!image.loadFromFile(filename)) {
      return false;
    }

    Vector2u imageSize = image.getSize();

    if (!setFormatAndComputeSizes(format, size, imageSize)) {
      return false;
    }

    logFormat(filename);

    std::vector<uint8_t> pixels(imageSize.width * imageSize.height, 0x00);
    std::size_t index = 0;

    switch (format.transparency) {
      case ConsoleFontFormat::Alpha:
        for (unsigned y = 0; y < imageSize.height; ++y) {
          for (unsigned x = 0; x < imageSize.width; ++x) {
            Color4u color = image.getPixel({ x, y });
            pixels[index++] = color.a;
          }
        }

        break;

      case ConsoleFontFormat::Grayscale:
        for (unsigned y = 0; y < imageSize.height; ++y) {
          for (unsigned x = 0; x < imageSize.width; ++x) {
            Color4u color = image.getPixel({ x, y });
            assert(color.r == color.g && color.g == color.b);
            pixels[index++] = color.r;
          }
        }

        break;

      case ConsoleFontFormat::ColorKey: {
        Color4u key = image.getPixel(getColorKeyPosition());

        for (unsigned y = 0; y < imageSize.height; ++y) {
          for (unsigned x = 0; x < imageSize.width; ++x) {
            Color4u color = image.getPixel({ x, y });

            if (color == key) {
              pixels[index++] = 0;
            } else {
              pixels[index++] = 255;
            }
          }
        }

        break;
      }

    }

    m_texture.create(imageSize);
    m_texture.update(pixels.data());

    return true;
  }


  /*
   * ColoredConsoleFont
   */

  const BareTexture *ColoredConsoleFont::getTexture() const {
    return &m_texture;
  }

  bool ColoredConsoleFont::loadFromFile(const Path& filename, ConsoleFontFormat format, Vector2u size) {
    Image image;

    if (!image.loadFromFile(filename)) {
      return false;
    }

    Vector2u imageSize = image.getSize();

    if (!setFormatAndComputeSizes(format, size, imageSize)) {
      return false;
    }

    logFormat(filename);

    switch (format.transparency) {
      case ConsoleFontFormat::Alpha:
        // nothing to do
        break;

      case ConsoleFontFormat::Grayscale:
        Log::error("Colored console font can not have grayscale transparency.");
        return false;

      case ConsoleFontFormat::ColorKey: {
        Color4u key = image.getPixel(getColorKeyPosition());

        for (unsigned y = 0; y < imageSize.height; ++y) {
          for (unsigned x = 0; x < imageSize.width; ++x) {
            Color4u color = image.getPixel({ x, y });

            if (color == key) {
              image.setPixel({ x, y }, { 0, 0, 0, 0 });
            }
          }
        }

        break;
      }

    }

    return m_texture.loadFromImage(image);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
