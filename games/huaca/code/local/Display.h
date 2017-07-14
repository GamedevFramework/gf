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
#ifndef HUACA_DISPLAY_H
#define HUACA_DISPLAY_H

#include <gf/Entity.h>
#include <gf/Message.h>

#include "Level.h"

namespace huaca {

  class Display : public gf::Entity {
  public:
    Display();

    void clear();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onKeyLooted(gf::Id id, gf::Message *msg);
    gf::MessageStatus onRunePressed(gf::Id id, gf::Message *msg);
    gf::MessageStatus onPortalDropped(gf::Id id, gf::Message *msg);
    gf::MessageStatus onSequenceFailed(gf::Id id, gf::Message *msg);

    gf::MessageStatus onResetLevel(gf::Id id, gf::Message *msg);
    gf::MessageStatus onNewLevel(gf::Id id, gf::Message *msg);

  private:
    struct ItemDisplay {
      gf::Texture *texture;
      bool active;
    };

    ItemDisplay m_runes[Level::RuneCount];
    ItemDisplay m_keys[Level::KeyDoorCount];
    ItemDisplay m_portals[Level::PortalCount];
  };


}


#endif // HUACA_DISPLAY_H
