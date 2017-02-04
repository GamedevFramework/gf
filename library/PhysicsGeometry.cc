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
#include <gf/PhysicsGeometry.h>

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "priv/Utils.h"

namespace gf {
inline namespace v1 {

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

  void CircleGeometry::renderAt(RenderTarget& target, Vector2f position, float angle) const {
    GF_UNUSED(angle);

    CircleShape shape(CircF(position, m_circle.radius));
    shape.setOrigin(m_circle.center);
    shape.setColor(Color::Transparent);
    shape.setOutlineColor(Color::Red);
    shape.setOutlineThickness(1.0f);
    target.draw(shape);
  }


  PolygonGeometry::PolygonGeometry(Polygon polygon)
  : PhysicsGeometry(PhysicsGeometry::Type::Polygon)
  , m_polygon(std::move(polygon))
  {

  }

  PolygonGeometry::PolygonGeometry(Vector2f size)
  : PhysicsGeometry(PhysicsGeometry::Type::Polygon)
  , m_polygon({
      { -size.width / 2, -size.height / 2 },
      {  size.width / 2, -size.height / 2 },
      {  size.width / 2,  size.height / 2 },
      { -size.width / 2,  size.height / 2 },
  })
  {

  }

  PolygonGeometry::PolygonGeometry(RectF rectangle)
  : PhysicsGeometry(PhysicsGeometry::Type::Polygon)
  , m_polygon({ rectangle.getTopLeft(), rectangle.getBottomLeft(), rectangle.getBottomRight(), rectangle.getTopRight() })
  {

  }


  const Polygon& PolygonGeometry::get() const {
    return m_polygon;
  }

  float PolygonGeometry::getArea() const {
    return m_polygon.getArea();
  }

  void PolygonGeometry::renderAt(RenderTarget& target, Vector2f position, float angle) const {
    ConvexShape shape(m_polygon);
    shape.setPosition(position);
    shape.setRotation(angle);
    shape.setColor(Color::Transparent);
    shape.setOutlineColor(Color::Red);
    shape.setOutlineThickness(1.0f);
    target.draw(shape);
  }

}
}
