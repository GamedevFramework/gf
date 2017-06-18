/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <gf/Activities.h>

#include <cassert>

#include <gf/Unused.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /*
   * ValueActivity
   */

  ValueActivity::ValueActivity(float from, float target, float& value, Time duration, Easing easing)
  : m_tween(from, target, value, duration, easing)
  {

  }

  ActivityStatus ValueActivity::run(Time time) {
    if (m_tween.isFinished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.isFinished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void ValueActivity::restart() {
    m_tween.restart();
  }

  /*
   * RotateToActivity
   */

  RotateToActivity::RotateToActivity(float origin, float target, float& angle, Time duration, Easing easing)
  : m_tween(origin, target, angle, duration, easing)
  {
    if (origin > target) {
      while (target - origin > gf::Pi) {
        target -= 2 * gf::Pi;
      }
    } else {
      while (origin - target > gf::Pi) {
        target += 2 * gf::Pi;
      }
    }

    assert(origin - gf::Pi <= target && target <= origin + gf::Pi);

    m_tween.setTarget(target);
  }

  ActivityStatus RotateToActivity::run(Time time) {
    if (m_tween.isFinished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.isFinished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void RotateToActivity::restart() {
    m_tween.restart();
  }

  /*
   * MoveToActivity
   */

  MoveToActivity::MoveToActivity(Vector2f origin, Vector2f target, Vector2f& position, Time duration, Easing easing)
  : m_tween(origin, target, position, duration, easing)
  {

  }

  ActivityStatus MoveToActivity::run(Time time) {
    if (m_tween.isFinished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.isFinished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void MoveToActivity::restart() {
    m_tween.restart();
  }

  /*
   * ColorActivity
   */

  ColorActivity::ColorActivity(Color4f origin, Color4f target, Color4f& color, Time duration, Easing easing)
  : m_tween(origin, target, color, duration, easing)
  {

  }

  ActivityStatus ColorActivity::run(Time time) {
    if (m_tween.isFinished()) {
      return ActivityStatus::Finished;
    }

    m_tween.update(time);
    return m_tween.isFinished() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void ColorActivity::restart() {
    m_tween.restart();
  }

  /*
   * CallbackActivity
   */

  CallbackActivity::CallbackActivity(std::function<void()> callback)
  : m_callback(std::move(callback))
  , m_called(false)
  {

  }

  ActivityStatus CallbackActivity::run(Time time) {
    gf::unused(time);

    if (!m_called) {
      m_callback();
      m_called = true;
    }

    return ActivityStatus::Finished;
  }

  void CallbackActivity::restart() {
    m_called = false;
  }

  /*
   * DelayActivity
   */

  DelayActivity::DelayActivity(Time duration)
  : m_elapsed() // 0
  , m_duration(duration)
  {

  }

  ActivityStatus DelayActivity::run(Time time) {
    m_elapsed += time;

    if (m_elapsed > m_duration) {
      return ActivityStatus::Finished;
    }

    return ActivityStatus::Running;
  }

  void DelayActivity::restart() {
    m_elapsed = Time::zero();
  }

  /*
   * SequenceActivity
   */

  SequenceActivity::SequenceActivity()
  : m_current(0)
  {

  }

  void SequenceActivity::addActivity(Activity& activity) {
    m_activities.push_back(&activity);
  }

  ActivityStatus SequenceActivity::run(Time time) {
    if (m_current == m_activities.size()) {
      return ActivityStatus::Finished;
    }

    auto status = m_activities[m_current]->run(time);

    if (status == ActivityStatus::Finished) {
      m_current++;
    }

    return m_current == m_activities.size() ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void SequenceActivity::restart() {
    m_current = 0;

    for (auto activity : m_activities) {
      activity->restart();
    }
  }

  /*
   * RepeatActivity
   */

  RepeatActivity::RepeatActivity(Activity& activity, unsigned repeat)
  : m_activity(activity)
  , m_count(0)
  , m_repeat(repeat)
  {

  }

  ActivityStatus RepeatActivity::run(Time time) {
    if (m_count > 0 && m_repeat == m_count) {
      return ActivityStatus::Finished;
    }

    auto status = m_activity.run(time);

    if (status == ActivityStatus::Finished) {
      m_activity.restart();
      m_count++;
    }

    return  (m_repeat > 0 && m_repeat == m_count) ? ActivityStatus::Finished : ActivityStatus::Running;
  }

  void RepeatActivity::restart() {
    m_count = 0;
    m_activity.restart();
  }

  /*
   * ParallelActivity
   */

  ParallelActivity::ParallelActivity(Finish finish)
  : m_finish(finish)
  , m_status(ActivityStatus::Running)
  {

  }

  void ParallelActivity::addActivity(Activity& activity) {
    m_activities.push_back(&activity);
  }

  ActivityStatus ParallelActivity::run(Time time) {
    if (m_status == ActivityStatus::Finished) {
      return ActivityStatus::Finished;
    }

    std::size_t finished = 0;

    for (auto activity : m_activities) {
      auto status = activity->run(time);

      if (status == ActivityStatus::Finished) {
        finished++;
      }
    }

    switch (m_finish) {
      case Finish::Any:
        if (finished > 0) {
          m_status = ActivityStatus::Finished;
        }
        break;

      case Finish::All:
        if (finished == m_activities.size()) {
          m_status = ActivityStatus::Finished;
        }
        break;
    }

    return m_status;
  }

  void ParallelActivity::restart() {
    m_status = ActivityStatus::Running;

    for (auto activity : m_activities) {
      activity->restart();
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
