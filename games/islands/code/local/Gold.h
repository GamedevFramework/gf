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
#ifndef BI_GOLD_H
#define BI_GOLD_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/Rect.h>
#include <gf/Texture.h>

#include "Messages.h"

namespace bi {
  class Gold: public gf::Entity {
  public:
    Gold();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    bool isGameOver() { return m_gameOver; }
    bool isWin() { return m_win; }

  private:
    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);
    gf::MessageStatus onGoldLooted(gf::Id id, gf::Message *msg);

  private:
    unsigned m_score;
    bool m_gameOver;
    bool m_win;
    gf::Font &m_font;
    const gf::Texture &m_texture;
    gf::RectF m_rect;
  };
}

#endif // BI_GOLD_H
