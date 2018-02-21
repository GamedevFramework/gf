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
#include <gf/Widgets.h>

#include <gf/Color.h>
#include <gf/Shape.h>
#include <gf/Text.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**********************/
  /*     TextWidget     */
  /**********************/

  TextWidget::TextWidget(Text &text)
  : m_text(text)
  , m_textOutlineThickness(0)
  , m_disabledTextColor(Color::Gray())
  , m_disabledTextOutlineColor(Color::Black)
  , m_defaultTextColor(Color::Black)
  , m_defaultTextOutlineColor(Color::White)
  , m_selectedTextColor(Color::Gray())
  , m_selectedTextOutlineColor(Color::White)
  {

  }

  void TextWidget::render(RenderTarget &target, const RenderStates &states) {
    switch(getState()) {
      case WidgetState::Disabled:
        m_text.setColor(m_disabledTextColor);
        m_text.setOutlineColor(m_disabledTextOutlineColor);
        break;
      case WidgetState::Default:
        m_text.setColor(m_defaultTextColor);
        m_text.setOutlineColor(m_defaultTextOutlineColor);
        break;
      case WidgetState::Selected:
        m_text.setColor(m_selectedTextColor);
        m_text.setOutlineColor(m_selectedTextOutlineColor);
        break;
    }

    m_text.setOutlineThickness(m_textOutlineThickness);
    m_text.draw(target, states);
  }

  bool TextWidget::contains(Vector2f coords) {
    return false;
  }

  void TextWidget::setTextOutlineThickness(float thickness) {
    m_textOutlineThickness = thickness;
  }

  /**********************/
  /*  TextShapeWidget  */
  /**********************/

  TextShapeWidget::TextShapeWidget(Text& text, Shape& shape)
  : TextWidget(text)
  , m_shape(shape)
  , m_shapeOutlineThickness(0)
  , m_disabledBackgroundColor(Color::Gray())
  , m_disabledBackgroundOutlineColor(Color::Black)
  , m_defaultBackgroundColor(Color::White)
  , m_defaultBackgroundOutlineColor(Color::Black)
  , m_selectedBackgroundColor(Color::White)
  , m_selectedBackgroundOutlineColor(Color::Gray())
  {

  }

  void TextShapeWidget::render(RenderTarget &target, const RenderStates &states) {
    switch(getState()) {
      case WidgetState::Disabled:
        m_shape.setColor(m_disabledBackgroundColor);
        m_shape.setOutlineColor(m_disabledBackgroundOutlineColor);
        break;
      case WidgetState::Default:
        m_shape.setColor(m_defaultBackgroundColor);
        m_shape.setOutlineColor(m_defaultBackgroundOutlineColor);
        break;
      case WidgetState::Selected:
        m_shape.setColor(m_selectedBackgroundColor);
        m_shape.setOutlineColor(m_selectedBackgroundOutlineColor);
        break;
    }

    m_shape.setOutlineThickness(m_shapeOutlineThickness);
    m_shape.draw(target, states);

    // draw text over background
    TextWidget::render(target, states);
  }

  bool TextShapeWidget::contains(Vector2f coords) {
    return false;
  }

  void TextShapeWidget::setBackgroundOutlineThickness(float thickness) {
    m_shapeOutlineThickness = thickness;
  }

  void TextShapeWidget::setDisabledBackgroundColor(const Color4f &color) {
    m_disabledBackgroundColor = color;
  }

  void TextShapeWidget::setDisabledBackgroundOutlineColor(const Color4f &color) {
    m_disabledBackgroundOutlineColor = color;
  }

  void TextShapeWidget::setDefaultBackgroundColor(const Color4f &color) {
    m_defaultBackgroundColor = color;
  }

  void TextShapeWidget::setDefaultBackgroundOutlineColor(const Color4f &color) {
    m_defaultBackgroundOutlineColor = color;
  }

  void TextShapeWidget::setSelectedBackgroundColor(const Color4f &color) {
    m_selectedBackgroundColor = color;
  }

  void TextShapeWidget::setSelectedBackgroundOutlineColor(const Color4f &color) {
    m_selectedBackgroundOutlineColor = color;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
