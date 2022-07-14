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
#include <gf/Spatial_Quadtree.h>

#include <algorithm>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Quadtree::Quadtree(const RectF& bounds)
  : m_root(Null)
  {
    m_root = allocateNode();
    m_nodes[m_root].bounds = bounds;
  }

  SpatialId Quadtree::insert(Handle handle, const RectF& bounds) {
    std::size_t index = allocateEntry();
    m_entries[index].handle = handle;
    m_entries[index].bounds = bounds;

    assert(m_root != Null);
    [[maybe_unused]] bool inserted = doInsert(index, m_root);
    assert(inserted);

    return static_cast<SpatialId>(index);
  }

  void Quadtree::modify(SpatialId id, RectF bounds) {
    std::size_t entryIndex = static_cast<std::size_t>(id);
    doRemove(entryIndex);
    Entry& entry = m_entries[entryIndex];
    entry.bounds = bounds;
    assert(m_root != Null);
    doInsert(entryIndex, m_root);
  }

  std::size_t Quadtree::query(const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind) {
    return doQuery(m_root, bounds, callback, kind);
  }

  void Quadtree::remove(SpatialId id) {
    std::size_t entryIndex = static_cast<std::size_t>(id);
    doRemove(entryIndex);
    disposeEntry(entryIndex);
  }

  void Quadtree::clear() {
    m_entries.clear();
    m_nodes.clear();
  }

  Handle Quadtree::operator[](SpatialId id) {
    std::size_t index = static_cast<std::size_t>(id);
    return m_entries[index].handle;
  }

  std::size_t Quadtree::allocateEntry() {
    return m_entries.allocate();
  }

  void Quadtree::disposeEntry(std::size_t index) {
    m_entries.dispose(index);
  }

  std::size_t Quadtree::allocateNode() {
    std::size_t index = m_nodes.allocate();

    Node& node = m_nodes[index];
    assert(node.entries.empty());
    node.parent = Null;
    node.children[0] = Null;
    node.children[1] = Null;
    node.children[2] = Null;
    node.children[3] = Null;

    return index;
  }

  void Quadtree::disposeNode(std::size_t index) {
    assert(m_nodes[index].entries.empty());
    m_nodes.dispose(index);
  }

  bool Quadtree::doInsert(std::size_t entryIndex, std::size_t nodeIndex) {
    Entry& entry = m_entries[entryIndex];
    Node& node = m_nodes[nodeIndex];

    if (!node.bounds.contains(entry.bounds)) {
      return false;
    }

    if (node.isLeaf()) {
      if (node.entries.size() < Size) {
        node.entries.push_back(entryIndex);
        entry.node = nodeIndex;
        return true;
      }

      subdivide(nodeIndex);
      // node reference may be invalid starting here

      if (m_nodes[nodeIndex].entries.size() < Size) {
        m_nodes[nodeIndex].entries.push_back(entryIndex);
        entry.node = nodeIndex;
        return true;
      }
    }

    for (auto childIndex : m_nodes[nodeIndex].children) {
      if (doInsert(entryIndex, childIndex)) {
        return true;
      }
    }

    m_nodes[nodeIndex].entries.push_back(entryIndex);
    entry.node = nodeIndex;

    sanitize(nodeIndex);
    return true;
  }

  std::size_t Quadtree::doQuery(std::size_t nodeIndex, const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind) {
    if (nodeIndex == Null) {
      return 0;
    }

    Node& node = m_nodes[nodeIndex];

    if (!node.bounds.intersects(bounds)) {
      return 0;
    }

    std::size_t found = 0;

    for (auto entryIndex : node.entries) {
      Entry& entry = m_entries[entryIndex];

      switch (kind) {
        case SpatialQuery::Contain:
          if (bounds.contains(entry.bounds)) {
            callback(entry.handle);
            ++found;
          }
          break;

        case SpatialQuery::Intersect:
          if (bounds.intersects(entry.bounds)) {
            callback(entry.handle);
            ++found;
          }
          break;
      }
    }

    if (!node.isLeaf()) {
      for (auto childIndex : node.children) {
        found += doQuery(childIndex, bounds, callback, kind);
      }
    }

    return found;
  }

  void Quadtree::doRemove(std::size_t entryIndex) {
    Entry& entry = m_entries[entryIndex];

    std::size_t nodeIndex = entry.node;
    Node& node = m_nodes[nodeIndex];

    auto it = std::find(node.entries.begin(), node.entries.end(), entryIndex);
    assert(it != node.entries.end());
    node.entries.erase(it);

    if (node.entries.empty()) {
      sanitize(node.parent);
    }
  }


  void Quadtree::subdivide(std::size_t nodeIndex) {
    assert(m_nodes[nodeIndex].isLeaf());

    std::size_t i = 0;

    for (auto quarter : { Quarter::UpperLeft, Quarter::UpperRight, Quarter::LowerRight, Quarter::LowerLeft }) {
      auto childIndex = allocateNode();
      m_nodes[nodeIndex].children[i++] = childIndex;
      Node& child = m_nodes[childIndex];
      child.bounds = computeBoxQuarter(m_nodes[nodeIndex].bounds, quarter);
      child.parent = nodeIndex;
    }

    assert(i == 4);
    // m_nodes may have grown before this point
    Node& node = m_nodes[nodeIndex];

    std::vector<std::size_t> entries;
    std::swap(entries, node.entries);

    for (auto entryIndex : entries) {
      Entry& entry = m_entries[entryIndex];
      bool inserted = false;

      for (auto childIndex : node.children) {
        Node& child = m_nodes[childIndex];
        if (child.bounds.contains(entry.bounds)) {
          child.entries.push_back(entryIndex);
          entry.node = childIndex;
          inserted = true;
          break;
        }
      }

      if (!inserted) {
        node.entries.push_back(entryIndex);
        assert(entry.node == nodeIndex);
      }
    }
  }

  void Quadtree::sanitize(std::size_t nodeIndex) {
    for (;;) {
      Node& node = m_nodes[nodeIndex];
      assert(!node.isLeaf());

      for (auto childIndex : node.children) {
        Node& child = m_nodes[childIndex];

        if (!child.entries.empty()) {
          return;
        }
      }

      for (auto& childIndex : node.children) {
        disposeNode(childIndex);
        childIndex = Null;
      }

      if (!node.entries.empty()) {
        return;
      }

      if (node.parent == Null) {
        return;
      } else {
        nodeIndex = node.parent;
      }
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
