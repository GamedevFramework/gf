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
#ifndef GF_STATIC_STRING_H
#define GF_STATIC_STRING_H

#include <cassert>
#include <cstddef>
#include <stdexcept>

#include "StringRef.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A static string
   */
  template<std::size_t N>
  class StaticString {
  public:
    StaticString() = default;

    StaticString(StringRef other)
    {
      assign(other);
    }

    StaticString& operator=(StringRef other) {
      assign(other);
      return *this;
    }

    void clear() {
      m_data[0] = '\0';
    }

    const char *getData() const {
      return m_data;
    }

    char *getData() {
      return m_data;
    }

    std::size_t getSize() const {
      return N;
    }

    char& operator[](std::size_t i) {
      assert(i <= N);
      return m_data[i];
    }

    char operator[](std::size_t i) const {
      assert(i <= N);
      return m_data[i];
    }

  private:
    void assign(StringRef other) {
      std::size_t size = other.getSize();

      if (size >= N) {
        throw std::runtime_error("String too long!");
      }

      for (std::size_t i = 0; i < size; ++i) {
        m_data[i] = other[i];
      }

      m_data[size] = '\0';
    }

  private:
    char m_data[N + 1];
  };

  template<typename Archive, std::size_t N>
  Archive& operator|(Archive& ar, StaticString<N>& str) {
    for (std::size_t i = 0; i <= N; ++i) {
      ar | str[i];
    }

    return ar;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STATIC_STRING_H
