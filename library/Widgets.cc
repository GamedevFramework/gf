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

#include <cassert>

#include <gf/Color.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    template<typename T>
    bool isInsideBounds(Vector2f coords, const T *shape) {
      Vector2f local = gf::transform(shape->getInverseTransform(), coords);
      return shape->getLocalBounds().contains(local);
    }

  }

  /*
   * TextWidget
   */

  TextWidget::TextWidget(Text &text)
  : m_text(&text)
  , m_textOutlineThickness(0)
  , m_disabledTextColor(Color::Gray(0.8f))
  , m_disabledTextOutlineColor(Color::Gray())
  , m_defaultTextColor(Color::Black)
  , m_defaultTextOutlineColor(Color::White)
  , m_selectedTextColor(Color::Gray())
  , m_selectedTextOutlineColor(Color::White)
  {

  }

  void TextWidget::render(RenderTarget &target, const RenderStates &states) {
    assert(m_text);

    switch(getState()) {
      case WidgetState::Disabled:
        m_text->setColor(m_disabledTextColor);
        m_text->setOutlineColor(m_disabledTextOutlineColor);
        break;
      case WidgetState::Default:
        m_text->setColor(m_defaultTextColor);
        m_text->setOutlineColor(m_defaultTextOutlineColor);
        break;
      case WidgetState::Selected:
        m_text->setColor(m_selectedTextColor);
        m_text->setOutlineColor(m_selectedTextOutlineColor);
        break;
    }

    m_text->setOutlineThickness(m_textOutlineThickness);
    m_text->draw(target, states);
  }

  bool TextWidget::contains(Vector2f coords) {
    return isInsideBounds(coords, m_text);
  }

  void TextWidget::setTextOutlineThickness(float thickness) {
    m_textOutlineThickness = thickness;
  }

  void TextWidget::setDisabledTextColor(const Color4f &color) {
    m_disabledTextColor = color;
  }

  void TextWidget::setDisabledTextOutlineColor(const Color4f &color) {
    m_disabledTextOutlineColor = color;
  }

  void TextWidget::setDefaultTextColor(const Color4f &color) {
    m_defaultTextColor = color;
  }

  void TextWidget::setDefaultTextOutlineColor(const Color4f &color) {
    m_defaultTextOutlineColor = color;
  }

  void TextWidget::setSelectedTextColor(const Color4f &color) {
    m_selectedTextColor = color;
  }

  void TextWidget::setSelectedTextOutlineColor(const Color4f &color) {
    m_selectedTextOutlineColor = color;
  }

  /*
   * TextShapeWidget
   */

  TextShapeWidget::TextShapeWidget(Text& text, Shape& shape)
  : TextWidget(text)
  , m_shape(&shape)
  , m_shapeOutlineThickness(0)
  , m_disabledBackgroundColor(Color::Gray(0.95f))
  , m_disabledBackgroundOutlineColor(Color::Gray())
  , m_defaultBackgroundColor(Color::White)
  , m_defaultBackgroundOutlineColor(Color::Black)
  , m_selectedBackgroundColor(Color::White)
  , m_selectedBackgroundOutlineColor(Color::Gray())
  {

  }

  void TextShapeWidget::render(RenderTarget &target, const RenderStates &states) {
    assert(m_shape);

    switch(getState()) {
      case WidgetState::Disabled:
        m_shape->setColor(m_disabledBackgroundColor);
        m_shape->setOutlineColor(m_disabledBackgroundOutlineColor);
        break;
      case WidgetState::Default:
        m_shape->setColor(m_defaultBackgroundColor);
        m_shape->setOutlineColor(m_defaultBackgroundOutlineColor);
        break;
      case WidgetState::Selected:
        m_shape->setColor(m_selectedBackgroundColor);
        m_shape->setOutlineColor(m_selectedBackgroundOutlineColor);
        break;
    }

    m_shape->setOutlineThickness(m_shapeOutlineThickness);
    m_shape->draw(target, states);

    // draw text over background
    TextWidget::render(target, states);
  }

  bool TextShapeWidget::contains(Vector2f coords) {
    return isInsideBounds(coords, m_shape);
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

  /*
   * TextButtonWidget
   */

  TextButtonWidget::TextButtonWidget(Text& text)
  : TextShapeWidget(text, m_rect)
  , m_radius(0)
  , m_padding(0)
  {
    updateGeometry();
  }

  void TextButtonWidget::updateGeometry() {
    RectF bounds = getText().getLocalBounds().extend(m_padding);
    m_rect.setSize(bounds.getSize());
    m_rect.setOrigin(getText().getOrigin() + m_padding);

    Vector2f position = getText().getPosition();
    Matrix3f matrix = gf::rotation(getText().getRotation(), position) * gf::scaling(getText().getScale(), position);

    m_rect.setPosition(gf::transform(matrix, position + bounds.getPosition() + m_padding));
    m_rect.setRotation(getText().getRotation());
    m_rect.setScale(getText().getScale());
    m_rect.setRadius(m_radius);
  }

  /*
   * SpriteWidget
   */

  SpriteWidget::SpriteWidget(Sprite& sprite)
  : m_disabledSprite(&sprite)
  , m_defaultSprite(&sprite)
  , m_selectedSprite(&sprite)
  {

  }

  SpriteWidget::SpriteWidget(Sprite& defaultSprite, Sprite& selectedSprite)
  : m_disabledSprite(&defaultSprite)
  , m_defaultSprite(&defaultSprite)
  , m_selectedSprite(&selectedSprite)
  {

  }

  SpriteWidget::SpriteWidget(Sprite& defaultSprite, Sprite& selectedSprite, Sprite& disabledSprite)
  : m_disabledSprite(&disabledSprite)
  , m_defaultSprite(&defaultSprite)
  , m_selectedSprite(&selectedSprite)
  {

  }

  void SpriteWidget::setDisabledSprite(Sprite& sprite) {
    m_disabledSprite = &sprite;
  }

  void SpriteWidget::setDefaultSprite(Sprite& sprite) {
    m_defaultSprite = &sprite;
  }

  void SpriteWidget::setSelectedSprite(Sprite& sprite) {
    m_selectedSprite = &sprite;
  }

  void SpriteWidget::render(RenderTarget &target, const RenderStates &states) {
    switch(getState()) {
      case WidgetState::Disabled:
        assert(m_disabledSprite);
        m_disabledSprite->draw(target, states);
        break;
      case WidgetState::Default:
        assert(m_defaultSprite);
        m_defaultSprite->draw(target, states);
        break;
      case WidgetState::Selected:
        assert(m_selectedSprite);
        m_selectedSprite->draw(target, states);
        break;
    }
  }

  bool SpriteWidget::contains(Vector2f coords) {
    switch(getState()) {
      case WidgetState::Disabled:
        assert(m_disabledSprite);
        return isInsideBounds(coords, m_disabledSprite);
      case WidgetState::Default:
        assert(m_defaultSprite);
        return isInsideBounds(coords, m_defaultSprite);
      case WidgetState::Selected:
        assert(m_selectedSprite);
        return isInsideBounds(coords, m_selectedSprite);
    }

    assert(false);
    return false;
  }

  /*
   *
   */

  ChoiceSpriteWidget::ChoiceSpriteWidget(Sprite& empty, Sprite& chosen)
  : SpriteWidget(empty)
  , m_empty(&empty)
  , m_chosen(&chosen)
  , m_isChosen(false)
  {

  }

  void ChoiceSpriteWidget::setChosen(bool chosen) {
    if (chosen != m_isChosen) {
      m_isChosen = chosen;
      updateSprites();
    }
  }

  void ChoiceSpriteWidget::setEmptySprite(Sprite& sprite) {
    m_empty = &sprite;
  }

  void ChoiceSpriteWidget::setChosenSprite(Sprite& sprite) {
    m_chosen = &sprite;
  }

  void ChoiceSpriteWidget::triggered() {
    m_isChosen = !m_isChosen;
    updateSprites();
  }

  void ChoiceSpriteWidget::updateSprites() {
    if (!m_isChosen) {
      setDisabledSprite(*m_empty);
      setDefaultSprite(*m_empty);
      setSelectedSprite(*m_empty);
    } else {
      setDisabledSprite(*m_chosen);
      setDefaultSprite(*m_chosen);
      setSelectedSprite(*m_chosen);
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
