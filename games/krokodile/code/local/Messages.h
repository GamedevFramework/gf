/*
 * Krokodile
 * Copyright (C) 2018 Hatunruna team
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
#ifndef KKD_MESSAGES_H
#define KKD_MESSAGES_H

#include <gf/Message.h>

using namespace gf::literals;

namespace kkd {

  struct KrokodileState : public gf::Message {
    static constexpr gf::Id type = "KrokodileState"_id;

    gf::Vector2f position;
    float angle;
    float energyRatio;
    float lifeRatio;
    int merges;
  };

  struct MergeRealized : public gf::Message {
    static constexpr gf::Id type = "MergeRealized"_id;
  };

  struct EndOfGame : public gf::Message {
    static constexpr gf::Id type = "EndOfGame"_id;
  };

}

#endif // KKD_MESSAGES_H
