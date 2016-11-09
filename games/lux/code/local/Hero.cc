/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016  Lux team
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

#include <cassert>

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "Messages.h"
#include "World.h"

namespace lux {

  static constexpr float HealthWidth = 200.0f;
  static constexpr float HealthHeight = 5.0f;
  static constexpr float HealthRadius = 2.0f;
  static constexpr float HealthThickness = 1.0f;
  static constexpr float HealthPadding = 40.0f;

  static constexpr float ScorePadding = 40.0f;

  HeroProperties::HeroProperties(gf::WindowGeometryTracker& tracker, gf::ResourceManager& resources)
  : gf::Entity(2)
  , m_tracker(tracker)
  , m_score(0)
  , m_healthPercent(1.0f)
  , m_font(resources.getFont("jupiter.ttf"))
  {

  }

  void HeroProperties::render(gf::RenderTarget& target) {
    gf::Vector2f position;
    position.x = m_tracker.getXCentered(HealthWidth);
    position.y = m_tracker.getYFromBottom(HealthPadding + HealthHeight);

    gf::RoundedRectangleShape healthBg({ HealthWidth, HealthHeight }, HealthRadius);
    healthBg.setPosition(position);
    healthBg.setColor(gf::Color::Transparent);
    healthBg.setOutlineColor(gf::Color::White);
    healthBg.setOutlineThickness(HealthThickness);
    target.draw(healthBg);

    gf::RoundedRectangleShape healthFg({ m_healthPercent * HealthWidth, HealthHeight }, HealthRadius);
    healthFg.setPosition(position);
    healthFg.setColor(gf::Color::Red);
    target.draw(healthFg);

    position.x += HealthWidth + ScorePadding;

    gf::Text score;
    score.setCharacterSize(24);
    score.setColor(gf::Color::White);
    score.setFont(m_font);
    score.setString("Score: " + std::to_string(m_score));
    score.setPosition(position);
    score.setAnchor(gf::Anchor::TopLeft);
    target.draw(score);
  }



  static constexpr float ShootPeriod = 0.4f;
  static constexpr float ShootVelocity = -400.0f;

  Hero::Hero(HeroProperties& prop, gf::MessageManager& messages, gf::ResourceManager& resources)
  : Ship(Health)
  , m_prop(prop)
  , m_messages(messages)
  , m_elapsedTime(0.0f)
  , m_targetPos(0.0f, 0.0f)
  , m_position(0.0f, 0.0f)
  , m_inGame(true)
  , m_weaponLevel(1)
  , m_shoot(makeSimplePlayerShoot(Origin::Hero, ShipClass::Antlia, m_weaponLevel, 0.1f, 0.5f))
  , m_texture(resources.getTexture("ship_antlia.png"))
  {
    m_messages.registerHandler<DeadMessage>(&Hero::onDead, this);
    m_messages.registerHandler<RestartGameMessage>(&Hero::onRestartGame, this);
    m_messages.registerHandler<WinGameMessage>(&Hero::onWinGame, this);
  }

  void Hero::setPosition(gf::Vector2f position) {
    m_targetPos = gf::clamp(position, WorldCenter - WorldSize / 2, WorldCenter + WorldSize / 2);
  }

  void Hero::update(float dt) {
    if (!m_inGame) {
      return;
    }

    gf::Vector2f diffPos = m_targetPos - m_position;

    if (std::abs(diffPos.x) > 0.1f || std::abs(diffPos.y) > 0.1f) {
      float distance = gf::euclideanLength(diffPos);
      float move = Speed * dt;

      if (distance < move) {
        m_position = m_targetPos;
      } else {
        m_position += diffPos / distance * move;
      }
    }

    LocationMessage loc;
    loc.origin = Origin::Hero;
    loc.position = m_position;
    loc.ship = this;
    m_messages.sendMessage(&loc);

    ScoreMessage score;
    score.score = m_prop.getScore();
    m_messages.sendMessage(&score);

    if (isDamaged()) {
      m_prop.setHealthPercent(0);

      DeadMessage dead;
      dead.origin = Origin::Hero;
      dead.ship = ShipClass::Antlia;
      dead.position = m_position;
      m_messages.sendMessage(&dead);
      m_inGame = false;
      return;
    }

    m_prop.setHealthPercent(getStructureHealthPercentage());

    m_elapsedTime += dt;

    gf::Vector2f dir(0.0f, ShootVelocity);
    m_shoot->shoot(dt, m_position, dir, m_messages);
  }

  void Hero::render(gf::RenderTarget& target) {
    if (isDamaged()) {
      return;
    }

    gf::Sprite sprite;
    sprite.setTexture(m_texture);
    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);
    sprite.setScale({ ScaleX, ScaleY });
    sprite.setRotation(-gf::Pi2);
    target.draw(sprite);
  }

  void Hero::upgradeWeapon() {
    if (m_weaponLevel > 5) {
      m_prop.addToScore(100);
    } else {
      m_shoot = makeSimplePlayerShoot(Origin::Hero, ShipClass::Antlia, ++m_weaponLevel, 0.1f, 0.5f);
    }
  }

  gf::MessageStatus Hero::onDead(gf::Id id, gf::Message *msg) {
    (void) id;

    auto dead = static_cast<DeadMessage *>(msg);

    if (dead->origin != Origin::Enemy) {
      return gf::MessageStatus::Keep;
    }

    switch (dead->ship) {
      case ShipClass::Antlia:
        break;

      case ShipClass::Bootes:
        m_prop.addToScore(30);
        break;

      case ShipClass::Cygnus:
        m_prop.addToScore(100);
        break;

      case ShipClass::Draco:
        m_prop.addToScore(250);
        break;

      case ShipClass::Eridanus:
        m_prop.addToScore(500);
        break;

      default:
        assert(false);
    }

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onRestartGame(gf::Id id, gf::Message *msg) {
    (void) id;
    (void) msg;

    m_inGame = true;
    m_weaponLevel = 1;
    m_shoot = makeSimplePlayerShoot(Origin::Hero, ShipClass::Antlia, m_weaponLevel, 0.1f, 0.5f);
    m_prop.resetScore();

    restore();

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onWinGame(gf::Id id, gf::Message *msg) {
    (void) id;

    auto winEvent = static_cast<WinGameMessage *>(msg);

    m_prop.addToScore(winEvent->bonusScore);

    return gf::MessageStatus::Keep;
  }

}
