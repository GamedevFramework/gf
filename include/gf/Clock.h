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
#ifndef GF_CLOCK_H
#define GF_CLOCK_H

#include <chrono>

#include "Portability.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Utility class that measures the elapsed time
   *
   * gf::Clock is a lightweight class for measuring time.
   * It is a thin wrapper around C++11
   * [std::chrono::steady_clock](http://en.cppreference.com/w/cpp/chrono/steady_clock).
   *
   * Its provides the most precise time that the underlying
   * OS can achieve (generally microseconds or nanoseconds).
   * It also ensures monotonicity, which means that the returned
   * time can never go backward, even if the system time is
   * changed.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::Clock clock;
   * ...
   * gf::Time time1 = clock.getElapsedTime();
   * ...
   * gf::Time time2 = clock.restart();
   * ~~~
   *
   * The gf::Time value returned by the clock can then be
   * converted to a number of seconds, milliseconds or even
   * microseconds.
   *
   * @sa gf::Time
   */
  class GF_API Clock {
  public:
    /**
     * @brief Default constructor
     *
     * The clock starts automatically after being constructed.
     */
    Clock();

    /**
     * @brief Get the elapsed time
     *
     * This function returns the time elapsed since the last call
     * to `restart()` (or the construction of the instance if `restart()`
     * has not been called).
     *
     * @return Time elapsed
     */
    Time getElapsedTime() const;

    /**
     * @brief Restart the clock
     *
     * This function puts the time counter back to zero.
     * It also returns the time elapsed since the clock was started.
     *
     * @return Time elapsed
     */
    Time restart();

  private:
    std::chrono::steady_clock::time_point m_start;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CLOCK_H
