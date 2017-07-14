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
#ifndef GF_MODEL_H
#define GF_MODEL_H

#include "Portability.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief A game object that can be updated
   *
   * gf::Model represents a game object that is updated but not rendered.
   * It is simpler than a gf::Entity because it has no priority and no
   * liveness property.
   *
   * It can typically be used to encapsulate a physics engine.
   *
   * Models can be grouped in a gf::ModelContainer.
   *
   * @sa gf::ModelContainer, gf::Entity
   */
  class GF_API Model {
  public:
    /**
     * @brief Destructor
     */
    virtual ~Model();

    /**
     * @brief Update the model's state
     *
     * Models are updated each frame. The time between two frames is
     * given as a parameter to help in the update. This time is in
     * seconds.
     *
     * @param time The time since the last update
     * @sa gf::ModelContainer::update()
     */
    virtual void update(Time time);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MODEL_H
