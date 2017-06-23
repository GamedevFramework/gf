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
#include <gf/PhysicsGeometry.h>

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  PhysicsGeometry::~PhysicsGeometry() {

  }

  CircleGeometry::CircleGeometry(float radius)
  : PhysicsGeometry(PhysicsGeometry::Type::Circle)
  , m_circle({ 0.0f, 0.0f }, radius)
  {

  }

  CircleGeometry::CircleGeometry(CircF circle)
  : PhysicsGeometry(PhysicsGeometry::Type::Circle)
  , m_circle(circle)
  {

  }

  const CircF& CircleGeometry::get() const {
    return m_circle;
  }

  float CircleGeometry::getArea() const {
    return gf::Pi * gf::square(m_circle.radius);
  }

  CircF CircleGeometry::getBoundingCircle() const {
    return m_circle;
  }

  void CircleGeometry::renderAt(RenderTarget& target, const RenderStates& states, Vector2f position, float angle) const {
    gf::unused(angle);

    CircleShape shape(CircF(position, m_circle.radius));
    shape.setOrigin(m_circle.center);
    shape.setColor(Color::Transparent);
    shape.setOutlineColor(Color::Red);
    shape.setOutlineThickness(1.0f);
    target.draw(shape, states);
  }


  PolygonGeometry::PolygonGeometry(Polygon polygon)
  : PhysicsGeometry(PhysicsGeometry::Type::Polygon)
  , m_polygon(std::move(polygon))
  , m_boundingCircle({ 0.0f, 0.0f }, 0.0f)
  {
    computeBoundingCircle();
  }

  PolygonGeometry::PolygonGeometry(Vector2f size)
  : PhysicsGeometry(PhysicsGeometry::Type::Polygon)
  , m_polygon({
      { -size.width / 2, -size.height / 2 },
      {  size.width / 2, -size.height / 2 },
      {  size.width / 2,  size.height / 2 },
      { -size.width / 2,  size.height / 2 },
  })
  , m_boundingCircle({ 0.0f, 0.0f }, 0.0f)
  {
    computeBoundingCircle();
  }

  PolygonGeometry::PolygonGeometry(RectF rectangle)
  : PhysicsGeometry(PhysicsGeometry::Type::Polygon)
  , m_polygon({ rectangle.getTopLeft(), rectangle.getBottomLeft(), rectangle.getBottomRight(), rectangle.getTopRight() })
  , m_boundingCircle({ 0.0f, 0.0f }, 0.0f)
  {
    computeBoundingCircle();
  }


  const Polygon& PolygonGeometry::get() const {
    return m_polygon;
  }

  float PolygonGeometry::getArea() const {
    return m_polygon.getArea();
  }

  CircF PolygonGeometry::getBoundingCircle() const {
    return m_boundingCircle;
  }

  void PolygonGeometry::renderAt(RenderTarget& target, const RenderStates& states, Vector2f position, float angle) const {
    ConvexShape shape(m_polygon);
    shape.setPosition(position);
    shape.setRotation(angle);
    shape.setColor(Color::Transparent);
    shape.setOutlineColor(Color::Red);
    shape.setOutlineThickness(1.0f);
    target.draw(shape, states);
  }

  void PolygonGeometry::computeBoundingCircle() {
    if (m_polygon.getPointCount() == 0) {
      return;
    }

    Vector2f min = m_polygon.getPoint(0);
    Vector2f max = m_polygon.getPoint(0);

    for (auto point : m_polygon) {
      min = gf::min(min, point);
      max = gf::max(max, point);
    }

    m_boundingCircle.center = (min + max) / 2;
    m_boundingCircle.radius = gf::euclideanDistance(min, max) / 2;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
