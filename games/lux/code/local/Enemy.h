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
#ifndef LUX_ENEMY_H
#define LUX_ENEMY_H

#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/Vector.h>

#include "Properties.h"
#include "Ship.h"
#include "Shoot.h"

namespace lux {

  class Enemy : public Ship {
  public:
    static constexpr float Width = 60.0f;
    static constexpr float Height = 60.0f;
    static constexpr float ScaleX = Width / 64.0f;
    static constexpr float ScaleY = Height / 64.0f;

    Enemy(ShipClass ship, gf::Vector2f position, gf::Vector2f velocity, float health, gf::Random& Random, gf::MessageManager& messages, gf::ResourceManager &resources);

    virtual void update(float dt) override;

    virtual void render(gf::RenderTarget& target) override;

    void setHeroPosition(gf::Vector2f position) {
      m_heroPos = position;
    }

  private:
    ShipClass m_ship;
    gf::Vector2f m_position;
    gf::Vector2f m_velocity;

    gf::Random& m_random;
    gf::MessageManager& m_messages;

    gf::Vector2f m_heroPos;
    gf::Texture *m_texture;

    std::unique_ptr<Shoot> m_shoot;
  };


  class EnemyManager : public gf::Entity {
  public:
    EnemyManager(gf::Random& Random, gf::MessageManager& messages, gf::ResourceManager &resources);

    EnemyManager(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;

    EnemyManager(EnemyManager&&) = delete;
    EnemyManager& operator=(EnemyManager&&) = delete;

    ~EnemyManager();

    void addEnemy(ShipClass ship, gf::Vector2f position, gf::Vector2f velocity);

    gf::MessageStatus onLocation(gf::Id id, gf::Message *msg);

    virtual void update(float dt) override;

    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::Random& m_random;
    gf::MessageManager& m_messages;
    gf::ResourceManager& m_resources;

    std::vector<Enemy*> m_enemies;
    gf::Vector2f m_heroPos;
  };

}

#endif // LUX_ENEMY_H
