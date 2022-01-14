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
#include <gf/Polygon.h>

#include <cassert>

#include <algorithm>
#include <numeric>

#include <gf/Geometry.h>
#include <gf/SerializationOps.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gfpriv/BasicGeometry.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Vector2f Polygon::getSupport(Vector2f direction, const Transform& transform) const {
    assert(!isEmpty());
    // get direction in the local coordinates
    direction = gf::inverseTransform(transform.rotation, direction);
    // compute support
    gf::Vector2f point = *std::max_element(begin(), end(), [direction](const Vector2f& lhs, const Vector2f& rhs){
      return gf::dot(direction, lhs) < gf::dot(direction, rhs);
    });
    // return support in the world coordinates
    return gf::transform(transform, point);
  }

  Vector2f Polygon::getSupport(Vector2f direction) const {
    return getSupport(direction, Transform());
  }

  bool Polygon::isConvex() const {
    auto size = getPointCount();

    if (size <= 3) {
      return true;
    }

    int currentSign = 0;

    for (std::size_t i = 0; i < size; ++i) {
      float x = gf::cross(getPoint((i + 1) % size) - getPoint(i), getPoint((i + 2) % size) - getPoint((i + 1) % size));

      if (std::abs(x) > Epsilon) {
        int sign = gf::sign(x);

        if (currentSign != 0 && sign != currentSign) {
          return false;
        }

        currentSign = sign;
      }
    }

    return true;
  }

  Winding Polygon::getWinding() const {
    float area = gf::priv::computeSignedArea(getRawPoints());

    if (area > 0) {
      return Winding::Clockwise;
    }

    if (area < 0) {
      return Winding::Counterclockwise;
    }

    return Winding::None;
  }

  bool Polygon::contains(Vector2f point) const {
    return gf::priv::computeWindingNumber(point, getRawPoints()) != 0;
  }

  float Polygon::getArea() const {
    return std::abs(gf::priv::computeSignedArea(getRawPoints()) / 2);
  }

  Vector2f Polygon::getPrevPoint(std::size_t i) const {
    if (i > 0) {
      return getPoint(i - 1);
    }

    return getLastPoint();
  }

  Vector2f Polygon::getNextPoint(std::size_t i) const {
    if (i < getPointCount() - 1) {
      return getPoint(i + 1);
    }

    return getFirstPoint();
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
