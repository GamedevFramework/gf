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
#ifndef KKD_HUD_H
#define KKD_HUD_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Clock.h>

#include "Messages.h"
#include "Singletons.h"

namespace kkd {
  class Hud : public gf::Entity {
  public:
    Hud();

    void reset();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onKrokodileState(gf::Id id, gf::Message *msg);

  private:
    gf::Font& m_font;
    const gf::Texture& m_texture;

    int m_merges = 0;
    float m_energyRatio = 0.0f;
    float m_lifeRatio = 0.0f;

    gf::Clock m_clock;
  };
}

#endif // KKD_HUD_H
