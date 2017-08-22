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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Text.h>

#include <algorithm>
#include <limits>

#include <gf/Color.h>
#include <gf/Font.h>
#include <gf/StringUtils.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Text::Text()
  : m_string()
  , m_font(nullptr)
  , m_characterSize(0)
  , m_color(Color::Black)
  , m_vertices(PrimitiveType::Triangles)
  , m_bounds(0.0f, 0.0f, 0.0f, 0.0f)
  , m_outlineColor(Color::Black)
  , m_outlineThickness(0.0f)
  , m_outlineVertices(PrimitiveType::Triangles)
  , m_paragraphWidth(0.0f)
  , m_align(Alignment::None)
  {

  }

  Text::Text(std::string string, Font& font, unsigned characterSize)
  : m_string(std::move(string))
  , m_font(&font)
  , m_characterSize(characterSize)
  , m_color(Color::Black)
  , m_vertices(PrimitiveType::Triangles)
  , m_bounds(0.0f, 0.0f, 0.0f, 0.0f)
  , m_outlineColor(Color::Black)
  , m_outlineThickness(0.0f)
  , m_outlineVertices(PrimitiveType::Triangles)
  , m_paragraphWidth(0.0f)
  , m_align(Alignment::None)
  {
    updateGeometry();
  }

  void Text::setString(std::string string) {
    if (m_string == string) {
      return;
    }

    m_string = std::move(string);
    updateGeometry();
  }

  void Text::setCharacterSize(unsigned characterSize) {
    if (m_characterSize == characterSize) {
      return;
    }

    m_characterSize = characterSize;
    updateGeometry();
  }

  void Text::setFont(Font& font) {
    if (m_font == &font) {
      return;
    }

    m_font = &font;
    updateGeometry();
  }

  void Text::setColor(const Color4f& color) {
    m_color = color;

    auto count = m_vertices.getVertexCount();

    for (std::size_t i = 0; i < count; ++i) {
      m_vertices[i].color = color;
    }
  }

  void Text::setOutlineColor(const Color4f& color) {
    m_outlineColor = color;

    auto count = m_outlineVertices.getVertexCount();

    for (std::size_t i = 0; i < count; ++i) {
      m_outlineVertices[i].color = color;
    }
  }

  void Text::setOutlineThickness(float thickness) {
    if (m_outlineThickness == thickness) {
      return;
    }

    m_outlineThickness = thickness;
    updateGeometry();
  }

  void Text::setParagraphWidth(float paragraphWidth) {
    if (m_paragraphWidth == paragraphWidth) {
      return;
    }

    m_paragraphWidth = paragraphWidth;
    updateGeometry();
  }

  void Text::setAlignment(Alignment align) {
    if (m_align == align) {
      return;
    }

    m_align = align;
    updateGeometry();
  }

  void Text::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, m_bounds);
  }

  VertexBuffer Text::commitGeometry() const {
    VertexBuffer buffer;
    buffer.load(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
    return buffer;
  }

  VertexBuffer Text::commitOutlineGeometry() const {
    VertexBuffer buffer;
    buffer.load(m_outlineVertices.getVertexData(), m_outlineVertices.getVertexCount(), m_outlineVertices.getPrimitiveType());
    return buffer;
  }

  void Text::draw(RenderTarget& target, RenderStates states) {
    if (m_font == nullptr || m_characterSize == 0) {
      return;
    }

    states.transform *= getTransform();
    states.texture = m_font->getTexture(m_characterSize);

    if (m_outlineThickness > 0) {
      target.draw(m_outlineVertices, states);
    }

    target.draw(m_vertices, states);
  }

  static void addGlyphVertex(VertexArray& array, const Glyph& glyph, const Vector2f& position, const Color4f& color) {
    Vertex vertices[4];

    vertices[0].position = position + glyph.bounds.getTopLeft();
    vertices[1].position = position + glyph.bounds.getTopRight();
    vertices[2].position = position + glyph.bounds.getBottomLeft();
    vertices[3].position = position + glyph.bounds.getBottomRight();

    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

    vertices[0].texCoords = glyph.textureRect.getTopLeft();
    vertices[1].texCoords = glyph.textureRect.getTopRight();
    vertices[2].texCoords = glyph.textureRect.getBottomLeft();
    vertices[3].texCoords = glyph.textureRect.getBottomRight();

    // first triangle
    array.append(vertices[0]);
    array.append(vertices[1]);
    array.append(vertices[2]);

    // second triangle
    array.append(vertices[2]);
    array.append(vertices[1]);
    array.append(vertices[3]);
  }

  void Text::updateGeometry() {
    if (m_font == nullptr || m_characterSize == 0 || m_string.empty()) {
      return;
    }

    m_vertices.clear();
    m_outlineVertices.clear();
    m_bounds = RectF();

    float spaceWidth = m_font->getGlyph(' ', m_characterSize).advance;
    float lineHeight = m_font->getLineSpacing(m_characterSize);

    std::vector<Paragraph> paragraphs = makeParagraphs(m_string, spaceWidth);

    Vector2f position(0.0f, 0.0f);

    Vector2f min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector2f max(0.0f, 0.0f);

    for (auto& paragraph : paragraphs) {
//       std::printf("Paragraph with %zu lines\n", paragraph.lines.size());

      for (auto& line : paragraph.lines) {
//         std::printf("\tLine with %zu words\n", line.words.size());
//         std::printf("\t\tindent: %f\n", line.indent);
//         std::printf("\t\tspacing: %f (%f)\n", line.spacing, spaceWidth);

        position.x = line.indent;

        for (auto& word : line.words) {
          char32_t prevCodepoint = '\0';

          for (char32_t currCodepoint : word) {

            position.x += m_font->getKerning(prevCodepoint, currCodepoint, m_characterSize);
            prevCodepoint = currCodepoint;

            if (m_outlineThickness > 0) {
              const Glyph& glyph = m_font->getGlyph(currCodepoint, m_characterSize, m_outlineThickness);

              addGlyphVertex(m_outlineVertices, glyph, position, m_outlineColor);

              min = gf::min(min, position + glyph.bounds.getTopLeft());
              max = gf::max(max, position + glyph.bounds.getBottomRight());
            }

            const Glyph& glyph = m_font->getGlyph(currCodepoint, m_characterSize);

            addGlyphVertex(m_vertices, glyph, position, m_color);

            if (m_outlineThickness == 0.0f) {
              min = gf::min(min, position + glyph.bounds.getTopLeft());
              max = gf::max(max, position + glyph.bounds.getBottomRight());
            }

            position.x += glyph.advance;
          }

          position.x += line.spacing;
        }

        position.y += lineHeight;
      }
    }

    m_bounds = RectF(min, max - min);

    if (m_align != Alignment::None) {
      m_bounds.left = 0;
      m_bounds.width = m_paragraphWidth;
    }
  }

  float Text::getWordWidth(const std::u32string& word) {
    assert(m_font != nullptr);
    assert(m_characterSize > 0);
    assert(!word.empty());

    float width = 0.0f;
    char32_t prevCodepoint = '\0';

    for (char32_t currCodepoint : word) {
      width += m_font->getKerning(prevCodepoint, currCodepoint, m_characterSize);
      prevCodepoint = currCodepoint;

      const Glyph& glyph = m_font->getGlyph(currCodepoint, m_characterSize);
      width += glyph.advance;
    }

    return width;
  }

  std::vector<Text::Paragraph> Text::makeParagraphs(const std::string& str, float spaceWidth) {
    std::u32string unicodeString = computeUnicodeString(str);
    std::vector<std::u32string> paragraphs = splitInParagraphs(unicodeString);
    std::vector<Paragraph> out;

    for (const auto& simpleParagraph : paragraphs) {
      std::vector<std::u32string> words = splitInWords(simpleParagraph);

      Paragraph paragraph;

      if (m_align == Alignment::None) {
        Line line;
        line.words = std::move(words);
        line.indent = 0.0f;
        line.spacing = spaceWidth;
        paragraph.lines.push_back(std::move(line));
      } else {
        Line currentLine;
        float currentWidth = 0.0f;

        for (const auto& word : words) {
          float wordWith = getWordWidth(word);

          if (!currentLine.words.empty() && currentWidth + spaceWidth + wordWith > m_paragraphWidth) {
            auto wordCount = currentLine.words.size();

            switch (m_align) {
              case Alignment::Left:
                currentLine.indent = 0.0f;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Right:
                currentLine.indent = m_paragraphWidth - currentWidth;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Center:
                currentLine.indent = (m_paragraphWidth - currentWidth) / 2;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Justify:
                currentLine.indent = 0.0f;

                if (wordCount > 1) {
                  currentLine.spacing = spaceWidth + (m_paragraphWidth - currentWidth) / (wordCount - 1);
                } else {
                  currentLine.spacing = 0.0f;
                }

                break;

              case Alignment::None:
                assert(false);
                break;
            }

            paragraph.lines.push_back(std::move(currentLine));
            currentLine.words.clear();
          }

          if (currentLine.words.empty()) {
            currentWidth = wordWith;
          } else {
            currentWidth += spaceWidth + wordWith;
          }

          currentLine.words.push_back(word);
        }

        // add the last line
        if (!currentLine.words.empty()) {
            switch (m_align) {
              case Alignment::Left:
              case Alignment::Justify:
                currentLine.indent = 0.0f;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Right:
                currentLine.indent = m_paragraphWidth - currentWidth;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Center:
                currentLine.indent = (m_paragraphWidth - currentWidth) / 2;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::None:
                assert(false);
                break;
            }

            paragraph.lines.push_back(std::move(currentLine));
        }
      }

      out.push_back(std::move(paragraph));
      paragraph.lines.clear();
    }

    return out;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
