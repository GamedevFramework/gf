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
#ifndef BI_TURRET_MANAGER_H
#define BI_TURRET_MANAGER_H

#include <deque>
#include <vector>

#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {
  class TurretManager: public gf::Entity {
  public:
    TurretManager();

    void addTurret(gf::Vector2f position);

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Turret {
      struct Bullet {
        gf::Vector2f position;
        gf::Vector2f velocity;
        float timeElapsed;
        bool active;
      };

      gf::Vector2f position;
      std::deque<Bullet> bullets;
      float timeElapsed;
    };

  private:
    std::vector<Turret> m_turrets;
    gf::Vector2f m_heroPosition;
    const gf::Texture &m_texture;
    gf::RectF m_rect;
  };
}

#endif // BI_TURRET_MANAGER_H
