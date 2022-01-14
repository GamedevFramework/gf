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
#ifndef GF_SPATIAL_SIMPLE_SPATIAL_INDEX_H
#define GF_SPATIAL_SIMPLE_SPATIAL_INDEX_H

#include <vector>

#include "CoreApi.h"
#include "Handle.h"
#include "Rect.h"
#include "SpatialTypes.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_spatial
   * @brief An very simple spatial index
   *
   * @sa gf::Quadtree, gf::RStarTree
   */
  class GF_CORE_API SimpleSpatialIndex {
  public:
    /**
     * @brief Constructor
     */
    SimpleSpatialIndex();

     /**
     * @brief Insert an object in the tree
     *
     * @param handle A handle that represents the object to insert
     * @param bounds The bounds of the object
     * @returns A spatial id
     */
    SpatialId insert(Handle handle, const RectF& bounds);

    /**
     * @brief Modify the bounds of an object
     *
     * @param id The spatial id of the object
     * @param bounds The new bounds of the object
     */
    void modify(SpatialId id, RectF bounds);

    /**
     * @brief Query objects in the tree
     *
     * @param bounds The bounds of the query
     * @param callback The callback to apply to found objects
     * @param kind The kind of spatial query
     * @returns The number of objects found
     */
    std::size_t query(const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind = SpatialQuery::Intersect);

    /**
     * @brief Remove an object from the tree
     *
     * @param id The spatial id of the object
     */
    void remove(SpatialId id);

    /**
     * @brief Remove all the objects from the tree
     */
    void clear();

    /**
     * @brief Get the handle associated to a spatial id
     *
     * @param id The spatial id of the object
     */
    Handle operator[](SpatialId id);

  private:
    static constexpr std::size_t Null = static_cast<std::size_t>(-1);
    static constexpr std::size_t Occupied = static_cast<std::size_t>(-2);

    struct Entry {
      Handle handle;
      RectF bounds;
      std::size_t next;
    };

    std::vector<Entry> m_entries;
    std::size_t m_firstFreeEntry;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_SIMPLE_SPATIAL_INDEX_H
