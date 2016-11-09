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
#ifndef LUX_MESSAGES_H
#define LUX_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

#include "Properties.h"

namespace lux {
  class Ship;

  struct LocationMessage : public gf::Message {
    static const gf::Id type = "Location"_id;
    Origin origin;
    gf::Vector2f position;
    Ship *ship;
  };

  struct ShootMessage : public gf::Message {
    static const gf::Id type = "Shoot"_id;
    Origin origin;
    ShipClass ship;
    gf::Vector2f position;
    gf::Vector2f velocity;
  };

  struct DeadMessage : public gf::Message {
    static const gf::Id type = "Dead"_id;
    Origin origin;
    ShipClass ship;
    gf::Vector2f position;
  };

  struct ScoreMessage : public gf::Message {
    static const gf::Id type = "Score"_id;
    unsigned long score;
  };

  struct RestartGameMessage : public gf::Message {
    static const gf::Id type = "RestartGame"_id;
  };

  struct DropBonusMessage : public gf::Message {
    static const gf::Id type = "DropBonus"_id;
    gf::Vector2f position;
  };

  struct WinGameMessage : public gf::Message {
    static const gf::Id type = "WinGame"_id;
    unsigned int bonusScore;
  };

  struct LifeMessage : public gf::Message {
    static const gf::Id type = "Life"_id;
    float life;
  };

}

#endif // LUX_MESSAGES_H
