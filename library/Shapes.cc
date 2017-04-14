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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Shapes.h>

#include <cassert>

#include <gf/Math.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RectangleShape::RectangleShape(Vector2f size)
  : m_size(size)
  {
    updateGeometry();
  }

  RectangleShape::RectangleShape(const RectF& rect)
  : m_size(rect.size)
  {
    updateGeometry();
    setPosition(rect.position);
  }

  void RectangleShape::setSize(Vector2f size) {
    if (m_size == size) {
      return;
    }

    m_size = size;
    updateGeometry();
  }

  std::size_t RectangleShape::getPointCount() const {
    return 4;
  }

  Vector2f RectangleShape::getPoint(std::size_t index) const {
    switch (index) {
      case 0:
        return { 0.0f, 0.0f };
      case 1:
        return { m_size.width, 0.0f };
      case 2:
        return { m_size.width, m_size.height };
      case 3:
        return { 0.0f, m_size.height };
    }

    return { 0.0f, 0.0f };
  }


  CircleShape::CircleShape(float radius, std::size_t pointCount)
  : m_radius(radius)
  , m_pointCount(pointCount)
  {
    updateGeometry();
  }

  CircleShape::CircleShape(const CircF& circ, std::size_t pointCount)
  : m_radius(circ.radius)
  , m_pointCount(pointCount)
  {
    updateGeometry();
    setPosition(circ.center - circ.radius);
  }

  void CircleShape::setRadius(float radius) {
    if (m_radius == radius) {
      return;
    }

    m_radius = radius;
    updateGeometry();
  }

  void CircleShape::setPointCount(std::size_t pointCount) {
    if (m_pointCount == pointCount) {
      return;
    }

    m_pointCount = pointCount;
    updateGeometry();
  }

  std::size_t CircleShape::getPointCount() const {
    return m_pointCount;
  }

  Vector2f CircleShape::getPoint(std::size_t index) const {
    assert(index < m_pointCount);
    float angle = index * 2.0f * Pi / m_pointCount - Pi / 2.0f;
    float x = m_radius * std::cos(angle);
    float y = m_radius * std::sin(angle);
    return { m_radius + x, m_radius + y };
  }


  ConvexShape::ConvexShape(std::size_t pointCount)
  {
    setPointCount(pointCount);
  }

  ConvexShape::ConvexShape(const Polygon& polygon) {
    std::size_t pointCount = polygon.getPointCount();
    m_points.resize(pointCount);

    for (std::size_t i = 0; i < pointCount; ++i) {
      m_points[i] = polygon.getPoint(i);
    }

    updateGeometry();
  }

  void ConvexShape::setPointCount(std::size_t pointCount) {
    m_points.resize(pointCount);
    updateGeometry();
  }

  void ConvexShape::setPoint(std::size_t index, Vector2f point) {
    assert(index < m_points.size());
    m_points[index] = point;
    updateGeometry();
  }

  std::size_t ConvexShape::getPointCount() const {
    return m_points.size();
  }

  Vector2f ConvexShape::getPoint(std::size_t index) const {
    assert(index < m_points.size());
    return m_points[index];
  }

  StarShape::StarShape(float minRadius, float maxRadius, std::size_t branches)
  : m_minRadius(minRadius)
  , m_maxRadius(maxRadius)
  , m_branches(branches)
  {
    updateGeometry();
  }

  void StarShape::setMinRadius(float minRadius) {
    if (m_minRadius == minRadius) {
      return;
    }

    m_minRadius = minRadius;
    updateGeometry();
  }

  void StarShape::setMaxRadius(float maxRadius) {
    if (m_maxRadius == maxRadius) {
      return;
    }

    m_maxRadius = maxRadius;
    updateGeometry();
  }

  void StarShape::setBranches(std::size_t branches) {
    if (m_branches == branches) {
      return;
    }

    m_branches = branches;
    updateGeometry();
  }

  std::size_t StarShape::getPointCount() const {
    return 2 * m_branches;
  }

  Vector2f StarShape::getPoint(std::size_t index) const {
    float angle = index * Pi / m_branches - Pi / 2.0f;

    float radius = (index % 2 == 0) ? m_maxRadius : m_minRadius;
    Vector2f radial = radius * gf::unit(angle);
    return m_maxRadius + radial;
  }

  RoundedRectangleShape::RoundedRectangleShape(Vector2f size, float radius, std::size_t cornerPointCount)
  : m_size(size)
  , m_radius(radius)
  , m_cornerPointCount(cornerPointCount)
  {
    updateGeometry();
  }

  RoundedRectangleShape::RoundedRectangleShape(const RectF& rect, float radius, std::size_t cornerPointCount)
  : m_size(rect.size)
  , m_radius(radius)
  , m_cornerPointCount(cornerPointCount)
  {
    updateGeometry();
    setPosition(rect.position);
  }

  void RoundedRectangleShape::setSize(Vector2f size) {
    if (m_size == size) {
      return;
    }

    m_size = size;
    updateGeometry();
  }

  void RoundedRectangleShape::setRadius(float radius) {
    if (m_radius == radius) {
      return;
    }

    m_radius = radius;
    updateGeometry();
  }

  void RoundedRectangleShape::setCornerPointCount(std::size_t cornerPointCount) {
    if (cornerPointCount < 2) {
      cornerPointCount = 2;
    }

    if (m_cornerPointCount == cornerPointCount) {
      return;
    }

    m_cornerPointCount = cornerPointCount;
    updateGeometry();
  }

  std::size_t RoundedRectangleShape::getPointCount() const {
    return m_cornerPointCount * 4;
  }

  static Vector2f computeCenter(std::size_t quarter, Vector2f size, float radius) {
    switch (quarter) {
      case 0:
        return { radius, radius };
      case 1:
        return { size.width - radius, radius };
      case 2:
        return { size.width - radius, size.height - radius };
      case 3:
        return { radius, size.height - radius };
      default:
        assert(false);
        break;
    }

    return { 0.0f, 0.0f };
  }

  Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    std::size_t quarter = index / m_cornerPointCount;
    assert(quarter <= 3);

    /*
     * quarter 0: top left (clockwise)
     * quarter 1: top right (clockwise)
     * quarter 2: bottom right (clockwise)
     * quarter 3: bottom left (clockwise)
     */

    Vector2f center = computeCenter(quarter, m_size, m_radius);

    std::size_t quarterIndex = index % m_cornerPointCount;
    float angle = Pi2 * static_cast<float>(quarterIndex) / (m_cornerPointCount - 1) + quarter * Pi2 + Pi;
    Vector2f radial = m_radius * gf::unit(angle);

    return center + radial;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
