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
#include <gf/Console.h>

#include <cassert>
#include <cstdarg>
#include <cstdio>

#include <memory>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <gf/Color.h>
#include <gf/Image.h>
#include <gf/RenderTarget.h>
#include <gf/StringUtils.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>

#include <gf/Unused.h>

namespace gf {

template<>
struct EnableBitmaskOperators<Console::PrintOption> {
  static constexpr bool value = true;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    struct ConsoleLine {
      std::vector<std::string> words;
      int indent;
    };

    struct ConsoleParagraph {
      std::vector<ConsoleLine> lines;
    };

  }

  static std::vector<std::string> splitInParagraphs(const std::string& str) {
    std::vector<std::string> out;
    boost::algorithm::split(out, str, boost::is_any_of("\n"), boost::algorithm::token_compress_on);
    out.erase(std::remove_if(out.begin(), out.end(), [](const std::string& s) {
      return s.empty();
    }), out.end());
    return out;
  }

  static std::vector<std::string> splitInWords(const std::string& str) {
    std::vector<std::string> out;
    boost::algorithm::split(out, str, boost::is_any_of(" \t"), boost::algorithm::token_compress_on);
    out.erase(std::remove_if(out.begin(), out.end(), [](const std::string& s) {
      return s.empty();
    }), out.end());
    return out;
  }

  static bool isColorControl(char c) {
    switch (c) {
      case ConsoleColorControl1:
      case ConsoleColorControl2:
      case ConsoleColorControl3:
      case ConsoleColorControl4:
      case ConsoleColorControl5:
      case ConsoleColorControlStop:
        return true;

      default:
        break;
    }

    return false;
  }

  static int getWordWidth(const std::string& word) {
    int width = 0;

    for (const auto& c : word) {
      if (!isColorControl(c)) {
        ++width;
      }
    }

    return width;
  }

  // adaptation of the algorithm in gf::Text

  static std::vector<ConsoleParagraph> makeParagraphs(const std::string& message, ConsoleAlignment alignment, int paragraphWidth) {
    std::vector<std::string> paragraphs = splitInParagraphs(message);
    std::vector<ConsoleParagraph> out;

    for (const auto& simpleParagraph : paragraphs) {
      std::vector<std::string> words = splitInWords(simpleParagraph);

      ConsoleParagraph paragraph;

      ConsoleLine currentLine;
      int currentWidth = 0;

      for (const auto& word : words) {
        int wordWidth = getWordWidth(word);

        if (!currentLine.words.empty() && currentWidth + 1 + wordWidth > paragraphWidth) {
          switch (alignment) {
            case ConsoleAlignment::Left:
              currentLine.indent = 0;
              break;

            case ConsoleAlignment::Right:
              currentLine.indent = paragraphWidth - currentWidth;
              break;

            case ConsoleAlignment::Center:
              currentLine.indent = (paragraphWidth - currentWidth) / 2;
              break;
          }

          paragraph.lines.push_back(std::move(currentLine));
          currentLine.words.clear();
        }

        if (currentLine.words.empty()) {
          currentWidth = wordWidth;
        } else {
          currentWidth += 1 + wordWidth;
        }

        currentLine.words.push_back(word);
      }

      // add the last line
      if (!currentLine.words.empty()) {
        switch (alignment) {
          case ConsoleAlignment::Left:
            currentLine.indent = 0;
            break;

          case ConsoleAlignment::Right:
            currentLine.indent = paragraphWidth - currentWidth;
            break;

          case ConsoleAlignment::Center:
            currentLine.indent = (paragraphWidth - currentWidth) / 2;
            break;
        }

        paragraph.lines.push_back(std::move(currentLine));
      }

      out.push_back(std::move(paragraph));
      paragraph.lines.clear();
    }

    return out;
  }


  /*
   * ConsoleFont
   */

  ConsoleFont::ConsoleFont()
  : m_characterSize(0, 0)
  {
    m_mapping.reserve(256);
  }

  static constexpr uint8_t AsciiToSpecial[256] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,  76,  77,   0,   0,   0,   0,   0,
     71,  70,  72,   0,   0,   0,   0,   0,  64,  65,  67,  66,   0,  73,  68,  69,
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
     32,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,  33,  34,  35,  36,  37,
     38, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
    143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,  39,  40,  41,  42,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     43,  44,  45,  46,  49,   0,   0,   0,   0,  81,  78,  87,  88,   0,   0,  55,
     53,  50,  52,  51,  47,  48,   0,   0,  85,  86,  82,  84,  83,  79,  80,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,  56,  54,   0,   0,   0,   0,   0,
     74,  75,  57,  58,  59,  60,  61,  62,  63,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  };

  static bool imageHasAlpha(const Image& image) {
    Vector2u size = image.getSize();
    const uint8_t *pixels = image.getPixelsPtr();

    for (unsigned i = 0; i < size.width * size.height; ++i) {
      if (pixels[4 * i + 3] < 255) {
        return true;
      }
    }

    return false;
  }

  bool ConsoleFont::loadFromFile(const Path& filename, Layout layout, Vector2u size) {
    Image image;

    if (!image.loadFromFile(filename)) {
      return false;
    }

    bool grayscale = false;

    switch (layout) {
      case Layout::GrayscaleInColumn:
        grayscale = true;
        layout = Layout::InColumn;
        break;

      case Layout::GrayscaleInRow:
        grayscale = true;
        layout = Layout::InRow;
        break;

      case Layout::GrayscaleSpecial:
        grayscale = true;
        layout = Layout::Special;
        break;

      case Layout::GrayscaleCustom:
        grayscale = true;
        layout = Layout::Custom;
        break;

      default:
        break;
    }

    bool hasAlpha = imageHasAlpha(image);
    Color4u reference(0xFF, 0xFF, 0xFF, 0xFF); // assume white

    if (size == Vector2u(0, 0)) {
      switch (layout) {
        case Layout::InColumn:
        case Layout::InRow:
          size = Vector2u(16u, 16u);
          break;

        case Layout::Special:
          size = Vector2u(32u, 8u);
          break;

        case Layout::Custom:
          Log::error("Undefined size for a custom font\n");
          return false;

        default:
          assert(false);
          break;
      }
    }

    unsigned charsCount = size.width * size.height;
    Vector2u imageSize = image.getSize();

    if (imageSize.width % size.width != 0 || imageSize.height % size.height != 0) {
      Log::error("Image size (%ux%u) is not a multiple of font layout (%ux%u)\n", imageSize.width, imageSize.height, size.width, size.height);
      return false;
    }

    m_characterSize = imageSize / size;

    Log::info("Console font '%s': %ux%u with characters %ux%u, %s\n", filename.string().c_str(),
      size.width, size.height, m_characterSize.width, m_characterSize.height,
      grayscale ? "grayscale" : (hasAlpha ? "antialiased" : "standard")
    );

    std::vector<uint8_t> pixels(charsCount * m_characterSize.width * m_characterSize.height, 0x00);

    std::size_t index = 0;

    for (unsigned y = 0; y < imageSize.height; ++y) {
      for (unsigned x = 0; x < imageSize.width; ++x) {
        Color4u color = image.getPixel({ x, y });

        if (grayscale) {
          assert(color.r == color.g && color.g == color.b);
          pixels[index++] = color.r;
        } else {
          if (hasAlpha) {
            pixels[index++] = color.a;
          } else {
            if (color == reference) {
              pixels[index++] = 255;
            } else {
              pixels[index++] = 0;
            }
          }
        }
      }
    }

    m_texture.create(imageSize);
    m_texture.update(pixels.data());

    // define mapping

    m_mapping.clear();

    switch (layout) {
      case Layout::InColumn:
        for (unsigned i = 0; i < charsCount; ++i) {
          m_mapping.push_back({ i / size.height, i % size.height });
        }
        break;

      case Layout::InRow:
        for (unsigned i = 0; i < charsCount; ++i) {
          m_mapping.push_back({ i % size.width, i / size.width });
        }
        break;

      case Layout::Special:
        for (auto c : AsciiToSpecial) {
          m_mapping.push_back({ c % size.width, c / size.width });
        }
        break;

      case Layout::Custom:
        m_mapping.resize(charsCount, { 0u, 0u }); // waiting for custom mapping
        break;

      default:
        assert(false);
        break;
    }

    return true;
  }

  RectF ConsoleFont::getTextureRect(uint8_t c) const {
    assert(c < m_mapping.size());
    RectU rect(m_mapping[c] * m_characterSize, m_characterSize);
    return m_texture.computeTextureCoords(rect);
  }

  RectF ConsoleFont::getTextureRect(SpecialChar c) const {
    return getTextureRect(static_cast<uint8_t>(c));
  }

  const AlphaTexture *ConsoleFont::getTexture() const {
    return &m_texture;
  }

  void ConsoleFont::mapAsciiCode(uint8_t c, Vector2u position) {
    if (c < m_mapping.size()) {
      m_mapping[c] = position;
    }
  }

  void ConsoleFont::mapAsciiCode(SpecialChar c, Vector2u position) {
    mapAsciiCode(static_cast<uint8_t>(c), position);
  }

  void ConsoleFont::mapAsciiCodeRange(uint8_t c, std::size_t count, Vector2u position) {
    Vector2u fontSize = m_texture.getSize() / m_characterSize;

    for (std::size_t i = 0; i < count; ++i) {
      m_mapping[c++] = position;
      ++position.x;

      if (position.x == fontSize.width) {
        position.x = 0;
        ++position.y;
        assert(position.y < fontSize.height);
      }
    }
  }

  void ConsoleFont::mapString(StringRef str, Vector2u position) {
    Vector2u fontSize = m_texture.getSize() / m_characterSize;

    for (auto c : str) {
      m_mapping[c] = position;
      ++position.x;

      if (position.x == fontSize.width) {
        position.x = 0;
        ++position.y;
        assert(position.y < fontSize.height);
      }
    }
  }

  /*
   * Console
   */

  Console::Console(const ConsoleFont& font, Vector2i size)
  : m_font(&font)
  , m_data(size)
  , m_background(Color::Black)
  , m_foreground(Color::White)
  , m_effect(ConsoleEffect::None)
  , m_alignment(ConsoleAlignment::Left)
  , m_fadingAmount(1.0f)
  , m_fadingColor(Color::Black)
  {
    clear();
  }

  void Console::clear() {
    for (auto& cell : m_data) {
      cell.bg = m_background;
      cell.fg = m_foreground;
      cell.c = ' ';
    }
  }

  Color4f Console::computeColor(ConsoleEffect effect, const Color4f& existing, const Color4f& current) {
    Color4f result;

    if (effect.isDefault()) {
      effect = m_effect;
    }

    switch (effect.getKind()) {
      case ConsoleEffect::None:
        return existing;

      case ConsoleEffect::Set:
        return current;

      case ConsoleEffect::Multiply:
        return existing * current;

      case ConsoleEffect::Lighten:
        return gf::max(existing, current);

      case ConsoleEffect::Darken:
        return gf::min(existing, current);

      case ConsoleEffect::Screen:
        return 1.0f - (1.0f - existing) * (1.0f - current);

      case ConsoleEffect::ColorDodge:
        for (std::size_t i = 0; i < 3; ++i) {
          if (existing.data[i] != 1.0f) {
            result.data[i] = current.data[i] / (1.0f - existing.data[i]);
          } else {
            result.data[i] = 1.0f;
          }
        }

        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffect::ColorBurn:
        for (std::size_t i = 0; i < 3; ++i) {
          if (existing.data[i] != 0.0f) {
            result.data[i] = (1.0f - current.data[i]) / existing.data[i];
          } else {
            result.data[i] = 0.0f;
          }
        }

        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffect::Add:
        return gf::clamp(existing + current, 0.0f, 1.0f);

      case ConsoleEffect::AddAlpha:
        return gf::clamp(existing + effect.getAlpha() * current, 0.0f, 1.0f);

      case ConsoleEffect::Burn:
        return gf::clamp(existing + current - Color::White, 0.0f, 1.0f);

      case ConsoleEffect::Overlay:
        for (std::size_t i = 0; i < 3; ++i) {
          if (current.data[i] < 0.5f) {
            result.data[i] = 2 * current.data[i] * existing.data[i];
          } else {
            result.data[i] = 1.0f - 2 * (1.0f - current.data[i]) * (1.0f - existing.data[i]);
          }
        }

        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffect::Alpha:
        return gf::lerp(existing, current, effect.getAlpha());

      case ConsoleEffect::Default:
        assert(false);
        break;
    }

    return Color::Black;
  }

  void Console::setCharBackground(Vector2i position, const Color4f& color, ConsoleEffect effect) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position).bg = computeColor(effect, m_data(position).bg, color);
  }

  const Color4f& Console::getCharBackground(Vector2i position) const {
    assert(m_data.isValid(position));
    return m_data(position).bg;
  }

  void Console::setCharForeground(Vector2i position, const Color4f& color) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position).fg = color;
  }

  const Color4f& Console::getCharForeground(Vector2i position) const {
    assert(m_data.isValid(position));
    return m_data(position).fg;
  }

  void Console::setChar(Vector2i position, uint8_t c) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position).c = c;
  }

  void Console::setChar(Vector2i position, SpecialChar c) {
    setChar(position, static_cast<uint8_t>(c));
  }

  uint8_t Console::getChar(Vector2i position) const {
    assert(m_data.isValid(position));
    return m_data(position).c;
  }

  void Console::putChar(Vector2i position, uint8_t c, ConsoleEffect effect) {
    if (!m_data.isValid(position)) {
      return;
    }

    Cell& cell = m_data(position);
    cell.fg = m_foreground;
    cell.bg = computeColor(effect, m_data(position).bg, m_background);
    cell.c = c;
  }

  void Console::putChar(Vector2i position, SpecialChar c, ConsoleEffect effect) {
    putChar(position, static_cast<uint8_t>(c), effect);
  }

  void Console::putChar(Vector2i position, uint8_t c, const Color4f& foreground, const Color4f& background) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position) = { foreground, background, c };
  }

  void Console::putChar(Vector2i position, SpecialChar c, const Color4f& foreground, const Color4f& background) {
    putChar(position, static_cast<uint8_t>(c), foreground, background);
  }

  int Console::putWord(Vector2i position, ConsoleEffect effect, const std::string& message, const Color4f& foreground, const Color4f& background) {
    int width = 0;

    for (auto c : message) {
      switch (c) {
        case ConsoleColorControl1:
          m_foreground = m_controls[0].fg;
          m_background = m_controls[0].bg;
          break;

        case ConsoleColorControl2:
          m_foreground = m_controls[1].fg;
          m_background = m_controls[1].bg;
          break;

        case ConsoleColorControl3:
          m_foreground = m_controls[2].fg;
          m_background = m_controls[2].bg;
          break;

        case ConsoleColorControl4:
          m_foreground = m_controls[3].fg;
          m_background = m_controls[3].bg;
          break;

        case ConsoleColorControl5:
          m_foreground = m_controls[4].fg;
          m_background = m_controls[4].bg;
          break;

        case ConsoleColorControlStop:
          m_foreground = foreground;
          m_background = background;
          break;

        default:
          putChar(position, c, effect);
          ++position.x;
          ++width;
          break;
      }
    }

    return width;
  }

  int Console::printInternal(const RectI& rect, ConsoleEffect effect, ConsoleAlignment alignment, const std::string& message, PrintOptionFlags flags) {
    // checks
    Vector2i consoleSize = m_data.getSize();

    if (rect.width < 0 || rect.height < 0) {
      Log::warning("Size of console text rectangle is invalide\n");
      return 0;
    }

    if (rect.left < 0 || rect.top < 0 || rect.left + rect.width > consoleSize.width || rect.top + rect.height > consoleSize.height) {
      Log::warning("Position of console text is outside the console\n");
      return 0;
    }

    Color4f currentBackground = m_background;
    Color4f currentForeground = m_foreground;
    int lineCount = 0;

    if (flags.test(PrintOption::Split)) {
      // multiple line
      int paragraphWidth = rect.width;

      if (paragraphWidth == 0) {
        switch (alignment) {
          case ConsoleAlignment::Left:
            paragraphWidth = consoleSize.width - rect.left;
            break;

          case ConsoleAlignment::Center:
            paragraphWidth = consoleSize.width;
            break;

          case ConsoleAlignment::Right:
            paragraphWidth = rect.left + 1;
            break;
        }
      }

      auto paragraphs = makeParagraphs(message, alignment, paragraphWidth);
      Vector2i position = rect.position;

      for (const auto& paragraph : paragraphs) {
        if (flags.test(PrintOption::CountOnly)) {
          lineCount += paragraph.lines.size();
        } else {
          for (const auto& line : paragraph.lines) {
            if (rect.height > 0 && lineCount >= rect.height) {
              break;
            }

            Vector2i localPosition = position;
            localPosition.x += line.indent;
            auto wordCount = line.words.size();

            for (const auto& word : line.words) {
              localPosition.x += putWord(localPosition, effect, word, currentForeground, currentBackground);
              wordCount--;

              if (wordCount > 0) {
                putChar(localPosition, ' ', effect);
                ++localPosition.x;
              }
            }

            ++lineCount;
            ++position.y;
          }
        }
      }

    } else {
      // single line
      assert(rect.width == 0 && rect.height == 0);
      Vector2i position = rect.position;
      int width = getWordWidth(message);

      switch (alignment) {
        case ConsoleAlignment::Left:
          break;

        case ConsoleAlignment::Center:
          position.x -= width / 2;
          break;

        case ConsoleAlignment::Right:
          position.x -= width;
          break;
      }

      putWord(position, effect, message, currentForeground, currentBackground);
      lineCount = 1;
    }

    m_background = currentBackground;
    m_foreground = currentForeground;
    return lineCount;
  }

  void Console::print(Vector2i position, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(RectI(position, { 0, 0 }), m_effect, m_alignment, message);
  }

  void Console::print(Vector2i position, ConsoleEffect effect, ConsoleAlignment alignment, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(RectI(position, { 0, 0 }), effect, alignment, message);
  }

  int Console::printRect(const RectI& rect, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(rect, m_effect, m_alignment, message, PrintOption::Split);
    return 0;
  }

  int Console::printRect(const RectI& rect, ConsoleEffect effect, ConsoleAlignment alignment, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(rect, effect, alignment, message, PrintOption::Split);
    return 0;
  }

  int Console::getHeight(const RectI& rect, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(rect, m_effect, m_alignment, message, gf::Console::PrintOption::Split | gf::Console::PrintOption::CountOnly);
    return 0;
  }

  void Console::setColorControl(ConsoleColorControl ctrl, const Color4f& foreground, const Color4f& background) {
    assert(1 <= ctrl && ctrl <= ColorControlCount);
    ColorControl& cc = m_controls[ctrl - 1];
    cc.fg = foreground;
    cc.bg = background;
  }

  void Console::drawRectangle(const RectI& rect, PrintAction action, ConsoleEffect effect) {
    Vector2i offset;

    for (offset.x = 0; offset.x < rect.width; ++offset.x) {
      for (offset.y = 0; offset.y < rect.height; ++offset.y) {
        auto position = rect.position + offset;

        if (!m_data.isValid(position)) {
          continue;
        }

        setCharBackground(position, m_background, effect);

        if (action == PrintAction::Clear) {
          setChar(position, ' ');
        }
      }
    }
  }

  void Console::drawHorizontalLine(Vector2i left, int width, ConsoleEffect effect) {
    for (int i = 0; i < width; ++i) {
      putChar(left, SpecialChar::WallHorizontalLine, effect);
      ++left.x;
    }
  }

  void Console::drawVerticalLine(Vector2i top, int height, ConsoleEffect effect) {
    for (int i = 0; i < height; ++i) {
      putChar(top, SpecialChar::WallVerticalLine, effect);
      ++top.y;
    }
  }

  void Console::drawFrame(const RectI& rect, PrintAction action, ConsoleEffect effect, const char *fmt, ...) {
    drawRectangle(rect, action, effect);

    int xWest = rect.left;
    int xEast = rect.left + rect.width - 1;
    int yNorth = rect.top;
    int ySouth = rect.top + rect.height - 1;
    putChar({ xWest, yNorth }, SpecialChar::WallNorthWest, effect);
    putChar({ xEast, yNorth }, SpecialChar::WallNorthEast, effect);
    putChar({ xWest, ySouth }, SpecialChar::WallSouthWest, effect);
    putChar({ xEast, ySouth }, SpecialChar::WallSouthEast, effect);
    drawHorizontalLine({ xWest + 1, yNorth }, rect.width - 2, effect);
    drawHorizontalLine({ xWest + 1, ySouth }, rect.width - 2, effect);
    drawVerticalLine({ xWest, yNorth + 1 }, rect.height - 2, effect);
    drawVerticalLine({ xEast, yNorth + 1 }, rect.height - 2, effect);

    if (fmt == nullptr) {
      return;
    }

    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    std::swap(m_background, m_foreground);
    print({ xWest + 1, yNorth }, ConsoleEffect::Set, ConsoleAlignment::Left, " %s ", message.c_str());
    std::swap(m_background, m_foreground);
  }

  void Console::blit(const RectI& src, Console& con, Vector2i dst, float foregroundAlpha, float backgroundAlpha) const {
    Vector2i origin = src.position;
    Vector2i size = src.size;
    Vector2i target = dst;

    // clip source

    if (origin.x < 0) {
      size.width += origin.x;
      target.x -= origin.x;
      origin.x = 0;
    }

    int maxWidth = getWidth() - origin.x;

    if (size.width > maxWidth) {
      size.width = maxWidth;
    }

    if (origin.y < 0) {
      size.height += origin.y;
      target.y -= origin.y;
      origin.y = 0;
    }

    int maxHeight = getHeight() - origin.y;

    if (size.height > maxHeight) {
      size.height = maxHeight;
    }

    // clip destination

    if (target.x < 0) {
      size.width += target.x;
      origin.x -= target.x;
      target.x = 0;
    }

    int dx = target.x + size.width - con.getWidth();

    if (dx > 0) {
      size.width -= dx;
    }

    if (target.y < 0) {
      size.height += target.y;
      origin.y -= target.y;
      target.y = 0;
    }

    int dy = target.y + size.height - con.getHeight();

    if (dy > 0) {
      size.height -= dy;
    }

    if (size.width <= 0 || size.height <= 0) {
      return;
    }

    // blit

    Vector2i offset;

    for (offset.y = 0; offset.y < size.height; ++offset.y) {
      for (offset.x = 0; offset.x < size.width; ++offset.x) {
        assert(con.m_data.isValid(target + offset));
        assert(m_data.isValid(origin + offset));

        auto& targetCell = con.m_data(target + offset);
        auto& originCell = m_data(origin + offset);

        targetCell.bg = gf::lerp(targetCell.bg, originCell.bg, backgroundAlpha);
        targetCell.fg = gf::lerp(targetCell.fg, originCell.fg, foregroundAlpha);
        targetCell.c = originCell.c;
      }
    }

  }

  void Console::draw(RenderTarget& target, RenderStates states) {
    if (m_font == nullptr) {
      return;
    }

    auto consoleSize = m_data.getSize();

    VertexArray backgroundVertices(PrimitiveType::Triangles);
    backgroundVertices.reserve(consoleSize.width * consoleSize.height * 6);

    VertexArray foregroundVertices(PrimitiveType::Triangles);
    foregroundVertices.reserve(consoleSize.width * consoleSize.height * 6);

    Color4f color;
    auto characterSize = m_font->getCharacterSize();

    for (auto position : m_data.getPositionRange()) {
      const auto& data = m_data(position);

      Vertex vertices[4];

      if (m_fadingAmount == 1.0f) {
        color = data.bg;
      } else {
        color = gf::lerp(m_fadingColor, data.bg, m_fadingAmount);
      }

      vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

      RectF rect(position * characterSize, characterSize);

      vertices[0].position = rect.getTopLeft();
      vertices[1].position = rect.getTopRight();
      vertices[2].position = rect.getBottomLeft();
      vertices[3].position = rect.getBottomRight();

      backgroundVertices.append(vertices[0]);
      backgroundVertices.append(vertices[1]);
      backgroundVertices.append(vertices[2]);

      backgroundVertices.append(vertices[2]);
      backgroundVertices.append(vertices[1]);
      backgroundVertices.append(vertices[3]);

      if (m_fadingAmount == 1.0f) {
        color = data.fg;
      } else {
        color = gf::lerp(m_fadingColor, data.fg, m_fadingAmount);
      }

      vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

      // same positions

      RectF textureRect = m_font->getTextureRect(data.c);
      vertices[0].texCoords = textureRect.getTopLeft();
      vertices[1].texCoords = textureRect.getTopRight();
      vertices[2].texCoords = textureRect.getBottomLeft();
      vertices[3].texCoords = textureRect.getBottomRight();

      foregroundVertices.append(vertices[0]);
      foregroundVertices.append(vertices[1]);
      foregroundVertices.append(vertices[2]);

      foregroundVertices.append(vertices[2]);
      foregroundVertices.append(vertices[1]);
      foregroundVertices.append(vertices[3]);
    }

    states.transform *= getTransform();
    target.draw(backgroundVertices, states);

    states.texture = m_font->getTexture();
    target.draw(foregroundVertices, states);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
