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
 */
#include <gf/Curves.h>

#include <cassert>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static Vector2f quadraticInterp(Vector2f p0, Vector2f p1, Vector2f p2, float t) {
    assert(0 <= t && t <= 1);
    return p0 * (1 - t) * (1 - t) + p1 * 2.0f * (1 - t) * t + p2 * t * t;
  }

  static Vector2f cubicInterp(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float t) {
    assert(0 <= t && t <= 1);
    return p0 * (1 - t) * (1 - t) * (1 - t)
         + p1 * (1 - t) * (1 - t) * t * 3.0f
         + p2 * (1 - t) * t * t * 3.0f
         + p3 * t * t * t;
  }


  Line::Line(Vector2f p0, Vector2f p1)
  : m_p0(p0)
  , m_p1(p1)
  {
    updateGeometry();
  }

  std::size_t Line::getPointCount() const {
    return 2;
  }

  Vector2f Line::getPoint(std::size_t index) const {
    switch (index) {
      case 0:
        return m_p0;
      case 1:
        return m_p1;
      default:
        assert(false);
        break;
    }

    return Vector2f(0.0f, 0.0f);
  }


  QuadraticBezierCurve::QuadraticBezierCurve(Vector2f p0, Vector2f p1, Vector2f p2, std::size_t pointCount)
  : m_pointCount(pointCount)
  , m_p0(p0)
  , m_p1(p1)
  , m_p2(p2)
  {
    assert(pointCount >= 2);
    updateGeometry();
  }

  std::size_t QuadraticBezierCurve::getPointCount() const {
    return m_pointCount;
  }

  Vector2f QuadraticBezierCurve::getPoint(std::size_t index) const {
    float t = static_cast<float>(index) / (m_pointCount - 1);
    return quadraticInterp(m_p0, m_p1, m_p2, t);
  }


  CubicBezierCurve::CubicBezierCurve(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, std::size_t pointCount)
  : m_pointCount(pointCount)
  , m_p0(p0)
  , m_p1(p1)
  , m_p2(p2)
  , m_p3(p3)
  {
    assert(pointCount >= 2);
    updateGeometry();
  }


  std::size_t CubicBezierCurve::getPointCount() const {
    return m_pointCount;
  }

  Vector2f CubicBezierCurve::getPoint(std::size_t index) const {
    float t = static_cast<float>(index) / (m_pointCount - 1);
    return cubicInterp(m_p0, m_p1, m_p2, m_p3, t);
  }

  CompoundCurve::CompoundCurve(Vector2f origin)
  {
    m_points.push_back(origin);
    updateGeometry();
  }

  CompoundCurve& CompoundCurve::setOrigin(Vector2f origin) {
    assert(!m_points.empty());
    m_points[0] = origin;
    updateGeometry();
    return *this;
  }

  CompoundCurve& CompoundCurve::lineTo(Vector2f p1) {
    assert(!m_points.empty());
    m_points.push_back(p1);
    updateGeometry();
    return *this;
  }

  CompoundCurve& CompoundCurve::quadraticCurveTo(Vector2f p1, Vector2f p2, std::size_t pointCount) {
    assert(!m_points.empty());
    assert(pointCount >= 2);

    Vector2f p0 = m_points.back();

    for (std::size_t i = 1; i < pointCount; ++i) {
      float t = static_cast<float>(i) / (pointCount - 1);
      m_points.push_back(quadraticInterp(p0, p1, p2, t));
    }

    updateGeometry();
    return *this;
  }

  CompoundCurve& CompoundCurve::cubicCurveTo(Vector2f p1, Vector2f p2, Vector2f p3, std::size_t pointCount) {
    assert(!m_points.empty());
    assert(pointCount >= 2);

    Vector2f p0 = m_points.back();

    for (std::size_t i = 1; i < pointCount; ++i) {
      float t = static_cast<float>(i) / (pointCount - 1);
      m_points.push_back(cubicInterp(p0, p1, p2, p3, t));
    }

    updateGeometry();
    return *this;
  }

  void CompoundCurve::close() {
    setClosed();
  }

  CompoundCurve& CompoundCurve::clear(Vector2f origin) {
    setClosed(false);
    m_points.clear();
    m_points.push_back(origin);
    updateGeometry();
    return *this;
  }

  std::size_t CompoundCurve::getPointCount() const {
    return m_points.size();
  }

  Vector2f CompoundCurve::getPoint(std::size_t index) const {
    return m_points[index];
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
