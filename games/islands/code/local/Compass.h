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
#ifndef BI_COMPASS_H
#define BI_COMPASS_H

#include <deque>

#include <gf/Activities.h>
#include <gf/Entity.h>
#include <gf/Texture.h>

#include "Messages.h"

namespace bi {
  class Compass: public gf::Entity {
  public:
    Compass();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onNearestTreasure(gf::Id id, gf::Message *msg);

  private:
    gf::Vector2f m_position;
    float m_angle;

    float m_angleCurrent;
    float m_angleRange;
    gf::RotateToActivity m_angleActivity;

    gf::Time m_timeElapsed;
    bool m_displayed;

    const gf::Texture &m_texture;
    gf::RectF m_compass;
    gf::RectF m_pointer;

    gf::Time m_timeUntilNextRadar;

    struct Radar {
      float radius;
      gf::Time lifetime;
    };

    std::deque<Radar> m_radars;
  };

}

#endif // BI_COMPASS_H
