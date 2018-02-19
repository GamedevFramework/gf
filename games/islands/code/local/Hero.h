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
#ifndef BI_HERO_H
#define BI_HERO_H

#include <gf/Entity.h>
#include <gf/Move.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"
#include "Steam.h"

namespace bi {
  class Hero: public gf::Entity {
  public:
    Hero(Steam& steam, const gf::Vector2f postion);

    void moveForward() {
      m_move = gf::LinearMove::Forward;
    }

    void moveBackward() {
      m_move = gf::LinearMove::Backward;
    }

    void moveStop() {
      m_move = gf::LinearMove::None;
    }

    void turnLeft() {
      m_turn = gf::AngularMove::Left;
    }

    void turnRight() {
      m_turn = gf::AngularMove::Right;
    }

    void turnNone() {
      m_turn = gf::AngularMove::None;
    }

    void sendInitialPosition() const;
    bool isOnIsland() const;
    bool scanAvailable() const;

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStopScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onGameOver(gf::Id id, gf::Message *msg);

  private:
    Steam& m_steam;

    gf::LinearMove m_move;
    gf::AngularMove m_turn;

    gf::Vector2f m_position;
    float m_angle;

    const gf::Texture &m_texture;
    gf::RectF m_tricorn;
    gf::RectF m_boat;

    float m_timeElapsed;
    bool m_alternateStep;
    bool m_isOnIsland;
    bool m_isFrozen;

    bool m_isGameOver;
  };
}

#endif // BI_HERO_H
