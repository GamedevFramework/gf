/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#define GF_STAGGER_H

#include "CoreApi.h"
#include "MapCell.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_utilities
   * @brief A helper for computing coordinates in a staggered map
   */
  class GF_CORE_API StaggerHelper {
  public:
    StaggerHelper(MapCellAxis axis, MapCellIndex index)
    : m_axis(axis)
    , m_index(index)
    {

    }

    /**
     * @brief Compute the center of the tile
     *
     * @param coords The coordinates of the tile in the map
     * @param size The size of the tile in the map
     * @returns The position of the center
     */
    Vector2f computeCenter(Vector2i coords, Vector2f size) const;

    /**
     * @brief Compute the position of the tile
     *
     * @param coords The coordinates of the tile in the map
     * @param size The size of the tile in the map
     * @returns The position of the center
     */
    Vector2f computeTilePosition(Vector2i coords, Vector2f size) const;

    /**
     * @brief Compute the coordinates of a point
     *
     * @param point The point in the map
     * @param size The size of the tile in the map
     * @returns The coordinates of the point
     */
    Vector2i computeCoords(Vector2f point, Vector2f size) const;

    /**
     * @brief Compute the bounds of the layer
     *
     * @param layer The size of the layer in tiles
     * @param size The size of the tile in the map
     * @returns The bounds of the map
     */
    RectF computeBounds(Vector2i layer, Vector2f size) const;

  private:
    MapCellAxis m_axis;
    MapCellIndex m_index;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STAGGER_H
