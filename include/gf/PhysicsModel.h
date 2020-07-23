/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#ifndef GF_PHYSICS_MODEL_H
#define GF_PHYSICS_MODEL_H

#include <vector>

#include "GraphicsApi.h"
#include "Model.h"
#include "Ref.h"
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
  class GF_GRAPHICS_API PhysicsModel : public gf::Model {
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
    std::vector<Ref<PhysicsBody>> m_staticBodies;
    std::vector<Ref<PhysicsBody>> m_dynamicBodies;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PHYSICS_MODEL_H
