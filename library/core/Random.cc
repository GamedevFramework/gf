/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <gf/Random.h>

#include <algorithm>
#include <functional>
#include <iterator>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    // see http://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
    // and http://www.pcg-random.org/posts/cpp-seeding-surprises.html
    std::mt19937 getCorrectlyInitializedEngine() {
      std::mt19937::result_type data[std::mt19937::state_size];
      std::random_device source;
      std::generate(std::begin(data), std::end(data), std::ref(source));
      std::seed_seq seeds(std::begin(data), std::end(data));
      return std::mt19937(seeds);
    }

  } // anonymous namespace

  Random::Random()
  : m_engine(getCorrectlyInitializedEngine())
  {

  }

  Vector2f Random::computePosition(const RectF& area) {
    std::uniform_real_distribution<float> distX(area.min.x, area.max.x);
    std::uniform_real_distribution<float> distY(area.min.y, area.max.y);
    float x = distX(m_engine);
    float y = distY(m_engine);
    return { x, y };
  }

  Vector2i Random::computePosition(const RectI& area) {
    std::uniform_int_distribution<int> distX(area.min.x, area.max.x);
    std::uniform_int_distribution<int> distY(area.min.y, area.max.y);
    int x = distX(m_engine);
    int y = distY(m_engine);
    return { x, y };
  }

  Vector2f Random::computePosition(const CircF& area) {
    float angle = computeAngle();
    float radius = computeRadius(0.0f, area.radius);
    return area.center + radius * gf::unit(angle);
  }

  float Random::computeRadius(float radiusMin, float radiusMax) {
    std::uniform_real_distribution<float> dist(gf::square(radiusMin), gf::square(radiusMax));
    return std::sqrt(dist(m_engine));
  }

  float Random::computeAngle() {
    std::uniform_real_distribution<float> dist(0.0f, 2.0f * gf::Pi);
    return dist(m_engine);
  }

  gf::Id Random::computeId() {
    return computeUniformInteger(std::numeric_limits<gf::Id>::min() + 1, std::numeric_limits<gf::Id>::max());
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
