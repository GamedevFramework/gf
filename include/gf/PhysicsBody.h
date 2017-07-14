#ifndef GF_PHYSICS_BODY_H
#define GF_PHYSICS_BODY_H

#include <memory>

#include "Circ.h"
#include "PhysicsGeometry.h"
#include "Polygon.h"
#include "Rect.h"
#include "Time.h"
#include "Transform.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;
  struct Penetration;

  /**
   * @ingroup game
   * @brief A physics body
   *
   * A body has several properties:
   *
   * - [restitution](https://en.wikipedia.org/wiki/Coefficient_of_restitution) (default: 0)
   * - [static and dynamic friction](https://en.wikipedia.org/wiki/Friction#Coefficient_of_friction) (default: 0)
   * - [linear damping](https://en.wikipedia.org/wiki/Damping_ratio) (default: 0)
   * - [density](https://en.wikipedia.org/wiki/Density) (default: 1)
   *
   * @sa gf::PhysicsGeometry, gf::PhysicsModel
   */
  class GF_API PhysicsBody {
  public:
    /**
     * @brief Type of body
     */
    enum Type {
      Static, ///< Static body with infinite mass
      Dynamic, ///< Dynamic bodies with finite mass
    };

    /**
     * @brief Constructor
     *
     * @param geometry The geometry of the body
     * @param type The type of the body (default: dynamic)
     */
    PhysicsBody(const PhysicsGeometry& geometry, Type type = Dynamic);

    /**
     * @brief Get the type of the body
     *
     * @returns The type of the body
     */
    Type getType() const {
      return m_type;
    }

    /**
     * @brief Update the model's state
     *
     * @param dt The time (in seconds) since the last update
     */
    void step(float dt);

    /**
     * @brief Render the body
     *
     * This function is for debugging purpose. The body is outlined in red.
     *
     * @param target The render target
     * @param states The render states to use for drawing
     */
    void render(RenderTarget& target, const RenderStates& states) const;

    /**
     * @name Position, angle, velocity and acceleration
     * @{
     */

    /**
     * @brief Get the position of the body
     *
     * @returns The current position of the body
     */
    Vector2f getPosition() const {
      return m_position;
    }

    /**
     * @brief Set the position of the body
     *
     * @param position The new position of the body
     */
    void setPosition(Vector2f position);

    /**
     * @brief Move the body
     *
     * @param offset The offset to apply to the position of the body
     */
    void move(Vector2f offset);

    /**
     * @brief Get the linear velocity of the body
     *
     * @returns The current linear velocity
     */
    Vector2f getLinearVelocity() const {
      return m_linearVelocity;
    }

    /**
     * @brief Set the linear velocity of the body
     *
     * @param velocity The new velocity
     */
    void setLinearVelocity(Vector2f velocity) {
      m_linearVelocity = velocity;
    }

    /**
     * @brief Apply a linear impulse
     *
     * The impulse changes the velocity.
     *
     * @param impulse The impulse to apply to the velocity of the body
     */
    void applyLinearImpulse(Vector2f impulse);

    /**
     * @brief Get the acceleration of the body
     *
     * @returns The current acceleration
     */
    Vector2f getAcceleration() const {
      return m_acceleration;
    }

    /**
     * @brief Apply a force
     *
     * The force changes the acceleration (by Newton's law)
     *
     * @param force The force to apply to the acceleration of the body
     */
    void applyForce(Vector2f force);

    /**
     * @brief Get the angle of the body
     *
     * @returns The angle of the body
     */
    float getAngle() const {
      return m_angle;
    }

    /**
     * @brief Set the angle of the body
     *
     * @param angle The new angle
     */
    void setAngle(float angle) {
      m_angle = angle;
    }

    /**
     * @brief Change the angle of the body
     *
     * @param arc The arc to add to the current angle
     */
    void turn(float arc) {
      m_angle += arc;
    }

    /**
     * @brief Set the velocity direction from the angle of the body
     */
    void setVelocityFromAngle();

    /**
     * @brief Update the internal transform
     *
     * Normally, you should not call this function.
     *
     * @sa getTransform()
     */
    void updateTransform();

    /**
     * @brief Get the current transform
     *
     * @returns The current transform
     * @sa updateTransform()
     */
    const Transform& getTransform() const;


    /** @} */

    /**
     * @name Body properties
     * @{
     */

    /**
     * @brief Set the restitution of the body of the body
     *
     * @param restitution The new restitution
     */
    void setRestitution(float restitution) {
      m_restitution = restitution;
    }

    /**
     * @brief Get the restitution of the body of the body
     *
     * @returns The current restitution
     */
    float getRestitution() const {
      return m_restitution;
    }

    /**
     * @brief Set the static friction coefficient of the body
     *
     * @param friction The new friction coefficient
     */
    void setStaticFriction(float friction) {
      m_staticFriction = friction;
    }

    /**
     * @brief Get the static friction coefficient of the body
     *
     * @returns The current friction coefficient
     */
    float getStaticFriction() const {
      return m_staticFriction;
    }

    /**
     * @brief Set the dynamic friction coefficient of the body
     *
     * @param friction The new friction coefficient
     */
    void setDynamicFriction(float friction) {
      m_dynamicFriction = friction;
    }

    /**
     * @brief Get the dynamic friction coefficient of the body
     *
     * @returns The current friction coefficient
     */
    float getDynamicFriction() const {
      return m_dynamicFriction;
    }

    /**
     * @brief Set the linear damping of the body
     *
     * @param damping The new damping
     */
    void setLinearDamping(float damping) {
      m_linearDamping = damping;
    }

    /**
     * @brief Get the linear damping of the body
     *
     * @returns The current damping
     */
    float getLinearDamping() const {
      return m_linearDamping;
    }

    /**
     * @brief Set the density of the body
     *
     * This changes the mass of the body, according to its geometry.
     *
     * @param density The new density
     */
    void setDensity(float density);

    /**
     * @brief Get the inverse mass of the body
     *
     * The inverse mass is zero for static objects.
     *
     * @returns The current inverse mass
     */
    float getInverseMass() const {
      return m_inverseMass;
    }

    /** @} */

    /**
     * @brief Check if the body collides with another body
     *
     * @param other The other body
     * @param p Data to fill if there is a collision
     */
    bool collidesWith(const PhysicsBody& other, Penetration& p) const;

  private:
    Type m_type;

    Vector2f m_position;
    Vector2f m_linearVelocity;
    Vector2f m_acceleration;

    float m_angle;

    float m_inverseMass;

    float m_restitution;
    float m_staticFriction;
    float m_dynamicFriction;
    float m_linearDamping;

    Transform m_transform;

    const PhysicsGeometry& m_geometry;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PHYSICS_BODY_H
