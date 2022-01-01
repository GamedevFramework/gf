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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/BasicText.h>

#include <algorithm>
#include <limits>

#include <gf/Font.h>
#include <gf/StringUtils.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  BasicText::BasicText()
  : m_string()
  , m_font(nullptr)
  , m_characterSize(0)
  , m_outlineThickness(0.0f)
  , m_lineSpacingFactor(1.0f)
  , m_letterSpacingFactor(1.0f)
  , m_paragraphWidth(0.0f)
  , m_align(Alignment::None)
  {

  }

  BasicText::BasicText(std::string string, Font& font, unsigned characterSize)
  : m_string(std::move(string))
  , m_font(&font)
  , m_characterSize(characterSize)
  , m_outlineThickness(0.0f)
  , m_lineSpacingFactor(1.0f)
  , m_letterSpacingFactor(1.0f)
  , m_paragraphWidth(0.0f)
  , m_align(Alignment::None)
  {

  }

  void BasicText::setString(std::string string) {
    m_string = std::move(string);
  }

  void BasicText::setCharacterSize(unsigned characterSize) {
    m_characterSize = characterSize;
  }

  void BasicText::setFont(Font& font) {
    m_font = &font;
  }

  const AlphaTexture *BasicText::getFontTexture() {
    if (m_font == nullptr) {
      return nullptr;
    }

    return m_font->getTexture(m_characterSize);
  }


  void BasicText::setOutlineThickness(float thickness) {
    m_outlineThickness = thickness;
  }

  void BasicText::setLineSpacing(float spacingFactor) {
    m_lineSpacingFactor = spacingFactor;
  }

  void BasicText::setLetterSpacing(float spacingFactor) {
    m_letterSpacingFactor = spacingFactor;
  }

  void BasicText::setParagraphWidth(float paragraphWidth) {
    m_paragraphWidth = paragraphWidth;
  }

  void BasicText::setAlignment(Alignment align) {
    m_align = align;
  }

  namespace {

    struct ParagraphLine {
      std::vector<std::string_view> words;
      float indent = 0.0f;
      float spacing = 0.0f;
    };

    struct Paragraph {
      std::vector<ParagraphLine> lines;
    };

    float getWordWidth(std::string_view word, unsigned characterSize, Font& font) {
      assert(characterSize > 0);
      assert(!word.empty());

      float width = 0.0f;
      char32_t prevCodepoint = '\0';

      for (char32_t currCodepoint : gf::codepoints(word)) {
        width += font.getKerning(prevCodepoint, currCodepoint, characterSize);
        prevCodepoint = currCodepoint;

        const Glyph& glyph = font.getGlyph(currCodepoint, characterSize);
        width += glyph.advance;
      }

      return width;
    }

    std::vector<Paragraph> makeParagraphs(const std::string& str, float spaceWidth, float paragraphWidth, Alignment align, unsigned characterSize, Font& font) {
      std::vector<std::string_view> paragraphs = splitInParagraphs(str);
      std::vector<Paragraph> out;

      for (auto simpleParagraph : paragraphs) {
        std::vector<std::string_view> words = splitInWords(simpleParagraph);

        Paragraph paragraph;

        if (align == Alignment::None) {
          ParagraphLine line;
          line.words = std::move(words);
          line.indent = 0.0f;
          line.spacing = spaceWidth;
          paragraph.lines.push_back(std::move(line));
        } else {
          ParagraphLine currentLine;
          float currentWidth = 0.0f;

          for (auto word : words) {
            float wordWith = getWordWidth(word, characterSize, font);

            if (!currentLine.words.empty() && currentWidth + spaceWidth + wordWith > paragraphWidth) {
              auto wordCount = currentLine.words.size();

              switch (align) {
                case Alignment::Left:
                  currentLine.indent = 0.0f;
                  currentLine.spacing = spaceWidth;
                  break;

                case Alignment::Right:
                  currentLine.indent = paragraphWidth - currentWidth;
                  currentLine.spacing = spaceWidth;
                  break;

                case Alignment::Center:
                  currentLine.indent = (paragraphWidth - currentWidth) / 2;
                  currentLine.spacing = spaceWidth;
                  break;

                case Alignment::Justify:
                  currentLine.indent = 0.0f;

                  if (wordCount > 1) {
                    currentLine.spacing = spaceWidth + (paragraphWidth - currentWidth) / (wordCount - 1);
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
            switch (align) {
              case Alignment::Left:
              case Alignment::Justify:
                currentLine.indent = 0.0f;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Right:
                currentLine.indent = paragraphWidth - currentWidth;
                currentLine.spacing = spaceWidth;
                break;

              case Alignment::Center:
                currentLine.indent = (paragraphWidth - currentWidth) / 2;
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

    void addGlyphVertex(VertexArray& array, const Glyph& glyph, const Vector2f& position) {
      Vertex vertices[4];

      vertices[0].position = position + glyph.bounds.getTopLeft();
      vertices[1].position = position + glyph.bounds.getTopRight();
      vertices[2].position = position + glyph.bounds.getBottomLeft();
      vertices[3].position = position + glyph.bounds.getBottomRight();

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

  } // anonymous namespace

  void BasicText::updateGeometry(VertexArray& vertices, VertexArray& outlineVertices) {
    if (m_font == nullptr || m_characterSize == 0 || m_string.empty()) {
      return;
    }

    vertices.clear();
    outlineVertices.clear();

    m_bounds = RectF();

    float spaceWidth = m_font->getGlyph(' ', m_characterSize).advance;
    float additionalSpace = (spaceWidth / 3) * (m_letterSpacingFactor - 1.0f); // same as SFML even if weird
    spaceWidth += additionalSpace;
    float lineHeight = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;

    std::vector<Paragraph> paragraphs = makeParagraphs(m_string, spaceWidth, m_paragraphWidth, m_align, m_characterSize, *m_font);

    Vector2f position(0.0f, 0.0f);

    Vector2f min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector2f max(0.0f, 0.0f);

    for (auto paragraph : paragraphs) {
//       std::printf("Paragraph with %zu lines\n", paragraph.lines.size());

      for (const auto& line : paragraph.lines) {
//         std::printf("\tLine with %zu words\n", line.words.size());
//         std::printf("\t\tindent: %f\n", line.indent);
//         std::printf("\t\tspacing: %f (%f)\n", line.spacing, spaceWidth);

        position.x = line.indent;

        for (auto word : line.words) {
          char32_t prevCodepoint = '\0';

          for (char32_t currCodepoint : gf::codepoints(word)) {

            position.x += m_font->getKerning(prevCodepoint, currCodepoint, m_characterSize);
            prevCodepoint = currCodepoint;

            if (m_outlineThickness > 0) {
              const Glyph& glyph = m_font->getGlyph(currCodepoint, m_characterSize, m_outlineThickness);

              addGlyphVertex(outlineVertices, glyph, position);

              min = gf::min(min, position + glyph.bounds.getTopLeft());
              max = gf::max(max, position + glyph.bounds.getBottomRight());
            }

            const Glyph& glyph = m_font->getGlyph(currCodepoint, m_characterSize);

            addGlyphVertex(vertices, glyph, position);

            if (m_outlineThickness == 0.0f) {
              min = gf::min(min, position + glyph.bounds.getTopLeft());
              max = gf::max(max, position + glyph.bounds.getBottomRight());
            }

            position.x += glyph.advance + additionalSpace;
          }

          position.x += line.spacing;
        }

        position.y += lineHeight;
      }
    }

    m_bounds = RectF::fromMinMax(min, max);

    if (m_align != Alignment::None && m_paragraphWidth > 0.0f) {
      m_bounds.min.x = 0;
      m_bounds.max.x = m_paragraphWidth;
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
