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

#include <cassert>

#include <queue>

namespace gf {
inline namespace v1 {

  static constexpr float Skin = 0.15f;

  bool collides(const CircF& lhs, const CircF& rhs, Penetration& p) {
    Vector2f n = rhs.getCenter() - lhs.getCenter();
    float r = lhs.radius + rhs.radius;
    float d2 = gf::squareLength(n);

    if (d2 > gf::square(r)) {
      return false;
    }

    float d = std::sqrt(d2);

    if (d > 0) {
      p.depth = r - d;
      p.normal = n / d;
    } else {
      p.depth = r / 2;
      p.normal = { 1.0f, 0.0f };
    }

    return p.depth > Skin;
  }

  bool collides(const RectF& lhs, const CircF& rhs, Penetration& p) {
    Vector2f n = rhs.getCenter() - lhs.getCenter();

    Vector2f closest = n;
    Vector2f extent = lhs.size / 2;

    closest = gf::clamp(closest, -extent, extent);

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

    if (d2 > gf::square(r) && !inside) {
      return false;
    }

    float d = std::sqrt(d2);

    if (inside) {
      p.normal = -normal / d;
      p.depth = r + d;
    } else {
      p.normal = normal / d;
      p.depth = r - d;
    }

    return p.depth > Skin;
  }

  bool collides(const CircF& lhs, const RectF& rhs, Penetration& p) {
    bool collide = collides(rhs, lhs, p);
    p.normal = -p.normal;
    return collide;
  }

  bool collides(const RectF& lhs, const RectF& rhs, Penetration& p) {
    Vector2f n = rhs.getCenter() - lhs.getCenter();
    Vector2f overlap = lhs.size / 2 + rhs.size / 2 - gf::abs(n);

    if (overlap.x <= 0) {
      return false;
    }

    if (overlap.y <= 0) {
      return false;
    }

    if (overlap.x < overlap.y) {
      if (n.x < 0) {
        p.normal = { -1.0f, 0.0f };
      } else {
        p.normal = {  1.0f, 0.0f };
      }

      p.depth = overlap.x;
    } else {
      if (n.y < 0) {
        p.normal = { 0.0f, -1.0f };
      } else {
        p.normal = { 0.0f,  1.0f };
      }

      p.depth = overlap.y;
    }

    return p.depth > Skin;
  }

  /*
   * Polygon-polygon collision
   * Implementation of the GJK and EPA algorithms
   */

  static constexpr float Epsilon = std::numeric_limits<float>::epsilon();

  // types (in an anonymous namespace)

  namespace {
    class Simplex {
    public:
      Simplex()
      : m_size(0)
      {

      }

      unsigned getSize() const {
        return m_size;
      }

      void add(Vector2f a) {
        assert(m_size < 3);
        m_v[m_size] = a;
        m_size++;
      }

      void remove(unsigned index) {
        assert(index < 3);

        switch (index) {
          case 0:
            m_v[0] = m_v[1];
          case 1:
            m_v[1] = m_v[2];
          default:
            break;
        }

        m_size--;
      }

      Vector2f getLast() const {
        assert(m_size > 0);
        return m_v[m_size - 1];
      }

      Vector2f operator[](unsigned index) const {
        assert(index < 3);
        return m_v[index];
      }

    private:
      unsigned m_size;
      Vector2f m_v[3];
    };

    struct Edge {
      Vector2f p1;
      Vector2f p2;
      Vector2f normal;
      float distance;
    };

    struct EdgeCompare { // for priority_queue
      bool operator()(const Edge& lhs, const Edge& rhs) {
        return lhs.distance > rhs.distance;
      }
    };

    enum class Winding {
      Clockwise,
      Counterclockwise,
    };

  }

  static Edge getEdge(Vector2f p1, Vector2f p2, Winding winding) {
    Edge ret;
    ret.normal = p2 - p1;

    if (winding == Winding::Clockwise) {
      ret.normal = -gf::perp(ret.normal);
    } else {
      ret.normal = gf::perp(ret.normal);
    }

    ret.normal = gf::normalize(ret.normal);

    ret.distance = std::abs(gf::dot(p1, ret.normal));
    ret.p1 = p1;
    ret.p2 = p2;

    return ret;
  }

  namespace {

    class ExpandingSimplex {
    public:
      ExpandingSimplex(const Simplex& simplex) {
        m_winding = getWinding(simplex);

        unsigned size = simplex.getSize();

        for (unsigned i = 0; i < size; ++i) {
          unsigned j = (i + 1) % size;
          m_queue.push(getEdge(simplex[i], simplex[j], m_winding));
        }
      }

      Edge getClosestEdge() const {
        return m_queue.top();
      }

      void expand(Vector2f point) {
        Edge edge = m_queue.top();
        m_queue.pop();

        m_queue.push(getEdge(edge.p1, point, m_winding));
        m_queue.push(getEdge(point, edge.p2, m_winding));
      }

    private:
      static Winding getWinding(const Simplex& simplex) {
        unsigned size = simplex.getSize();

        for (unsigned i = 0; i < size; ++i) {
          unsigned j = (i + 1) % size;
          float x = gf::cross(simplex[i], simplex[j]);

          if (x > 0) {
            return Winding::Clockwise;
          } else if (x < 0) {
            return Winding::Counterclockwise;
          }
        }

        assert(false);
        return Winding::Clockwise;
      }

    private:
      std::priority_queue<Edge, std::vector<Edge>, EdgeCompare> m_queue;
      Winding m_winding;
    };

  }

  static Vector2f getSupport(const Polygon& lhs, const Polygon& rhs, Vector2f direction) {
    return lhs.getSupport(direction) - rhs.getSupport(-direction);
  }

  static bool checkSimplex(Simplex& simplex, Vector2f& direction) {
    Vector2f a = simplex.getLast();
    Vector2f ao = -a;

    if (simplex.getSize() == 3) {
      Vector2f b = simplex[1];
      Vector2f c = simplex[0];

      Vector2f ab = b - a;
      Vector2f ac = c - a;

      Vector2f abPerp = inverseVectorTripleProduct(ac, ab, ab);
      Vector2f acPerp = inverseVectorTripleProduct(ab, ac, ac);

      if (gf::dot(acPerp, ao) >= 0) {
        simplex.remove(1);
        direction = acPerp;
      } else {
        if (gf::dot(abPerp, ao) < 0) {
          return true;
        }

        simplex.remove(0);
        direction = abPerp;
      }
    } else {
      Vector2f b = simplex[0];
      Vector2f ab = b - a;

      direction = inverseVectorTripleProduct(ab, ao, ab);

      if (gf::squareLength(direction) < Epsilon) {
        direction = gf::perp(ab);
      }
    }

    return false;
  }


  static constexpr unsigned MaxIterations = 100;

  bool collides(const Polygon& lhs, const Polygon& rhs, Penetration& p) {
    /*
     * Gilbert-Johnson-Keerthi (GJK) algorithm
     * adapted from http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/
     */

    Vector2f direction = rhs.getCenter() - lhs.getCenter();

    Simplex simplex;
    simplex.add(getSupport(lhs, rhs, direction));

    if (gf::dot(simplex.getLast(), direction) <= 0.0f) {
      return false;
    }

    direction = -direction;

    for (;;) {
      simplex.add(getSupport(lhs, rhs, direction));

      if (gf::dot(simplex.getLast(), direction) <= 0.0f) {
        return false;
      }

      if (checkSimplex(simplex, direction)) {
        // there is a collision, we do not return yet
        break;
      }
    }

    /*
     * Expanding Polytope Algorithm (EPA)
     * adapted from http://www.dyn4j.org/2010/05/epa-expanding-polytope-algorithm/
     */

    ExpandingSimplex expandingSimplex(simplex);
    Edge edge;
    Vector2f point;

    for (unsigned i = 0; i < MaxIterations; ++i) {
      edge = expandingSimplex.getClosestEdge();
      point = getSupport(lhs, rhs, edge.normal);
      float distance = gf::dot(point, edge.normal);

      if (distance - edge.distance < Skin) {
        p.normal = edge.normal;
        p.depth = distance;
        return true;
      }

      expandingSimplex.expand(point);
    }

    p.normal = edge.normal;
    p.depth = gf::dot(point, edge.normal);
    return true;
  }


}
}
