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
#ifndef GF_ENTITY_CONTAINER_H
#define GF_ENTITY_CONTAINER_H

#include <type_traits>
#include <vector>

#include "Portability.h"
#include "RenderStates.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Entity;
  class RenderTarget;

  /**
   * @ingroup game
   * @brief A collection of entities
   *
   * gf::EntityContainer represents a collection of entities that are
   * updated and rendered automatically. The entity manager takes care of
   * the liveness of the entities and remove the dead entities from the
   * collection.
   *
   * The entity manager is *not* responsible for the memory of the entities.
   * The entities must be allocated by the user and not deleted while they
   * are handled by the entity manager.
   *
   * Generally, you only need one entity manager in your game. You create it
   * at the beginning of the game and put all your entities in it. Then you
   * can call gf::EntityContainer::update() and gf::EntityContainer::render() in your
   * game loop.
   *
   * @sa gf::Entity
   */
  class GF_API EntityContainer {
  public:

    /**
     * @brief Update the entities
     *
     * This function first eliminates the dead entities, then sort them
     * by priority. Finally, each entity is updated.
     *
     * @param time The time since the last update
     * @sa gf::Entity::update()
     */
    void update(Time time);

    /**
     * @brief Render the entities on the target
     *
     * The entities are rendered by priority: lower priority first and
     * higher priority last.
     *
     * @param target The render target
     * @param states The render states to use for drawing
     * @sa gf::Entity::render()
     */
    void render(RenderTarget& target, const RenderStates& states = RenderStates());

    /**
     * @name Entities management
     * @{
     */

    /**
     * @brief Add an entity to the collection
     *
     * The entity must not be deleted while it is handled by the entity
     * manager.
     *
     * @param entity An entity
     * @sa removeEntity()
     */
    void addEntity(Entity& entity);

    /**
     * @brief Remove an entity from the collection
     *
     * @param entity The entity to remove
     * @return The removed entity or `nullptr` if the entity was not present
     * @sa addEntity()
     */
    Entity *removeEntity(Entity *entity);

    /**
     * @brief Remove a typed entity from the collection
     *
     * This function is a shortcut to avoid typecasting.
     *
     * Example:
     *
     * ~~~{.cc}
     *
     * class Foo : public Entity {
     *   ...
     * };
     *
     * Foo foo;
     * manager.addEntity(foo);
     *
     * ...
     *
     * Foo *removed = manager.removeTypedEntity<Foo>(&foo);
     * removed->doSomething();
     * ~~~
     *
     * @sa removeEntity()
     */
    template<typename E>
    E *removeTypedEntity(E *entity) {
      static_assert(std::is_base_of<Entity, E>::value, "E must be an Entity");
      return static_cast<E*>(removeEntity(entity));
    }

    /** @} */

  private:
    std::vector<Entity *> m_entities;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ENTITY_CONTAINER_H
