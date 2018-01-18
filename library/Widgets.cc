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

#include <gf/Shapes.h>
#include <gf/Text.h>
#include <gf/Widgets.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**********************/
  /*       Widget       */
  /**********************/
  void Widget::setDisabled() {
    m_state = WidgetState::Disabled;
  }

  void Widget::setDefault() {
    m_state = WidgetState::Default;
  }

  void Widget::setSelected() {
    m_state = WidgetState::Selected;
  }

  void Widget::setCallback(std::function<void()> &callback) {
    m_handler = callback;
  }

  void Widget::trigger() {
    m_handler();
  }

  /**********************/
  /*     TextWidget     */
  /**********************/

  TextWidget::TextWidget(Text &text)
    : m_text(text)
  {}

  void TextWidget::render(RenderTarget &target, const RenderStates &states) {
    m_text.draw(target, states);
  }

  RectF TextWidget::getGlobalBounds() {
    return m_text.getLocalBounds();
  }

  /**********************/
  /*  TextButtonWidget  */
  /**********************/

  TextButtonWidget::TextButtonWidget(Text &text)
    : TextWidget(text)
  {
    m_state = WidgetState::Default;

    m_disabledBackgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
    m_defaultBackgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
    m_selectedBackgroundColor = {0.0f, 0.0f, 0.0f, 0.0f};
    m_disabledOutlineColor = {0.0f, 0.0f, 0.0f, 0.0f};
    m_defaultOutlineColor = {0.0f, 0.0f, 0.0f, 0.0f};
    m_selectedOutlineColor = {0.0f, 0.0f, 0.0f, 0.0f};
  }

  void TextButtonWidget::render(RenderTarget &target, const RenderStates &states) {
    RoundedRectangleShape rectangle;
    rectangle.setSize(m_text.getLocalBounds().getSize());
    rectangle.setRadius(m_radius);
    switch(m_state) {
      case WidgetState::Disabled:
        rectangle.setColor(m_disabledBackgroundColor);
        rectangle.setOutlineColor(m_disabledOutlineColor);
        break;
      case WidgetState::Default:
        rectangle.setColor(m_defaultBackgroundColor);
        rectangle.setOutlineColor(m_defaultOutlineColor);
        break;
      case WidgetState::Selected:
        rectangle.setColor(m_selectedBackgroundColor);
        rectangle.setOutlineColor(m_selectedOutlineColor);
        break;
      default:
        rectangle.setColor({0.0f, 0.0f, 0.0f, 0.0f});
        rectangle.setOutlineColor({0.0f, 0.0f, 0.0f, 0.0f});
        break;
    }
    rectangle.setOutlineThickness(m_outlineThickness);
    rectangle.setPosition(m_text.getPosition());
    rectangle.draw(target, states);

    m_text.draw(target, states);
  }

  RectF TextButtonWidget::getGlobalBounds() {
    RectF bounds;
    bounds.setPosition(m_text.getPosition());
    bounds.setSize(m_text.getLocalBounds().getSize());
    return bounds;
  }

  void TextButtonWidget::setRadius(const float radius) {
    m_radius = radius;
  }

  void TextButtonWidget::setOutlineThickness(const float thickness) {
    m_outlineThickness = thickness;
  }

  void TextButtonWidget::setDisabledBackgroundColor(const Color4f &color) {
    m_disabledBackgroundColor = color;
  }

  void TextButtonWidget::setDefaultBackgroundColor(const Color4f &color) {
    m_defaultBackgroundColor = color;
  }

  void TextButtonWidget::setSelectedBackgroundColor(const Color4f &color) {
    m_selectedBackgroundColor = color;
  }

  void TextButtonWidget::setDisabledOutlineColor(const Color4f &color) {
    m_disabledOutlineColor = color;
  }

  void TextButtonWidget::setDefaultOutlineColor(const Color4f &color) {
    m_defaultOutlineColor = color;
  }

  void TextButtonWidget::setSelectedOutlineColor(const Color4f &color) {
    m_selectedOutlineColor = color;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
