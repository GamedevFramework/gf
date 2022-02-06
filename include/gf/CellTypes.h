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
#ifndef GF_CELL_TYPES_H
#define GF_CELL_TYPES_H

#include <cstdint>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_cells
   * @brief Cell index for staggered or hexagonal maps
   *
   * @sa gf::CellAxis
   */
  enum class CellIndex {
    Odd,  ///< A odd cell index
    Even, ///< An even cell index
  };

  /**
   * @ingroup core_cells
   * @brief Cell axis for staggered or hexagonal maps
   *
   * @sa gf::CellIndex
   */
  enum class CellAxis {
    X,  ///< The x cell axis
    Y,  ///< The y cell axis
  };

  /**
   * @ingroup core_cells
   * @brief The orientation of the cells
   *
   * @sa gf::TileLayer
   */
  enum class CellOrientation : uint8_t {
    Unknown     = 0x00, ///< An unknown orientation
    Orthogonal  = 0x01, ///< An orthogonal orientation
    Isometric   = 0x02, ///< An isometric orientation
    Staggered   = 0x03, ///< A staggered orientation
    Hexagonal   = 0x04, ///< A hexagonal orientation
  };

  /**
   * @ingroup core_cells
   * @brief A flag to indicate how to flip a tile
   *
   * @sa gf::TileLayer
   */
  enum class Flip : uint8_t {
    Horizontally  = 0x01, ///< The tile is flipped horizontally
    Vertically    = 0x02, ///< The tile is flipped vertically
    Diagonally    = 0x04, ///< The tile is flipped anti-diagonally
    Rotation60    = 0x04, ///< The tile is rotated 60° clockwise
    Rotation120   = 0x08, ///< The tile is rotated 120° clockwise
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CELL_TYPES_H
