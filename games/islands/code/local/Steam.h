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
#ifndef BI_STEAM_H
#define BI_STEAM_H

#include <deque>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {

  class Steam : public gf::Entity {
  public:
    Steam();

    void run() {
      m_running = true;
    }

    void stop() {
      m_running = false;
    }

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Cloud {
      gf::Vector2f position;
      float angle;
      float radius;
      float alpha;
    };

    std::deque<Cloud> m_clouds;
    gf::Vector2f m_heroPosition;
    float m_heroAngle;
    float m_timer;
    bool m_running;
  };

}

#endif // BI_STEAM_H
