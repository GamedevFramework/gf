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
#include "Bonus.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Hero.h"
#include "Messages.h"
#include "World.h"

namespace lux {

  BonusManager::BonusManager(gf::Random& random, gf::MessageManager& messages, gf::ResourceManager& resources)
  : m_random(random)
  , m_lifeTexture(resources.getTexture("bonus_life.png"))
  , m_weaponTexture(resources.getTexture("bonus_weapon.png"))
  {
    messages.registerHandler<DropBonusMessage>(&BonusManager::onDropBonus, this);
    messages.registerHandler<LocationMessage>(&BonusManager::onLocation, this);
  }

  void BonusManager::addBonus(gf::Vector2f position, gf::Vector2f velocity, BonusType type) {
    Bonus bonus;
    bonus.position = position;
    bonus.velocity = velocity;
    bonus.type = type;
    bonus.active = true;

    m_bonus.push_back(bonus);
  }

  static constexpr float BonusExtra = 20.0f;

  void BonusManager::update(gf::Time time) {
    float dt = time.asSeconds();
    gf::RectF view(WorldCenter - WorldSize / 2, WorldSize);
    view.extend(BonusExtra);

    for (Bonus& bonus : m_bonus) {
      bonus.position += bonus.velocity * dt;

      if (!view.contains(bonus.position)) {
        bonus.active = false;
      }
    }

    m_bonus.erase(std::remove_if(m_bonus.begin(), m_bonus.end(), [](const Bonus& b) { return !b.active; }), m_bonus.end());
  }

  static constexpr float BonusSize = 60.0f;
  static constexpr float RatioLifeBonus = BonusSize / 256.0f;
  static constexpr float RatioWeaponBonus = BonusSize / 256.0f;

  void BonusManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Sprite sprite;

    for (const Bonus& bonus : m_bonus) {
      switch(bonus.type) {
        case BonusType::Life:
          sprite.setTexture(m_lifeTexture);
          sprite.setScale(RatioLifeBonus);
          break;

        case BonusType::UpgradeWeapon:
          sprite.setTexture(m_weaponTexture);
          sprite.setScale(RatioWeaponBonus);
          break;
      }

      sprite.setAnchor(gf::Anchor::Center);
      sprite.setPosition(bonus.position);
      target.draw(sprite, states);
    }
  }

  static bool isTargetReachedByBonus(gf::Vector2f shipPos, gf::Vector2f bonusPos) {
    gf::RectF rectShip(shipPos, { Hero::Width, Hero::Height });
    gf::RectF rectBonus(bonusPos, { BonusSize, BonusSize });
    return rectShip.intersects(rectBonus);
  }

  static constexpr float BonusLinearVelocty = 120.0f;

  gf::MessageStatus BonusManager::onDropBonus(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    auto drop = static_cast<DropBonusMessage*>(msg);

    BonusType bonusType = static_cast<BonusType>(m_random.computeUniformInteger(0, 1));
    addBonus(drop->position, { 0.0f, BonusLinearVelocty }, bonusType);

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus BonusManager::onLocation(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    auto loc = static_cast<LocationMessage*>(msg);

    // Check if is player location
    if (loc->origin != Origin::Hero) {
      return gf::MessageStatus::Keep;
    }

    // Check if one bonus is hit
    for (Bonus& bonus : m_bonus) {
      if (isTargetReachedByBonus(loc->position, bonus.position)) {
        // Remove the bonus
        bonus.active = false;

        // Check the type of bonus
        switch(bonus.type) {
          case BonusType::Life:
            loc->ship->restore(10.0f);
            break;

          case BonusType::UpgradeWeapon:
            Hero *hero = static_cast<Hero*>(loc->ship);
            hero->upgradeWeapon();
            break;
        }
      }
    }

    return gf::MessageStatus::Keep;
  }

}
