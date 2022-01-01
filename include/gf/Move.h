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
#ifndef GF_MOVE_H
#define GF_MOVE_H

#include <cstdint>

#include "CoreApi.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_vocabulary
   * @brief An angular move
   *
   * An angular move represents a rotation move to the right or to the left.
   *
   * @sa gf::LinearMove
   */
  enum class AngularMove : uint8_t {
    None,   ///< No angular move
    Left,   ///< Move to the left (i.e. counter-clockwise)
    Right,  ///< Move to the right (i.e. clockwise)
  };

  /**
   * @ingroup core_vocabulary
   * @brief Get the angular factor from the angular move
   *
   * @return @f$ -1 @f$, @f$ 0 @f$ or @f$ 1 @f$ with respect to the move
   */
  GF_CORE_API float angularFactor(AngularMove move);

  /**
   * @ingroup core_vocabulary
   * @brief A linear move
   *
   * A linear move represents a translation move forward or backward.
   *
   * @sa gf::AngularMove
   */
  enum class LinearMove : uint8_t {
    None,       ///< No linear move
    Forward,    ///< Move forward
    Backward,   ///< Move backward
  };

  /**
   * @ingroup core_vocabulary
   * @brief Get the linear factor from the linear move
   *
   * @return @f$ -1 @f$, @f$ 0 @f$ or @f$ 1 @f$ with respect to the move
   */
  GF_CORE_API float linearFactor(LinearMove move);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MOVE_H
