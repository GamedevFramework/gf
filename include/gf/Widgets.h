/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 * Copyright (C) 2018 Lilian Franchi
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
#ifndef GF_WIDGETS_H
#define GF_WIDGETS_H

#include "BasicSprite.h"
#include "BasicText.h"
#include "GraphicsApi.h"
#include "Shapes.h"
#include "Vector.h"
#include "VertexArray.h"
#include "Widget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;

  /**
   * @ingroup graphics_widgets
   * @brief A simple text widget
   *
   * @sa gf::Text
   */
  class GF_GRAPHICS_API TextWidget : public Widget {
  public:
    /**
     * @brief Constructor
     *
     * @param text A text to convert to a widget.
     * @param font Font used to draw the text
     * @param characterSize Base size of characters, in pixels
     */
    TextWidget(std::string text, Font& font, unsigned characterSize = 30);

    void draw(RenderTarget &target, const RenderStates& states) override;

    bool contains(Vector2f coords) override;

	  /**
     * @brief Set the text's string
     *
     * The text string is in UTF-8 format.
     * A text's string is empty by default.
     *
     * @param string New string in UTF-8 format
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
    const std::string& getString() const;

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
    Alignment getAlignment() const;

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
    float getParagraphWidth() const;

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
    float getLineSpacing() const;

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
    float getLetterSpacing() const;

    /**
     * @brief Set the character size
     *
     * @param characterSize New character size, in pixels
     */
    void setCharacterSize(unsigned characterSize);

    /**
     * @brief Get the character size
     *
     * @return Size of the characters, in pixels
     *
     * @sa setCharacterSize()
     */
    unsigned getCharacterSize() const;

    /**
     * @brief Set the thickness of the outline.
     *
     * @param thickness The new value of the outline thickness.
     */
    void setTextOutlineThickness(float thickness);

    /**
     * @brief Set the text's color of the button when disabled.
     *
     * @param color The new color to apply
     */
    void setDisabledTextColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when disabled.
     *
     * @param color The new color to apply
     */
    void setDisabledTextOutlineColor(const Color4f &color);

    /**
     * @brief Set the text's color of the button when it's in a normal state.
     *
     * @param color The new color to apply
     */
    void setDefaultTextColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when it's in a normal state.
     *
     * @param color The new color to apply
     */
    void setDefaultTextOutlineColor(const Color4f &color);

    /**
     * @brief Set the text's color of the button when selected.
     *
     * @param color The new color to apply
     */
    void setSelectedTextColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when selected.
     *
     * @param color The new color to apply
     */
    void setSelectedTextOutlineColor(const Color4f &color);


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
      return m_basic.getLocalBounds();
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

  protected:
    void updateCurrentStateColors();
    void updateColors(Color4f textColor, Color4f outlineColor);
    void updateGeometry();

    void onStateChanged() override;

    BasicText& getText() {
      return m_basic;
    }

  private:
    BasicText m_basic;
    VertexArray m_vertices;
    VertexArray m_outlineVertices;

    Color4f m_disabledTextColor;
    Color4f m_disabledTextOutlineColor;

    Color4f m_defaultTextColor;
    Color4f m_defaultTextOutlineColor;

    Color4f m_selectedTextColor;
    Color4f m_selectedTextOutlineColor;
  };

  /**
   * @ingroup graphics_widgets
   * @brief A text within a rounded rectangle widget
   *
   * @sa gf::Text, gf::RoundedRectangleShape
   */
  class GF_GRAPHICS_API TextButtonWidget : public TextWidget {
  public:
    /**
     * @brief Construct a text button widget.
     *
     * @param text The text to bind to the text button.
     * @param font Font used to draw the text
     * @param characterSize Base size of characters, in pixels
     */
    TextButtonWidget(std::string text, Font& font, unsigned characterSize = 30);

    void draw(RenderTarget &target, const RenderStates& states) override;

    bool contains(Vector2f coords) override;

    /**
     * @brief Set the thickness of the outline.
     *
     * @param thickness The new value of the outline thickness.
     */
    void setBackgroundOutlineThickness(float thickness);

    /**
     * @brief Set the background's color of the button when disabled.
     *
     * @param color The new color to apply
     */
    void setDisabledBackgroundColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when disabled.
     *
     * @param color The new color to apply
     */
    void setDisabledBackgroundOutlineColor(const Color4f &color);

    /**
     * @brief Set the background's color of the button when it's in a normal state.
     *
     * @param color The new color to apply
     */
    void setDefaultBackgroundColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when it's in a normal state.
     *
     * @param color The new color to apply
     */
    void setDefaultBackgroundOutlineColor(const Color4f &color);

    /**
     * @brief Set the background's color of the button when selected.
     *
     * @param color The new color to apply
     */
    void setSelectedBackgroundColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when selected.
     *
     * @param color The new color to apply
     */
    void setSelectedBackgroundOutlineColor(const Color4f &color);

    /**
     * @brief Set the radius of the corners
     *
     * @param radius The new radius
     */
    void setRadius(float radius) {
      m_radius = radius;
      updateGeometry();
    }

    /**
     * @brief Set the padding around the text
     *
     * @param padding The new padding
     */
    void setPadding(float padding) {
      m_padding = padding;
      updateGeometry();
    }

  protected:
    void updateGeometry();

    void onStateChanged() override;

  private:
    gf::RoundedRectangleShape m_rect;

    float m_backgroundOutlineThickness;

    Color4f m_disabledBackgroundColor;
    Color4f m_disabledBackgroundOutlineColor;

    Color4f m_defaultBackgroundColor;
    Color4f m_defaultBackgroundOutlineColor;

    Color4f m_selectedBackgroundColor;
    Color4f m_selectedBackgroundOutlineColor;

    float m_radius;
    float m_padding;
  };

  /**
   * @ingroup graphics_widgets
   * @brief A widget with a set of sprites
   */
  class GF_GRAPHICS_API SpriteWidget : public Widget {
  public:
    /**
     * @brief Constructor with no texture
     */
    SpriteWidget() = default;

    /**
     * @brief Constructor with a single texture and three rectangles
     *
     * @param texture The texture used for the widget
     * @param defaultRect The rectangle in the texture used for default state
     * @param selectedRect The rectangle in the texture used for selected state
     * @param disabledRect The rectangle in the texture used for disabled state
     */
    SpriteWidget(const Texture& texture, const RectF& defaultRect, const RectF& selectedRect, const RectF& disabledRect);

    /**
     * @brief Constructor with three full textures
     *
     * @param defaultTexture The texture used for default state
     * @param selectedTexture The texture used for selected state
     * @param disabledTexture The texture used for disabled state
     */
    SpriteWidget(const Texture& defaultTexture, const Texture& selectedTexture, const Texture& disabledTexture);

    void draw(RenderTarget &target, const RenderStates& states) override;

    bool contains(Vector2f coords) override;

    /**
     * @brief Set the texture for disabled state
     *
     * @param texture The texture used for disabled state
     * @param textureRect The rectangle in the texture used for disabled state
     */
    void setDisabledSprite(const Texture& texture, const RectF& textureRect);

    /**
     * @brief Unset the source texture of the disabled sprite
     *
     */
    void unsetDisabledSprite();

    /**
     * @brief Set the texture for default state
     *
     * @param texture The texture used for default state
     * @param textureRect The rectangle in the texture used for default state
     */
    void setDefaultSprite(const Texture& texture, const RectF& textureRect);

    /**
     * @brief Unset the source texture of the default sprite
     *
     */
    void unsetDefaultSprite();

    /**
     * @brief Set the texture for selected state
     *
     * @param texture The texture used for selected state
     * @param textureRect The rectangle in the texture used for selected state
     */
    void setSelectedSprite(const Texture& texture, const RectF& textureRect);

    /**
     * @brief Unset the source texture of the selected sprite
     *
     */
    void unsetSelectedSprite();

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
      return getSprite().getLocalBounds();
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

  private:
    void updateGeometry();

    void onStateChanged() override;

  private:
    BasicSprite& getSprite();
    const BasicSprite& getSprite() const;

  private:
    BasicSprite m_disabledSprite;
    BasicSprite m_defaultSprite;
    BasicSprite m_selectedSprite;
    Vertex m_vertices[4];
  };

  /**
   * @ingroup graphics_widgets
   * @brief A choice sprite widget
   *
   * A choice widget may be used to represent a checkbox or a radio button.
   */
  class GF_GRAPHICS_API ChoiceSpriteWidget : public Widget {
  public:
    /**
     * @brief Constructor
     *
     * @param texture The texture used for the widget
     * @param emptyRect The rectangle in the texture for the empty state
     * @param chosenRect The rectangle in the texture for the chosen state
     */
    ChoiceSpriteWidget(const Texture& texture, const RectF& emptyRect, const RectF& chosenRect);

    /**
     * @brief Constructor
     *
     * @param emptyTexture The texture for the empty state
     * @param chosenTexture The texture for the chosen state
     */
    ChoiceSpriteWidget(const Texture& emptyTexture, const Texture& chosenTexture);

    void draw(RenderTarget &target, const RenderStates& states) override;

    bool contains(Vector2f coords) override;

    /**
     * @brief Set the state of the widget
     *
     * @param chosen The new state of the widget
     *
     * @sa isChosen()
     */
    void setChosen(bool chosen = true);

    /**
     * @brief Check if the widget is in the chosen state
     *
     * @sa setChosen()
     */
    bool isChosen() const noexcept {
      return m_isChosen;
    }

    /**
     * @brief Set the sprite for the empty state
     *
     * @param texture The texture used for the empty state
     * @param textureRect The rectangle in the texture used for the empty state
     *
     * @sa setChosenSprite()
     */
    void setEmptySprite(const Texture& texture, const RectF& textureRect);

    /**
     * @brief Set the sprite for the chosen state
     *
     * @param texture The texture used for the chosen state
     * @param textureRect The rectangle in the texture used for the chosen state
     *
     * @sa setEmptySprite()
     */
    void setChosenSprite(const Texture& texture, const RectF& textureRect);

  protected:
    void triggered() override;

  private:
    void updateGeometry();

  private:
    BasicSprite& getSprite();
    const BasicSprite& getSprite() const;

  private:
    BasicSprite m_empty;
    BasicSprite m_chosen;
    bool m_isChosen;
    Vertex m_vertices[4];
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_WIDGETS_H
