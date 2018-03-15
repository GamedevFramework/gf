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
#ifndef GF_SPATIAL_H
#define GF_SPATIAL_H

#include <memory>
#include <iterator>
#include <vector>

#include <boost/container/static_vector.hpp>

#include "Box.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum class SpatialQuery {
    Contain,
    Intersect,
  };

  template<typename T, typename U = float, std::size_t Size = 16>
  class QuadTree {
  public:
    static_assert(Size > 0, "Size can not be 0");

    QuadTree(const Box<U, 2>& bounds)
    : m_root(bounds)
    {

    }

    bool insert(const T& value, const Box<U, 2>& bounds) {
      return m_root->insert(value, bounds);
    }

    template<typename Iter>
    std::size_t query(const Box<T, 2>& bounds, Iter out, SpatialQuery kind = SpatialQuery::Intersect) const {
      return m_root->query(bounds, out, kind);
    }

    std::vector<T> query(const Box<T, 2>& bounds, SpatialQuery kind = SpatialQuery::Intersect) const {
      std::vector<T> res;
      m_root->query(bounds, std::back_inserter(res), kind);
      return res;
    }

    void clear() {
      m_root.clear();
    }

  private:
    struct Object {
      T value;
      Box<U, 2> bounds;
    };

    class Node {
    public:
      Node(const Box<U, 2>& bounds)
      : m_bounds(bounds)
      , m_children{ nullptr, nullptr, nullptr, nullptr }
      {
        m_data.reserve(Size);
      }

      Node(const Node&) = delete;

      Node(Node&& other)
      : m_bounds(other.m_bounds)
      , m_data(std::move(other.m_data))
      {
        for (std::size_t i = 0; i < 4; ++i) {
          m_children[i] = std::exchange(other.m_children[i], nullptr);
        }
      }

      Node& operator=(const Node&) = delete;

      Node& operator=(Node&& other) {
        std::swap(m_bounds, other.m_bounds);
        std::swap(m_data, other.m_data);
        std::swap(m_children, other.m_children);
        return *this;
      }

      ~Node() {
        for (std::size_t i = 0; i < 4; ++i) {
          delete m_children[i];
        }
      }

      bool isLeaf() const {
        return m_children[0] == nullptr;
      }

      bool insert(const T& value, Box<U, 2> bounds) {
        if (!m_bounds.contains(bounds)) {
          return false;
        }

        if (isLeaf()) {
          if (m_data.size() < Size) {
            m_data.push_back({ value, bounds });
            return true;
          }

          subdivide();
        }

        for (std::size_t i = 0; i < 4; ++i) {
          if (m_children[i]->insert(value, bounds)) {
            return true;
          }
        }

        clearChildrenIfEmpty();

        m_data.push_back({ value, bounds });
        return true;
      }

      template<typename Iter>
      std::size_t query(const Box<T, 2>& bounds, Iter out, SpatialQuery kind) const {
        if (!m_bounds.intersects(bounds)) {
          return 0;
        }

        std::size_t found = 0;

        for (auto& object : m_data) {
          switch (kind) {
            case SpatialQuery::Contain:
              if (bounds.contains(object.bounds)) {
                *out++ = object.value;
                ++found;
              }
              break;

            case SpatialQuery::Intersect:
              if (bounds.intersects(object.bounds)) {
                *out++ = object.value;
                ++found;
              }
              break;
          }
        }

        if (!isLeaf()) {
          for (std::size_t i = 0; i < 4; ++i) {
            found += m_children[i]->query(bounds, out, kind);
          }
        }

        return found;
      }

      void clear() {
        m_data.clear();

        for (std::size_t i = 0; i < 4; ++i) {
          // no need to explicitly clear children
          delete m_children[i];
        }
      }

    private:
      void subdivide() {
        auto upperLeft = std::make_unique<Node>(computeBoxQuadrant(m_bounds, Quadrant::UpperLeft));
        auto upperRight = std::make_unique<Node>(computeBoxQuadrant(m_bounds, Quadrant::UpperRight));
        auto lowerRight = std::make_unique<Node>(computeBoxQuadrant(m_bounds, Quadrant::LowerRight));
        auto lowerLeft = std::make_unique<Node>(computeBoxQuadrant(m_bounds, Quadrant::LowerLeft));

        m_children[0] = upperLeft.release();
        m_children[1] = upperRight.release();
        m_children[2] = lowerRight.release();
        m_children[3] = lowerLeft.release();

        std::vector<Object> data;

        for (auto& object : m_data) {
          for (std::size_t i = 0; i < 4; ++i) {
            if (!m_children[i]->insert(object.value, object.box)) {
              data.push_back(object);
            }
          }
        }

        std::swap(m_data, data);
      }

      void clearChildrenIfEmpty() {
        for (std::size_t i = 0; i < 4; ++i) {
          if (!m_children[i]->m_data.empty()) {
            return;
          }
        }

        for (std::size_t i = 0; i < 4; ++i) {
          delete m_children[i];
        }
      }

    private:
      Box<U, 2> m_bounds;
      std::vector<Object> m_data;
      Node *m_children[4];
    };

  private:
    Node m_root;
  };



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_H
