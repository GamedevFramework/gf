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
#include <gf/Console.h>

#include <cassert>
#include <cstdarg>
#include <cstdio>

#include <memory>

#include <gf/Color.h>
#include <gf/ConsoleChar.h>
#include <gf/Image.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/StringUtils.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>

#include <gfpriv/TextureCoords.h>

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
      std::vector<std::string_view> words;
      int indent;
    };

    struct ConsoleParagraph {
      std::vector<ConsoleLine> lines;
    };

    bool isColorControl(char32_t c) {
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

    int getWordWidth(std::string_view word) {
      int width = 0;

      for (char32_t c : gf::codepoints(word)) {
        if (!isColorControl(c)) {
          ++width;
        }
      }

      return width;
    }

    // adaptation of the algorithm in gf::Text

    std::vector<ConsoleParagraph> makeParagraphs(const std::string& message, ConsoleAlignment alignment, int paragraphWidth) {
      auto paragraphs = splitInParagraphs(message);
      std::vector<ConsoleParagraph> out;

      for (auto simpleParagraph : paragraphs) {
        auto words = splitInWords(simpleParagraph);

        ConsoleParagraph paragraph;

        ConsoleLine currentLine;
        int currentWidth = 0;

        for (auto word : words) {
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

  } // anonymous namespace


  /*
   * Console
   */

  Console::Console(const ConsoleFont& font, Vector2i size)
  : m_font(&font)
  , m_data(size)
  , m_fadingAmount(1.0f)
  , m_fadingColor(Color::Black)
  {
    clear();
  }

  void Console::clear(const ConsoleStyle& style) {
    for (auto& cell : m_data) {
      cell.fg = style.foreground;
      cell.bg = style.background;
      cell.c = ' ';
    }
  }

  Color4f Console::computeColor(ConsoleEffect effect, Color4f existing, Color4f current) {
    Color4f result;

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
          if (existing[i] != 1.0f) {
            result[i] = current[i] / (1.0f - existing[i]);
          } else {
            result[i] = 1.0f;
          }
        }

        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffect::ColorBurn:
        for (std::size_t i = 0; i < 3; ++i) {
          if (existing[i] != 0.0f) {
            result[i] = (1.0f - current[i]) / existing[i];
          } else {
            result[i] = 0.0f;
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
          if (current[i] < 0.5f) {
            result[i] = 2 * current[i] * existing[i];
          } else {
            result[i] = 1.0f - 2 * (1.0f - current[i]) * (1.0f - existing[i]);
          }
        }

        return gf::clamp(result, 0.0f, 1.0f);

      case ConsoleEffect::Alpha:
        return gf::lerp(existing, current, effect.getAlpha());
    }

    assert(false);
    return Color::Black;
  }

  void Console::setCharBackground(Vector2i position, Color4f color, ConsoleEffect effect) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position).bg = computeColor(effect, m_data(position).bg, color);
  }

  Color4f Console::getCharBackground(Vector2i position) const {
    assert(m_data.isValid(position));
    return m_data(position).bg;
  }

  void Console::setCharForeground(Vector2i position, Color4f color) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position).fg = color;
  }

  Color4f Console::getCharForeground(Vector2i position) const {
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

  void Console::putChar(Vector2i position, char16_t c, const ConsoleStyle& style) {
    if (!m_data.isValid(position)) {
      return;
    }

    Cell& cell = m_data(position);
    cell.fg = style.foreground;
    cell.bg = computeColor(style.effect, m_data(position).bg, style.background);
    cell.c = c;
  }

  void Console::putChar(Vector2i position, char16_t c, Color4f foreground, Color4f background) {
    if (!m_data.isValid(position)) {
      return;
    }

    m_data(position) = { foreground, background, c };
  }

  int Console::putWord(Vector2i position, std::string_view message, const ConsoleStyle& style) {
    int width = 0;
    ConsoleStyle localStyle = style;

    for (auto c : gf::codepoints(message)) {
      switch (c) {
        case ConsoleColorControl1:
          localStyle.foreground = m_controls[0].fg;
          localStyle.background = m_controls[0].bg;
          break;

        case ConsoleColorControl2:
          localStyle.foreground = m_controls[1].fg;
          localStyle.background = m_controls[1].bg;
          break;

        case ConsoleColorControl3:
          localStyle.foreground = m_controls[2].fg;
          localStyle.background = m_controls[2].bg;
          break;

        case ConsoleColorControl4:
          localStyle.foreground = m_controls[3].fg;
          localStyle.background = m_controls[3].bg;
          break;

        case ConsoleColorControl5:
          localStyle.foreground = m_controls[4].fg;
          localStyle.background = m_controls[4].bg;
          break;

        case ConsoleColorControlStop:
          localStyle.foreground = style.foreground;
          localStyle.background = style.background;
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

          putChar(position, static_cast<char16_t>(c), localStyle);
          ++position.x;
          ++width;
          break;
      }
    }

    return width;
  }

  int Console::printInternal(const RectI& rect, const std::string& message, const ConsoleStyle& style, Flags<PrintOption> flags) {
    // checks
    Vector2i consoleSize = m_data.getSize();

    if (rect.min.x < 0 || rect.min.y < 0 || rect.max.x > consoleSize.width || rect.max.y > consoleSize.height) {
      Log::warning("Position of console text is outside the console\n");
      return 0;
    }

    int lineCount = 0;

    if (flags.test(PrintOption::Split)) {
      // multiple line
      int paragraphWidth = rect.getWidth();

      if (paragraphWidth == 0) {
        switch (style.alignment) {
          case ConsoleAlignment::Left:
            paragraphWidth = consoleSize.width - rect.min.x;
            break;

          case ConsoleAlignment::Center:
            paragraphWidth = consoleSize.width;
            break;

          case ConsoleAlignment::Right:
            paragraphWidth = rect.min.x + 1;
            break;
        }
      }

      auto paragraphs = makeParagraphs(message, style.alignment, paragraphWidth);
      Vector2i position = rect.getPosition();

      for (const auto& paragraph : paragraphs) {
        if (flags.test(PrintOption::CountOnly)) {
          lineCount += static_cast<int>(paragraph.lines.size());
        } else {
          for (const auto& line : paragraph.lines) {
            if (rect.min.y + lineCount >= rect.max.y) {
              break;
            }

            Vector2i localPosition = position;
            localPosition.x += line.indent;
            auto wordCount = line.words.size();

            for (auto word : line.words) {
              localPosition.x += putWord(localPosition, word, style);
              wordCount--;

              if (wordCount > 0) {
                putChar(localPosition, ' ', style);
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
      assert(rect.min == rect.max);
      Vector2i position = rect.getPosition();

      int width = getWordWidth(message);

      switch (style.alignment) {
        case ConsoleAlignment::Left:
          break;

        case ConsoleAlignment::Center:
          position.x -= width / 2;
          break;

        case ConsoleAlignment::Right:
          position.x -= width;
          break;
      }

      putWord(position, message, style);
      lineCount = 1;
    }

    return lineCount;
  }

  void Console::print(Vector2i position, const ConsoleStyle& style, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(RectI::fromPositionSize(position, { 0, 0 }), message, style);
  }

  int Console::printRect(const RectI& rect, const ConsoleStyle& style, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    printInternal(rect, message, style, PrintOption::Split);
    return 0;
  }

  int Console::getHeight(const RectI& rect, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    auto message = formatString(fmt, ap);
    va_end(ap);

    return printInternal(rect, message, ConsoleStyle(), gf::Console::PrintOption::Split | gf::Console::PrintOption::CountOnly);
  }

  void Console::setColorControl(ConsoleColorControl ctrl, Color4f foreground, Color4f background) {
    assert(1 <= ctrl && ctrl <= ColorControlCount);
    ColorControl& cc = m_controls[ctrl - 1];
    cc.fg = foreground;
    cc.bg = background;
  }

  void Console::drawRectangle(const RectI& rect, const ConsoleStyle& style, PrintAction action) {
    Vector2i position;

    for (position.x = rect.min.x; position.x < rect.max.x; ++position.x) {
      for (position.y = rect.min.y; position.y < rect.max.y; ++position.y) {
        if (!m_data.isValid(position)) {
          continue;
        }

        setCharBackground(position, style.background, style.effect);

        if (action == PrintAction::Clear) {
          setChar(position, ' ');
        }
      }
    }
  }

  void Console::drawHorizontalLine(Vector2i left, int width, const ConsoleStyle& style) {
    for (int i = 0; i < width; ++i) {
      putChar(left, ConsoleChar::BoxDrawingsLightHorizontal, style);
      ++left.x;
    }
  }

  void Console::drawVerticalLine(Vector2i top, int height, const ConsoleStyle& style) {
    for (int i = 0; i < height; ++i) {
      putChar(top, ConsoleChar::BoxDrawingsLightVertical, style);
      ++top.y;
    }
  }

  void Console::drawFrame(const RectI& rect, const ConsoleStyle& style, PrintAction action, const char *title, ...) {
    drawRectangle(rect, style, action);

    auto size = rect.getSize();

    int xWest = rect.min.x;
    int xEast = rect.max.x - 1;
    int yNorth = rect.min.y;
    int ySouth = rect.max.y - 1;
    putChar({ xWest, yNorth }, ConsoleChar::BoxDrawingsLightDownAndRight, style);
    putChar({ xEast, yNorth }, ConsoleChar::BoxDrawingsLightDownAndLeft, style);
    putChar({ xWest, ySouth }, ConsoleChar::BoxDrawingsLightUpAndRight, style);
    putChar({ xEast, ySouth }, ConsoleChar::BoxDrawingsLightUpAndLeft, style);
    drawHorizontalLine({ xWest + 1, yNorth }, size.width - 2, style);
    drawHorizontalLine({ xWest + 1, ySouth }, size.width - 2, style);
    drawVerticalLine({ xWest, yNorth + 1 }, size.height - 2, style);
    drawVerticalLine({ xEast, yNorth + 1 }, size.height - 2, style);

    if (title == nullptr) {
      return;
    }

    va_list ap;
    va_start(ap, title);
    auto message = formatString(title, ap);
    va_end(ap);

    ConsoleStyle localStyle = style;
    std::swap(localStyle.foreground, localStyle.background);
    localStyle.effect = ConsoleEffect::Set;
    localStyle.alignment = ConsoleAlignment::Left;
    print({ xWest + 1, yNorth }, localStyle, " %s ", message.c_str());
  }

  void Console::blit(const RectI& src, Console& con, Vector2i dst, float foregroundAlpha, float backgroundAlpha) const {
    Vector2i origin = src.getPosition();
    Vector2i size = src.getSize();
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

  void Console::draw(RenderTarget& target, const RenderStates& states) {
    if (m_font == nullptr) {
      return;
    }

    auto consoleSize = m_data.getSize();

    VertexArray backgroundVertices(PrimitiveType::Triangles);
    backgroundVertices.reserve(static_cast<std::size_t>(consoleSize.width) * static_cast<std::size_t>(consoleSize.height) * 6);

    VertexArray foregroundVertices(PrimitiveType::Triangles);
    foregroundVertices.reserve(static_cast<std::size_t>(consoleSize.width) * static_cast<std::size_t>(consoleSize.height) * 6);

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

      RectF rect = RectF::fromPositionSize(position * characterSize, characterSize);

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
      vertices[0].texCoords = gf::priv::computeTextureCoords(textureRect.getTopLeft());
      vertices[1].texCoords = gf::priv::computeTextureCoords(textureRect.getTopRight());
      vertices[2].texCoords = gf::priv::computeTextureCoords(textureRect.getBottomLeft());
      vertices[3].texCoords = gf::priv::computeTextureCoords(textureRect.getBottomRight());

      foregroundVertices.append(vertices[0]);
      foregroundVertices.append(vertices[1]);
      foregroundVertices.append(vertices[2]);

      foregroundVertices.append(vertices[2]);
      foregroundVertices.append(vertices[1]);
      foregroundVertices.append(vertices[3]);
    }

    RenderStates localStates = states;

    localStates.transform *= getTransform();
    target.draw(backgroundVertices, localStates);

    localStates.texture[0] = m_font->getTexture();
    target.draw(foregroundVertices, localStates);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
