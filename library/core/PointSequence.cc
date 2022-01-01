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
#include <gf/PointSequence.h>

#include <cassert>

#include <algorithm>
#include <numeric>

#include <gf/Geometry.h>
#include <gf/SerializationOps.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  PointSequence::PointSequence(Span<const Vector2f> points)
  : m_points(points.begin(), points.end())
  {

  }


  Vector2f PointSequence::getPoint(std::size_t index) const {
    assert(index < m_points.size());
    return m_points[index];
  }

  Vector2f PointSequence::getCenter() const {
    assert(!m_points.empty());
    return std::accumulate(m_points.begin(), m_points.end(), Vector2f(0.0f, 0.0f)) / m_points.size();
  }

  void PointSequence::applyTransform(const Matrix3f& mat) {
    for (auto& point : m_points) {
      point = gf::transform(mat, point);
    }
  }

  void PointSequence::simplify(float distance) {
    m_points = gf::simplifyPoints(m_points, distance);
  }

  void PointSequence::reverse() {
    std::reverse(m_points.begin(), m_points.end());
  }

  Serializer& operator|(Serializer& ar, const PointSequence& sequence) {
    uint64_t size = sequence.getPointCount();
    ar | size;

    for (uint64_t i = 0; i < size; ++i) {
      Vector2f point = sequence.getPoint(i);
      ar | point;
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, PointSequence& sequence) {
    sequence = PointSequence();

    uint64_t size = 0;
    ar | size;

    for (uint64_t i = 0; i < size; ++i) {
      Vector2f point;
      ar | point;
      sequence.addPoint(point);
    }

    return ar;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
