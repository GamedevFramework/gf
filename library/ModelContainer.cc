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
#include <gf/ModelContainer.h>

#include <algorithm>

#include <gf/Model.h>


namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void ModelContainer::update(Time time) {
    for (auto model : m_models) {
      model->update(time);
    }
  }

  void ModelContainer::addModel(Model& model) {
    m_models.push_back(&model);
  }

  Model *ModelContainer::removeModel(Model *model) {
    // erase-remove idiom
    auto it = std::remove(m_models.begin(), m_models.end(), model);

    if (it != m_models.end()) {
      m_models.erase(it, m_models.end());
      return model;
    }

    return nullptr;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
