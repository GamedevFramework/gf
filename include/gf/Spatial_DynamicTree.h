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
#ifndef GF_SPATIAL_DYNAMIC_TREE_H
#define GF_SPATIAL_DYNAMIC_TREE_H

#include <cassert>
#include <vector>

#include "BlockAllocator.h"
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
   * @brief An implementation of dynamic tree
   */
  class GF_CORE_API DynamicTree {
  public:
    /**
     * @brief Constructor
     */
    DynamicTree();

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
    std::size_t allocateNode();
    void disposeNode(std::size_t index);

    void doInsert(std::size_t leaf);
    void doRemove(std::size_t leaf);

    std::size_t balance(std::size_t iA);

  private:
    struct Node {
      Handle handle;
      RectF bounds;
      std::size_t parent;
      std::size_t child1;
      std::size_t child2;
      int32_t height;

      bool isLeaf() const {
        return child1 == NullIndex;
      }
    };

    BlockAllocator<Node> m_nodes;

    std::size_t m_root;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_DYNAMIC_TREE_H
