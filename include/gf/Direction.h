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
#ifndef GF_DIRECTION_H
#define GF_DIRECTION_H

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Main four directions
   *
   * gf::Direction represents one of the four main directions. A special value
   * is added to represent the center, it indicates no direction.
   *
   * @sa gf::Orientation
   */
  enum class Direction : int {
    Center  = -1, ///< The center, indicates no direction
    Up      =  0, ///< The up direction
    Right   =  1, ///< The right direction
    Down    =  2, ///< The down direction
    Left    =  3, ///< The left direction
  };

  /**
   * @ingroup core
   * @brief Get a unit vector from a direction
   *
   * @param direction The direction
   * @returns A unit vector representing the direction
   */
  GF_API Vector2f unit(Direction direction);

  /**
   * @ingroup core
   * @brief Get a vector from a direction
   *
   * The vector has its coordinates at -1, 0 or 1 depending on the direction.
   * It can be used to represent the displacement on a grid in the given
   * direction.
   *
   * @param direction The direction
   * @returns A vector representing the direction
   */
  GF_API Vector2i displacement(Direction direction);

  /**
   * @ingroup core
   * @brief Get an angle from a direction
   *
   * Up is at angle 0 and angle grows clockwise.
   *
   * @param direction The direction
   * @returns an angle (in radians) representing the direction
   */
  GF_API float angle(Direction direction);

  /**
   * @ingroup core
   * @brief Get the opposite direction
   *
   * @param direction The direction
   * @return The opposite direction
   */
  GF_API Direction opposite(Direction direction);

  /**
   * @ingroup core
   * @brief Get the orthogonal direction clockwise
   *
   * @param direction The direction
   * @return The orthogonal direction clockwise
   */
  GF_API Direction orthogonalCW(Direction direction);

  /**
   * @ingroup core
   * @brief Get the orthogonal direction counter-clockwise
   *
   * @param direction The direction
   * @return The orthogonal direction counter-clockwise
   */
  GF_API Direction orthogonalCCW(Direction direction);

  /**
   * @ingroup core
   * @brief Get the next direction clockwise
   *
   * @param direction The direction
   * @return The next direction clockwise
   */
  GF_API Direction nextCW(Direction direction);

  /**
   * @ingroup core
   * @brief Get the next direction counter-clockwise
   *
   * @param direction The direction
   * @return The next direction counter-clockwise
   */
  GF_API Direction nextCCW(Direction direction);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_DIRECTION_H
