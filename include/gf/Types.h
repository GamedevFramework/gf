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
#ifndef GF_TYPES_H
#define GF_TYPES_H

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Semantic type to represent "none"
   *
   * @sa gf::None, gf::AllType
   */
  struct GF_API NoneType { };

  /**
   * @ingroup core
   * @brief Constant to represent "none"
   *
   * @sa gf::NoneType, gf::All
   */
  constexpr NoneType None = { };

  /**
   * @ingroup core
   * @brief Semantic type to represent "all"
   *
   * @sa gf::All, gf::NoneType
   */
  struct GF_API AllType {  };

  /**
   * @ingroup core
   * @brief Constant to represent "all"
   *
   * @sa gf::AllType, gf::None
   */
  constexpr AllType All = { };

  /**
   * @ingroup core
   * @brief Semantic type to represent "zero"
   *
   * @sa gf::Zero
   */
  struct GF_API ZeroType { };

  /**
   * @ingroup core
   * @brief Constant to represent "zero"
   *
   * @sa gf::ZeroType
   */
  constexpr ZeroType Zero = { };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TYPES_H
