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
#include <gf/Polygon.h>

#include <cassert>

#include <algorithm>
#include <numeric>

#include <gf/Geometry.h>
#include <gf/SerializationOps.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

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

  namespace {

    // https://en.wikipedia.org/wiki/Shoelace_formula
    float getSignedArea(const std::vector<Vector2f>& points) {
      float area = 0.0f;

      for (std::size_t i = 0; i < points.size() - 1; ++i) {
        area += gf::cross(points[i], points[i + 1]);
      }

      area += gf::cross(points.back(), points.front());
      return area;
    }

  } // namespace

  Winding Polygon::getWinding() const {
    return getSignedArea(getRawPoints()) > 0 ? Winding::Clockwise : Winding::Counterclockwise;
  }

  namespace {
    enum class LineSide {
      Left,
      Right,
      None,
    };

    constexpr LineSide getSide(Vector2f a, Vector2f b) {
      float x = cross(a, b);
      if (x < 0) {
        return LineSide::Left;
      } else if (x > 0) {
        return LineSide::Right;
      }
      return LineSide::None;
    }
  }

  bool Polygon::contains(Vector2f point) const {
    LineSide previousSide = LineSide::None;

    for (std::size_t i = 0, n = getPointCount(); i < n; ++i) {
      Vector2f a = getPoint(i);
      Vector2f b = getPoint((i + 1) % n);

      Vector2f segment = b - a;
      Vector2f pointDirection = point - a;

      LineSide currentSide = getSide(segment, pointDirection);
      if (previousSide == LineSide::None) {
        previousSide = currentSide;
      }

      if (previousSide != currentSide) {
        return false;
      }
    }

    return true;
  }

  float Polygon::getArea() const {
    return std::abs(getSignedArea(getRawPoints()) / 2);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
