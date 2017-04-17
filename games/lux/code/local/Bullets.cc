/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016-2017  Lux team
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
#include "Bullets.h"

#include <algorithm>

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Messages.h"
#include "Ship.h"
#include "World.h"

namespace lux {

  Bullets::Bullets(gf::MessageManager& messages, gf::ResourceManager &resources)
  : m_bulletBlueTexture(resources.getTexture("bullet_blue.png"))
  , m_bulletGreenTexture(resources.getTexture("bullet_green.png"))
  , m_bulletYellowTexture(resources.getTexture("bullet_yellow.png"))
  , m_bulletPurpleTexture(resources.getTexture("bullet_purple.png"))
  {
    messages.registerHandler<LocationMessage>(&Bullets::onLocation, this);
    messages.registerHandler<ShootMessage>(&Bullets::onShoot, this);
  }

  void Bullets::addBullet(Origin origin, ShipClass shipClass, gf::Vector2f position, gf::Vector2f velocity) {
    Bullet bullet;
    bullet.origin = origin;
    bullet.position = position;
    bullet.velocity = velocity;

    switch (shipClass) {
      case ShipClass::Antlia:
        bullet.texture = &m_bulletBlueTexture;
        break;

      case ShipClass::Bootes:
        bullet.texture = &m_bulletGreenTexture;
        break;

      case ShipClass::Cygnus:
      case ShipClass::Draco:
        bullet.texture = &m_bulletPurpleTexture;
        break;

      case ShipClass::Eridanus:
        bullet.texture = &m_bulletYellowTexture;
        break;
    }

    bullet.active = true;

    m_bullets.push_back(bullet);
  }

  static constexpr float BulletsExtra = 100.0f;

  void Bullets::update(float dt) {
    gf::RectF view(WorldCenter - WorldSize / 2, WorldSize);
    view.extend(BulletsExtra);

    for (Bullet& bullet : m_bullets) {
      bullet.position += bullet.velocity * dt;

      if (!view.contains(bullet.position)) {
        bullet.active = false;
      }
    }

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), [](const Bullet& b) { return !b.active; }), m_bullets.end());
  }

  void Bullets::render(gf::RenderTarget& target) {
    for (Bullet& bullet : m_bullets) {
      gf::Sprite sprite;
      sprite.setTexture(*bullet.texture);
      sprite.setPosition(bullet.position);
      sprite.setAnchor(gf::Anchor::Center);
      target.draw(sprite);
    }
  }

  static constexpr bool isTargetReachedByBullet(gf::Vector2f shipPos, gf::Vector2f bulletPos) {
    return shipPos.x - 30.0f <= bulletPos.x && bulletPos.x <= shipPos.x + 30.0f
        && shipPos.y - 30.0f <= bulletPos.y && bulletPos.y <= shipPos.y + 30.0f;
  }


  gf::MessageStatus Bullets::onLocation(gf::Id id, gf::Message *msg) {
    (void) id;

    auto loc = static_cast<LocationMessage*>(msg);

    for (Bullet& bullet : m_bullets) {
      if (loc->origin == bullet.origin) {
        continue;
      }

      if (isTargetReachedByBullet(loc->position, bullet.position)) {
        loc->ship->impact(10.0f);
        bullet.active = false;
      }
    }

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Bullets::onShoot(gf::Id id, gf::Message *msg) {
    (void) id;

    auto shoot = static_cast<ShootMessage *>(msg);

    addBullet(shoot->origin, shoot->ship, shoot->position, shoot->velocity);

    return gf::MessageStatus::Keep;
  }

}
