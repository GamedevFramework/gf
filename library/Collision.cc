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
#include <gf/Collision.h>

#include <cassert>

#include <queue>

#include <gf/Log.h>
#include <gf/Transform.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static constexpr float Skin = 0.15f;

  bool collides(const CircF& lhs, const Transform& lhsTrans, const CircF& rhs, const Transform& rhsTrans, Penetration& p) {
    Vector2f lhsCenter = gf::transform(lhsTrans, lhs.getCenter());
    Vector2f rhsCenter = gf::transform(rhsTrans, rhs.getCenter());
    Vector2f normal = rhsCenter - lhsCenter;
    float radiusSum = lhs.radius + rhs.radius;
    float squareDistance = gf::squareLength(normal);

    if (squareDistance > gf::square(radiusSum)) {
      return false;
    }

    float distance = std::sqrt(squareDistance);

    if (distance > 0) {
      p.depth = radiusSum - distance;
      p.normal = normal / distance;
    } else {
      p.depth = radiusSum / 2;
      p.normal = { 1.0f, 0.0f };
    }

    return p.depth > Skin;
  }

  bool collides(const CircF& lhs, const CircF& rhs, Penetration& p) {
    return collides(lhs, Transform(), rhs, Transform(), p);
  }

  bool collides(const RectF& lhs, const CircF& rhs, Penetration& p) {
    Vector2f normal = rhs.getCenter() - lhs.getCenter();

    Vector2f closest = normal;
    Vector2f extent = lhs.size / 2;

    closest = gf::clamp(closest, -extent, extent);

    bool inside = false;

    if (normal == closest) {
      inside = true;

      if (std::abs(normal.x) > std::abs(normal.y)) {
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

    normal = normal - closest;

    float squareDistance = gf::squareLength(normal);
    float radius = rhs.radius;

    if (squareDistance > gf::square(radius) && !inside) {
      return false;
    }

    float distance = std::sqrt(squareDistance);

    if (inside) {
      p.normal = -normal / distance;
      p.depth = radius + distance;
    } else {
      p.normal = normal / distance;
      p.depth = radius - distance;
    }

    return p.depth > Skin;
  }

  bool collides(const CircF& lhs, const RectF& rhs, Penetration& p) {
    bool ret = collides(rhs, lhs, p);
    p.normal = -p.normal;
    return ret;
  }

  bool collides(const RectF& lhs, const RectF& rhs, Penetration& p) {
    Vector2f normal = rhs.getCenter() - lhs.getCenter();
    Vector2f overlap = lhs.size / 2 + rhs.size / 2 - gf::abs(normal);

    if (overlap.x <= 0) {
      return false;
    }

    if (overlap.y <= 0) {
      return false;
    }

    if (overlap.x < overlap.y) {
      if (normal.x < 0) {
        p.normal = { -1.0f, 0.0f };
      } else {
        p.normal = {  1.0f, 0.0f };
      }

      p.depth = overlap.x;
    } else {
      if (normal.y < 0) {
        p.normal = { 0.0f, -1.0f };
      } else {
        p.normal = { 0.0f,  1.0f };
      }

      p.depth = overlap.y;
    }

    return p.depth > Skin;
  }

  namespace {

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

  }

  static Vector2f getNormal(Vector2f p1, Vector2f p2, Winding winding) {
    Vector2f ret = p2 - p1;

    if (winding == Winding::Clockwise) {
      ret = -gf::perp(ret);
    } else {
      ret = gf::perp(ret);
    }

    ret = gf::normalize(ret);
    return ret;
  }

  bool collides(const CircF& lhs, const Transform& lhsTrans, const Polygon& rhs, const Transform& rhsTrans, Penetration& p) {
    std::size_t sz = rhs.getPointCount();
    Winding winding = rhs.getWinding();
    Vector2f center = gf::inverseTransform(rhsTrans, gf::transform(lhsTrans, lhs.getCenter()));

    std::vector<Edge> edges;

    for (std::size_t i = 0; i < sz; ++i) {
      std::size_t j = (i + 1) % sz;
      Edge edge;
      edge.p1 = rhs.getPoint(i);
      edge.p2 = rhs.getPoint(j);
      edge.normal = getNormal(edge.p1, edge.p2, winding);
      edge.distance = gf::dot(edge.normal, center - edge.p1);
      edges.push_back(edge);
    }

    Edge best = *std::max_element(edges.begin(), edges.end(), [](const Edge& lhs, const Edge& rhs) {
      return lhs.distance < rhs.distance;
    });

    if (best.distance > lhs.getRadius()) {
      return false;
    }

    if (best.distance < Epsilon) {
      p.normal = gf::transform(rhsTrans.rotation, -best.normal);
      p.depth = lhs.getRadius();
      return true;
    }

    if (gf::dot(center - best.p1, best.p2 - best.p1) <= 0) {
      if (gf::squareDistance(center, best.p1) > gf::square(lhs.getRadius())) {
        return false;
      }

      p.normal = gf::transform(rhsTrans.rotation, gf::normalize(best.p1 - center));
      p.depth = lhs.getRadius() - gf::euclideanDistance(center, best.p1);
      return true;
    }

    if (gf::dot(center - best.p2, best.p1 - best.p2) <= 0) {
      if (gf::squareDistance(center, best.p2) > gf::square(lhs.getRadius())) {
        return false;
      }

      p.normal = gf::transform(rhsTrans.rotation, gf::normalize(best.p2 - center));
      p.depth = lhs.getRadius() - gf::euclideanDistance(center, best.p2);
      return true;
    }

    p.normal = gf::transform(rhsTrans.rotation, -best.normal);
    p.depth = lhs.getRadius() - best.distance;
    return true;
  }

  bool collides(const CircF& lhs, const Polygon& rhs, Penetration& p) {
    return collides(lhs, Transform(), rhs, Transform(), p);
  }

  bool collides(const Polygon& lhs, const Transform& lhsTrans, const CircF& rhs, const Transform& rhsTrans, Penetration& p) {
    bool ret = collides(rhs, rhsTrans, lhs, lhsTrans, p);
    p.normal = -p.normal;
    return ret;
  }

  bool collides(const Polygon& lhs, const CircF& rhs, Penetration& p) {
    return collides(lhs, Transform(), rhs, Transform(), p);
  }

  /*
   * Polygon-polygon collision
   * Implementation of the GJK and EPA algorithms
   */

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
            // fallthrough
          case 1:
            m_v[1] = m_v[2];
            // fallthrough
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
  }

  static Edge getEdge(Vector2f p1, Vector2f p2, Winding winding) {
    Edge ret;
    ret.p1 = p1;
    ret.p2 = p2;
    ret.normal = getNormal(p1, p2, winding);
    ret.distance = std::abs(gf::dot(p1, ret.normal));
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
          }

          if (x < 0) {
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

  static Vector2f getSupport(const Polygon& lhs, const Transform& lhsTrans, const Polygon& rhs, const Transform& rhsTrans, Vector2f direction) {
    return lhs.getSupport(direction, lhsTrans) - rhs.getSupport(-direction, rhsTrans);
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

  bool collides(const Polygon& lhs, const Transform& lhsTrans, const Polygon& rhs, const Transform& rhsTrans, Penetration& p) {
    /*
     * Gilbert-Johnson-Keerthi (GJK) algorithm
     * adapted from http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/
     */

    // Vector2f direction = gf::transform(rhsTrans, rhs.getCenter()) - gf::transform(lhsTrans, lhs.getCenter());
    Vector2f direction(1,0);

    Simplex simplex;
    simplex.add(getSupport(lhs, lhsTrans, rhs, rhsTrans, direction));

    if (gf::dot(simplex.getLast(), direction) <= 0.0f) {
      return false;
    }

    direction = -direction;

    for (;;) {
      simplex.add(getSupport(lhs, lhsTrans, rhs, rhsTrans, direction));

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
      point = getSupport(lhs, lhsTrans, rhs, rhsTrans, edge.normal);
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

  bool collides(const Polygon& lhs, const Polygon& rhs, Penetration& p) {
    return collides(lhs, Transform(), rhs, Transform(), p);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
