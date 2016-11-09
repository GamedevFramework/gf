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
#include "Enemy.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "Messages.h"
#include "World.h"

namespace lux {

  static constexpr float ShootVelocity = 400.0f;
  static constexpr float BonusFrequency = 0.50f;

  Enemy::Enemy(ShipClass ship, gf::Vector2f position, gf::Vector2f velocity, float health, gf::Random& random, gf::MessageManager& messages, gf::ResourceManager &resources)
  : Ship(health)
  , m_ship(ship)
  , m_position(position)
  , m_velocity(velocity)
  , m_random(random)
  , m_messages(messages)
  , m_texture(nullptr)
  {
    switch (ship) {
      case ShipClass::Antlia:
        assert(false);
        break;
      case ShipClass::Bootes:
        m_texture = &resources.getTexture("ship_bootes.png");
        m_shoot = makeSimpleShoot(Origin::Enemy, ShipClass::Bootes, 0.5f);
        break;
      case ShipClass::Cygnus:
        m_texture = &resources.getTexture("ship_cygnus.png");
        m_shoot = makeBurstShoot(Origin::Enemy, ShipClass::Cygnus, 0.75f, 0.1f, 3);
        break;
      case ShipClass::Draco:
        m_texture = &resources.getTexture("ship_draco.png");
        m_shoot = makeConeShoot(Origin::Enemy, ShipClass::Draco, 0.75f);
        break;
      case ShipClass::Eridanus:
        m_texture = &resources.getTexture("ship_eridanus.png");
        m_shoot = makeContinuousSimpleShoot(Origin::Enemy, ShipClass::Eridanus, 0.5f);
        break;
    }

    assert(m_texture != nullptr);
  }


  void Enemy::update(float dt) {
    assert(isAlive());

    m_position += m_velocity * dt;

    if (m_position.y > WorldCenter.y + WorldSize.height / 2 + Height) {
      kill();
      assert(!isAlive());
      return;
    }

    LocationMessage loc;
    loc.origin = Origin::Enemy;
    loc.position = m_position;
    loc.ship = this;
    m_messages.sendMessage(&loc);

    if (isDamaged()) {
      DeadMessage dead;
      dead.origin = Origin::Enemy;
      dead.ship = m_ship;
      dead.position = m_position;
      m_messages.sendMessage(&dead);
      kill();

      // Generate the bonus

      if (m_random.computeBernoulli(BonusFrequency)) {
        DropBonusMessage bonus;
        bonus.position = m_position;
        m_messages.sendMessage(&bonus);
      }

      return;
    }

    gf::Vector2f dir = m_heroPos - m_position;
    dir = gf::normalize(dir) * ShootVelocity;

    m_shoot->shoot(dt, m_position, dir, m_messages);
  }

  void Enemy::render(gf::RenderTarget& target) {
    gf::Sprite sprite;
    sprite.setTexture(*m_texture);
    sprite.setScale({ ScaleX, ScaleY });
    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);
    sprite.setRotation(-gf::Pi2);
    target.draw(sprite);
  }


  EnemyManager::EnemyManager(gf::Random& random, gf::MessageManager& messages, gf::ResourceManager &resources)
  : m_random(random)
  , m_messages(messages)
  , m_resources(resources)
  {
    m_messages.registerHandler<LocationMessage>(&EnemyManager::onLocation, this);
  }

  EnemyManager::~EnemyManager() {
    for (auto enemy : m_enemies) {
      delete enemy;
    }
  }

  void EnemyManager::addEnemy(ShipClass ship, gf::Vector2f position, gf::Vector2f velocity) {
    auto enemy = new Enemy(ship, position, velocity, 15.0f, m_random, m_messages, m_resources);
    m_enemies.push_back(enemy);
  }

  gf::MessageStatus EnemyManager::onLocation(gf::Id id, gf::Message *msg) {
    assert(id == LocationMessage::type);
    auto loc = static_cast<LocationMessage*>(msg);

    if (loc->origin == Origin::Hero) {
      m_heroPos = loc->position;
    }

    return gf::MessageStatus::Keep;
  }

  void EnemyManager::update(float dt) {
    for (auto enemy : m_enemies) {
      enemy->setHeroPosition(m_heroPos);
      enemy->update(dt);
    }

    const auto trash = std::partition(m_enemies.begin(), m_enemies.end(), [](const Enemy *e) {
      return e->isAlive();
    });

    for (auto it = trash; it != m_enemies.end(); ++it) {
      assert(!(*it)->isAlive());
      delete *it;
      *it = nullptr;
    }

    m_enemies.erase(trash, m_enemies.end());
  }

  void EnemyManager::render(gf::RenderTarget& target) {
    for (auto enemy : m_enemies) {
      enemy->render(target);
    }
  }


}
