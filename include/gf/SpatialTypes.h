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
#ifndef GF_SPATIAL_TYPES_H
#define GF_SPATIAL_TYPES_H

#include <cstddef>
#include <functional>

#include "CoreApi.h"
#include "Handle.h"
#include "Rect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_spatial
   * @brief A spatial id
   *
   * A spatial id refers to an object in a spatial index.
   */
  enum SpatialId : std::size_t { };

  /**
   * @ingroup core_spatial
   * @brief A type of spatial structure
   *
   * @sa gf::SpatialStructure
   */
  enum class SpatialStructureType {
    Object, ///< The structure represents an actuel object
    Node,   ///< The structure represents an internal node
  };

  /**
   * @ingroup core_spatial
   * @brief A spatial structure
   *
   * A spatial structure reflects the internal states of a spatial index.
   *
   * @sa gf::QuadTree, gf::RStarTree
   */
  struct GF_CORE_API SpatialStructure {
    RectF bounds;               ///< The bounds of the structure
    SpatialStructureType type;  ///< The type of the structure
    int level;                  ///< The level of the structure
  };

  /**
   * @ingroup core_spatial
   * @brief A kind of spatial query
   */
  enum class SpatialQuery {
    Contain, ///< Search for all objects that contain the given bounds
    Intersect, ///< Search for all objects that intersect the given bounds
  };

  /**
   * @ingroup core_spatial
   * @brief A callback for spatial query
   */
  using SpatialQueryCallback = std::function<void(Handle)>;


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_TYPES_H
