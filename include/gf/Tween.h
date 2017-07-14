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
#ifndef GF_TWEEN_H
#define GF_TWEEN_H

#include <functional>

#include "Easings.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief An interpolation between two values
   *
   * @sa [Inbetweening - Wikipedia](https://en.wikipedia.org/wiki/Inbetweening)
   */
  template<typename T>
  class Tween {
  public:
    /**
     * @brief A setter function
     */
    using Setter = std::function<void(const T&)>;

    /**
     * @brief Constructor with a setter
     *
     * @param origin The origin value
     * @param target The target value
     * @param setter The setter
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    Tween(T origin, T target, Setter setter, Time duration, Easing easing = Ease::linear)
    : m_origin(origin)
    , m_target(target)
    , m_setter(setter)
    , m_elapsed() // 0
    , m_duration(duration)
    , m_easing(easing)
    {

    }

    /**
     * @brief Constructor with a reference
     *
     * @param origin The origin value
     * @param target The target value
     * @param value A reference on the value
     * @param duration The duration of the tween
     * @param easing The easing for the interpolation
     */
    Tween(T origin, T target, T& value, Time duration, Easing easing = Ease::linear)
    : m_origin(origin)
    , m_target(target)
    , m_setter([&value](const T& newValue) { value = newValue; })
    , m_elapsed() // 0
    , m_duration(duration)
    , m_easing(easing)
    {

    }

    /**
     * @brief Change the origin of the tween
     *
     * @param origin The new origin
     */
    void setOrigin(T origin) {
      m_origin = origin;
    }

    /**
     * @brief Get the origin of the tween
     *
     * @returns The current origin
     */
    constexpr T getOrigin() const noexcept {
      return m_origin;
    }

    /**
     * @brief Change the target of the tween
     *
     * @param target The new target
     */
    void setTarget(T target) {
      m_target = target;
    }

    /**
     * @brief Get the target of the tween
     *
     * @returns The current target
     */
    constexpr T getTarget() const noexcept {
      return m_target;
    }

    /**
     * @brief Change the duration of the tween
     *
     * @param duration The new duration
     */
    void setDuration(Time duration) {
      m_duration = duration;
    }

    /**
     * @brief Get the duration of the tween
     *
     * @returns The current duration
     */
    constexpr Time getDuration() const noexcept {
      return m_duration;
    }

    /**
     * @brief Get the current value of the tween
     *
     * @returns The interpolated value
     */
    T getValue() const {
      return gf::lerp(m_origin, m_target, m_easing(m_elapsed.asSeconds() / m_duration.asSeconds()));
    }

    /**
     * @brief Check if the tween is finished
     *
     * @returns True if the tween is finished
     */
    bool isFinished() const {
      return m_elapsed >= m_duration;
    }

    /**
     * @brief Update the tween
     *
     * @param time The time since the last update
     */
    void update(Time time) {
      m_elapsed += time;

      if (m_elapsed >= m_duration) {
        m_elapsed = m_duration;
      }

      m_setter(getValue());
    }

    /**
     * @brief Restart the tween
     */
    void restart() {
      m_elapsed = Time::zero();
    }

  private:
    T m_origin;
    T m_target;
    Setter m_setter;
    Time m_elapsed;
    Time m_duration;
    Easing m_easing;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TWEEN_H
