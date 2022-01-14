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
#include <gf/Random.h>

#include <cinttypes>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    constexpr uint64_t rotl(const uint64_t x, int k) {
      return (x << k) | (x >> (64 - k));
    }

    // https://prng.di.unimi.it/splitmix64.c
    struct SplitMix64 {
      constexpr
      SplitMix64(uint64_t seed)
      : m_state(seed)
      {
      }

      uint64_t next() {
        uint64_t z = (m_state += UINT64_C(0x9e3779b97f4a7c15));
        z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
        return z ^ (z >> 31);
      }

      uint64_t m_state;
    };


    uint64_t computeRandomSeed() {
      std::random_device rd;
      uint64_t result = rd();
      result = (result << 32) + rd();
      return result;
    }

  } // anonymous namespace

  // RandomEngine

  RandomEngine::RandomEngine()
  {
    for (auto & state : m_state) {
      state = computeRandomSeed();
    }
  }

  RandomEngine::RandomEngine(result_type seed)
  {
    SplitMix64 sm(seed);

    for (auto & state : m_state) {
      state = sm.next();
    }
  }

  auto RandomEngine::operator()() -> result_type {
    return next();
  }

  // https://prng.di.unimi.it/xoshiro256plusplus.c
  uint64_t RandomEngine::next() {
    const uint64_t result = rotl(m_state[0] + m_state[3], 23) + m_state[0];
    const uint64_t t = m_state[1] << 17;

    m_state[2] ^= m_state[0];
    m_state[3] ^= m_state[1];
    m_state[1] ^= m_state[2];
    m_state[0] ^= m_state[3];

    m_state[2] ^= t;

    m_state[3] = rotl(m_state[3], 45);

    return result;
  }

  void RandomEngine::shortJump() {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x180ec6d33cfd0aba),
      UINT64_C(0xd5a61266f0c9392c),
      UINT64_C(0xa9582618e03fc9aa),
      UINT64_C(0x39abdc4529b1661c)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for(auto jump : Jump) {
      for(int b = 0; b < 64; b++) {
        if (jump & UINT64_C(1) << b) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  void RandomEngine::longJump() {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x76e15d3efefdcbbf),
      UINT64_C(0xc5004e441c522fb3),
      UINT64_C(0x77710069854ee241),
      UINT64_C(0x39109bb02acbe635)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for(auto jump : Jump) {
      for(int b = 0; b < 64; b++) {
        if (jump & UINT64_C(1) << b) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }


  // Random

  Random::Random()
  {
    m_engine.shortJump();
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
