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
#ifndef GF_SPATIAL_QUAD_TREE_H
#define GF_SPATIAL_QUAD_TREE_H

#include <cassert>
#include <memory>
#include <vector>

#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief An implementation of quadtree
   *
   * @sa gf::RStarTree
   * @sa [Quadtree - Wikipedia](https://en.wikipedia.org/wiki/Quadtree)
   */
  template<typename T, typename U = float, std::size_t Size = 16>
  class QuadTree {
    static_assert(Size > 0, "Size can not be 0");
  public:
    /**
     * @brief Constructor
     *
     * @param bounds The global bounds for objects in the tree
     */
    QuadTree(const Box<U, 2>& bounds)
    : m_root(bounds)
    {

    }

    /**
     * @brief Insert an object in the tree
     *
     * @param value The object to insert
     * @param bounds The bounds of the object
     * @returns True if the object has been inserted
     */
    bool insert(T value, const Box<U, 2>& bounds) {
      Node *node = m_root.chooseNode(bounds);

      if (node == nullptr) {
        return false;
      }

      return node->insert(std::move(value), bounds);
    }

    /**
     * @brief Query objects in the tree
     *
     * @param bounds The bounds of the query
     * @param callback The callback to apply to found objects
     * @param kind The kind of spatial query
     * @returns The number of objects found
     */
    std::size_t query(const Box<U, 2>& bounds, SpatialQueryCallback<T> callback, SpatialQuery kind = SpatialQuery::Intersect) const {
      return m_root.query(bounds, callback, kind);
    }

    /**
     * @brief Remove all the objects from the tree
     */
    void clear() {
      m_root.clear();
    }


    std::vector<SpatialStructure<U, 2>> getStructure() const {
      std::vector<SpatialStructure<U, 2>> structures;
      m_root.appendToStructure(structures, 0);
      return structures;
    }

  private:
    struct Entry {
      T value;
      Box<U, 2> bounds;
    };

    class Node {
    public:
      Node()
      : m_children(nullptr)
      {
        m_entries.reserve(Size);
      }

      Node(const Box<U, 2>& bounds)
      : m_bounds(bounds)
      , m_children(nullptr)
      {
        m_entries.reserve(Size);
      }

      bool isLeaf() const {
        return m_children == nullptr;
      }

      Node *chooseNode(const Box<U, 2>& bounds) {
        if (!m_bounds.contains(bounds)) {
          return nullptr;
        }

        if (isLeaf()) {
          if (m_entries.size() < Size) {
            return this;
          }

          subdivide();

          // try again
          if (m_entries.size() < Size) {
            return this;
          }
        }

        for (std::size_t i = 0; i < 4; ++i) {
          if (m_children[i].chooseNode(bounds) != nullptr) {
            return &m_children[i];
          }
        }

        clearChildrenIfEmpty();

        return this;
      }

      bool insert(T&& value, const Box<U, 2>& bounds) {
        m_entries.push_back({ std::move(value), bounds });
        return true;
      }

      std::size_t query(const Box<U, 2>& bounds, SpatialQueryCallback<T>& callback, SpatialQuery kind) const {
        if (!m_bounds.intersects(bounds)) {
          return 0;
        }

        std::size_t found = 0;

        for (auto& entry : m_entries) {
          switch (kind) {
            case SpatialQuery::Contain:
              if (bounds.contains(entry.bounds)) {
                callback(entry.value);
                ++found;
              }
              break;

            case SpatialQuery::Intersect:
              if (bounds.intersects(entry.bounds)) {
                callback(entry.value);
                ++found;
              }
              break;
          }
        }

        if (!isLeaf()) {
          for (std::size_t i = 0; i < 4; ++i) {
            found += m_children[i].query(bounds, callback, kind);
          }
        }

        return found;
      }

      void clear() {
        m_entries.clear();

        // no need to explicitly clear children
        m_children = nullptr;
      }

      void appendToStructure(std::vector<SpatialStructure<U, 2>>& structures, int level) const {
        structures.push_back({ m_bounds, SpatialStructureType::Node, level });

        for (auto& entry : m_entries) {
          structures.push_back( { entry.bounds, SpatialStructureType::Object, level });
        }

        if (m_children) {
          for (std::size_t i = 0; i < 4; ++i) {
            m_children[i].appendToStructure(structures, level + 1);
          }
        }
      }

    private:
      void subdivide() {
        m_children = std::make_unique<Node[]>(4);

        m_children[0].m_bounds = computeBoxQuarter(m_bounds, Quarter::UpperLeft);
        m_children[1].m_bounds = computeBoxQuarter(m_bounds, Quarter::UpperRight);
        m_children[2].m_bounds = computeBoxQuarter(m_bounds, Quarter::LowerRight);
        m_children[3].m_bounds = computeBoxQuarter(m_bounds, Quarter::LowerLeft);

        std::vector<Entry> entries;

        for (auto& entry : m_entries) {
          bool inserted = false;

          for (std::size_t i = 0; i < 4; ++i) {
            if (m_children[i].m_bounds.contains(entry.bounds)) {
              m_children[i].m_entries.push_back(std::move(entry));
              inserted = true;
              break;
            }
          }

          if (!inserted) {
            entries.push_back(std::move(entry));
          }
        }

        std::swap(m_entries, entries);
      }

      void clearChildrenIfEmpty() {
        assert(!isLeaf());

        for (std::size_t i = 0; i < 4; ++i) {
          if (!m_children[i].m_entries.empty()) {
            return;
          }
        }

        m_children = nullptr;
      }

    private:
      Box<U, 2> m_bounds;
      std::vector<Entry> m_entries;
      std::unique_ptr<Node[]> m_children;
    };

  private:
    Node m_root;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_QUAD_TREE_H
