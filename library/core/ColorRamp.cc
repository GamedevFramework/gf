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
#include <gf/ColorRamp.h>

#include <cassert>
#include <utility>

#include <gf/Color.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  ColorRamp::ColorRamp()
  : m_min(0.0f)
  , m_max(0.0f)
  {

  }

  bool ColorRamp::isEmpty() const {
    return m_map.empty();
  }

  void ColorRamp::addColorStop(float offset, const Color4f& color) {
    if (isEmpty()) {
      m_min = m_max = offset;
    } else {
      if (offset < m_min) {
        m_min = offset;
      }

      if (offset > m_max) {
        m_max = offset;
      }
    }

    m_map.insert(std::make_pair(offset, color));
  }

  Color4f ColorRamp::computeColor(float offset) const {
    if (m_map.empty()) {
      return Color::White;
    }

    if (offset < m_min) {
      return m_map.begin()->second;
    }

    if (offset > m_max) {
      return m_map.rbegin()->second;
    }

    auto it = m_map.lower_bound(offset);
    assert(it != m_map.end());

    float t2 = it->first;
    Color4f c2 = it->second;

    if (it == m_map.begin()) {
      return c2;
    }

    --it;
    float t1 = it->first;
    Color4f c1 = it->second;

    return gf::lerp(c1, c2, (offset - t1) / (t2 - t1));
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
