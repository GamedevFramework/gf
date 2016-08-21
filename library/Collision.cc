/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <gf/Collision.h>

namespace gf {
inline namespace v1 {

  static constexpr float Skin = 0.15f;

  bool collides(const CircF& lhs, const CircF& rhs, Manifold& m) {
    Vector2f n = rhs.getCenter() - lhs.getCenter();
    float r = lhs.radius + rhs.radius;
    float d2 = gf::squareLength(n);

    if (d2 > r * r) {
      return false;
    }

    float d = std::sqrt(d2);

    if (d > 0) {
      m.penetration = r - d;
      m.normal = n / d;
    } else {
      m.penetration = r / 2;
      m.normal = { 1.0f, 0.0f };
    }

    return m.penetration > Skin;
  }

  bool collides(const RectF& lhs, const CircF& rhs, Manifold& m) {
    Vector2f n = rhs.getCenter() - lhs.getCenter();

    Vector2f closest = n;
    Vector2f extent = lhs.size / 2;

    closest.x = gf::clamp(closest.x, -extent.x, extent.x);
    closest.y = gf::clamp(closest.y, -extent.y, extent.y);

    bool inside = false;

    if (n == closest) {
      inside = true;

      if (std::abs(n.x) > std::abs(n.y)) {
        if (closest.x > 0) {
          closest.x = extent.x;
        } else {
          closest.x = -extent.x;
        }
      } else {
        if (closest.y > 0) {
          closest.y = extent.y;
        } else {
          closest.y = -extent.y;
        }
      }
    }

    Vector2f normal = n - closest;

    float d2 = gf::squareLength(normal);
    float r = rhs.radius;

    if (d2 > r * r && !inside) {
      return false;
    }

    float d = std::sqrt(d2);

    if (inside) {
      m.normal = -normal / d;
      m.penetration = r + d;
    } else {
      m.normal = normal / d;
      m.penetration = r - d;
    }

    return m.penetration > Skin;
  }

  bool collides(const CircF& lhs, const RectF& rhs, Manifold& m) {
    bool collide = collides(rhs, lhs, m);
    m.normal = -m.normal;
    return collide;
  }

  bool collides(const RectF& lhs, const RectF& rhs, Manifold& m) {
    Vector2f n = rhs.getCenter() - lhs.getCenter();

    Vector2f leftExtent = lhs.size / 2;
    Vector2f rightExtent = rhs.size / 2;
    Vector2f overlap = leftExtent + rightExtent - gf::abs(n);

    if (overlap.x <= 0) {
      return false;
    }

    if (overlap.y <= 0) {
      return false;
    }

    if (overlap.x > overlap.y) {
      if (n.x < 0) {
        m.normal = { -1.0f, 0.0f };
      } else {
        m.normal = {  1.0f, 0.0f };
      }

      m.penetration = overlap.x;
    } else {
      if (n.y < 0) {
        m.normal = { 0.0f, -1.0f };
      } else {
        m.normal = { 0.0f,  1.0f };
      }

      m.penetration = overlap.y;
    }

    return m.penetration > Skin;
  }

}
}
