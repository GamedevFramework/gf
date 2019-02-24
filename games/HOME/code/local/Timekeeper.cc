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
