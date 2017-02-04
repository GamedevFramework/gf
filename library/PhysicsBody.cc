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
#include <gf/PhysicsBody.h>

#include <gf/Collision.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

namespace gf {
inline namespace v1 {

#if 0
  /*
   * Geometry
   */

  class PhysicsBody::Geometry {
  public:
    enum class Type {
      Circle,
      Polygon,
    };

    Geometry(Type type)
    : m_type(type)
    {

    }

    virtual ~Geometry() {
      // nothing to do
    }

    Type getType() const {
      return m_type;
    }

    virtual float getArea() const = 0;
    virtual void render(RenderTarget& target, Vector2f position, float angle) = 0;

  private:
    Type m_type;
  };

  namespace {
    class CircleGeometry : public PhysicsBody::Geometry {
    public:
      CircleGeometry(const CircF& geometry)
      : PhysicsBody::Geometry(PhysicsBody::PhysicsGeometry::Type::Circle)
      , m_radius(geometry.radius)
      {

      }

      float getRadius() const {
        return m_radius;
      }

      CircF getShape(const Matrix3f& transform) {
        Vector2f center(transform.xz, transform.yz);
        return CircF(center, m_radius);
      }

      virtual float getArea() const override {
        return Pi * gf::square(m_radius);
      }

      virtual void render(RenderTarget& target, Vector2f position, float angle) override {
        (void) angle;

        CircleShape shape(CircF(position, m_radius));
        shape.setColor(Color::Transparent);
        shape.setOutlineColor(Color::Red);
        shape.setOutlineThickness(1.0f);
        target.draw(shape);
      }

    private:
      float m_radius;
    };

    class PolygonGeometry : public PhysicsBody::Geometry {
    public:
      PolygonGeometry(const RectF& geometry)
      : PhysicsBody::Geometry(PhysicsBody::PhysicsGeometry::Type::Polygon)
      , m_points({ geometry.getTopLeft(), geometry.getBottomLeft(), geometry.getBottomRight(), geometry.getTopRight() })
      {
        translate(geometry.getCenter());
      }

      PolygonGeometry(const Polygon& geometry)
      : PhysicsBody::Geometry(PhysicsBody::PhysicsGeometry::Type::Polygon)
      , m_points(geometry.begin(), geometry.end())
      {
        translate(geometry.getCenter());
      }

      virtual float getArea() const override {
        Polygon polygon(m_points.begin(), m_points.end());
        return polygon.getArea();
      }

      virtual void render(RenderTarget& target, Vector2f position, float angle) override {
        ConvexShape shape(Polygon(m_points.begin(), m_points.end()));
        shape.setPosition(position);
        shape.setRotation(angle);
        shape.setColor(Color::Transparent);
        shape.setOutlineColor(Color::Red);
        shape.setOutlineThickness(1.0f);
        target.draw(shape);
      }

      Polygon getShape(const Matrix3f& transform) {
        Polygon shape(m_points.begin(), m_points.end());
        shape.applyTransform(transform);
        return shape;
      }

    private:
      void translate(gf::Vector2f center) {
        for (auto& p : m_points) {
          p -= center;
        }
      }

    private:
      std::vector<Vector2f> m_points;
    };

  }
#endif

  /*
   * PhysicsBody
   */

  static constexpr float DefaultRestitution = 0.0f;
  static constexpr float DefaultStaticFriction = 0.0f;
  static constexpr float DefaultDynamicFriction = 0.0f;

  PhysicsBody::PhysicsBody(const PhysicsGeometry& geometry, Type type)
  : m_type(type)
  , m_position(0.0f, 0.0f)
  , m_linearVelocity(0.0f, 0.0f)
  , m_acceleration(0.0f, 0.0f)
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

  void PhysicsBody::update(float dt) {
    // https://www.niksula.hut.fi/~hkankaan/Homepages/gravity.html
    m_linearVelocity += m_acceleration * dt / 2;
    m_position += m_linearVelocity * dt;
    m_linearVelocity += m_acceleration * dt / 2;

    m_linearVelocity *= 1.0f / (1.0f + dt * m_linearDamping);
    m_acceleration = Vector2f(0.0f, 0.0f);
  }

  void PhysicsBody::render(RenderTarget& target) {
    m_geometry.renderAt(target, getPosition(), getAngle());
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

  float PhysicsBody::getAngle() const {
    return gf::angle(m_linearVelocity);
  }

  void PhysicsBody::setAngle(float angle) {
    m_linearVelocity = gf::unit(angle) * gf::euclideanLength(m_linearVelocity);
  }

  void PhysicsBody::turn(float arc) {
    Matrix3f mat = gf::rotation(arc);
    m_linearVelocity = gf::transform(mat, m_linearVelocity);
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

  const Isometry& PhysicsBody::getTransform() const {
    return m_transform;
  }

  bool PhysicsBody::collidesWith(const PhysicsBody& other, Penetration& p) const {
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

}
}
