/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/RandomBinaryTree.h>

#include <cassert>
#include <queue>
#include <stack>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RandomBinaryTree::RandomBinaryTree(const RectI& area)
  {
    m_nodes.push_back(Node(area, 0, 0));
  }

  void RandomBinaryTree::create(Random& random, int levelMax, Vector2i minSize, Vector2i maxSize, float maxRatio) {
    // clear the current nodes
    Node root = m_nodes.front();
    m_nodes.clear();
    root.setChildrenIndices(0, 0);
    m_nodes.push_back(root);

    std::queue<std::size_t> indices;
    indices.push(0);

    while (!indices.empty()) {
      std::size_t current = indices.front();
      indices.pop();

      Node& node = m_nodes[current];
      int level = node.getLevel();

      if (level == levelMax) {
        continue;
      }

      ++level; // level of the children
      std::size_t next = m_nodes.size();

      assert(node.isLeaf());

      auto area = node.getArea();

      if (area.width > maxSize.width || area.height > maxSize.height) {
        bool splitHorizontally = random.computeBernoulli(0.5);

        if (area.width >= maxRatio * area.height) {
          splitHorizontally = false;
        } else if (area.height >= maxRatio * area.width) {
          splitHorizontally = true;
        }

        if (splitHorizontally) {
          if (area.height <= 2 * minSize.height) {
            continue;
          }

          assert(minSize.height < area.height - minSize.height);
          int height = random.computeUniformInteger(minSize.height, area.height - minSize.height);
          int split = area.top + height;

          Node left(RectI(area.left, area.top, area.width, height), current, level);
          Node right(RectI(area.left, split, area.width, area.height - height), current, level);

          node.setChildrenIndices(next, next + 1);
          // do not use `node` after this position

          m_nodes.push_back(left);
          m_nodes.push_back(right);

        } else {

          if (area.width <= 2 * minSize.width) {
            continue;
          }

          assert(minSize.width < area.width - minSize.width);
          int width = random.computeUniformInteger(minSize.width, area.width - minSize.width);
          int split = area.left + width;

          Node left(RectI(area.left, area.top, width, area.height), current, level);
          Node right(RectI(split, area.top, area.width - width, area.height), current, level);

          node.setChildrenIndices(next, next + 1);
          // do not use `node` after this position

          m_nodes.push_back(left);
          m_nodes.push_back(right);
        }

        indices.push(next);
        indices.push(next + 1);
      }
    }
  }

  const RandomBinaryTree::Node& RandomBinaryTree::getRoot() const {
    return m_nodes.front();
  }

  const RandomBinaryTree::Node& RandomBinaryTree::getLeftChild(const Node& node) const {
    auto index = node.getLeftChildIndex();
    assert(index < m_nodes.size());
    return m_nodes[index];
  }

  const RandomBinaryTree::Node& RandomBinaryTree::getRightChild(const Node& node) const {
    auto index = node.getRightChildIndex();
    assert(index < m_nodes.size());
    return m_nodes[index];
  }

  const RandomBinaryTree::Node& RandomBinaryTree::getParent(const Node& node) const {
    auto index = node.getParentIndex();
    assert(index < m_nodes.size());
    return m_nodes[index];
  }

  const RandomBinaryTree::Node& RandomBinaryTree::find(Vector2i position) const {
    if (!getRoot().contains(position)) {
      throw std::runtime_error("Wrong position in the tree");
    }

    std::size_t current = 0;

    for (;;) {
      const Node& node = m_nodes[current];

      if (node.isLeaf()) {
        return node;
      }

      if (getRightChild(node).contains(position)) {
        current = node.getRightChildIndex();
        continue;
      }

      if (getLeftChild(node).contains(position)) {
        current = node.getLeftChildIndex();
        continue;
      }

      return node;
    }

    assert(false);
    return getRoot();
  }

  namespace {
    enum class Order {
      Pre,
      In,
      Post,
    };

    template <Order order>
    bool traverse(const RandomBinaryTree& tree, const RandomBinaryTree::Node& node, RandomBinaryTree::Callback& callback) {
      if (node.isLeaf()) {
        return callback(node);
      }

      if (order == Order::Pre) {
        if (!callback(node)) {
          return false;
        }
      }

      if (!traverse<order>(tree, tree.getLeftChild(node), callback)) {
        return false;
      }

      if (order == Order::In) {
        if (!callback(node)) {
          return false;
        }
      }

      if (!traverse<order>(tree, tree.getRightChild(node), callback)) {
        return false;
      }

      if (order == Order::Post) {
        if (!callback(node)) {
          return false;
        }
      }

      return true;
    }

  } // anonymous namespace

  bool RandomBinaryTree::traversePreOrder(Callback callback) const {
    return traverse<Order::Pre>(*this, getRoot(), callback);
  }

  bool RandomBinaryTree::traverseInOrder(Callback callback) const {
    return traverse<Order::In>(*this, getRoot(), callback);
  }

  bool RandomBinaryTree::traversePostOrder(Callback callback) const {
    return traverse<Order::Post>(*this, getRoot(), callback);
  }

  bool RandomBinaryTree::traverseLevelOrder(Callback callback) const {
    std::queue<std::size_t> queue;
    queue.push(0);

    while (!queue.empty()) {
      std::size_t current = queue.front();
      queue.pop();

      const Node& node = m_nodes[current];

      if (!callback(node)) {
        return false;
      }

      if (node.isLeaf()) {
        continue;
      }

      auto left = node.getLeftChildIndex();
      assert(left != 0);
      queue.push(left);

      auto right = node.getRightChildIndex();
      assert(right != 0);
      queue.push(right);
    }

    return true;
  }

  bool RandomBinaryTree::traverseInvertedLevelOrder(Callback callback) const {
    std::queue<std::size_t> queue;
    std::stack<std::size_t> stack;

    queue.push(0);

    while (!queue.empty()) {
      std::size_t current = queue.front();
      queue.pop();

      stack.push(current);

      const Node& node = m_nodes[current];

      if (node.isLeaf()) {
        continue;
      }

      auto left = node.getLeftChildIndex();
      assert(left != 0);
      queue.push(left);

      auto right = node.getRightChildIndex();
      assert(right != 0);
      queue.push(right);
    }

    while (!stack.empty()) {
      std::size_t current = stack.top();
      stack.pop();

      if (!callback(m_nodes[current])) {
        return false;
      }
    }

    return true;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
