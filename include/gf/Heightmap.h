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
#ifndef GF_HEIGHTMAP_H
#define GF_HEIGHTMAP_H

#include "Array2D.h"
#include "Noise.h"
#include "Portability.h"
#include "Rect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @brief A heightmap
   *
   * @sa gf::midpointDisplacement2D(), gf::diamondSquare2D()
   */
  class GF_API Heightmap {
  public:
    /**
     * @brief Constructor
     *
     * @param size The size of the heightmap
     */
    Heightmap(Vector2i size);

    /**
     * @brief Get the size of the heightmap
     *
     * @returns The current size of the heightmap
     */
    Vector2i getSize() const {
      return m_data.getSize();
    }

    /**
     * @brief Reset the heightmap
     *
     * The values are all set to @f$ 0.0 @f$
     */
    void reset();

    /**
     * @brief Get the value at the specified position
     *
     * @param position A position
     * @returns The value at the given position
     */
    double getValue(Vector2i position) const {
      return m_data(position);
    }

    /**
     * @brief Set the value at the specified position
     *
     * @param position A position
     * @param value The new value
     */
    void setValue(Vector2i position, double value) {
      m_data(position) = value;
    }

    /**
     * @brief Get the minimum and maximum of the heightmap
     *
     * @returns A pair with the first element being the minimum and the second element the maximum
     */
    std::pair<double, double> getMinMax() const;

    /**
     * @brief Normalize the heightmap
     *
     * @param min The new minimum value (defaults to @f$ 0.0f @f$)
     * @param max The new maximum value (defaults to @f$ 1.0f @f$)
     */
    void normalize(double min = 0.0, double max = 1.0);

    /**
     * @brief Add a noise to the heightmap
     *
     * @param noise The noise to use
     * @param scale The scale of the noise
     */
    void addNoise(Noise2D& noise, double scale = 1.0);

    /**
     * @brief Get a sub-map of the heightmap
     *
     * @param area The area of the sub-map in the heightmap
     */
    Heightmap subMap(RectI area) const;

  private:
    Array2D<double, int> m_data;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_HEIGHTMAP_H
