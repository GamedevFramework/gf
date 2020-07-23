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
#ifndef GF_HEXAGON_H
#define GF_HEXAGON_H

#include "CoreApi.h"
#include "MapCell.h"
#include "Math.h"
#include "Rect.h"
#include "Vector.h"

#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

   /**
   * @ingroup game
   * @brief A helper for computing coordinates in a hexagonal map
   */
  class GF_CORE_API HexagonHelper {
  public:

    /**
     * @brief Constructor
     *
     * @param axis The orientation of hexagon cells. X for pointy and Y for flat
     * @param index The index of data storage. Odd or Even indicate on which col or row is the offset
     *
     * @sa gf::MapCellAxis and gf::MapCellIndex
     */
    HexagonHelper(MapCellAxis axis, MapCellIndex index)
    : m_axis(axis)
    , m_index(index) {
    }

    /**
     * @brief Get the hexagon size
     *
     * @param radius Radius of hexagon
     *
     * @returns The current hexagon size
     */
    Vector2f getHexagonSize(float radius) const noexcept;

    /**
     * @brief Get the bounds for hexagon cells
     *
     * @param size Number of cells
     * @param radius Radius of hexagon
     *
     * @returns The current hexagon size
     */
    RectF computeBounds(Vector2i size, float radius) const noexcept;

    /**
     * @brief Compute the center of the hexagon
     *
     * @param coords The size of the hexagon in the map
     * @param radius Radius of hexagon
     *
     * @returns The position of the center
     */
    Vector2f computeCenter(Vector2i coords, float radius) const;

    /**
     * @brief Compute the six corners of the hexagon
     *
     * @param coords The size of the hexagon in the map
     * @param radius Radius of hexagon
     *
     * @returns The position of six corners
     */
    std::vector<Vector2f> computeCorners(Vector2i coords, float radius) const;

  private:
    MapCellAxis m_axis;
    MapCellIndex m_index;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_HEXAGON_H
