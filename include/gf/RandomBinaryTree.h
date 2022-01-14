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
#ifndef GF_RANDOM_BINARY_TREE_H
#define GF_RANDOM_BINARY_TREE_H

#include <functional>
#include <memory>

#include "CoreApi.h"
#include "Random.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_roguelike
   * @brief A random binary space partionning tree
   */
  class GF_CORE_API RandomBinaryTree {
  public:

    /**
     * @brief A node of the random binary space partionning tree
     */
    class Node {
    public:
      /**
       * @brief Constructor
       *
       * @param area The area of the node
       * @param parent The index of the parent node
       * @param level The level of the node
       */
      Node(const RectI& area, std::size_t parent, int level)
      : m_parent(parent)
      , m_left(0)
      , m_right(0)
      , m_level(level)
      , m_area(area)
      {

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
       * @brief Check if a node is a leaf
       *
       * A leaf has no children.
       *
       * @returns True if the node is a leaf
       */
      bool isLeaf() const {
        return m_left == 0 && m_right == 0;
      }

      /**
       * @brief Get the parent's indes of the node
       *
       * The root of the tree is its own parent.
       *
       * @returns The parent's index of the node
       */
      std::size_t getParentIndex() const {
        return m_parent;
      }

      /**
       * @brief Get the left child's index
       *
       * @returns The left child's index if it exists or 0
       */
      std::size_t getLeftChildIndex() const {
        return m_left;
      }

      /**
       * @brief Get the right child's index
       *
       * @returns The right child's index if it exists or 0
       */
      std::size_t getRightChildIndex() const {
        return m_right;
      }

      /**
       * @brief Get the area of the node
       *
       * @return The area of the node
       */
      const RectI& getArea() const {
        return m_area;
      }

      /**
       * @brief Check if the area of the node contains a position
       *
       * @param position The position to check
       */
      bool contains(Vector2i position) const {
        return m_area.contains(position);
      }

      /**
       * @brief Set the children indices of the node
       *
       * @param left The index of the left child
       * @param right The index of the right child
       */
      void setChildrenIndices(std::size_t left, std::size_t right) {
        m_left = left;
        m_right = right;
      }

    private:
      std::size_t m_parent;
      std::size_t m_left;
      std::size_t m_right;
      int m_level;
      RectI m_area;
    };

    /**
     * @brief A callback function for traversing the tree
     *
     * The function returns a boolean indicating if the traversal can
     * continue.
     */
    using Callback = std::function<bool(const RandomBinaryTree&, const Node&)>;

    /**
     * @brief Constructor
     *
     * @param area The area to split for this node
     */
    RandomBinaryTree(const RectI& area);

    /**
     * @brief Create the BSP tree
     *
     * @param random A random generator
     * @param levelMax The maximum level of a node
     * @param minSize The minimum size under which a node cannot be split
     * @param maxSize The maximum size over which a node must be split
     * @param maxRatio The maximum ratio between the width and height
     */
    void create(Random& random, int levelMax, Vector2i minSize, Vector2i maxSize, float maxRatio = 1.5f);

    /**
     * @brief Get the root node of the tree
     *
     * The root node always exist.
     */
    const Node& getRoot() const;

    /**
     * @brief Get the left child of a node
     *
     * @returns The left child if it exists or the root node
     */
    const Node& getLeftChild(const Node& node) const;

    /**
     * @brief Get the right child of a node
     *
     * @returns The right child if it exists or the root node
     */
    const Node& getRightChild(const Node& node) const;

    /**
     * @brief Get the parent of the node
     *
     * The root ot the tree is its own parent.
     *
     * @returns The father of the node
     */
    const Node& getParent(const Node& node) const;


    /**
     * @brief Check if the tree contains a position
     *
     * @param position The position to check
     * @returns True if the tree contains the position
     */
    bool contains(Vector2i position) const {
      return getRoot().contains(position);
    }

    /**
     * @brief Find the deepest node containing a position
     *
     * An exception is thrown if the position is outside the tree.
     *
     * @param position The position to find
     * @returns A node containig the position
     *
     * @sa contains()
     */
    const Node& find(Vector2i position) const;

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
    std::vector<Node> m_nodes;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RANDOM_BINARY_TREE_H
