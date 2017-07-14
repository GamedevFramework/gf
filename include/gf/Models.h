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
#ifndef GF_MODELS_H
#define GF_MODELS_H

#include "Model.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Fixed timestep model
   *
   * This model takes another model that needs a fixed timestep and provides
   * it event if the original timestep is not fixed. This may be useful for
   * physics model, for example.
   */
  class GF_API FixedTimestepModel : public Model {
  public:
    /**
     * @brief Constructor
     *
     * @param model The original model
     * @param timestep The fixed timestep
     */
    FixedTimestepModel(Model& model, Time timestep = gf::seconds(1 / 60.0f));

    virtual void update(Time time) override;

  private:
    Model& m_model;
    Time m_timestep;
    Time m_elapsed;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_MODELS_H
