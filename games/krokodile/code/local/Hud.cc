/*
 * Krokodile
 * Copyright (C) 2018 Hatunruna team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Hud.h"

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace kkd {

  Hud::Hud()
  : gf::Entity(10)
  , m_font(gResourceManager().getFont("Gravity-Regular.otf"))
  , m_texture(gTextureAtlas().getTexture())
  , m_merges(0)
  , m_energyRatio(0)
  , m_lifeRatio(0)
  {
    // register message handler
    gMessageManager().registerHandler<KrokodileState>(&Hud::onKrokodileState, this);
  }

  void Hud::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    static constexpr float Padding = 15.0f;
    static constexpr float RatioWarning = 0.8f;

    gf::Coordinates coords(target);

    gf::Vector2f mergesPosition = coords.getAbsolutePoint({ Padding, Padding}, gf::Anchor::BottomLeft);
    gf::Vector2f pentaPosition = coords.getAbsolutePoint({ Padding, Padding }, gf::Anchor::BottomRight);

    gf::Vector2f heartPosition = mergesPosition;

    unsigned characterSize = coords.getRelativeCharacterSize(0.08f);
    float HudIconsScale = characterSize / 128.0f / 1.25f;

    // merges info

    gf::Sprite mergesSprite(m_texture, gTextureAtlas().getTextureRect("forks"));
    mergesSprite.setPosition(mergesPosition);
    mergesSprite.setScale(HudIconsScale);
    mergesSprite.setAnchor(gf::Anchor::BottomLeft);
    target.draw(mergesSprite, states);

    mergesPosition.x += mergesSprite.getLocalBounds().getWidth() * HudIconsScale + Padding;

    gf::Text mergesText(std::to_string(m_merges), m_font, characterSize);
    mergesText.setColor(gf::Color::White);
    mergesText.setOutlineColor(gf::Color::Black);
    mergesText.setOutlineThickness(characterSize / 30.0f);
    mergesText.setPosition(mergesPosition);
    mergesText.setAnchor(gf::Anchor::BottomLeft);
    target.draw(mergesText, states);

    // energy info

    gf::Sprite heartSprite(m_texture);

    if (m_energyRatio < RatioWarning) {
      heartSprite.setTextureRect(gTextureAtlas().getTextureRect("heart_red"));
    } else {
      heartSprite.setTextureRect(gTextureAtlas().getTextureRect("heart_rose"));
    }

    heartPosition.y -= mergesSprite.getLocalBounds().getHeight() * HudIconsScale + Padding;

    heartSprite.setScale(HudIconsScale);
    heartSprite.setPosition(heartPosition);
    heartSprite.setAnchor(gf::Anchor::BottomLeft);
    target.draw(heartSprite, states);

    // timer

    gf::Sprite clockSprite(m_texture, gTextureAtlas().getTextureRect("clock"));
    clockSprite.setScale(HudIconsScale);
    clockSprite.setPosition({ Padding, Padding });
    target.draw(clockSprite, states);

    gf::Text timer(gf::niceNum(m_clock.getElapsedTime().asSeconds(), 1.0f), m_font, characterSize);
    timer.setColor(gf::Color::White);
    timer.setOutlineColor(gf::Color::Black);
    timer.setOutlineThickness(characterSize / 30.0f);
    timer.setPosition({ 2.0f * Padding + clockSprite.getLocalBounds().getWidth() * HudIconsScale, Padding });
    timer.setAnchor(gf::Anchor::TopLeft);
    target.draw(timer, states);

    // penta

    gf::ConvexShape pentaBackground(5);
    pentaBackground.setPoint(0, { 128.0f * 0.5f, 128.0f * 0.0f });
    pentaBackground.setPoint(1, { 128.0f * 1.0f, 128.0f * 0.28f });
    pentaBackground.setPoint(2, { 128.0f * 0.83f, 128.0f * 1.0f });
    pentaBackground.setPoint(3, { 128.0f * 0.19f, 128.0f * 1.0f });
    pentaBackground.setPoint(4, { 128.0f * 0.0f,  128.0f * 0.28f });

    pentaBackground.setOutlineThickness(5.0f);
    pentaBackground.setOutlineColor(gf::Color::Opaque(0.3f));
    pentaBackground.setColor(gf::Color::Opaque(0.6f));
    pentaBackground.setScale(HudIconsScale * 3.0f);
    pentaBackground.setPosition(pentaPosition);
    pentaBackground.setAnchor(gf::Anchor::BottomRight);
    target.draw(pentaBackground, states);

    gf::Sprite pentaSprite(m_texture, gTextureAtlas().getTextureRect("penta"));
    pentaSprite.setScale(HudIconsScale * 3.0f);
    pentaSprite.setPosition(pentaPosition);
    pentaSprite.setAnchor(gf::Anchor::BottomRight);
    target.draw(pentaSprite, states);

  }

  void Hud::reset() {
    m_clock.restart();
  }

  gf::MessageStatus Hud::onKrokodileState(gf::Id type, gf::Message *msg) {
    assert(type == KrokodileState::type);
    gf::unused(type);

    auto state = static_cast<KrokodileState*>(msg);

    m_energyRatio = state->energyRatio;
    m_lifeRatio = state->lifeRatio;
    m_merges = state->merges;

    return gf::MessageStatus::Keep;
  }

}
