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
#ifndef BI_WAVE_MANAGER_H
#define BI_WAVE_MANAGER_H

#include <deque>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

namespace bi {

  class WaveManager : public gf::Entity {
  public:
    WaveManager();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Bubble {
      gf::Vector2f position;
      float angle;
      float radius;
      float lifetime;
    };

    std::deque<Bubble> m_bubbles;

    gf::Vector2f m_p0;
    gf::Vector2f m_p1;

    float m_elapsed;

    gf::Vector2f m_hero;
  };

}


#endif // BI_WAVE_MANAGER_H
