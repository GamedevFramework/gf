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
#ifndef GF_MODEL_CONTAINER_H
#define GF_MODEL_CONTAINER_H

#include <vector>

#include "Portability.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Model;

  /**
   * @ingroup game
   * @brief A collection of models
   *
   * gf::ModelContainer represents a collection of models that are updated
   * automatically.
   *
   * The model manager is *not* responsible for the memory of the models. The
   * models must be allocated by the user and not deleted while they are
   * handled by the model manager.
   *
   * Generally, you only need one model manager in your game. You create it
   * at the beginning of the game and put all your models in it. Then you
   * can call gf::ModelContainer::update() in your game loop.
   *
   * @sa gf::Model
   */
  class GF_API ModelContainer {
  public:

    /**
     * @brief Update the models
     *
     * @param time The time since the last update
     * @sa gf::Model::update()
     */
    void update(Time time);

    /**
     * @brief Add a model to the collection
     *
     * The model must not be deleted while it is handled by the model manager.
     *
     * @param model A model
     * @sa removeModel()
     */
    void addModel(Model& model);

    /**
     * @brief Remove a model from the collection
     *
     * @param model The model to remove
     * @return The removed model or `nullptr` if the model was not present
     * @sa addModel()
     */
    Model *removeModel(Model *model);

  private:
    std::vector<Model *> m_models;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MODEL_CONTAINER_H
