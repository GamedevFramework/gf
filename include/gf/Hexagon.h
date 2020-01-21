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
#ifndef GF_STAGGER_H
#define GF_HEXAGON_H

#include <gf/Math.h>
#include <gf/Vector.h>

#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

   /**
   * @ingroup game
   * @brief A helper for computing coordinates in a hexagonal map
   *
   * @param coordinateUnitLength The length of the unit of coordinate system to represent the hexagons
   */
  class HexagonHelper {
  public:

    /**
     * @brief Constructor
     *
     * @param coordinateUnitLength The coordinate system unit length to represent the hexagons
     * */
    HexagonHelper(float coordinateUnitLength)
    : m_coordinateUnitLength(coordinateUnitLength)
    , m_hexagonSize({ coordinateUnitLength * 2.0f, coordinateUnitLength * Sqrt3 }) {
    }

    /**
     * @brief Set the coordinate system unit length
     *
     * @param coordinateUnitLength The new coordinate system unit length to represent the hexagons
     */
    void setCoordinateUnitLength(float coordinateUnitLength);

    /**
     * @brief Get the coordinate system unit length
     *
     * @returns The current coordinate system unit length
     */
    float getCoordinateUnitLength() const noexcept {
      return m_coordinateUnitLength;
    }

    /**
     * @brief Get the hexagon size
     *
     * @returns The current hexagon size
     */
    Vector2f getHexagonSize() const noexcept {
      return m_hexagonSize;
    }


    /**
     * @brief Compute the center of the hexagon
     *
     * @param coords The size of the hexagon in the map
     * @returns The position of the center
     */
    Vector2f computeCenter(Vector2i coords) const;

    /**
     * @brief Compute the six corners of the hexagon
     *
     * @param coords The size of the hexagon in the map
     * @returns The position of six corners
     */
    std::vector<Vector2f> computeCorners(Vector2i coords) const;

  private:
    float m_coordinateUnitLength;
    Vector2f m_hexagonSize;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_HEXAGON_H
