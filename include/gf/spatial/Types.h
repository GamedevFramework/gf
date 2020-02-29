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
#ifndef GF_SPATIAL_TYPES_H
#define GF_SPATIAL_TYPES_H

#include <cstddef>
#include <functional>

#include <gf/Box.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A type of spatial structure
   *
   * @sa gf::SpatialStructure
   */
  enum class SpatialStructureType {
    Object, ///< The structure represents an actuel object
    Node,   ///< The structure represents an internal node
  };

  /**
   * @ingroup core
   * @brief A spatial structure
   *
   * A spatial structure reflects the internal states of a spatial index.
   *
   * @sa gf::QuadTree, gf::RStarTree
   */
  template<typename U, std::size_t N>
  struct SpatialStructure {
    Box<U, N> bounds;           ///< The bounds of the structure
    SpatialStructureType type;  ///< The type of the structure
    int level;                  ///< The level of the structure
  };

  /**
   * @ingroup core
   * @brief A kind of spatial query
   */
  enum class SpatialQuery {
    Contain, ///< Search for all objects that contain the given bounds
    Intersect, ///< Search for all objects that intersect the given bounds
  };

  /**
   * @ingroup core
   * @brief A callback for spatial query
   */
  template<typename T>
  using SpatialQueryCallback = std::function<void(const T&)>;


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_TYPES_H
