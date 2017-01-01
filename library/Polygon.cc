/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <gf/Polygon.h>

#include <cassert>

#include <algorithm>
#include <numeric>

#include <gf/Transform.h>
#include <gf/VectorOps.h>

namespace gf {
inline namespace v1 {

  void Polygon::addPoint(Vector2f point) {
    m_points.push_back(point);
  }

  std::size_t Polygon::getPointCount() const {
    return m_points.size();
  }

  Vector2f Polygon::getPoint(std::size_t index) const {
    assert(index < m_points.size());
    return m_points[index];
  }

  Vector2f Polygon::getCenter() const {
    assert(!m_points.empty());
    return std::accumulate(m_points.begin(), m_points.end(), Vector2f(0.0f, 0.0f)) / m_points.size();
  }

  Vector2f Polygon::getSupport(Vector2f direction) const {
    assert(!m_points.empty());
    return *std::max_element(m_points.begin(), m_points.end(), [direction](const Vector2f& lhs, const Vector2f& rhs){
      return gf::dot(direction, lhs) < gf::dot(direction, rhs);
    });
  }

  void Polygon::applyTransform(const Matrix3f& mat) {
    for (auto& point : m_points) {
      point = gf::transform(mat, point);
    }
  }

}
}
