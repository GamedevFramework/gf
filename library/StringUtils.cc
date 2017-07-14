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
#include <gf/StringUtils.h>

#include <cassert>
#include <cmath>

#include <algorithm>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  std::string niceNum(float num, float precision) {
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

  std::u32string computeUnicodeString(const std::string& str) {
    static constexpr uint8_t utf8Table[4][2] = {
      { 0x7F, 0x00 },
      { 0x1F, 0xC0 },
      { 0x0F, 0xE0 },
      { 0x07, 0xF0 }
    };

    std::u32string out;

    for (std::size_t k = 0; k < str.size(); ++k) {
      uint8_t c = str[k];
      char32_t codepoint = 0;

      for (std::size_t i = 0; i < 4; ++i) {
        if ((c & ~utf8Table[i][0]) == utf8Table[i][1]) {
          codepoint = c & utf8Table[i][0];

          for (std::size_t j = 0; j < i; ++j) {
            ++k;

            assert(k < str.size());
            c = str[k];

            assert((c & ~0x3F) == 0x80);
            codepoint = (codepoint << 6) + (c & 0x3F);
          }

          break;
        }
      }

      out.push_back(codepoint);
    }

    return out;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
