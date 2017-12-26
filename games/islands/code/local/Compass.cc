/*
 * Bygone Islands, the journey of a pirate in dangerous islands
 * Copyright (C) 2017  Hatunruna team
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
#include "Compass.h"

#include <iostream>

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Curves.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr gf::Time ScanCooldown = gf::seconds(6.0f);
  static constexpr gf::Time AngleChange = gf::seconds(0.5f);
  static constexpr float CompassSize = 80.0f;
  static constexpr float PointerSize = 80.0f;
  static constexpr float SpriteSize = 256.0f;

  static constexpr gf::Time TimeBetweenRadars = gf::seconds(1.5f);
  static constexpr float RadarRadiusIncrease = 100.0f;
  static constexpr gf::Time RadarLifetime = gf::seconds(5.0f);

  Compass::Compass()
  : gf::Entity(15)
  , m_position(0.0f, 0.0f)
  , m_angle(0.0f)
  , m_angleCurrent(0.0f)
  , m_angleRange(0.0f)
  , m_angleActivity(0.0f, 0.0f, m_angleCurrent, AngleChange, gf::Ease::bounceInOut)
  , m_timeElapsed() // 0
  , m_displayed(false)
  , m_texture(gTextureAtlas().getTexture())
  , m_compass(gTextureAtlas().getTextureRect("compass"))
  , m_pointer(gTextureAtlas().getTextureRect("pointer"))
  , m_timeUntilNextRadar() // 0
  {
    // Register message
    gMessageManager().registerHandler<HeroPosition>(&Compass::onHeroPosition, this);
    gMessageManager().registerHandler<StartScan>(&Compass::onStartScan, this);
    gMessageManager().registerHandler<NearestTreasure>(&Compass::onNearestTreasure, this);
  }

  void Compass::update(gf::Time time) {
    if (!m_displayed) {
      return;
    }

    float dt = time.asSeconds();
    m_timeElapsed += time;

    if (m_timeElapsed > ScanCooldown) {
      m_timeElapsed = gf::Time::zero();
      m_displayed = false;
      StopScan message;
      gMessageManager().sendMessage(&message);
    }

    auto status = m_angleActivity.run(time);

    if (status == gf::ActivityStatus::Finished) {
      m_angleRange /= 1.2f;
      m_angleActivity.setOrigin(m_angleActivity.getTarget());
      m_angleActivity.setTarget(gRandom().computeUniformFloat(m_angle - m_angleRange, m_angle + m_angleRange));
      m_angleActivity.restart();
    }

    // radars

    while (!m_radars.empty() && m_radars.front().lifetime < gf::Time::zero()) {
      m_radars.pop_front();
    }

    m_timeUntilNextRadar -= time;

    if (m_timeUntilNextRadar < gf::Time::zero()) {
      m_timeUntilNextRadar += TimeBetweenRadars;

      Radar radar;
      radar.radius = CompassSize / 2;
      radar.lifetime = RadarLifetime;

      m_radars.push_back(radar);
    }

    for (auto& radar : m_radars) {
      radar.radius += RadarRadiusIncrease * dt;
      radar.lifetime -= time;
    }
  }

  void Compass::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (!m_displayed) {
      return;
    }

    gf::Color4f radarColor = gf::Color::fromRgba32(0x4C, 0x3C, 0x34);

    gf::CircleShape circle;
    circle.setColor(gf::Color::Transparent);
    circle.setOutlineThickness(2.0f);
    circle.setPosition(m_position);

    // Draw the radar
    for (const auto& radar : m_radars) {
      radarColor.a = radar.lifetime.asSeconds() / RadarLifetime.asSeconds();
      circle.setOutlineColor(radarColor);
      circle.setRadius(radar.radius);
      circle.setAnchor(gf::Anchor::Center);
      target.draw(circle, states);
    }

    // Draw the background
    gf::Sprite compass;
    compass.setTexture(m_texture);
    compass.setTextureRect(m_compass);
    compass.setScale(CompassSize / SpriteSize);
    compass.setPosition(m_position);
    compass.setAnchor(gf::Anchor::Center);
    target.draw(compass, states);

    // Draw the pointer
    gf::Sprite pointer;
    pointer.setTexture(m_texture);
    pointer.setTextureRect(m_pointer);
    pointer.setScale(PointerSize / SpriteSize);
    pointer.setPosition(m_position);
    pointer.setRotation(m_angleCurrent);
    pointer.setAnchor(gf::Anchor::Center);
    target.draw(pointer, states);
  }


  gf::MessageStatus Compass::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    gf::unused(id);
    gf::unused(msg);

    m_displayed = true;
    m_radars.clear();
    m_timeUntilNextRadar = gf::Time::zero();

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Compass::onNearestTreasure(gf::Id id, gf::Message *msg) {
    assert(id == NearestTreasure::type);
    gf::unused(id);

    auto nearestTreasure = static_cast<NearestTreasure*>(msg);
    m_angle = gf::angle(nearestTreasure->position - m_position);

    m_angleRange = gf::Pi2;
    m_angleActivity.setOrigin(gRandom().computeUniformFloat(m_angle - m_angleRange, m_angle + m_angleRange));
    m_angleActivity.setTarget(gRandom().computeUniformFloat(m_angle - m_angleRange, m_angle + m_angleRange));
    m_angleActivity.restart();

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Compass::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    gf::unused(id);

    auto hero = static_cast<HeroPosition*>(msg);
    m_position = hero->position;
    return gf::MessageStatus::Keep;
  }

}
