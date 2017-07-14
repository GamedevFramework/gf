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
#ifndef GF_ENTITY_H
#define GF_ENTITY_H

#include "Portability.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;

  /**
   * @ingroup game
   * @brief A game entity
   *
   * gf::Entity represents a game entity, i.e. an object that is updated
   * and rendered every frame.
   *
   * Entities are ordered by priority when they are renderer, with lower
   * priority being rendered first and higher priority being renderered
   * last. The default priority is @f$ 0 @f$.
   *
   * Entities can become dead, in which case they are neither updated nor
   * rendered anymore.
   *
   * Entities can be grouped in an gf::EntityContainer.
   *
   * @sa gf::EntityContainer
   */
  class GF_API Entity {
  public:
    /**
     * @brief Contructor
     *
     * By default, the entity is alive.
     *
     * @param priority the priority of the entity (default: 0)
     */
    Entity(int priority = 0)
    : m_priority(priority)
    , m_liveness(Liveness::ALIVE)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~Entity();

    /**
     * @brief Get the priority of the entity
     *
     * @return The priority
     */
    int getPriority() const {
      return m_priority;
    }

    /**
     * @name Entity liveness
     * @{
     */

    /**
     * @brief Check if the entity is alive
     *
     * @return True if the entity is alive
     * @sa setAlive(), kill();
     */
    bool isAlive() const {
      return m_liveness == Liveness::ALIVE;
    }

    /**
     * @brief Set the entity alive (again)
     *
     * This function can be called to revive an entity that has been killed.
     */
    void setAlive() {
      m_liveness = Liveness::ALIVE;
    }

    /**
     * @brief Kill an entity
     *
     * This function kills the entity. After being killed, the entity must
     * not be updated or rendered.
     */
    void kill() {
      m_liveness = Liveness::DEAD;
    }

    /** @} */

    /**
     * @brief Update the entity's state
     *
     * Entities are updated each frame. The time between two frames is
     * given as a parameter to help in the update. This time is in
     * seconds.
     *
     * @param time The time since the last update
     * @sa gf::EntityContainer::update()
     */
    virtual void update(Time time);

    /**
     * @brief Render the entity
     *
     * @param target The render target
     * @param states The render states to use for drawing
     * @sa gf::EntityContainer::render()
     */
    virtual void render(RenderTarget& target, const RenderStates& states);

  private:
    enum class Liveness : int {
      ALIVE,
      DEAD,
    };

    int m_priority;
    Liveness m_liveness;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ENTITY_H
