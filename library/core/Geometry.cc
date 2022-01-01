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
#include <gf/Geometry.h>

#include <cassert>
#include <set>

#include <gf/Log.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Bresenham::Bresenham(Vector2i p0, Vector2i p1)
  : m_p0(p0)
  , m_p1(p1)
  , m_delta(p1 - p0)
  {
    m_step.x = gf::sign(m_delta.x);
    m_step.y = gf::sign(m_delta.y);
    m_error = std::max(m_step.x * m_delta.x, m_step.y * m_delta.y);
    m_delta *= 2;
  }

  bool Bresenham::step(Vector2i& res) {
    if (m_step.x * m_delta.x > m_step.y * m_delta.y) {
      if (m_p0.x == m_p1.x) {
        return true;
      }

      m_p0.x += m_step.x;
      m_error -= m_step.y * m_delta.y;

      if (m_error < 0) {
        m_p0.y += m_step.y;
        m_error += m_step.x * m_delta.x;
      }
    } else {
      if (m_p0.y == m_p1.y) {
        return true;
      }

      m_p0.y += m_step.y;
      m_error -= m_step.x * m_delta.x;

      if (m_error < 0) {
        m_p0.x += m_step.x;
        m_error += m_step.y * m_delta.y;
      }
    }

    res = m_p0;
    return false;
  }

  std::vector<Vector2i> generateLine(Vector2i p0, Vector2i p1) {
    Bresenham bresenham(p0, p1);

    std::vector<Vector2i> ret;
    ret.push_back(p0);

    Vector2i p;

    while (!bresenham.step(p)) {
      ret.push_back(p);
    }

    assert(ret.back() == p1);
    ret.pop_back(); // to remove p1

    return ret;
  }

  /*
   * Midpoint Displacement 1D
   */

  std::vector<Vector2f> midpointDisplacement1D(Vector2f p0, Vector2f p1, Random& random, unsigned iterations, Vector2f direction, float initialFactor, float reductionFactor) {
    direction = initialFactor * gf::euclideanDistance(p0, p1) * gf::normalize(direction);

    std::vector<Vector2f> ret;

    std::size_t size = static_cast<std::size_t>(1) << iterations;
    std::size_t count = size + 1;
    ret.resize(count);
    ret[0] = p0;
    ret[count - 1] = p1;

    std::size_t step = size / 2;

    while (step > 0) {
      for (std::size_t i = step; i < size; i += 2 * step) {
        assert(i - step < count);
        Vector2f prev = ret[i - step];
        assert(i + step < count);
        Vector2f next = ret[i + step];
        Vector2f mid = (prev + next) / 2;
        mid += random.computeUniformFloat(-0.5f, 0.5f) * direction;
        ret[i] = mid;
      }

      direction *= reductionFactor;
      step /= 2;
    }

    return ret;
  }

  std::vector<Vector2f> midpointDisplacement1D(Vector2f p0, Vector2f p1, Random& random, unsigned iterations, float initialFactor, float reductionFactor) {
    return midpointDisplacement1D(p0, p1, random, iterations, gf::perp(p1 - p0), initialFactor, reductionFactor);
  }

  /*
   * Midpoint Displacement 2D
   */

  namespace {

    int computePowerOfTwoSize(Vector2i size) {
      int actualSize = 1;

      while (actualSize + 1 < size.height || actualSize + 1 < size.width) {
        actualSize = actualSize * 2;
      }

      return actualSize;
    }

    void initializeCorners(Heightmap& map, Span<const double> initialValues, int d) {
      if (initialValues.getSize() == 0) {
        map.setValue({ 0, 0 }, 0.0);
        map.setValue({ 0, d }, 0.0);
        map.setValue({ d, d }, 0.0);
        map.setValue({ d, 0 }, 0.0);
      } else if (initialValues.getSize() < 4) {
        map.setValue({ 0, 0 }, initialValues[0]);
        map.setValue({ 0, d }, initialValues[0]);
        map.setValue({ d, d }, initialValues[0]);
        map.setValue({ d, 0 }, initialValues[0]);
      } else {
        map.setValue({ 0, 0 }, initialValues[0]);
        map.setValue({ 0, d }, initialValues[1]);
        map.setValue({ d, d }, initialValues[2]);
        map.setValue({ d, 0 }, initialValues[3]);
      }
    }

  } // anonymous namespace

  Heightmap midpointDisplacement2D(Vector2i size, Random& random, Span<const double> initialValues) {
    int actualSize = computePowerOfTwoSize(size);

    int d = actualSize;
    actualSize = actualSize + 1;

    Heightmap map({ actualSize, actualSize });
    initializeCorners(map, initialValues, d);

    while (d >= 2) {
      int d2 = d / 2;

      std::uniform_real_distribution<double> dist(-static_cast<double>(d), static_cast<double>(d));

      for (int y = d2; y < actualSize; y += d) {
        for (int x = d2; x < actualSize; x += d) {
          double ne = map.getValue({ x - d2, y - d2 });
          double nw = map.getValue({ x - d2, y + d2 });
          double se = map.getValue({ x + d2, y - d2 });
          double sw = map.getValue({ x + d2, y + d2 });

          // center
          double center = (ne + nw + se + sw) / 4;
          map.setValue({ x, y }, center + dist(random.getEngine()));

          // north
          double north = (ne + nw) / 2;
          map.setValue({ x - d2, y }, north + dist(random.getEngine()));

          // south
          double south = (se + sw) / 2;
          map.setValue({ x + d2, y }, south + dist(random.getEngine()));

          // east
          double east = (ne + se) / 2;
          map.setValue({ x, y - d2 }, east + dist(random.getEngine()));

          // west
          double west = (nw + sw) / 2;
          map.setValue({ x, y + d2 }, west + dist(random.getEngine()));
        }
      }

      d = d2;
    }

    Vector2i offset = (actualSize - size) / 2;
    return map.subMap(RectI::fromPositionSize(offset, size));
  }

  /*
   * Diamond-Square
   */

  namespace {

    void diamond(Heightmap& map, Random& random, Vector2i pos, int d) {
      double value = (map.getValue({ pos.x - d, pos.y - d })
                    + map.getValue({ pos.x - d, pos.y + d })
                    + map.getValue({ pos.x + d, pos.y - d })
                    + map.getValue({ pos.x + d, pos.y + d })) / 4;

      double noise = random.computeUniformFloat(-static_cast<double>(d), static_cast<double>(d));

      map.setValue(pos, value + noise);
    }

    void square(Heightmap& map, Random& random, Vector2i pos, int d) {
      Vector2i size = map.getSize();

      double value = 0.0;
      int n = 0;

      if (pos.x >= d) {
        value += map.getValue({ pos.x - d, pos.y });
        ++n;
      }

      if (pos.x + d < size.width) {
        value += map.getValue({ pos.x + d, pos.y });
        ++n;
      }

      if (pos.y >= d) {
        value += map.getValue({ pos.x, pos.y - d });
        ++n;
      }

      if (pos.y + d < size.height) {
        value += map.getValue({ pos.x, pos.y + d });
        ++n;
      }

      assert(n > 0);
      value = value / n;

      double noise = random.computeUniformFloat(-static_cast<double>(d), static_cast<double>(d));

      map.setValue(pos, value + noise);
    }

  } // anonymous namespace

  Heightmap diamondSquare2D(Vector2i size, Random& random, Span<const double> initialValues) {
    int actualSize = computePowerOfTwoSize(size);

    int d = actualSize;
    actualSize = actualSize + 1;

    Heightmap map({ actualSize, actualSize });
    initializeCorners(map, initialValues, d);

    while (d >= 2) {
      int d2 = d / 2;

      for (int y = d2; y < actualSize; y += d) {
        for (int x = d2; x < actualSize; x += d) {
          diamond(map, random, { x, y }, d2);
        }
      }

      for (int y = 0; y < actualSize; y += d) {
        for (int x = d2; x < actualSize; x += d) {
          square(map, random, { x, y }, d2);
        }
      }

      for (int y = d2; y < actualSize; y += d) {
        for (int x = 0; x < actualSize; x += d) {
          square(map, random, { x, y }, d2);
        }
      }

      d = d2;
    }

    Vector2i offset = (actualSize - size) / 2;
    return map.subMap(RectI::fromPositionSize(offset, size));
  }

  /*
   * Convex Hull
   */

  namespace {

    void findHull(const std::vector<Vector2f>& in, std::vector<Vector2f>& out, Vector2f a, Vector2f b) {
      if (in.empty()) {
        return;
      }

      Vector2f perp = gf::perp(a - b);

      auto it = std::max_element(in.begin(), in.end(), [a, perp](Vector2f p1, Vector2f p2) {
        Vector2f ap1 = p1 - a;
        Vector2f ap2 = p2 - a;

        float d1 = gf::dot(ap1, perp);
        assert(d1 > 0);

        float d2 = gf::dot(ap2, perp);
        assert(d2 > 0);

        return d1 < d2;
      });

      Vector2f c = *it;

      std::vector<Vector2f> s1;
      std::vector<Vector2f> s2;

      for (auto& p : in) {
        if (p == c) {
          continue;
        }

        if (gf::cross(c - a, p - a) < 0) { // P is left of AC
          s1.push_back(p);
        }

        if (gf::cross(b - c, p - c) < 0) { // P is left of CB
          s2.push_back(p);
        }
      }

      findHull(s1, out, a, c);
      out.push_back(c);
      findHull(s2, out, c, b);
    }

    void quickHull(Span<const Vector2f> in, std::vector<Vector2f>& out) {
      const Vector2f *pa;
      const Vector2f *pb;

      std::tie(pa, pb) = std::minmax_element(in.begin(), in.end(), [](Vector2f p1, Vector2f p2) {
        return p1.x < p2.x;
      });

      const Vector2f a = *pa;
      const Vector2f b = *pb;

      std::vector<Vector2f> s1;
      std::vector<Vector2f> s2;

      for (auto& p : in) {
        if (p == a || p == b) {
          continue;
        }

        if (gf::cross(b - a, p - a) < 0) { // P is left of AB
          s1.push_back(p);
        } else { // P is left of BA
          s2.push_back(p);
        }
      }

      out.push_back(a);
      findHull(s1, out, a, b);
      out.push_back(b);
      findHull(s2, out, b, a);
    }

  } // anonymous namespace

  Polygon convexHull(Span<const Vector2f> points) {
    if (points.getSize() <= 3) {
      return Polygon(points);
    }

    std::vector<Vector2f> out;
    quickHull(points, out);
    return Polygon(out);
  }


  namespace {

    float distanceOfPointToLine(gf::Vector2f point, gf::Vector2f l1, gf::Vector2f l2) {
      return std::abs(cross(l1 - l2, point - l1)) / euclideanDistance(l1, l2);
    }

    void simplifyPointsRecursive(Span<const Vector2f> points, float distance, std::vector<Vector2f>& result) {
      float maxDistance = 0.0f;
      std::size_t maxIndex = 0;

      std::size_t last = points.getSize() - 1;

      for (std::size_t i = 1; i < last; ++i) {
        float currentDistance = distanceOfPointToLine(points[i], points[0], points[last]);

        if (currentDistance > maxDistance) {
          maxDistance = currentDistance;
          maxIndex = i;
        }
      }

      if (maxDistance > distance) {
        simplifyPointsRecursive(points.first(maxIndex), distance, result);
        result.push_back(points[maxIndex]);
        simplifyPointsRecursive(points.lastExcept(maxIndex), distance, result);
      }
    }

  } // anonymous namespace

  std::vector<Vector2f> simplifyPoints(Span<const Vector2f> points, float distance) {
    std::vector<Vector2f> result;
    result.push_back(points[0]);
    simplifyPointsRecursive(points, distance, result);
    result.push_back(points[points.getSize() - 1]);
    return result;
  }


  namespace {

    template<typename Iterator>
    Iterator findNextSegment(Iterator begin, Iterator end, gf::Vector2i endPoint, bool& needReverse) {
      for (auto it = begin; it != end; ++it) {
        if (it->p0 == endPoint) {
          needReverse = false;
          return it;
        }

        if (it->p1 == endPoint) {
          needReverse = true;
          return it;
        }
      }

      return end;
    }

  } // anonymous namespace

  bool operator<(const SegmentI& lhs, const SegmentI& rhs) {
    return std::tie(lhs.p0.x, lhs.p0.y, lhs.p1.x, lhs.p1.y) < std::tie(rhs.p0.x, rhs.p0.y, rhs.p1.x, rhs.p1.y);
  }

  std::vector<Polyline> buildLines(Span<const SegmentI> segments) {
    std::vector<Polyline> lines;

    std::multiset<SegmentI> remaining(segments.begin(), segments.end());

     for (auto it = remaining.begin(); it != remaining.end(); /* not here */) {
      // start a new line
      Polyline polyline(gf::Polyline::Loop);

      SegmentI first = *it;
      polyline.addPoint(first.p0);

      Vector2i endPoint = first.p1;

      for (;;) {
        polyline.addPoint(endPoint);

        bool needReverse;
        auto next = findNextSegment(std::next(it), remaining.end(), endPoint, needReverse);

        if (next == remaining.end()) {
          // the line is a chain
          polyline.setType(gf::Polyline::Chain);
          break;
        }

        SegmentI chosen = *next;
        remaining.erase(next);

        if (needReverse) {
          std::swap(chosen.p0, chosen.p1);
        }

        assert(chosen.p0 == endPoint);
        endPoint = chosen.p1;

        if (endPoint == first.p0) {
          // the line is a loop
          break;
        }
      }

      auto prev = it++;
      remaining.erase(prev);

      lines.push_back(std::move(polyline));
    }

    return lines;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
