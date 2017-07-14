/*
 * Huaca, find the ritual to escape the temple
 * Copyright (C) 2016-2017  Hatunruna team
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
#ifndef HUACA_HERO_H
#define HUACA_HERO_H

#include <gf/Animation.h>
#include <gf/Direction.h>
#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

namespace huaca {

  class Hero : public gf::Entity {
  public:
    Hero();

    void goRight() {
      m_isRunning = true;
      m_direction = gf::Direction::Right;
    }

    void goLeft() {
      m_isRunning = true;
      m_direction = gf::Direction::Left;
    }

    void goUp() {
      m_isRunning = true;
      m_direction = gf::Direction::Up;
    }

    void goDown() {
      m_isRunning = true;
      m_direction = gf::Direction::Down;
    }

    void stop() {
      m_isRunning = false;
    }

    void dropPortal();

    virtual void update(gf::Time time) override;

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onNewLevel(gf::Id id, gf::Message *msg);
    gf::MessageStatus onResetLevel(gf::Id id, gf::Message *msg);

  private:
    gf::Vector2f m_initialPosition;
    gf::Vector2f m_position;

    gf::Direction m_direction;
    bool m_isRunning;

    gf::Animation m_static[4];
    gf::Animation m_running[4];
    gf::Animation *m_currentAnimation;
  };

}

#endif // HUACA_HERO_H
