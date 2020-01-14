/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Lilian Franchi
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
#include <gf/RenderTarget.h>
#include <gf/Texture.h>
#include <gf/VectorOps.h>


#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    template<typename T, typename U>
    bool isInsideBounds(Vector2f coords, const T& basic, const U& widget) {
      Vector2f local = gf::transform(widget.getInverseTransform(), coords);
      return basic.getLocalBounds().contains(local);
    }

  }

  /*
   * TextWidget
   */

  TextWidget::TextWidget(std::string text, Font& font, unsigned characterSize)
  : m_basic(std::move(text), font, characterSize)
  , m_vertices(PrimitiveType::Triangles)
  , m_outlineVertices(PrimitiveType::Triangles)
  , m_disabledTextColor(Color::Gray(0.8f))
  , m_disabledTextOutlineColor(Color::Gray())
  , m_defaultTextColor(Color::Black)
  , m_defaultTextOutlineColor(Color::White)
  , m_selectedTextColor(Color::Gray())
  , m_selectedTextOutlineColor(Color::White)
  {
    updateGeometry();
  }

  void TextWidget::draw(RenderTarget &target, const RenderStates& states) {
    if (m_basic.getFont() == nullptr || m_basic.getCharacterSize() == 0) {
      return;
    }

    RenderStates localStates = states;

    localStates.transform *= getTransform();
    localStates.texture = m_basic.getFontTexture();

    if (m_basic.getOutlineThickness() > 0) {
      target.draw(m_outlineVertices, localStates);
    }

    target.draw(m_vertices, localStates);
  }

  bool TextWidget::contains(Vector2f coords) {
    return isInsideBounds(coords, m_basic, *this);
  }

  void TextWidget::setCharacterSize(unsigned characterSize) {
    m_basic.setCharacterSize(characterSize);
    updateGeometry();
  }

  void TextWidget::setTextOutlineThickness(float thickness) {
    m_basic.setOutlineThickness(thickness);
    updateGeometry();
  }

  void TextWidget::setDisabledTextColor(const Color4f &color) {
    m_disabledTextColor = color;

    if (isDisabled()) {
      updateCurrentStateColors();
    }
  }

  void TextWidget::setDisabledTextOutlineColor(const Color4f &color) {
    m_disabledTextOutlineColor = color;

    if (isDisabled()) {
      updateCurrentStateColors();
    }
  }

  void TextWidget::setDefaultTextColor(const Color4f &color) {
    m_defaultTextColor = color;

    if (isDefault()) {
      updateCurrentStateColors();
    }
  }

  void TextWidget::setDefaultTextOutlineColor(const Color4f &color) {
    m_defaultTextOutlineColor = color;

    if (isDefault()) {
      updateCurrentStateColors();
    }
  }

  void TextWidget::setSelectedTextColor(const Color4f &color) {
    m_selectedTextColor = color;

    if (isSelected()) {
      updateCurrentStateColors();
    }
  }

  void TextWidget::setSelectedTextOutlineColor(const Color4f &color) {
    m_selectedTextOutlineColor = color;

    if (isSelected()) {
      updateCurrentStateColors();
    }
  }

  void TextWidget::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  void TextWidget::updateCurrentStateColors() {
    switch(getState()) {
      case WidgetState::Disabled:
        updateColors(m_disabledTextColor, m_disabledTextOutlineColor);
        break;
      case WidgetState::Default:
        updateColors(m_defaultTextColor, m_defaultTextOutlineColor);
        break;
      case WidgetState::Selected:
        updateColors(m_selectedTextColor, m_selectedTextOutlineColor);
        break;
    }
  }

  void TextWidget::updateColors(Color4f textColor, Color4f outlineColor) {
    for (auto& vertex : m_vertices) {
      vertex.color = textColor;
    }

    for (auto& vertex : m_outlineVertices) {
      vertex.color = outlineColor;
    }
  }

  void TextWidget::updateGeometry() {
    m_basic.updateGeometry(m_vertices, m_outlineVertices);
    updateCurrentStateColors();
  }

  void TextWidget::onStateChanged() {
    updateCurrentStateColors();
  }

  /*
   * TextButtonWidget
   */

  TextButtonWidget::TextButtonWidget(std::string text, Font& font, unsigned characterSize)
  : TextWidget(std::move(text), font, characterSize)
  , m_backgroundOutlineThickness(0)
  , m_disabledBackgroundColor(Color::Gray(0.95f))
  , m_disabledBackgroundOutlineColor(Color::Gray())
  , m_defaultBackgroundColor(Color::White)
  , m_defaultBackgroundOutlineColor(Color::Black)
  , m_selectedBackgroundColor(Color::White)
  , m_selectedBackgroundOutlineColor(Color::Gray())
  , m_radius(0)
  , m_padding(0)
  {
    updateGeometry();
  }

  void TextButtonWidget::draw(RenderTarget &target, const RenderStates& states) {
    m_rect.draw(target, states);

    // draw text over background
    TextWidget::draw(target, states);
  }

  bool TextButtonWidget::contains(Vector2f coords) {
    return isInsideBounds(coords, m_rect, m_rect);
  }

  void TextButtonWidget::setBackgroundOutlineThickness(float thickness) {
    m_backgroundOutlineThickness = thickness;
  }

  void TextButtonWidget::setDisabledBackgroundColor(const Color4f &color) {
    m_disabledBackgroundColor = color;
  }

  void TextButtonWidget::setDisabledBackgroundOutlineColor(const Color4f &color) {
    m_disabledBackgroundOutlineColor = color;
  }

  void TextButtonWidget::setDefaultBackgroundColor(const Color4f &color) {
    m_defaultBackgroundColor = color;
  }

  void TextButtonWidget::setDefaultBackgroundOutlineColor(const Color4f &color) {
    m_defaultBackgroundOutlineColor = color;
  }

  void TextButtonWidget::setSelectedBackgroundColor(const Color4f &color) {
    m_selectedBackgroundColor = color;
  }

  void TextButtonWidget::setSelectedBackgroundOutlineColor(const Color4f &color) {
    m_selectedBackgroundOutlineColor = color;
  }

  void TextButtonWidget::updateGeometry() {
    m_rect.setOutlineThickness(m_backgroundOutlineThickness);

    switch(getState()) {
      case WidgetState::Disabled:
        m_rect.setColor(m_disabledBackgroundColor);
        m_rect.setOutlineColor(m_disabledBackgroundOutlineColor);
        break;
      case WidgetState::Default:
        m_rect.setColor(m_defaultBackgroundColor);
        m_rect.setOutlineColor(m_defaultBackgroundOutlineColor);
        break;
      case WidgetState::Selected:
        m_rect.setColor(m_selectedBackgroundColor);
        m_rect.setOutlineColor(m_selectedBackgroundOutlineColor);
        break;
    }

    RectF bounds = getLocalBounds().grow(m_padding);
    m_rect.setSize(bounds.getSize());
    m_rect.setOrigin(getOrigin() + m_padding);

    Vector2f position = getPosition();
    Matrix3f matrix = gf::rotation(getRotation(), position) * gf::scaling(getScale(), position);

    m_rect.setPosition(gf::transform(matrix, position + bounds.getPosition() + m_padding));
    m_rect.setRotation(getRotation());
    m_rect.setScale(getScale());
    m_rect.setRadius(m_radius);
  }

  void TextButtonWidget::onStateChanged() {
    TextWidget::onStateChanged();
    updateGeometry();
  }

  /*
   * SpriteWidget
   */

  SpriteWidget::SpriteWidget(const Texture& texture, const RectF& defaultRect, const RectF& selectedRect, const RectF& disabledRect)
  : m_disabledSprite(texture, disabledRect)
  , m_defaultSprite(texture, defaultRect)
  , m_selectedSprite(texture, selectedRect)
  {
    updateGeometry();
  }

  SpriteWidget::SpriteWidget(const Texture& defaultTexture, const Texture& selectedTexture, const Texture& disabledTexture)
  : m_disabledSprite(defaultTexture)
  , m_defaultSprite(selectedTexture)
  , m_selectedSprite(disabledTexture)
  {
    updateGeometry();
  }

  void SpriteWidget::draw(RenderTarget &target, const RenderStates& states) {
    const BasicSprite& sprite = getSprite();

    if (!sprite.hasTexture()) {
      return;
    }

    RenderStates localStates = states;

    localStates.transform *= getTransform();
    localStates.texture = &sprite.getTexture();
    target.draw(m_vertices, 4, PrimitiveType::TriangleStrip, localStates);
  }

  bool SpriteWidget::contains(Vector2f coords) {
    return isInsideBounds(coords, getSprite(), *this);
  }

  void SpriteWidget::setDisabledSprite(const Texture& texture, const RectF& textureRect) {
    m_disabledSprite.setTexture(texture);
    m_disabledSprite.setTextureRect(textureRect);

    if (isDisabled()) {
      updateGeometry();
    }
  }

  void SpriteWidget::unsetDisabledSprite() {
    m_disabledSprite.unsetTexture();

    if (isDisabled()) {
      updateGeometry();
    }
  }

  void SpriteWidget::setDefaultSprite(const Texture& texture, const RectF& textureRect) {
    m_defaultSprite.setTexture(texture);
    m_defaultSprite.setTextureRect(textureRect);

    if (isDefault()) {
      updateGeometry();
    }
  }

  void SpriteWidget::unsetDefaultSprite() {
    m_defaultSprite.unsetTexture();

    if (isDefault()) {
      updateGeometry();
    }
  }

  void SpriteWidget::setSelectedSprite(const Texture& texture, const RectF& textureRect) {
    m_selectedSprite.setTexture(texture);
    m_selectedSprite.setTextureRect(textureRect);

    if (isSelected()) {
      updateGeometry();
    }
  }

  void SpriteWidget::unsetSelectedSprite() {
    m_selectedSprite.unsetTexture();

    if (isSelected()) {
      updateGeometry();
    }
  }

  void SpriteWidget::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  void SpriteWidget::updateGeometry() {
    getSprite().updateGeometry(m_vertices);
  }

  void SpriteWidget::onStateChanged() {
    updateGeometry();
  }

  BasicSprite& SpriteWidget::getSprite() {
    switch(getState()) {
      case WidgetState::Disabled:
        return m_disabledSprite;
      case WidgetState::Default:
        return m_defaultSprite;
      case WidgetState::Selected:
        return m_selectedSprite;
    }

    assert(false);
    return m_defaultSprite;
  }

  const BasicSprite& SpriteWidget::getSprite() const {
    switch(getState()) {
      case WidgetState::Disabled:
        return m_disabledSprite;
      case WidgetState::Default:
        return m_defaultSprite;
      case WidgetState::Selected:
        return m_selectedSprite;
    }

    assert(false);
    return m_defaultSprite;
  }

  /*
   * ChoiceSpriteWidget
   */

  ChoiceSpriteWidget::ChoiceSpriteWidget(const Texture& texture, const RectF& emptyRect, const RectF& chosenRect)
  : m_empty(texture, emptyRect)
  , m_chosen(texture, chosenRect)
  , m_isChosen(false)
  {
    updateGeometry();
  }

  ChoiceSpriteWidget::ChoiceSpriteWidget(const Texture& emptyTexture, const Texture& chosenTexture)
  : m_empty(emptyTexture)
  , m_chosen(chosenTexture)
  , m_isChosen(false)
  {
    updateGeometry();
  }

  void ChoiceSpriteWidget::draw(RenderTarget &target, const RenderStates& states) {
    const BasicSprite& sprite = getSprite();

    if (!sprite.hasTexture()) {
      return;
    }

    RenderStates localStates = states;

    localStates.transform *= getTransform();
    localStates.texture = &sprite.getTexture();
    target.draw(m_vertices, 4, PrimitiveType::TriangleStrip, localStates);
  }

  bool ChoiceSpriteWidget::contains(Vector2f coords) {
    return isInsideBounds(coords, getSprite(), *this);
  }

  void ChoiceSpriteWidget::setChosen(bool chosen) {
    if (chosen != m_isChosen) {
      m_isChosen = chosen;
      updateGeometry();
    }
  }

  void ChoiceSpriteWidget::setEmptySprite(const Texture& texture, const RectF& textureRect) {
    m_empty.setTexture(texture);
    m_empty.setTextureRect(textureRect);

    if (!isChosen()) {
      updateGeometry();
    }
  }

  void ChoiceSpriteWidget::setChosenSprite(const Texture& texture, const RectF& textureRect) {
    m_chosen.setTexture(texture);
    m_chosen.setTextureRect(textureRect);

    if (isChosen()) {
      updateGeometry();
    }
  }

  void ChoiceSpriteWidget::triggered() {
    m_isChosen = !m_isChosen;
    updateGeometry();
  }

  void ChoiceSpriteWidget::updateGeometry() {
    getSprite().updateGeometry(m_vertices);
  }

  BasicSprite& ChoiceSpriteWidget::getSprite() {
    if (isChosen()) {
      return m_chosen;
    }

    return m_empty;
  }

  const BasicSprite& ChoiceSpriteWidget::getSprite() const {
    if (isChosen()) {
      return m_chosen;
    }

    return m_empty;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
