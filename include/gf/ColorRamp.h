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
#ifndef GF_COLOR_RAMP_H
#define GF_COLOR_RAMP_H

#include <map>

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A color ramp
   *
   *
   * @sa gf::Color4f
   */
  struct GF_API ColorRamp {
  public:
    /**
     * @brief Default constructor
     *
     * The color ramp is empty.
     */
    ColorRamp();

    /**
     * @brief Check if the color ramp is empty
     *
     * @return True if the ramp is empty
     */
    bool isEmpty() const;

    /**
     * @brief Add a color stop
     *
     * @param offset The offset of the color
     * @param color The color
     */
    void addColorStop(float offset, const Color4f& color);

    /**
     * @brief Compute a color from an offset
     *
     * @param offset The offset of the wanted color
     * @return A color
     */
    Color4f computeColor(float offset) const;

  private:
    float m_min;
    float m_max;
    std::map<float, Color4f> m_map;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_COLOR_RAMP_H
