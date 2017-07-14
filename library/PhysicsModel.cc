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
#include <gf/PhysicsModel.h>

#include <gf/PhysicsBody.h>
#include <gf/Collision.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  PhysicsModel::PhysicsModel(Vector2f gravity)
  : m_gravity(gravity)
  {

  }

  void PhysicsModel::addBody(PhysicsBody& body) {
    switch (body.getType()) {
      case PhysicsBody::Static:
        body.updateTransform();
        m_staticBodies.push_back(&body);
        break;

      case PhysicsBody::Dynamic:
        body.updateTransform();
        m_dynamicBodies.push_back(&body);
        break;
    }
  }

  void PhysicsModel::removeBody(PhysicsBody& body) {
    m_dynamicBodies.erase(std::remove(m_dynamicBodies.begin(), m_dynamicBodies.end(), &body), m_dynamicBodies.end());
    m_staticBodies.erase(std::remove(m_staticBodies.begin(), m_staticBodies.end(), &body), m_staticBodies.end());
  }

  void PhysicsModel::clear() {
    m_staticBodies.clear();
    m_dynamicBodies.clear();
  }

  namespace {
    struct ExtendedPenetration {
      Penetration p;
      PhysicsBody *firstPhysicsBody;
      PhysicsBody *secondPhysicsBody;
    };
  }

  static void resolveCollision(const ExtendedPenetration& data) {
    Vector2f relativeVelocity = data.secondPhysicsBody->getLinearVelocity() - data.firstPhysicsBody->getLinearVelocity();
    float velocityAlongNormal = gf::dot(relativeVelocity, data.p.normal);

    if (velocityAlongNormal > 0) {
      return;
    }

    float firstInverseMass = data.firstPhysicsBody->getInverseMass();
    float secondInverseMass = data.secondPhysicsBody->getInverseMass();
    float inverseMassSum = firstInverseMass + secondInverseMass;

    float restitution = std::max(data.firstPhysicsBody->getRestitution(), data.secondPhysicsBody->getRestitution());

    float j = -(1 + restitution) * velocityAlongNormal / inverseMassSum;

    Vector2f impulse = j * data.p.normal;
    data.firstPhysicsBody->applyLinearImpulse(-impulse);
    data.secondPhysicsBody->applyLinearImpulse(impulse);

    Vector2f tangent = relativeVelocity - (gf::dot(relativeVelocity, data.p.normal) * data.p.normal);
    float tangentLength = gf::euclideanLength(tangent);

    if (tangentLength < Epsilon) {
      return;
    }

    tangent /= tangentLength;

    float jt = -gf::dot(relativeVelocity, tangent) / inverseMassSum;

    if (std::abs(jt) < gf::Epsilon) {
      return;
    }

    float staticFriction = std::sqrt(data.firstPhysicsBody->getStaticFriction() * data.secondPhysicsBody->getStaticFriction());
    float dynamicFriction = std::sqrt(data.firstPhysicsBody->getDynamicFriction() * data.secondPhysicsBody->getDynamicFriction());

    Vector2f tangentImpulse;


    if (std::abs(jt) < j * staticFriction) {
      tangentImpulse = jt * tangent;
    } else {
      tangentImpulse = -j * dynamicFriction * tangent;
    }

    data.firstPhysicsBody->applyLinearImpulse(-tangentImpulse);
    data.secondPhysicsBody->applyLinearImpulse(tangentImpulse);
  }

  static constexpr float PenetrationPercentCorrection = 0.4f; // 20% to 80%
  static constexpr float PenetrationSlop = 0.05f; // 0.01 to 0.1

  static void correctPosition(const ExtendedPenetration& data) {
    float firstInverseMass = data.firstPhysicsBody->getInverseMass();
    float secondInverseMass = data.secondPhysicsBody->getInverseMass();
    float inverseMassSum = firstInverseMass + secondInverseMass;
    Vector2f correction = (std::max(data.p.depth - PenetrationSlop, 0.0f) / inverseMassSum * PenetrationPercentCorrection) * data.p.normal;

    data.firstPhysicsBody->move(- firstInverseMass * correction);
    data.secondPhysicsBody->move(secondInverseMass * correction);
  }

  void PhysicsModel::update(Time time) {
    for (auto body : m_dynamicBodies) {
      body->applyForce(m_gravity);
    }

    float dt = time.asSeconds();

    for (auto body : m_dynamicBodies) {
      body->step(dt);
      body->updateTransform();
    }

    ExtendedPenetration data;
    std::vector<ExtendedPenetration> penetrations;

    for (auto body : m_dynamicBodies) {
      data.firstPhysicsBody = body;

      for (auto otherBody : m_dynamicBodies) {
        if (body == otherBody) {
          continue;
        }

        if (body < otherBody) {
          continue;
        }

        if (body->collidesWith(*otherBody, data.p)) {
          data.secondPhysicsBody = otherBody;
          penetrations.push_back(data);
        }
      }

      for (auto otherBody : m_staticBodies) {
        if (body->collidesWith(*otherBody, data.p)) {
          data.secondPhysicsBody = otherBody;
          penetrations.push_back(data);
        }
      }
    }

    for (const auto& p : penetrations) {
      resolveCollision(p);
    }

    for (const auto& p : penetrations) {
      correctPosition(p);
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
