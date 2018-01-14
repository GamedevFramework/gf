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
#include "TurretManager.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float TurretSize = 76.0f;
  static constexpr float SpriteSize = 256.0f;
  static constexpr float TurretTargetRadius = 350.0f;
  static constexpr float HitRadius = 30.0f;
  static constexpr float FireCooldown = 1.0f;
  static constexpr float BulletVelocity = 200.0f;

  TurretManager::TurretManager()
  : gf::Entity(10)
  , m_texture(gTextureAtlas().getTexture())
  , m_rect(gTextureAtlas().getTextureRect("turret"))
  {
    gMessageManager().registerHandler<HeroPosition>(&TurretManager::onHeroPosition, this);
  }

  void TurretManager::addTurret(gf::Vector2f position) {
    Turret turret;
    turret.position = position;
    turret.timeElapsed = 0.0f;
    m_turrets.push_back(turret);
  }

  void TurretManager::update(gf::Time time) {
    float dt = time.asSeconds();

    for (auto &turret: m_turrets) {
      // Update the bullet
      for (auto &bullet: turret.bullets) {
        bullet.position += bullet.velocity * dt;
        bullet.timeElapsed += dt;
        float distance = gf::squareDistance(m_heroPosition, bullet.position);

        if (distance <= HitRadius * HitRadius) {
          // Set hit message here
          bullet.active = false;
          bi::GameOver message;
          gMessageManager().sendMessage(&message);
        } else if (bullet.timeElapsed >= 6.0f) {
          bullet.active = false;
        }
      }

      // Check if the turret has spoted the hero
      turret.timeElapsed += dt;

      if (turret.timeElapsed >= FireCooldown) {
        turret.timeElapsed -= FireCooldown;
        float distance =  gf::squareDistance(m_heroPosition, turret.position);

        if (distance <= TurretTargetRadius * TurretTargetRadius) {
          // Create the bullet
          Turret::Bullet bullet;
          bullet.position = turret.position;
          bullet.velocity = BulletVelocity * gf::normalize(m_heroPosition - turret.position);
          bullet.active = true;
          bullet.timeElapsed = 0.0f;

          turret.bullets.push_back(bullet);
        }
      }

      // Remove the last bullet
      auto trash = std::partition(turret.bullets.begin(), turret.bullets.end(), [](Turret::Bullet &bullet) {
        return bullet.active;
      });

      turret.bullets.erase(trash, turret.bullets.end());
    }
  }

  void TurretManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto &turret: m_turrets) {
      // Draw the turret
      gf::Sprite sprite;
      sprite.setTexture(m_texture);
      sprite.setTextureRect(m_rect);
      sprite.setPosition(turret.position);
      sprite.setScale(TurretSize / SpriteSize);
      sprite.setAnchor(gf::Anchor::Center);

      target.draw(sprite, states);

      // Draw the bullet
      for (auto &bullet: turret.bullets) {
        gf::CircleShape shape;
        shape.setRadius(6.0f);
        shape.setColor(gf::Color::Black);
        shape.setPosition(bullet.position);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape, states);
      }
    }
  }

  gf::MessageStatus TurretManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    gf::unused(id);

    auto hero = static_cast<HeroPosition*>(msg);
    m_heroPosition = hero->position;
    return gf::MessageStatus::Keep;
  }
}
