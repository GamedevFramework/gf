/*
 * Huaca, find the ritual to escape the temple
 * Copyright (C) 2016  Hatunruna team
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

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "Messages.h"
#include "Singletons.h"

namespace huaca {

  static constexpr float LevelTime = 120.0f; // 2 min for each level


  Timer::Timer(const gf::WindowGeometryTracker& tracker)
  : m_tracker(tracker)
  , m_totalTime(LevelTime)
  , m_remainingTime(LevelTime)
  {
    gMessageManager().registerHandler<NewLevelMessage>(&Timer::onNewLevel, this);
  }

  void Timer::update(float dt) {
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

  void Timer::render(gf::RenderTarget& target) {
    float x = m_tracker.getXFromRight(TimerWidth + Padding);
    float y = m_tracker.getYFromBottom(TimerHeight + Padding);

    gf::Color4f color = gf::Color::rgba(0x89865DFF);
    color.a = 0.5f;

    gf::RoundedRectangleShape shape({ TimerWidth, TimerHeight });
    shape.setRadius(TimerCorner);
    shape.setPosition({ x, y });
    shape.setColor(color);
    target.draw(shape);

    float remaining = m_remainingTime / m_totalTime * TimerHeight;
    color.a = 1.0f;

    shape.setSize({ TimerWidth, remaining });
    shape.setRadius(std::min(remaining / 2, TimerCorner));
    shape.setPosition({ x, y + TimerHeight - remaining });
    shape.setColor(color);
    target.draw(shape);
  }

  gf::MessageStatus Timer::onNewLevel(gf::Id id, gf::Message *msg) {
    (void) id; // not used
    (void) msg; // not used

    m_remainingTime = m_totalTime;
    return gf::MessageStatus::Keep;
  }

}
