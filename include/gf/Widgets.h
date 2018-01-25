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

#include "RenderTarget.h"
#include "Text.h"
#include "Widget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A simple TextWidget
   */
  class GF_API TextWidget : public Widget {
  public:

    /**
     * @brief Constructor
     *
     * @param text A text to convert to a widget.
     */
    TextWidget(Text &text);

    /**
     * @brief Render the widget
     *
     * @param target The render target
     * @param states The render states to use.
     */
    virtual void render(RenderTarget &target, const RenderStates &states) override;

    /**
     * @brief Gives the global bounds of the widget
     *
     * @return RectF containing position and the size of the widget.
     */
    virtual RectF getGlobalBounds() override;

  protected:
    Text& m_text;

    Color4f m_disabledTextColor;
    Color4f m_disabledOutlineColor;

    float m_disabledOutlineThickness;
  };

  /**
   * @ingroup graphics
   * @brief A text button representative widget.
   *
   * @sa gf::Text gf::TextWidget.
   */
  class GF_API TextButtonWidget : public TextWidget {
  public:
    /**
     * @brief Construct a text button widget.
     *
     * @param text The text to bind to the text button.
     */
    TextButtonWidget(Text &text);

    /**
     * @brief Render the widget on the target
     *
     * @param target The render target
     * @param states The render states to use for drawing
     */
    virtual void render(RenderTarget &target, const RenderStates &states) override;

    /**
     * @brief Get the global bounds of the widget
     *
     * @return RectF containing position and size of the widget
     */
    virtual RectF getGlobalBounds() override;

    /**
     * @brief Set the radius of the button.
     *
     * 0 disable the outline and make the button rectangle.
     *
     * @param radius The new value of the radius to apply.
     */
    void setRadius(const float radius);

    /**
     * @brief Set the thickness of the outline.
     *
     * @param thickness The new value of the outline thickness.
     */
    void setOutlineThickness(const float thickness);

    /**
     * @brief Set the background's color of the button when disabled.
     *
     * @param color The new color to apply
     */
    void setDisabledBackgroundColor(const Color4f &color);

    /**
     * @brief Set the background's color of the button when it's in a normal state.
     *
     * @param color The new color to apply
     */
    void setDefaultBackgroundColor(const Color4f &color);

    /**
     * @brief Set the background's color of the button when selected.
     *
     * @param color The new color to apply
     */
    void setSelectedBackgroundColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when disabled.
     *
     * @param color The new color to apply
     */
    void setDisabledOutlineColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when it's in a normal state.
     *
     * @param color The new color to apply
     */
    void setDefaultOutlineColor(const Color4f &color);

    /**
     * @brief Set the outline's color of the button when selected.
     *
     * @param color The new color to apply
     */
    void setSelectedOutlineColor(const Color4f &color);

  protected:
    float m_radius;
    float m_outlineThickness;

    Color4f m_disabledBackgroundColor;
    Color4f m_defaultBackgroundColor;
    Color4f m_selectedBackgroundColor;

    Color4f m_disabledOutlineColor;
    Color4f m_defaultOutlineColor;
    Color4f m_selectedOutlineColor;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_WIDGETS_H
