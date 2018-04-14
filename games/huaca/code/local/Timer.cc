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
#include "Timer.h"

#include <gf/Coordinates.h>
#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>

#include "Messages.h"
#include "Singletons.h"

namespace huaca {

  static constexpr float LevelTime = 120.0f; // 2 min for each level


  Timer::Timer()
  : m_totalTime(LevelTime)
  , m_remainingTime(LevelTime)
  {
    gMessageManager().registerHandler<NewLevelMessage>(&Timer::onNewLevel, this);
  }

  void Timer::update(gf::Time time) {
    float dt = time.asSeconds();

    m_remainingTime -= dt;

    if (m_remainingTime < 0) {
      ResetLevelMessage msg;
      gMessageManager().sendMessage(&msg);

      m_remainingTime = m_totalTime;
    }
  }

  static constexpr float Padding = 20.0f;

  static constexpr float TimerWidth = 10.0f;
  static constexpr float TimerHeight = 300.0f;
  static constexpr float TimerCorner = 5.0f;

  void Timer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    gf::Vector2f position = coordinates.getAbsolutePoint({ TimerWidth + Padding, TimerHeight + Padding }, gf::Anchor::BottomRight);

//     float x = m_tracker.getXFromRight(TimerWidth + Padding);
//     float y = m_tracker.getYFromBottom(TimerHeight + Padding);

    gf::Color4f color = gf::Color::fromRgba32(0x89865DFF);
    color.a = 0.5f;

    gf::RoundedRectangleShape shape({ TimerWidth, TimerHeight });
    shape.setRadius(TimerCorner);
    shape.setPosition(position);
    shape.setColor(color);
    target.draw(shape, states);

    float remaining = m_remainingTime / m_totalTime * TimerHeight;
    color.a = 1.0f;

    position.y += TimerHeight - remaining;

    shape.setSize({ TimerWidth, remaining });
    shape.setRadius(TimerCorner);
    shape.setPosition(position);
    shape.setColor(color);
    target.draw(shape, states);
  }

  gf::MessageStatus Timer::onNewLevel(gf::Id id, gf::Message *msg) {
    gf::unused(id);
    gf::unused(msg);

    m_remainingTime = m_totalTime;
    return gf::MessageStatus::Keep;
  }

}
