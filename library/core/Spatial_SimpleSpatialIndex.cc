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
#include <gf/Spatial_SimpleSpatialIndex.h>

#include <cassert>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SimpleSpatialIndex::SimpleSpatialIndex()
  : m_firstFreeEntry(Null)
  {
  }

  SpatialId SimpleSpatialIndex::insert(Handle handle, const RectF& bounds) {
    std::size_t index = Null;

    if (m_firstFreeEntry != Null) {
      index = m_firstFreeEntry;
      m_firstFreeEntry = m_entries[m_firstFreeEntry].next;
    } else {
      index = m_entries.size();
      m_entries.push_back(Entry{});
    }

    assert(index < m_entries.size());
    Entry& entry = m_entries[index];
    entry.handle = handle;
    entry.bounds = bounds;
    entry.next = Occupied;

    return static_cast<SpatialId>(index);
  }

  void SimpleSpatialIndex::modify(SpatialId id, RectF bounds) {
    std::size_t index = static_cast<std::size_t>(id);
    assert(index < m_entries.size());
    m_entries[index].bounds = bounds;
  }

  std::size_t SimpleSpatialIndex::query(const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind) {
    std::size_t found = 0;

    for (auto& entry : m_entries) {
      if (entry.next != Occupied) {
        continue;
      }

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

    return found;
  }

  void SimpleSpatialIndex::remove(SpatialId id) {
    std::size_t index = static_cast<std::size_t>(id);
    assert(index < m_entries.size());
    Entry& entry = m_entries[index];
    assert(entry.next == Occupied);
    entry.next = m_firstFreeEntry;
    m_firstFreeEntry = index;
  }

  void SimpleSpatialIndex::clear() {
    m_entries.clear();
    m_firstFreeEntry = Null;
  }

  Handle SimpleSpatialIndex::operator[](SpatialId id) {
    std::size_t index = static_cast<std::size_t>(id);
    assert(index < m_entries.size());
    return m_entries[index].handle;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
