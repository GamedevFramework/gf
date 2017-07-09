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
    Vector2i p;

    while (!bresenham.step(p)) {
      ret.push_back(p);
    }

    return ret;
  }


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
//       gf::Log::info("Step %zu\n", step);

      for (std::size_t i = step; i < size; i += 2 * step) {
        assert(i - step < count);
        Vector2f prev = ret[i - step];
        assert(i + step < count);
        Vector2f next = ret[i + step];
        Vector2f mid = (prev + next) / 2;
        mid += random.computeUniformFloat(-0.5f, 0.5f) * direction;
        ret[i] = mid;
//         gf::Log::info("%zu: [%zu](%f,%f) . [%zu](%f,%f) -> [%zu](%f,%f)\n", i, i - step, prev.x, prev.y, i + step, next.x, next.y, i, mid.x, mid.y);
      }

      direction *= reductionFactor;
      step /= 2;
    }

    return ret;
  }

  std::vector<Vector2f> midpointDisplacement1D(Vector2f p0, Vector2f p1, Random& random, unsigned iterations, float initialFactor, float reductionFactor) {
    return midpointDisplacement1D(p0, p1, random, iterations, gf::perp(p1 - p0), initialFactor, reductionFactor);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
