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
#ifndef GF_STRING_UTILS_H
#define GF_STRING_UTILS_H

#include <cstdarg>

#include <vector>
#include <string>

#include "Portability.h"
#include "StringRef.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Create a string representation of a floating point number
   *
   * It is based on [Python niceNum](https://mail.python.org/pipermail/python-list/1999-October/005748.html).
   *
   * @param num The number to display
   * @param precision The precision to use for display
   * @returns A string representing the number
   */
  GF_API std::string niceNum(float num, float precision);

  /**
   * @ingroup core
   * @brief Compute a UTF-32 string from a UTF-8 string
   *
   * @param str A UTF-8 string
   * @returns The corresponding UTF-32 string
   */
  GF_API std::u32string computeUnicodeString(StringRef str);

  /**
   * @ingroup core
   * @brief Format a string like printf
   *
   * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
   */
  GF_API std::string formatString(const char *fmt, ...) GF_FORMAT(1, 2);

  /**
   * @ingroup core
   * @brief Format a string like vprintf
   *
   * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
   * @param ap The arguments of the format string
   */
  GF_API std::string formatString(const char *fmt, va_list ap);


  /**
   * @ingroup core
   */
  std::vector<std::u32string> splitInParagraphs(const std::u32string& str);

  std::vector<std::u32string> splitInWords(const std::u32string& str);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STRING_UTILS_H
