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
#ifndef GF_SPATIAL_R_STAR_TREE_H
#define GF_SPATIAL_R_STAR_TREE_H

#include <cassert>
#include <vector>

#include <boost/container/static_vector.hpp>

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
   * @brief An implemntation of a R* tree
   *
   * More precisely, this class implements the [Revised R* tree](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.367.7273&rep=rep1&type=pdf).
   *
   * @sa gf::QuadTree
   * @sa [R* tree - Wikipedia](https://en.wikipedia.org/wiki/R*_tree)
   */
  class GF_CORE_API RStarTree {
  public:
    static constexpr std::size_t MaxSize = 16;
    static constexpr std::size_t MinSize = 4;

    /**
     * @brief Constructor
     */
    RStarTree();

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
    std::size_t allocateEntry();
    void disposeEntry(std::size_t index);

    std::size_t allocateNode();
    void disposeNode(std::size_t index);

    RectF computeBounds(std::size_t nodeIndex);
    void updateBoundsForChild(std::size_t parentIndex, const RectF& bounds, std::size_t childIndex);

    void doInsert(std::size_t entryIndex, const RectF& bounds);

    std::size_t chooseSubtree(std::size_t nodeIndex, const RectF& bounds);
    std::size_t chooseNode(std::size_t nodeIndex, const RectF& bounds);
    std::size_t searchForCoveringNode(std::size_t nodeIndex, const RectF& bounds);
    bool existsEmptyVolumeExtension(std::size_t nodeIndex, const RectF& bounds);

    struct Candidate {
      std::size_t index;
      float overlap;
      bool isCandidate;
    };

    template<typename OverlapEnlargement>
    std::size_t findCandidates(std::size_t nodeIndex, std::size_t t, std::size_t p, const RectF& bounds, std::vector<Candidate>& candidates);

    std::size_t doInsertInLeaf(std::size_t nodeIndex, std::size_t entryIndex, const RectF& entryBounds);
    std::size_t doInsertInBranch(std::size_t nodeIndex, std::size_t childIndex, const RectF& childBounds);

    std::size_t doQuery(std::size_t nodeIndex, const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind);

    void getEntriesAndDispose(std::size_t nodeIndex, std::vector<std::size_t>& eliminated);
    void doRemove(std::size_t entryIndex);

    void validate() const;
    std::size_t validateNode(std::size_t nodeIndex) const;

  private:
    static constexpr std::size_t Size = MaxSize + 1;

    struct Entry {
      Handle handle;
      RectF bounds;
      std::size_t node;
    };

    BlockAllocator<Entry> m_entries;

    struct Member {
      RectF bounds;
      std::size_t index;
    };

    struct Node {
      enum NodeType {
        Branch,
        Leaf,
      };

      RectF bounds;
      std::size_t parent;
      NodeType type;
      boost::container::static_vector<Member, Size> members;
    };

    BlockAllocator<Node> m_nodes;
    std::size_t m_root;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_R_STAR_TREE_H
