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
#ifndef GF_ORIENTATION_H
#define GF_ORIENTATION_H

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Cardinal and ordinal orientation
   *
   * gf::Orientation represents one the four cardinal orientations and the
   * four ordinal (or intercardinal) orientations. A special value is added to
   * represent the center, it indicates no orientation.
   *
   * @sa gf::Direction
   */
  enum class Orientation : int {
    Center    = -1, ///< The center, indicates no orientation
    North     =  0, ///< The north orientation
    NorthEast =  1, ///< The north-east orientation
    East      =  2, ///< The east orientation
    SouthEast =  3, ///< The south-east orientation
    South     =  4, ///< The south orientation
    SouthWest =  5, ///< The south-west orientation
    West      =  6, ///< The west orientation
    NorthWest =  7, ///< The north-west orientation
  };


  /**
   * @ingroup core
   * @brief Get a unit vector from an orientation
   *
   * @param orientation The orientation
   * @returns A unit vector representing the orientation
   */
  GF_API Vector2f unit(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get a vector from an orientation
   *
   * The vector has its coordinates at -1, 0 or 1 depending on the orientation.
   * It can be used to represent the displacement on a grid in the given
   * orientation.
   *
   * @param orientation The orientation
   * @returns A vector representing the orientation
   */
  GF_API Vector2i displacement(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get an angle from a orientation
   *
   * North is at angle 0 and angle grows clockwise.
   *
   * @param orientation The orientation
   * @returns an angle (in radians) representing the orientation
   */
  GF_API float angle(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get the opposite orientation
   *
   * @param orientation The orientation
   * @return The opposite orientation
   */
  GF_API Orientation opposite(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get the orthogonal orientation clockwise
   *
   * @param orientation The orientation
   * @return The orthogonal orientation clockwise
   */
  GF_API Orientation orthogonalCW(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get the orthogonal orientation counter-clockwise
   *
   * @param orientation The orientation
   * @return The orthogonal orientation counter-clockwise
   */
  GF_API Orientation orthogonalCCW(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get the next orientation clockwise
   *
   * @param orientation The orientation
   * @return The next orientation clockwise
   */
  GF_API Orientation nextCW(Orientation orientation);

  /**
   * @ingroup core
   * @brief Get the next orientation counter-clockwise
   *
   * @param orientation The orientation
   * @return The next orientation counter-clockwise
   */
  GF_API Orientation nextCCW(Orientation orientation);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_ORIENTATION_H
