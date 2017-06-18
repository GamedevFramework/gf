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
#include <gf/EntityContainer.h>

#include <algorithm>

#include <gf/Entity.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void EntityContainer::update(Time time) {
    // erase-remove idiom
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const Entity *e) {
      return !e->isAlive();
    }), m_entities.end());

    std::sort(m_entities.begin(), m_entities.end(), [](const Entity * e1, const Entity * e2) {
      return e1->getPriority() < e2->getPriority();
    });

    for (auto entity : m_entities) {
      entity->update(time);
    }
  }

  void EntityContainer::render(RenderTarget& target, const RenderStates& states) {
    for (auto entity : m_entities) {
      entity->render(target, states);
    }
  }

  void EntityContainer::addEntity(Entity& entity) {
    m_entities.push_back(&entity);
  }

  Entity *EntityContainer::removeEntity(Entity *entity) {
    // erase-remove idiom
    auto it = std::remove(m_entities.begin(), m_entities.end(), entity);

    if (it != m_entities.end()) {
      m_entities.erase(it, m_entities.end());
      return entity;
    }

    return nullptr;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
