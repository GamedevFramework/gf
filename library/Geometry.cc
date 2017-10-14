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
#include <gf/Geometry.h>

#include <cassert>

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

    std::size_t size = 1 << iterations;
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

  static int computePowerOfTwoSize(Vector2i size) {
    int actualSize = 1;

    while (actualSize + 1 < size.height || actualSize + 1 < size.width) {
      actualSize = actualSize * 2;
    }

    return actualSize;
  }

  static void initializeCorners(Heightmap& map, ArrayRef<double> initialValues, int d) {
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

  Heightmap midpointDisplacement2D(Vector2i size, Random& random, ArrayRef<double> initialValues) {
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
    return map.subMap({ offset, size });
  }

  /*
   * Diamond-Square
   */

  static void diamond(Heightmap& map, Random& random, Vector2i pos, int d) {
    double value = (map.getValue({ pos.x - d, pos.y - d })
                  + map.getValue({ pos.x - d, pos.y + d })
                  + map.getValue({ pos.x + d, pos.y - d })
                  + map.getValue({ pos.x + d, pos.y + d })) / 4;

    double noise = random.computeUniformFloat(-static_cast<double>(d), static_cast<double>(d));

    map.setValue(pos, value + noise);
  }

  static void square(Heightmap& map, Random& random, Vector2i pos, int d) {
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

  Heightmap diamondSquare2D(Vector2i size, Random& random, ArrayRef<double> initialValues) {
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
    return map.subMap({ offset, size });
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
