/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <gf/UI.h>

#include <cmath>
#include <algorithm>
#include <string>

static std::string niceNum(float num, float precision) {
  float accpow = std::floor(std::log10(precision));

  int digits = 0;

  if (num < 0) {
    digits = static_cast<int>(std::fabs(num / std::pow(10, accpow) - 0.5f));
  } else {
    digits = static_cast<int>(std::fabs(num / std::pow(10, accpow) + 0.5f));
  }

  std::string result;

  if (digits > 0) {
    int curpow = static_cast<int>(accpow);

    for (int i = 0; i < curpow; ++i) {
      result += '0';
    }

    while (digits > 0) {
      char adigit = (digits % 10) + '0';

      if (curpow == 0 && result.length() > 0) {
        result += '.';
        result += adigit;
      } else {
        result += adigit;
      }

      digits /= 10;
      curpow += 1;
    }

    for (int i = curpow; i < 0; ++i) {
      result += '0';
    }

    if (curpow <= 0) {
      result += ".0";
    }

    if (num < 0) {
      result += '-';
    }

    std::reverse(result.begin(), result.end());
  } else {
    result = "0";
  }

  return result;
}


namespace gf {
inline namespace v1 {


}
}
