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
#ifndef LUX_BONUS_H
#define LUX_BONUS_H

#include <gf/Entity.h>
#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

namespace lux {

  enum BonusType : int {
    Life = 0,
    UpgradeWeapon = 1,
  };

  class BonusManager : public gf::Entity {
  public:
    BonusManager(gf::Random& random, gf::MessageManager& messages, gf::ResourceManager& resources);

    void addBonus(gf::Vector2f position, gf::Vector2f velocity, BonusType type);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

    gf::MessageStatus onDropBonus(gf::Id id, gf::Message *msg);
    gf::MessageStatus onLocation(gf::Id id, gf::Message *msg);

  private:
    struct Bonus {
      gf::Vector2f position;
      gf::Vector2f velocity;
      BonusType type;
      bool active;
    };

    gf::Random& m_random;
    std::vector<Bonus> m_bonus;
    gf::Texture& m_lifeTexture;
    gf::Texture& m_weaponTexture;
  };

}

#endif // LUX_BONUS_H
