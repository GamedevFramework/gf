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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_TIME_H
#define GF_TIME_H

#include <cstdint>
#include <chrono>
#include <ratio>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Represents a time value
   *
   * gf::Time encapsulates a time value in a flexible way.
   * It allows to define a time value either as a number of
   * seconds, milliseconds or microseconds. It also works the
   * other way round: you can read a time value as either
   * a number of seconds, milliseconds or microseconds.
   *
   * By using such a flexible interface, the API doesn't
   * impose any fixed type or resolution for time values,
   * and let the user choose its own favorite representation.
   *
   * Since they represent a time span and not an absolute time
   * value, times can also be negative.
   *
   * gf::Time is a thin wrapper around C++11
   * [std::chrono::steady_clock](http://en.cppreference.com/w/cpp/chrono/steady_clock).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::Time t1 = gf::seconds(0.1f);
   * int32_t milli = t1.asMilliseconds(); // 100
   *
   * gf::Time t2 = gf::milliseconds(30);
   * int64_t micro = t2.asMicroseconds(); // 30000
   *
   * gf::Time t3 = gf::microseconds(-800000);
   * float sec = t3.asSeconds(); // -0.8
   * ~~~
   *
   * @sa gf::Clock
   */
  class GF_API Time {
  public:
    /**
     * @brief Default constructor
     *
     * Sets the time value to zero.
     */
    constexpr Time()
    : m_duration(std::chrono::steady_clock::duration::zero())
    {

    }

    /**
     * @brief Constructor with a duration
     *
     * @param duration A duration expressed with a `std::chrono` type
     */
    constexpr explicit Time(std::chrono::steady_clock::duration duration)
    : m_duration(duration)
    {

    }

    /**
     * @brief Return the time value as a number of seconds
     *
     * @return Time in seconds
     *
     * @sa asMilliseconds(), asMicroseconds()
     */
    float asSeconds() const;

    /**
     * @brief Return the time value as a number of milliseconds
     *
     * @return Time in milliseconds
     *
     * @sa asSeconds(), asMicroseconds()
     */
    int32_t asMilliseconds() const;

    /**
     * @brief Return the time value as a number of microseconds
     *
     * @return Time in microseconds
     *
     * @sa asSeconds(), asMilliseconds()
     */
    int64_t asMicroseconds() const;

    /**
     * @brief Return the time value as a duration
     *
     * @return Time as a duration (`std::chrono` type)
     */
    std::chrono::steady_clock::duration asDuration() const {
      return m_duration;
    }

    /**
     * @brief Addition and assignement
     *
     * @param other The time to add to the current time
     * @return The current time
     */
    Time& operator+=(Time other) {
      m_duration += other.m_duration;
      return *this;
    }

    /**
     * @brief Substraction and assignement
     *
     * @param other The time to substract to the current time
     * @return The current time
     */
    Time& operator-=(Time other) {
      m_duration -= other.m_duration;
      return *this;
    }

    static const Time Zero;

  private:
    std::chrono::steady_clock::duration m_duration;
  };

  /**
   * @relates Time
   * @brief Construct a time value from a number of seconds
   *
   * @param amount Number of seconds
   *
   * @return Time value constructed from the amount of seconds
   *
   * @sa milliseconds(), microseconds()
   */
  GF_API Time seconds(float amount);

  /**
   * @relates Time
   * @brief Construct a time value from a number of milliseconds
   *
   * @param amount Number of milliseconds
   *
   * @return Time value constructed from the amount of milliseconds
   *
   * @sa seconds(), microseconds()
   */
  GF_API Time milliseconds(int32_t amount);

  /**
   * @relates Time
   * @brief Construct a time value from a number of microseconds
   *
   * @param amount Number of microseconds
   *
   * @return Time value constructed from the amount of microseconds
   *
   * @sa seconds(), milliseconds()
   */
  GF_API Time microseconds(int64_t amount);

  /**
   * @relates Time
   * @brief Equality operator
   *
   * @param rhs First time
   * @param lhs Second time
   * @return True if the first time and the second time are the same
   */
  inline
  bool operator==(Time rhs, Time lhs) {
    return rhs.asDuration() == lhs.asDuration();
  }

  /**
   * @relates Time
   * @brief Inequality operator
   *
   * @param rhs First time
   * @param lhs Second time
   * @return True if the first time and the second time are different
   */
  inline
  bool operator!=(Time rhs, Time lhs) {
    return rhs.asDuration() != lhs.asDuration();
  }

  /**
   * @relates Time
   * @brief Lesser than operator
   *
   * @param rhs First time
   * @param lhs Second time
   * @return True if the first time is lesser than the second time
   */
  inline
  bool operator<(Time rhs, Time lhs) {
    return rhs.asDuration() < lhs.asDuration();
  }

  /**
   * @relates Time
   * @brief Greater than operator
   *
   * @param rhs First time
   * @param lhs Second time
   * @return True if the first time is greater than the second time
   */
  inline
  bool operator>(Time rhs, Time lhs) {
    return rhs.asDuration() > lhs.asDuration();
  }

  /**
   * @relates Time
   * @brief Lesser or equal operator
   *
   * @param rhs First time
   * @param lhs Second time
   * @return True if the first time is lesser or equal than the second time
   */
  inline
  bool operator<=(Time rhs, Time lhs) {
    return rhs.asDuration() <= lhs.asDuration();
  }

  /**
   * @relates Time
   * @brief Greater or equal than operator
   *
   * @param rhs First time
   * @param lhs Second time
   * @return True if the first time is greater or equal than the second time
   */
  inline
  bool operator>=(Time rhs, Time lhs) {
    return rhs.asDuration() >= lhs.asDuration();
  }

  /**
   * @relates Time
   * @brief Addition of two times
   *
   * @param rhs First time
   * @param lhs Second time
   * @return The sum of two times
   */
  inline
  Time operator+(Time rhs, Time lhs) {
    return Time(rhs.asDuration() + lhs.asDuration());
  }

  /**
   * @relates Time
   * @brief Substraction of two times
   *
   * @param rhs First time
   * @param lhs Second time
   * @return The difference of two times
   */
  inline
  Time operator-(Time rhs, Time lhs) {
    return Time(rhs.asDuration() - lhs.asDuration());
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TIME_H
