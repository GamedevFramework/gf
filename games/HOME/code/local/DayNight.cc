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
#include "DayNight.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "Messages.h"
#include "Singletons.h"

namespace home {

  DayNight::DayNight(const Timekeeper& timekeeper)
  : gf::Entity(400)
  , m_timekeeper(timekeeper)
  , m_texture(gResourceManager().getTexture("images/fov.png"))
  , m_position({ 0.0f, 0.0f })
  , m_color(gf::Color::Opaque(0.0f))
  {
    gMessageManager().registerHandler<HeroPosition>(&DayNight::onHeroPosition, this);
  }

  void DayNight::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_timekeeper.isDusk()) {
      m_color = gf::Color::Opaque(m_timekeeper.getDuskRatio());
    }

    if (m_timekeeper.isDawn()) {
      m_color = gf::Color::Opaque(1.0f - m_timekeeper.getDawnRatio());
    }

    gf::Sprite fov;
    fov.setTexture(m_texture);
    fov.setAnchor(gf::Anchor::Center);
    fov.setPosition(m_position);
    fov.setColor(m_color);
    target.draw(fov, states);
  }

  gf::MessageStatus DayNight::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    m_position = static_cast<HeroPosition*>(msg)->position;
    return gf::MessageStatus::Keep;
  }

}
