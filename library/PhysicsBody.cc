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
#include <gf/PhysicsBody.h>

#include <gf/Collision.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static constexpr float DefaultRestitution = 0.0f;
  static constexpr float DefaultStaticFriction = 0.0f;
  static constexpr float DefaultDynamicFriction = 0.0f;

  PhysicsBody::PhysicsBody(const PhysicsGeometry& geometry, Type type)
  : m_type(type)
  , m_position(0.0f, 0.0f)
  , m_linearVelocity(0.0f, 0.0f)
  , m_acceleration(0.0f, 0.0f)
  , m_angle(0.0f)
  , m_inverseMass(1.0f)
  , m_restitution(DefaultRestitution)
  , m_staticFriction(DefaultStaticFriction)
  , m_dynamicFriction(DefaultDynamicFriction)
  , m_linearDamping(0.0f)
  , m_geometry(geometry)
  {
    if (m_type == Static) {
      m_inverseMass = 0.0f;
    } else {
      setDensity(1.0f);
    }
  }

  void PhysicsBody::step(float dt) {
    // https://www.niksula.hut.fi/~hkankaan/Homepages/gravity.html
    m_linearVelocity += m_acceleration * dt / 2;
    m_position += m_linearVelocity * dt;
    m_linearVelocity += m_acceleration * dt / 2;

    m_linearVelocity *= 1.0f / (1.0f + dt * m_linearDamping);
    m_acceleration = Vector2f(0.0f, 0.0f);
  }

  void PhysicsBody::render(RenderTarget& target, const RenderStates& states) const {
    m_geometry.renderAt(target, states, getPosition(), getAngle());
  }

  void PhysicsBody::setPosition(Vector2f position) {
    m_position = position;
  }

  void PhysicsBody::move(Vector2f offset) {
    m_position += offset;
  }

  void PhysicsBody::applyLinearImpulse(Vector2f impulse) {
    m_linearVelocity += impulse * m_inverseMass;
  }

  void PhysicsBody::applyForce(Vector2f force) {
    m_acceleration += force * m_inverseMass;
  }

  void PhysicsBody::setVelocityFromAngle() {
    m_linearVelocity = gf::unit(m_angle) * gf::euclideanLength(m_linearVelocity);
  }

  void PhysicsBody::setDensity(float density) {
    float mass = m_geometry.getArea() * density;

    if (mass > Epsilon) {
      m_inverseMass = 1 / mass;
    } else {
      m_inverseMass = 0.0f;
    }
  }

  void PhysicsBody::updateTransform() {
    m_transform.setOffset(getPosition());
    m_transform.setAngle(getAngle());
  }

  const Transform& PhysicsBody::getTransform() const {
    return m_transform;
  }

  bool PhysicsBody::collidesWith(const PhysicsBody& other, Penetration& p) const {
    CircF circle = m_geometry.getBoundingCircle();
    circle.center = gf::transform(m_transform, circle.center);

    CircF otherCircle = other.m_geometry.getBoundingCircle();
    otherCircle.center = gf::transform(other.m_transform, otherCircle.center);

    if (!circle.intersects(otherCircle)) {
      return false;
    }

    switch (m_geometry.getType()) {
      case PhysicsGeometry::Type::Circle: {
        const CircF& circle = static_cast<const CircleGeometry&>(m_geometry).get();

        switch (other.m_geometry.getType()) {
          case PhysicsGeometry::Type::Circle: {
            const CircF& otherCircle = static_cast<const CircleGeometry&>(other.m_geometry).get();
            return gf::collides(circle, m_transform, otherCircle, other.m_transform, p);
          }

          case PhysicsGeometry::Type::Polygon: {
            const Polygon& otherPolygon = static_cast<const PolygonGeometry&>(other.m_geometry).get();
            return gf::collides(circle, m_transform, otherPolygon, other.m_transform, p);
          }
        }

        break;
      }

      case PhysicsGeometry::Type::Polygon: {
        const Polygon& polygon = static_cast<const PolygonGeometry&>(m_geometry).get();

        switch (other.m_geometry.getType()) {
          case PhysicsGeometry::Type::Circle: {
            const CircF& otherCircle = static_cast<const CircleGeometry&>(other.m_geometry).get();
            return gf::collides(polygon, m_transform, otherCircle, other.m_transform, p);
          }

          case PhysicsGeometry::Type::Polygon: {
            const Polygon& otherPolygon = static_cast<const PolygonGeometry&>(other.m_geometry).get();
            return gf::collides(polygon, m_transform, otherPolygon, other.m_transform, p);
          }
        }

        break;
      }
    }

    return false;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
