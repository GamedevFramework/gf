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
#ifndef GF_NOISE_H
#define GF_NOISE_H

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief 2D A noise function
   */
  class GF_API Noise2D {
  public:
    /**
     * @brief Virtual destructor
     */
    virtual ~Noise2D();

    /**
     * @brief Take a 2D noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     */
    virtual double getValue(double x, double y) = 0;

    /**
     * @brief Take a 2D noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     *
     * @sa getValue()
     */
    double operator()(double x, double y) {
      return getValue(x, y);
    }

  };


  /**
   * @ingroup core
   * @brief 3D A noise function
   */
  class GF_API Noise3D {
  public:
    /**
     * @brief Virtual destructor
     */
    virtual ~Noise3D();

    /**
     * @brief Take a 3D noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @param z The z coordinate of the noise value
     * @return The noise value
     */
    virtual double getValue(double x, double y, double z) = 0;

    /**
     * @brief Take a 3D noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @param z The z coordinate of the noise value
     * @return The noise value
     */
    double operator()(double x, double y, double z) {
      return getValue(x, y, z);
    }
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_NOISE_H
