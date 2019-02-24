/*
 * H.O.M.E.
 * Copyright (C) 2019 Hatunruna team
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
#ifndef HOME_MESSAGES_H
#define HOME_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

#include "SupplyManager.h"

using namespace gf::literals;

namespace home {

  struct HarvestSupply : public gf::Message {
    static constexpr gf::Id type = "HarvestSupply"_id;
    SupplyKind kind;
    int quantity;
  };

  struct CursorClickedPosition : public gf::Message {
    static const gf::Id type = "CursorClickedPosition"_id;
    gf::Vector2f position;
  };

  struct HeroPosition : public gf::Message {
    static const gf::Id type = "HeroPosition"_id;
    gf::Vector2f position;
  };

  struct UnloadBackpack: public gf::Message {
    static const gf::Id type = "UnloadBackpack"_id;
    SupplyKind kind;
    int quantity;
  };






  struct ClockInfo: public gf::Message {
    static const gf::Id type = "ClockInfo"_id;
    float time;
    int nbDays;
  };

  struct GameOver: public gf::Message {
    static const gf::Id type = "GameOver"_id;
  };

  struct MaxResources: public gf::Message {
    static const gf::Id type = "MaxResources"_id;
  };

  struct Victory: public gf::Message {
    static const gf::Id type = "Victory"_id;
  };

}

#endif // HOME_MESSAGES_H
