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

  Polygon::Polygon(ArrayRef<Vector2f> points)
  : m_points(points.begin(), points.end())
  {

  }

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

  const Vector2f *Polygon::begin() const {
    return &m_points[0];
  }

  const Vector2f *Polygon::end() const {
    return &m_points[0] + m_points.size();
  }

  bool Polygon::isConvex() const {
    if (m_points.size() <= 3) {
      return true;
    }

    int currentSign = 0;

    for (std::size_t i = 0; i < m_points.size() - 2; ++i) {
      float x = gf::cross(m_points[i + 1] - m_points[i], m_points[i + 2] - m_points[i + 1]);

      if (x > Epsilon) {
        int sign = gf::sign(x);

        if (currentSign != 0 && sign != currentSign) {
          return false;
        }

        currentSign = sign;
      }
    }

    return true;
  }

  Winding Polygon::getWinding() const {
    for (std::size_t i = 0; i < m_points.size() - 1; ++i) {
      float x = gf::cross(m_points[i], m_points[i + 1]);

      if (x > 0) {
        return Winding::Clockwise;
      }

      if (x < 0) {
        return Winding::Counterclockwise;
      }
    }

    return gf::cross(m_points.back(), m_points.front()) > 0 ? Winding::Clockwise : Winding::Counterclockwise;
  }

  // https://en.wikipedia.org/wiki/Shoelace_formula
  float Polygon::getArea() const {
    float area = 0.0f;

    for (std::size_t i = 0; i < m_points.size() - 1; ++i) {
      area += gf::cross(m_points[i], m_points[i + 1]);
    }

    area += gf::cross(m_points.back(), m_points.front());
    return std::abs(area / 2);
  }

  void Polygon::applyTransform(const Matrix3f& mat) {
    for (auto& point : m_points) {
      point = gf::transform(mat, point);
    }
  }

}
}
