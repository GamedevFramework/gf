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
#include "Timekeeper.h"

#include <cassert>

namespace home {

  namespace {

    constexpr float TimeFactor = 1.0f / 15.0f; // 15 secondes = 1 hour in game
    constexpr float StartDusk = 20.0f; // Start of dusk - 8 pm
    constexpr float StartDawn = 8.0f; // Start of dawn - 8 am
    constexpr float TransitionDuration = 1.0f; // 1 hour in game

  }

  Timekeeper::Timekeeper()
  : m_time(StartDawn + TransitionDuration)
  , m_days(0)
  {

  }

  void Timekeeper::update(gf::Time time) {
    m_time += time.asSeconds() * TimeFactor;

    if (m_time >= 24.0f) {
      m_time -= 24.0f;
      m_days += 1;
    }
  }

  bool Timekeeper::isDawn() const {
    return StartDawn <= m_time && m_time <= StartDawn + TransitionDuration;
  }

  float Timekeeper::getDawnRatio() const {
    assert(isDawn());
    return (m_time - StartDawn) / TransitionDuration;
  }

  bool Timekeeper::isDusk() const {
    return StartDusk <= m_time && m_time <= StartDusk + TransitionDuration;
  }

  float Timekeeper::getDuskRatio() const {
    assert(isDusk());
    return (m_time - StartDusk) / TransitionDuration;
  }

}
