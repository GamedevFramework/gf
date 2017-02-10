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
#include <gf/Time.h>

#include <type_traits>

namespace gf {
inline namespace v1 {

  Time::Time()
  : m_duration(0)
  {
  }

  float Time::asSeconds() const {
    return std::chrono::duration_cast<std::chrono::duration<float>>(m_duration).count();
  }

  int32_t Time::asMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::duration<int32_t, std::milli>>(m_duration).count();
  }

  int64_t Time::asMicroseconds() const {
    return std::chrono::duration_cast<std::chrono::duration<int64_t, std::micro>>(m_duration).count();
  }

  Time::Time(std::chrono::steady_clock::duration duration)
    : m_duration(duration) {
  }

  Time seconds(float amount) {
    std::chrono::duration<float> time(amount);
    return Time(std::chrono::duration_cast<std::chrono::steady_clock::duration>(time));
  }

  Time milliseconds(int32_t amount) {
    std::chrono::duration<int32_t, std::milli> time(amount);
    return Time(std::chrono::duration_cast<std::chrono::steady_clock::duration>(time));
  }

  Time microseconds(int64_t amount) {
    std::chrono::duration<int64_t, std::micro> time(amount);
    return Time(std::chrono::duration_cast<std::chrono::steady_clock::duration>(time));
  }

}
}
