/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#ifndef GF_LIGHT_POINT_EMISSION_H
#define GF_LIGHT_POINT_EMISSION_H

#include "GraphicsApi.h"
#include "Sprite.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_light
   * @brief Light point emission
   *
   * @sa gf::LightSystem
   */
  class GF_GRAPHICS_API LightPointEmission : public Sprite {
  public:
    LightPointEmission();

    LightPointEmission(const Texture& texture);

    void setLocaleCastCenter(Vector2f center) {
      m_center = center;
    }

    Vector2f getLocalCastCenter() const {
      return m_center;
    }

    void setSourceRadius(float radius) {
      m_radius = radius;
    }

    float getSourceRadius() const {
      return m_radius;
    }

    void setShadowOverExtendMultiplier(float multiplier) {
      m_multiplier = multiplier;
    }

    float getShadowOverExtendMultiplier() const {
      return m_multiplier;
    }

    void setActive(bool active = true) {
      m_active = active;
    }

    bool isActive() const {
      return m_active;
    }

  private:
    Vector2f m_center;
    float m_radius;
    float m_multiplier;
    bool m_active = true;
  };



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_LIGHT_POINT_EMISSION_H
