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

#include <gf/Color.h>
#include <gf/ConsoleChar.h>
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
      std::vector<std::u32string> words;
      int indent;
    };

    struct ConsoleParagraph {
      std::vector<ConsoleLine> lines;
    };

  }

  static bool isColorControl(char32_t c) {
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

  static int getWordWidth(const std::u32string& word) {
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
    auto unicodeString = computeUnicodeString(message);
    auto paragraphs = splitInParagraphs(unicodeString);
    std::vector<ConsoleParagraph> out;

    for (const auto& simpleParagraph : paragraphs) {
      auto words = splitInWords(simpleParagraph);

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

  void Console::setChar(Vector2i position, char16_t c) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position).c = c;
  }

  char16_t Console::getChar(Vector2i position) const {
    assert(m_data.isValid(position));
    return m_data(position).c;
  }

  void Console::putChar(Vector2i position, char16_t c, ConsoleEffect effect) {
    if (!m_data.isValid(position)) {
      return;
    }

    Cell& cell = m_data(position);
    cell.fg = m_foreground;
    cell.bg = computeColor(effect, m_data(position).bg, m_background);
    cell.c = c;
  }

  void Console::putChar(Vector2i position, char16_t c, const Color4f& foreground, const Color4f& background) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position) = { foreground, background, c };
  }

  int Console::putWord(Vector2i position, ConsoleEffect effect, const std::u32string& message, const Color4f& foreground, const Color4f& background) {
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
          if (c >= 0x10000) {
            // outside BMP
            c = '\0';
          }

          if (c < 0x20) {
            // control chars
            c = '\0';
          }

          putChar(position, static_cast<char16_t>(c), effect);
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

      std::u32string unicodeString = computeUnicodeString(message);
      int width = getWordWidth(unicodeString);

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

      putWord(position, effect, unicodeString, currentForeground, currentBackground);
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
      putChar(left, ConsoleChar::BoxDrawingsLightHorizontal, effect);
      ++left.x;
    }
  }

  void Console::drawVerticalLine(Vector2i top, int height, ConsoleEffect effect) {
    for (int i = 0; i < height; ++i) {
      putChar(top, ConsoleChar::BoxDrawingsLightVertical, effect);
      ++top.y;
    }
  }

  void Console::drawFrame(const RectI& rect, PrintAction action, ConsoleEffect effect, const char *fmt, ...) {
    drawRectangle(rect, action, effect);

    int xWest = rect.left;
    int xEast = rect.left + rect.width - 1;
    int yNorth = rect.top;
    int ySouth = rect.top + rect.height - 1;
    putChar({ xWest, yNorth }, ConsoleChar::BoxDrawingsLightDownAndRight, effect);
    putChar({ xEast, yNorth }, ConsoleChar::BoxDrawingsLightDownAndLeft, effect);
    putChar({ xWest, ySouth }, ConsoleChar::BoxDrawingsLightUpAndRight, effect);
    putChar({ xEast, ySouth }, ConsoleChar::BoxDrawingsLightUpAndLeft, effect);
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
