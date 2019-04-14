/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <memory>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

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

  std::string formatString(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    std::string res = formatString(fmt, ap);
    va_end(ap);
    return res;
  }

  std::string formatString(const char *fmt, va_list ap) {
    if (fmt == nullptr) {
      return "";
    }

    va_list test;
    va_copy(test, ap);

    int size = std::vsnprintf(nullptr, 0, fmt, test);
    assert(size >= 0);

    ++size; // for '\0'

    auto buffer = std::make_unique<char[]>(size);
    std::vsnprintf(buffer.get(), size, fmt, ap);

    return std::string(buffer.get());
  }

  std::string escapeString(StringRef str) {
    std::string out;

    for (auto c : str) {
      switch (c) {
        case '\n':
          out.append("\\n");
          break;
        case '\t':
          out.append("\\t");
          break;
        case '\"':
          out.append("\\\"");
          break;
        case '\\':
          out.append("\\\\");
          break;
        default:
          out.push_back(c);
          break;
      }
    }

    return out;
  }

  std::vector<StringRef> splitInParagraphs(StringRef str) {
    std::vector<StringRef> out;
    boost::algorithm::split(out, str, boost::is_any_of("\n"), boost::algorithm::token_compress_on);
    out.erase(std::remove_if(out.begin(), out.end(), [](StringRef s) {
      return s.isEmpty();
    }), out.end());
    return out;
  }

  std::vector<StringRef> splitInWords(StringRef str) {
    std::vector<StringRef> out;
    boost::algorithm::split(out, str, boost::is_any_of(U" \t"), boost::algorithm::token_compress_on);
    out.erase(std::remove_if(out.begin(), out.end(), [](StringRef s) {
      return s.isEmpty();
    }), out.end());
    return out;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
