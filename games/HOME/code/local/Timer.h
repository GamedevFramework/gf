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
#ifndef HOME_TIMER_H
#define HOME_TIMER_H

#include <gf/Entity.h>
#include <gf/Font.h>

#include "Timekeeper.h"

namespace home {

  class Timer : public gf::Entity {
  public:
    Timer(const Timekeeper& timekeeper);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const Timekeeper& m_timekeeper;
    gf::Font &m_font;
  };

}

#endif // HOME_TIMER_H
