/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Lilian Franchi
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

#include "Shapes.h"
#include "Vector.h"
#include "Widget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;

  class Text;
  class Sprite;

  /**
   * @ingroup graphics
   * @brief A simple text widget
   *
   * @sa gf::Text
   */
  class GF_API TextWidget : public Widget {
  public:

    /**
     * @brief Constructor
     *
     * @param text A text to convert to a widget.
     */
    TextWidget(Text &text);

    virtual void render(RenderTarget &target, const RenderStates &states) override;

    virtual bool contains(Vector2f coords) override;

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

  protected:
    Text& getText() {
      return *m_text;
    }

  private:
    Text *m_text;
    float m_textOutlineThickness;

    Color4f m_disabledTextColor;
    Color4f m_disabledTextOutlineColor;

    Color4f m_defaultTextColor;
    Color4f m_defaultTextOutlineColor;

    Color4f m_selectedTextColor;
    Color4f m_selectedTextOutlineColor;
  };

  /**
   * @ingroup graphics
   * @brief A text within a representative shape widget
   *
   * @sa gf::Text, gf::Shape
   */
  class GF_API TextShapeWidget : public TextWidget {
  public:
    /**
     * @brief Construct a text button widget.
     *
     * @param text The text to bind to the text button.
     * @param shape The shape for the background of the button
     */
    TextShapeWidget(Text &text, Shape& shape);

    virtual void render(RenderTarget &target, const RenderStates &states) override;

    virtual bool contains(Vector2f coords) override;

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

  private:
    Shape *m_shape;
    float m_shapeOutlineThickness;

    Color4f m_disabledBackgroundColor;
    Color4f m_disabledBackgroundOutlineColor;

    Color4f m_defaultBackgroundColor;
    Color4f m_defaultBackgroundOutlineColor;

    Color4f m_selectedBackgroundColor;
    Color4f m_selectedBackgroundOutlineColor;
  };

  /**
   * @ingroup graphics
   * @brief A text within a rounded rectangle widget
   *
   * @sa gf::Text, gf::RoundedRectangleShape
   */
  class GF_API TextButtonWidget : public TextShapeWidget {
  public:
    /**
     * @brief Constructor
     */
    TextButtonWidget(Text& text);

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

  private:
    void updateGeometry();

  private:
    gf::RoundedRectangleShape m_rect;
    float m_radius;
    float m_padding;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_WIDGETS_H
