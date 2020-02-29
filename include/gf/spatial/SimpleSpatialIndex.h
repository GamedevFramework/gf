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
#ifndef GF_SPATIAL_SIMPLE_SPATIAL_INDEX_H
#define GF_SPATIAL_SIMPLE_SPATIAL_INDEX_H

#include <vector>

#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief An very simple spatial index
   *
   * @sa gf::QuadTree, gf::RStarTree
   * @sa [Quadtree - Wikipedia](https://en.wikipedia.org/wiki/Quadtree)
   */
  template<typename T, typename U = float, std::size_t N = 2>
  class SimpleSpatialIndex {
  public:
    bool insert(T value, const gf::Box<U, N>& bounds) {
      m_entries.push_back({ std::move(value), bounds });
      return true;
    }

    std::size_t query(const gf::Box<U, N>& bounds, gf::SpatialQueryCallback<T> callback, gf::SpatialQuery kind) {
      std::size_t found = 0;

      for (auto& entry : m_entries) {
        switch (kind) {
          case gf::SpatialQuery::Contain:
            if (bounds.contains(entry.bounds)) {
              callback(entry.value);
              ++found;
            }
            break;

          case gf::SpatialQuery::Intersect:
            if (bounds.intersects(entry.bounds)) {
              callback(entry.value);
              ++found;
            }
            break;
        }
      }

      return found;
    }

  private:
    struct Entry {
      T value;
      gf::Box<U, N> bounds;
    };

    std::vector<Entry> m_entries;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_SIMPLE_SPATIAL_INDEX_H
