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
#include <gf/Triangulation.h>

#include <cassert>
#include <deque>
#include <list>
#include <set>
#include <tuple>

#include <gf/Circ.h>
#include <gf/Log.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    struct ExtendedVertex {
      explicit ExtendedVertex(const Vector2f& p)
      : origin(&p)
      , position(p)
      {

      }

      const Vector2f *origin;
      Vector2f position;
    };

    bool operator<(const ExtendedVertex& lhs, const ExtendedVertex& rhs) {
      return lhs.origin < rhs.origin;
    }

    bool operator!=(const ExtendedVertex& lhs, const ExtendedVertex& rhs) {
      return lhs.origin != rhs.origin;
    }

    class ExtendedEdge : public EdgeRef<ExtendedVertex> {
    public:
      ExtendedEdge(const ExtendedVertex& p0, const ExtendedVertex& p1)
      : EdgeRef(p0, p1)
      , m_mark(false)
      {
        assert(p0 != p1);

        if (p1 < p0) {
          std::swap(m_points[0], m_points[1]);
        }
      }

      void setMarked() const {
        m_mark = true;
      }

      bool isMarked() const {
        return m_mark;
      }

    private:
      mutable bool m_mark;
    };

    bool operator<(const ExtendedEdge& lhs, const ExtendedEdge& rhs) {
      return std::make_tuple(lhs[0], lhs[1]) < std::make_tuple(rhs[0], rhs[1]);
    }

    class ExtendedTriangle : public TriangleRef<const ExtendedVertex> {
    public:
      ExtendedTriangle(const ExtendedVertex& p0, const ExtendedVertex& p1, const ExtendedVertex& p2)
      : TriangleRef(p0, p1, p2)
      , m_mark(false)
      {
        // https://en.wikipedia.org/wiki/Circumscribed_circle

        Vector2f d01 = p0.position - p1.position;
        Vector2f d12 = p1.position - p2.position;
        Vector2f d20 = p2.position - p0.position;

        float d = 2 * (p0.position.x * d12.y + p1.position.x * d20.y + p2.position.x * d01.y);
//         assert(d > gf::Epsilon);

        float l0 = gf::squareLength(p0.position);
        float l1 = gf::squareLength(p1.position);
        float l2 = gf::squareLength(p2.position);

        m_circumcircle.center.x = (l0 * d12.y + l1 * d20.y + l2 * d01.y) / d;
        m_circumcircle.center.y = (- l0 * d12.x - l1 * d20.x - l2 * d01.x) / d;
        m_circumcircle.radius = gf::squareDistance(m_circumcircle.center, p0.position); // save the square distance instead of distance
      }

      ExtendedEdge getEdge(std::size_t i) const {
        switch (i) {
          case 0:
            return ExtendedEdge(*m_points[1], *m_points[2]);
          case 1:
            return ExtendedEdge(*m_points[2], *m_points[0]);
          case 2:
            return ExtendedEdge(*m_points[0], *m_points[1]);
          default:
            break;
        }

        assert(false);
        return ExtendedEdge(*m_points[0], *m_points[0]);
      }

      bool hasInCircumcircle(const ExtendedVertex& v) const {
        return gf::squareLength(v.position - m_circumcircle.center) <= m_circumcircle.radius;
      }

      void setMarked() {
        m_mark = true;
      }

      bool isMarked() const {
        return m_mark;
      }

      bool contains(const ExtendedVertex& v) {
        return m_points[0] == &v || m_points[1] == &v || m_points[2] == &v;
      }

    private:
      CircF m_circumcircle;
      bool m_mark;
    };



  } // anonymous namespace

  std::vector<TriangleRef<const Vector2f>> triangulation(Span<const Vector2f> points) {
    std::vector<TriangleRef<const Vector2f>> out;

    if (points.isEmpty()) {
      return out;
    }

    std::vector<ExtendedVertex> vertices;

    // transform the input and find the surrounding triangles

    Vector2f cornerNorthWest = points[0];
    Vector2f cornerSouthEast = points[1];

    for (auto& p : points) {
      vertices.push_back(ExtendedVertex(p));

      cornerNorthWest = gf::min(cornerNorthWest, p);
      cornerSouthEast = gf::max(cornerSouthEast, p);
    }

    static constexpr float PaddingFactor = 10.0f;

    auto diagonal = cornerSouthEast - cornerNorthWest;

    cornerNorthWest -= PaddingFactor * diagonal;
    cornerSouthEast += PaddingFactor * diagonal;

    Vector2f cornerNorthEast(cornerSouthEast.x, cornerNorthWest.y);
    Vector2f cornerSouthWest(cornerNorthWest.x, cornerSouthEast.y);

    ExtendedVertex extendedNorthWest(cornerNorthWest);
    ExtendedVertex extendedNorthEast(cornerNorthEast);
    ExtendedVertex extendedSouthWest(cornerSouthWest);
    ExtendedVertex extendedSouthEast(cornerSouthEast);

    std::vector<ExtendedTriangle> triangles;
    triangles.push_back(ExtendedTriangle(extendedNorthWest, extendedNorthEast, extendedSouthWest));
    triangles.push_back(ExtendedTriangle(extendedSouthWest, extendedNorthEast, extendedSouthEast));

    // add all points to the triangulation

    for (const auto& v : vertices) {
      std::set<ExtendedEdge> polygon;

      auto updatePolygon = [&polygon](const ExtendedEdge& e) {
        auto res = polygon.insert(e);

        if (!res.second) {
          res.first->setMarked();
        }
      };

      for (auto& t : triangles) {
        if (t.hasInCircumcircle(v)) {
          t.setMarked();
          updatePolygon(t.getEdge(0));
          updatePolygon(t.getEdge(1));
          updatePolygon(t.getEdge(2));
        }
      }

      triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [](const ExtendedTriangle& t) {
        return t.isMarked();
      }), triangles.end());

      for (auto& e : polygon) {
        if (!e.isMarked()) {
          triangles.push_back(ExtendedTriangle(v, e[0], e[1]));
        }
      }
    }

    // create the output

    for (auto& t : triangles) {
      if (t.contains(extendedNorthWest) || t.contains(extendedNorthEast) || t.contains(extendedSouthWest) || t.contains(extendedSouthEast)) {
        continue;
      }

      out.push_back(TriangleRef<const Vector2f>(*t[0].origin, *t[1].origin, *t[2].origin));
    }

    return out;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
