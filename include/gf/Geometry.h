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
#ifndef GF_GEOMETRY_H
#define GF_GEOMETRY_H

#include <vector>

#include "Portability.h"
#include "Random.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @brief Generate a line between two positions
   *
   * This function uses [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm).
   *
   * @param p0 The first point of the line
   * @param p1 The last point of the line
   * @return A series of point beginning at the first point and ending just before the last point
   */
  GF_API std::vector<Vector2i> generateLine(Vector2i p0, Vector2i p1);

  /**
   * @brief 1D midpoint displacement
   *
   * @param p0 The first end point
   * @param p1 The second end point
   * @param random A random engine
   * @param iterations The number of iterations
   * @param direction The direction to make a displacement
   * @param initialFactor The initial factor to apply to the displacement
   * @param reductionFactor The factor to apply at each iteration
   */
  GF_API std::vector<Vector2f> midpointDisplacement1D(Vector2f p0, Vector2f p1, Random& random, unsigned iterations, Vector2f direction, float initialFactor = 1.0f, float reductionFactor = 0.5f);

  /**
   * @brief 1D midpoint displacement
   *
   * The direction is perpendicular to the segment @f$ [P_0 P_1] @f$
   *
   * @param p0 The first end point
   * @param p1 The second end point
   * @param random A random engine
   * @param iterations The number of iterations
   * @param initialFactor The initial factor to apply to the displacement
   * @param reductionFactor The factor to apply at each iteration
   */
  GF_API std::vector<Vector2f> midpointDisplacement1D(Vector2f p0, Vector2f p1, Random& random, unsigned iterations, float initialFactor = 1.0f, float reductionFactor = 0.5f);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GEOMETRY_H
