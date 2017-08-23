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
#ifndef GF_ID_H
#define GF_ID_H

#include <cstdint>
#include <string>

#include "Portability.h"
#include "StringRef.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief An identifier
   */
  using Id = uint64_t;

  /**
   * @ingroup core
   * @brief The invalid id (which is 0)
   */
  constexpr Id InvalidId = 0;

  /**
   * @ingroup core
   * @brief Get an identifier from a string
   *
   * The hash is a Fowler–Noll–Vo 1a hash. This function is mainly for
   * internal use. You should instead use the user-defined literal `_id`.
   *
   * Example:
   *
   * ~~~{.cc}
   * gf::Id id = "A string"_id; // computed at compile time
   * ~~~
   *
   * @param str The string
   * @param sz The size of the string
   */
  constexpr Id hash(const char *str, std::size_t sz) {
    return sz == 0 ? 0xcbf29ce484222325 : (str[sz - 1] ^ hash(str, sz - 1)) * 0x100000001b3;
  }

  /**
   * @ingroup core
   * @brief Get an idenfitier from a string
   *
   * The hash is a Fowler–Noll–Vo 1a hash. This function can be used
   * for computing an id at runtime. For computing an id from a string
   * at compile time, you should use the the user-defined literal `_id`.
   *
   * @param str The string
   * @returns The hash of the string
   */
  constexpr Id hash(StringRef str) {
    return hash(str.getData(), str.getSize());
  }

  namespace literals {

    /**
     * @brief User defined operator for creating compile time ids from strings
     *
     * To use it, you have to use the `gf::literals` namespace:
     *
     * ~~~{.cc}
     * using namespace gf::literals;
     *
     * Id id = "FooBar"_id;
     * ~~~
     *
     */
    constexpr gf::Id operator"" _id(const char *str, std::size_t sz) {
      return gf::hash(str, sz);
    }

  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ID_H
