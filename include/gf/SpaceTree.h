/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_SPACE_TREE_H
#define GF_SPACE_TREE_H

#include <functional>
#include <memory>

#include "Portability.h"
#include "Random.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Binary space random partionning tree
   *
   * This class implements a random binary space partitioning tree. More
   * precisely, this class is a node in the tree.
   */
  class GF_API SpaceTree {
  public:
    /**
     * @brief A callback function for traversing the tree
     *
     * The function returns a boolean indicating if the traversal can
     * continue.
     */
    using Callback = std::function<bool(const SpaceTree&)>;

    /**
     * @brief Constructor
     *
     * @param area The area to split for this node
     */
    SpaceTree(const RectI& area);

    /**
     * @brief Get the area of the node
     *
     * @return The area of the node
     */
    const RectI& getArea() const {
      return m_area;
    }

    /**
     * @brief Get the level of the node in the tree
     *
     * The root of the tree is at level 0, its children at level 1, etc.
     *
     * @returns The level of the node
     */
    int getLevel() const {
      return m_level;
    }

    /**
     * @brief Remove the children of the node
     */
    void removeChildren();

    /**
     * @brief Split the node once
     *
     * This function may create two children if the conditions are met.
     *
     * @param random A random generator
     * @param minSize The minimum size under which a node cannot be split
     * @param maxRatio The maximum ratio between the width and height
     * @returns True if the node has actually been split
     */
    bool splitOnce(Random& random, Vector2i minSize, float maxRatio = 1.5f);

    /**
     * @brief Split a node recursively
     *
     * @param random A random generator
     * @param levelMax The maximum level of a node
     * @param minSize The minimum size under which a node cannot be split
     * @param maxSize The maximum size over which a node must be split
     * @param maxRatio The maximum ratio between the width and height
     */
    void splitRecursive(Random& random, int levelMax, Vector2i minSize, Vector2i maxSize, float maxRatio = 1.5f);

    /**
     * @brief Check if a node is a leaf
     *
     * A leaf has no children.
     *
     * @returns True if the node is a leaf
     */
    bool isLeaf() const {
      return !m_left && !m_right;
    }

    /**
     * @brief Get the left child
     *
     * @returns The left child if it exists or `nullptr`
     */
    const SpaceTree *getLeftChild() const {
      return m_left.get();
    }

    /**
     * @brief Get the right child
     *
     * @returns The right child if it exists or `nullptr`
     */
    const SpaceTree *getRightChild() const {
      return m_right.get();
    }

    /**
     * @brief Get the father of the node
     *
     * The root of the tree has no father.
     *
     * @returns The father of the node
     */
    const SpaceTree *getFather() const {
      return m_father;
    }

    /**
     * @brief Check if the area of the node contains a position
     *
     * @param position The position to check
     */
    bool contains(Vector2i position) const;

    /**
     * @brief Find the deepest node containing a position
     *
     * @param position The position to find
     * @returns A node or `nullptr` if the position is outside the tree
     */
    const SpaceTree *find(Vector2i position) const;

    /**
     * @brief Traverse the nodes in pre-order
     *
     * @param callback A function to call on each node
     *
     * @sa [Tree traversal - Wikipedia](https://en.wikipedia.org/wiki/Tree_traversal)
     */
    bool traversePreOrder(Callback callback) const;

    /**
     * @brief Traverse the nodes in in-order
     *
     * @param callback A function to call on each node
     *
     * @sa [Tree traversal - Wikipedia](https://en.wikipedia.org/wiki/Tree_traversal)
     */
    bool traverseInOrder(Callback callback) const;

    /**
     * @brief Traverse the nodes in post-order
     *
     * @param callback A function to call on each node
     *
     * @sa [Tree traversal - Wikipedia](https://en.wikipedia.org/wiki/Tree_traversal)
     */
    bool traversePostOrder(Callback callback) const;

    /**
     * @brief Traverse the nodes in level-order
     *
     * Level-order is also known as breadth first search
     *
     * @param callback A function to call on each node
     *
     * @sa [Tree traversal - Wikipedia](https://en.wikipedia.org/wiki/Tree_traversal)
     */
    bool traverseLevelOrder(Callback callback) const;

    /**
     * @brief Traverse the nodes in inverted level-order
     *
     * @param callback A function to call on each node
     *
     * @sa [Tree traversal - Wikipedia](https://en.wikipedia.org/wiki/Tree_traversal)
     */
    bool traverseInvertedLevelOrder(Callback callback) const;

  private:
    enum class Split {
      None,
      Vertical,
      Horizontal,
    };

    RectI m_area;

    Split m_split;
    int m_position;

    int m_level;

    std::unique_ptr<SpaceTree> m_left;
    std::unique_ptr<SpaceTree> m_right;

    SpaceTree *m_father;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPACE_TREE_H
