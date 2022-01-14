/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
#include <gfpriv/BasicGeometry.h>

#include <cassert>

#include <gf/VectorOps.h>

namespace gf {
namespace priv {

  // http://geomalgorithms.com/a03-_inclusion.html
  int computeWindingNumber(Vector2f point, Span<const Vector2f> points) {
    int wn = 0;
    std::size_t size = points.getSize();

    for (std::size_t i = 0; i < size; ++i) {
      Vector2f curr = points[i];
      Vector2f next = points[(i + 1) % size];

      if (curr.y <= point.y) {
        if (next.y > point.y) {
          if (gf::cross(next - curr, point - curr) > 0) {
            ++wn;
          }
        }
      } else {
        if (next.y < point.y) {
          if (gf::cross(next - curr, point - curr) < 0) {
            --wn;
          }
        }
      }
    }

    return wn;
  }

  // https://en.wikipedia.org/wiki/Shoelace_formula
  float computeSignedArea(Span<const Vector2f> points) {
    float area = 0.0f;
    std::size_t size = points.getSize();
    assert(size > 2);

    for (std::size_t i = 0; i < size - 1; ++i) {
      area += gf::cross(points[i], points[i + 1]);
    }

    area += gf::cross(points[size - 1], points[0]);
    return area;
  }

}
}
