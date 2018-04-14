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
#include "Hero.h"

#include <gf/Math.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Unused.h>

#include "Messages.h"
#include "Singletons.h"

namespace bi {
  static constexpr float HeroAngularVelocity = 1.5f;
  static constexpr float HeroVelocity = 100.0f;

  static constexpr float BoatAngulatVelocity = 0.5f;
  static constexpr float BoatVelocity = 200.0f;

  static constexpr float SpriteSize = 256.0f;
  static constexpr float HeroSize = 64.0f;
  static constexpr float BoatSize = 128.0f;

  static constexpr float StepTime = 0.25f;
  static constexpr float StepAngle = gf::degreesToRadians(10.0f);

  Hero::Hero(Steam& steam, const gf::Vector2f postion)
  : gf::Entity(10)
  , m_steam(steam)
  , m_move(gf::LinearMove::None)
  , m_turn(gf::AngularMove::None)
  , m_position(postion)
  , m_angle(0.0f)
  , m_texture(gTextureAtlas().getTexture())
  , m_tricorn(gTextureAtlas().getTextureRect("tricorn"))
  , m_boat(gTextureAtlas().getTextureRect("boat"))
  , m_timeElapsed(0.0f)
  , m_alternateStep(true)
  , m_isOnIsland(true)
  , m_isFrozen(false)
  , m_isGameOver(false)
  {
    gMessageManager().registerHandler<StartScan>(&Hero::onStartScan, this);
    gMessageManager().registerHandler<StopScan>(&Hero::onStopScan, this);
    gMessageManager().registerHandler<GameOver>(&Hero::onGameOver, this);
  }

  void Hero::sendInitialPosition() const {
    HeroPosition message;
    message.position = m_position;
    gMessageManager().sendMessage(&message);
  }

  bool Hero::isOnIsland() const {
    return m_isOnIsland;
  }

  bool Hero::scanAvailable() const {
    return m_isOnIsland && !m_isFrozen;
  }

  void Hero::update(gf::Time time) {
    if (m_isGameOver) {
      return;
    }

    float dt = time.asSeconds();
    m_timeElapsed += dt;

    if (!m_isFrozen) {
      // Set the new angle
      float angularVelocity = m_isOnIsland ? HeroAngularVelocity : BoatAngulatVelocity;
      m_angle += gf::angularFactor(m_turn) * angularVelocity * dt;

      // Manage the step "animation"
      while (m_timeElapsed > StepTime) {
        m_timeElapsed -= StepTime;
        m_alternateStep = !m_alternateStep;
      }

      // Set the velocity
      float velocity = m_isOnIsland ? HeroVelocity : BoatVelocity;
      float distance = gf::linearFactor(m_move) * velocity * dt;

      // Compute the new position
      m_position += gf::unit(m_angle) * distance;
    }

    if (!m_isOnIsland) {
      m_steam.run();
    } else {
      m_steam.stop();
    }

    // Send the position message
    HeroPosition message;
    message.position = m_position;
    message.angle = m_angle;
    gMessageManager().sendMessage(&message);

    m_isOnIsland = message.isOnIsland;
    m_position = message.position;
  }

  void Hero::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Sprite sprite;
    sprite.setTexture(m_texture);

    if (m_isOnIsland) {
      // Render the step
      float angleRendered = m_angle;

      if (!m_isFrozen && m_move != gf::LinearMove::None) {
        if (m_alternateStep) {
          angleRendered += StepAngle;
        } else {
          angleRendered -= StepAngle;
        }
      }

      sprite.setTextureRect(m_tricorn);
      sprite.setScale(HeroSize / SpriteSize);
      sprite.setRotation(angleRendered - gf::Pi2); // Pi/2 to align the hero front face

    } else {
      sprite.setTextureRect(m_boat);
      sprite.setScale(BoatSize / SpriteSize);
      sprite.setRotation(m_angle);
    }

    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite, states);
  }

  gf::MessageStatus Hero::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    gf::unused(id, msg);
    m_isFrozen = true;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onStopScan(gf::Id id, gf::Message *msg) {
    assert(id == StopScan::type);
    gf::unused(id, msg);
    m_isFrozen = false;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOver::type);
    gf::unused(id, msg);
    m_isGameOver = true;
    return gf::MessageStatus::Keep;
  }

}
