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
#include <gf/SpaceTree.h>

#include <cassert>

#include <queue>
#include <stack>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SpaceTree::SpaceTree(const RectI& area)
  : m_area(area)
  , m_split(Split::None)
  , m_position(0)
  , m_level(0)
  , m_left(nullptr)
  , m_right(nullptr)
  , m_father(nullptr)
  {

  }

  void SpaceTree::removeChildren() {
    m_left.reset();
    m_right.reset();
  }

  bool SpaceTree::splitOnce(Random& random, Vector2i minSize, float maxRatio) {
    if (m_left || m_right) {
      return false;
    }

    m_split = random.computeBernoulli(0.5) ? Split::Horizontal : Split::Vertical;

    if (m_area.width >= maxRatio * m_area.height) {
      m_split = Split::Vertical;
    } else if (m_area.height >= maxRatio * m_area.width) {
      m_split = Split::Horizontal;
    }

    if (m_split == Split::Horizontal) {
      if (m_area.height <= 2 * minSize.height) {
        m_split = Split::None;
        return false;
      }

      assert(minSize.height <= m_area.height - minSize.height);
      int height = random.computeUniformInteger(minSize.height, m_area.height - minSize.height);
      m_position = m_area.top + height;

      m_left = std::unique_ptr<SpaceTree>(new SpaceTree({ m_area.left, m_area.top, m_area.width, height }));
      m_right = std::unique_ptr<SpaceTree>(new SpaceTree({ m_area.left, m_position, m_area.width, m_area.height - height }));
    } else {
      assert(m_split == Split::Vertical);

      if (m_area.width <= 2 * minSize.width) {
        m_split = Split::None;
        return false;
      }

      assert(minSize.width <= m_area.width - minSize.width);
      int width = random.computeUniformInteger(minSize.width, m_area.width - minSize.width);
      m_position = m_area.left + width;

      m_left = std::unique_ptr<SpaceTree>(new SpaceTree({ m_area.left, m_area.top, width, m_area.height }));
      m_right = std::unique_ptr<SpaceTree>(new SpaceTree({ m_position, m_area.top, m_area.width - width, m_area.height }));
    }

    m_left->m_father = m_right->m_father = this;
    m_left->m_level = m_right->m_level = m_level + 1;

    return true;
  }

  void SpaceTree::splitRecursive(Random& random, int levelMax, Vector2i minSize, Vector2i maxSize, float maxRatio) {
    if (levelMax == 0) {
      return;
    }

    assert(!m_left && !m_right);

    if (m_area.width > maxSize.width || m_area.height > maxSize.height) {
      if (splitOnce(random, minSize, maxRatio)) {
        assert(m_left);
        m_left->splitRecursive(random, levelMax - 1, minSize, maxSize, maxRatio);
        assert(m_right);
        m_right->splitRecursive(random, levelMax - 1, minSize, maxSize, maxRatio);
      }
    }
  }

  bool SpaceTree::contains(Vector2i position) const {
    return m_area.contains(position);
  }

  const SpaceTree *SpaceTree::find(Vector2i position) const {
    if (!contains(position)) {
      return nullptr;
    }

    if (isLeaf()) {
      return this;
    }

    assert(m_left);
    if (m_left->contains(position)) {
      return m_left->find(position);
    }

    assert(m_right);
    if (m_right->contains(position)) {
      return m_right->find(position);
    }

    return this;
  }

  bool SpaceTree::traversePreOrder(Callback callback) const {
    if (!callback(*this)) {
      return false;
    }

    if (m_left && !m_left->traversePreOrder(callback)) {
      return false;
    }

    if (m_right && !m_right->traversePreOrder(callback)) {
      return false;
    }

    return true;
  }

  bool SpaceTree::traverseInOrder(Callback callback) const {
    if (m_left && !m_left->traverseInOrder(callback)) {
      return false;
    }

    if (!callback(*this)) {
      return false;
    }

    if (m_right && !m_right->traverseInOrder(callback)) {
      return false;
    }

    return true;
  }

  bool SpaceTree::traversePostOrder(Callback callback) const {
    if (m_left && !m_left->traversePostOrder(callback)) {
      return false;
    }

    if (m_right && !m_right->traversePostOrder(callback)) {
      return false;
    }

    if (!callback(*this)) {
      return false;
    }

    return true;
  }

  bool SpaceTree::traverseLevelOrder(Callback callback) const {
    std::queue<const SpaceTree *> queue;
    queue.push(this);

    while (!queue.empty()) {
      auto tree = queue.front();
      assert(tree);
      queue.pop();

      if (!callback(*tree)) {
        return false;
      }

      if (tree->m_left) {
        queue.push(tree->m_left.get());
      }

      if (tree->m_right) {
        queue.push(tree->m_right.get());
      }
    }

    return true;
  }

  bool SpaceTree::traverseInvertedLevelOrder(Callback callback) const {
    std::queue<const SpaceTree *> queue;
    std::stack<const SpaceTree *> stack;

    queue.push(this);

    while (!queue.empty()) {
      auto tree = queue.front();
      assert(tree);
      queue.pop();

      stack.push(tree);

      if (tree->m_left) {
        queue.push(tree->m_left.get());
      }

      if (tree->m_right) {
        queue.push(tree->m_right.get());
      }
    }

    while (!stack.empty()) {
      auto tree = stack.top();
      stack.pop();

      assert(tree);
      if (!callback(*tree)) {
        return false;
      }
    }

    return true;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
