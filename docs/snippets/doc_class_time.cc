/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
#include <gf/Time.h>

#include "doc_utils.h"

void dummyTimeUsage() {
  /// [time]
  gf::Time t1 = gf::seconds(0.1f);
  int32_t milli = t1.asMilliseconds(); // 100

  gf::Time t2 = gf::milliseconds(30);
  int64_t micro = t2.asMicroseconds(); // 30000

  gf::Time t3 = gf::microseconds(-800000);
  float sec = t3.asSeconds(); // -0.8
  /// [time]

  unused(milli, micro, sec);

  {
  /// [time_s]
  using namespace gf::literals;

  gf::Time time = 66.6_seconds;
  /// [time_s]

  unused(time);
  }

  {
  /// [time_ms]
  using namespace gf::literals;

  gf::Time time = 42_milliseconds;
  /// [time_ms]

  unused(time);
  }

  {
  /// [time_us]
  using namespace gf::literals;

  gf::Time time = 13_microseconds;
  /// [time_us]

  unused(time);
  }
}
