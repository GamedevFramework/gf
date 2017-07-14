#ifndef GF_PHYSICS_MODEL_H
#define GF_PHYSICS_MODEL_H

#include <vector>

#include "Model.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class PhysicsBody;

  /**
   * @ingroup game
   * @brief A model for physics simulation
   *
   * @sa gf::PhysicsBody, gf::FixedTimestepModel
   */
  class GF_API PhysicsModel : public gf::Model {
  public:
    /**
     * @brief Constructor
     *
     * @param gravity The gravity of the simulation (default: no gravity)
     */
    PhysicsModel(Vector2f gravity = { 0.0f, 0.0f });

    /**
     * @brief Set the gravity of the simulation
     *
     * @param gravity The new gravity
     */
    void setGravity(Vector2f gravity) noexcept {
      m_gravity = gravity;
    }

    /**
     * @brief Get the gravity of the simulation
     *
     * @returns The current gravity
     */
    Vector2f getGravity() const noexcept {
      return m_gravity;
    }

    /**
     * @brief Add a body to the simulation
     *
     * @param body A physics body
     */
    void addBody(PhysicsBody& body);

    /**
     * @brief Remove a body from the simulation
     *
     * @param body A physics body
     */
    void removeBody(PhysicsBody& body);

    /**
     * @brief Remove all bodies from the simulation
     */
    void clear();

    virtual void update(Time time) override;

  private:
    Vector2f m_gravity;
    std::vector<PhysicsBody*> m_staticBodies;
    std::vector<PhysicsBody*> m_dynamicBodies;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PHYSICS_MODEL_H
