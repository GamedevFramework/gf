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
#ifndef HUACA_MESSAGES_H
#define HUACA_MESSAGES_H

#include <gf/Message.h>
#include <gf/Rect.h>
#include <gf/Vector.h>

using namespace gf::literals;

namespace huaca {

  struct HeroPositionMessage : public gf::Message {
    static constexpr gf::Id type = "HeroPosition"_id;

    gf::Vector2f position;
    gf::RectF bounds;
    bool isRunning;
  };

  /*
   * level related messages
   */

  struct NewLevelMessage : public gf::Message {
    static constexpr gf::Id type = "NewLevel"_id;

    gf::Vector2f heroPosition;
  };

  struct ResetLevelMessage : public gf::Message {
    static constexpr gf::Id type = "ResetLevel"_id;

    // no data
  };

  /*
   * item related messages
   */

  struct KeyLootedMessage : public gf::Message {
    static constexpr gf::Id type = "KeyLooted"_id;

    unsigned number;
  };

  struct RunePressedMessage : public gf::Message {
    static constexpr gf::Id type = "RunePressed"_id;

    unsigned number;
  };

  struct PortalDroppedMessage : public gf::Message {
    static constexpr gf::Id type = "PortalDropped"_id;

    gf::Vector2f position;
  };

  struct SequenceFailedMessage : public gf::Message {
    static constexpr gf::Id type = "SequenceFailed"_id;

    // no data
  };

}

#endif // HUACA_MESSAGES_H
