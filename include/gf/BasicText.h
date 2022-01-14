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
#ifndef GF_BASIC_TEXT_H
#define GF_BASIC_TEXT_H

#include <string>

#include "Alignment.h"
#include "GraphicsApi.h"
#include "Vector.h"
#include "VertexArray.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Font;
  class AlphaTexture;

  /**
   * @ingroup graphics_text
   * @brief A basic text
   *
   * You should use gf::Text instead.
   *
   * @sa gf::Text
   */
  class GF_GRAPHICS_API BasicText {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty text.
     */
    BasicText();

    /**
     * @brief Construct the text from a string, font and size
     *
     * @param string Text assigned to the string in UTF-8 format
     * @param font Font used to draw the string
     * @param characterSize Base size of characters, in pixels
     */
    BasicText(std::string string, Font& font, unsigned characterSize = 30);

    /**
     * @brief Set the text's string
     *
     * The text string is in UTF-8 format.
     * A text's string is empty by default.
     *
     * @param string New string in UTF-8 format
     *
     * @sa getString()
     */
    void setString(std::string string);

    /**
     * @brief Get the text's string
     *
     * The text string is in UTF-8 format.
     *
     * @return Text's string
     *
     * @sa setString()
     */
    const std::string& getString() const {
      return m_string;
    }

    /**
     * @brief Set the character size
     *
     * The default size is 30.
     *
     * @param characterSize New character size, in pixels
     *
     * @sa getCharacterSize()
     */
    void setCharacterSize(unsigned characterSize);

    /**
     * @brief Get the character size
     *
     * @return Size of the characters, in pixels
     *
     * @sa setCharacterSize()
     */
    unsigned getCharacterSize() const {
      return m_characterSize;
    }

    /**
     * @brief Set the text's font
     *
     * The `font` argument refers to a font that must
     * exist as long as the text uses it. Indeed, the text
     * doesn't store its own copy of the font, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the font is destroyed and the text tries to
     * use it, the behavior is undefined.
     *
     * @param font New font
     *
     * @sa getFont()
     */
    void setFont(Font& font);

    /**
     * @brief Get the text's font
     *
     * If the text has no font attached, a `nullptr` pointer is returned.
     * The returned pointer is const, which means that you
     * cannot modify the font when you get it from this function.
     *
     * @return Pointer to the text's font
     *
     * @sa setFont()
     */
    const Font *getFont() const {
      return m_font;
    }

    const AlphaTexture *getFontTexture();

    /**
     * @brief Set the thickness of the text's outline
     *
     * By default, the outline thickness is 0.
     *
     * @param thickness New outline thickness, in pixels
     *
     * @sa getOutlineThickness()
     */
    void setOutlineThickness(float thickness);

    /**
     * @brief Get the outline thickness of the text
     *
     * @return Outline thickness of the text, in pixels
     *
     * @sa setOutlineThickness()
     */
    float getOutlineThickness() {
      return m_outlineThickness;
    }

    /**
     * @brief Set the line spacing factor
     *
     * The default spacing between lines is defined by the font.
     * This method enables you to set a factor for the spacing
     * between lines. By default the line spacing factor is 1.
     *
     * @param spacingFactor New line spacing factor
     *
     * @see getLineSpacing()
     *
     */
    void setLineSpacing(float spacingFactor);

     /**
     * @brief Get the size of the line spacing factor
     *
     * @return The size of the line spacing factor
     *
     * @see setLineSpacing
     */
    float getLineSpacing() const {
      return m_lineSpacingFactor;
    }

    /**
     * @brief Set the letter spacing factor
     *
     * The default spacing between letters is defined by the font.
     * This factor doesn't directly apply to the existing
     * spacing between each character, it rather adds a fixed
     * space between them which is calculated from the font
     * metrics and the character size.
     * Note that factors below 1 (including negative numbers) bring
     * characters closer to each other.
     * By default the letter spacing factor is 1.
     *
     * @param spacingFactor New letter spacing factor
     *
     * @see getLetterSpacing()
     */
    void setLetterSpacing(float spacingFactor);

     /**
     * @brief Get the size of the letter spacing factor
     *
     * @returns The size of the letter spacing factor
     *
     * @see setLetterSpacing()
     */
    float getLetterSpacing() const {
      return m_letterSpacingFactor;
    }

    /**
     * @brief Set the paragraph width for aligned text
     *
     * By default, the paragraph width is 0.
     *
     * @param paragraphWidth New paragraph width in pixels
     * @sa getParagraphWidth()
     */
    void setParagraphWidth(float paragraphWidth);

    /**
     * @brief Get the paragraph width
     *
     * @return Paragraph width in pixels
     * @sa setParagraphWidth()
     */
    float getParagraphWidth() const {
      return m_paragraphWidth;
    }

    /**
     * @brief Set the alignement of the text
     *
     * By default, the text is not aligned.
     *
     * @param align New alignement
     * @sa getAlignment()
     */
    void setAlignment(Alignment align);

    /**
     * @brief Get the alignment of the text
     *
     * @return Current alignment of the text
     * @sa setAlignment()
     */
    Alignment getAlignment() const {
      return m_align;
    }

    /**
     * @brief Get the local bounding rectangle of the entity
     *
     * The returned rectangle is in local coordinates, which means
     * that it ignores the transformations (translation, rotation,
     * scale, ...) that are applied to the entity.
     * In other words, this function returns the bounds of the
     * entity in the entity's coordinate system.
     *
     * @return Local bounding rectangle of the entity
     */
    RectF getLocalBounds() const {
      return m_bounds;
    }

    void updateGeometry(VertexArray& vertices, VertexArray& outlineVertices);

  private:
    std::string m_string;
    Font *m_font;

    unsigned m_characterSize;
    float m_outlineThickness;
    float m_lineSpacingFactor;
    float m_letterSpacingFactor;

    float m_paragraphWidth;
    Alignment m_align;

    RectF m_bounds;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BASIC_TEXT_H
