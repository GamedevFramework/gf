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
   * @brief Cardinal and ordinal directions
   *
   * gf::Direction represents one the four cardinal directions and the four
   * ordinal (or intercardinal) directions. A special value is added to
   * represent the center, it indicates no direction.
   *
   */
  class GF_API Direction {
  public:
    /**
     * @brief Constructor
     *
     * By default, no direction, i.e. center (see gf::Direction::Center)
     */
    Direction()
    : m_dir(-1)
    {
    }

    /**
     * @brief Get a unit vector from a direction
     *
     * @returns A unit vector representing the direction
     */
    Vector2f getUnit() const;

    /**
     * @brief Get a vector from a direction
     *
     * The vector has its coordinates at -1, 0 or 1 depending on the direction.
     * It can be used to represent the displacement on a grid in the given
     * direction.
     *
     * @returns A vector representing the direction
     */
    Vector2i getVector() const;

    /**
     * @brief Get an angle from a direction
     *
     * North is at angle 0 and angle grows clockwise.
     *
     * @returns an angle (in radians) representing the direction
     */
    float getAngle() const;

    /**
     * @brief Get the opposite direction
     *
     * @return The opposite direction
     */
    Direction getOppositeDirection() const;

    /**
     * @brief Get the orthogonal direction clockwise
     *
     * @return The orthogonal direction clockwise
     */
    Direction getOrthogonalDirectionCW() const;

    /**
     * @brief Get the orthogonal direction counter-clockwise
     *
     * @return The orthogonal direction counter-clockwise
     */
    Direction getOrthogonalDirectionCCW() const;

    /**
     * @brief Get the next direction clockwise
     *
     * @return The next direction clockwise
     */
    Direction getNextDirectionCW() const;

    /**
     * @brief Get the next direction counter-clockwise
     *
     * @return The next direction counter-clockwise
     */
    Direction getNextDirectionCCW() const;

    static const Direction Center;    ///< The center, indicates no direction
    static const Direction North;     ///< The north direction
    static const Direction NorthEast; ///< The north-east direction
    static const Direction East;      ///< The east direction
    static const Direction SouthEast; ///< The south-east direction
    static const Direction South;     ///< The south direction
    static const Direction SouthWest; ///< The south-west direction
    static const Direction West;      ///< The west direction
    static const Direction NorthWest; ///< The north-west direction

  private:
    constexpr Direction(int dir)
    : m_dir(dir)
    {

    }

    int m_dir;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_DIRECTION_H
