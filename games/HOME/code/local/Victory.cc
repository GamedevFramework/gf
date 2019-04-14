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
#include "Victory.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace home {

  VictoryScreen::VictoryScreen()
  : m_texture(gResourceManager().getTexture("images/victory.jpg"))
  {

  }

  void VictoryScreen::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);

    gf::Vector2f size = coords.getRelativeSize({ 1.0f, 1.0f });
    gf::Vector2f scale = size / m_texture.getSize();

    gf::Sprite victory;
    victory.setTexture(m_texture);
    victory.setPosition(coords.getCenter());
    victory.setScale(std::min(scale.x, scale.y));
    victory.setAnchor(gf::Anchor::Center);
    target.draw(victory, states);
  }

}
