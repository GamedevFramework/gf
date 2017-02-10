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
#ifndef GF_TEXT_H
#define GF_TEXT_H

#include <string>

#include "Alignment.h"
#include "Portability.h"
#include "Transformable.h"
#include "Vector.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Font;

  /**
   * @ingroup graphics
   * @brief Graphical text that can be drawn to a render target
   *
   * gf::Text is a drawable class that allows to easily display
   * some text with custom style and color on a render target.
   *
   * It inherits all the functions from gf::Transformable:
   * position, rotation, scale, origin. It also adds text-specific
   * properties such as the font to use, the character size, the
   * global color and the text to display of course.
   * It also provides convenience functions to calculate the
   * graphical size of the text.
   *
   * gf::Text works in combination with the gf::Font class, which
   * loads and provides the glyphs (visual characters) of a given font.
   *
   * The separation of gf::Font and gf::Text allows more flexibility
   * and better performances: indeed a gf::Font is a heavy resource,
   * and any operation on it is slow (often too slow for real-time
   * applications). On the other side, a gf::Text is a lightweight
   * object which can combine the glyphs data and metrics of a gf::Font
   * to display any text on a render target.
   *
   * It is important to note that the gf::Text instance doesn't
   * copy the font that it uses, it only keeps a reference to it.
   * Thus, a gf::Font must not be destructed while it is
   * used by a gf::Text (i.e. never write a function that
   * uses a local gf::Font instance for creating a text).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * // Declare and load a font
   * gf::Font font;
   * font.loadFromFile("arial.ttf");
   *
   * // Create a text
   * gf::Text text("hello", font);
   * text.setCharacterSize(30);
   * text.setColor(gf::Color::Red);
   *
   * // Draw it
   * rendered.draw(text);
   * ~~~
   *
   * @sa gf::Font
   */
  class GF_API Text : public Transformable {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty text.
     */
    Text();

    /**
     * @brief Construct the text from a string, font and size
     *
     * @param string Text assigned to the string in UTF-8 format
     * @param font Font used to draw the string
     * @param characterSize Base size of characters, in pixels
     */
    Text(std::string string, Font& font, unsigned characterSize = 30);

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

    /**
     * @brief Set the fill color of the text
     *
     * By default, the text's fill color is opaque black.
     * Setting the fill color to a transparent color with an outline
     * will cause the outline to be displayed in the fill area of the text.
     *
     * @param color New fill color of the text
     *
     * @sa getColor()
     */
    void setColor(const Color4f& color);

    /**
     * @brief Get the fill color of the text
     *
     * @return Fill color of the text
     *
     * @sa setColor()
     */
    const Color4f& getColor() const {
      return m_color;
    }

    /**
     * @brief Set the outline color of the text
     *
     * By default, the text's outline color is opaque black.
     *
     * @param color New outline color of the text
     *
     * @sa getOutlineColor()
     */
    void setOutlineColor(const Color4f& color);

    /**
     * @brief Get the outline color of the text
     *
     * @return Outline color of the text
     *
     * @sa setOutlineColor()
     */
    const Color4f& getOutlineColor() const {
      return m_outlineColor;
    }

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

    /**
     * @brief Set the anchor origin of the entity
     *
     * Compute the origin of the entity based on the local bounds and
     * the specified anchor. Internally, this function calls
     * `Transformable::setOrigin()`.
     *
     * @param anchor The anchor of the entity
     * @sa getLocalBounds(), Transformable::setOrigin()
     */
    void setAnchor(Anchor anchor);

    /**
     * @brief Create a buffer with the current geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current geometry
     */
    VertexBuffer commitGeometry() const;

    /**
     * @brief Create a buffer with the current outline geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current outline geometry
     */
    VertexBuffer commitOutlineGeometry() const;

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    void updateGeometry();

  private:
    struct Line {
      std::vector<std::u32string> words;
      float indent = 0.0f;
      float spacing = 0.0f;
    };

    struct Paragraph {
      std::vector<Line> lines;
    };

    float getWordWidth(const std::u32string& word);

    std::vector<Paragraph> makeParagraphs(const std::string& str, float spaceWidth);

  private:
    std::string m_string;
    Font *m_font;
    unsigned m_characterSize;
    Color4f m_color;
    VertexArray m_vertices;
    RectF m_bounds;

    Color4f m_outlineColor;
    float m_outlineThickness;
    VertexArray m_outlineVertices;

    float m_paragraphWidth;
    Alignment m_align;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TEXT_H
