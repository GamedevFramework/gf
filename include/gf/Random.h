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
#ifndef GF_RANDOM_H
#define GF_RANDOM_H

#include <cstdint>
#include <limits>
#include <random>

#include "Circ.h"
#include "CoreApi.h"
#include "Id.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_utilities
   * @brief A random engine
   *
   *
   * @sa gf::Random
   */
  class GF_CORE_API RandomEngine {
  public:
    using result_type = uint64_t;

    RandomEngine();

    RandomEngine(result_type seed);

    static constexpr result_type min() {
      return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max() {
      return std::numeric_limits<result_type>::max();
    }

    result_type operator()();

    void shortJump();
    void longJump();

  private:
    uint64_t next();

  private:
    uint64_t m_state[4];
  };

  /**
   * @ingroup core_utilities
   * @brief A set of random utilities
   *
   * gf::Random is a wrapper around [C++11 standard random features](http://en.cppreference.com/w/cpp/numeric/random).
   * It embeds a random engine and provides several distributions above this engine.
   *
   * @sa gf::RandomEngine
   */
  class GF_CORE_API Random {
  public:
    /**
     * @brief Default constructor with complex initialization
     *
     * This constructor initializes the Mersenne Twister thanks to a random
     * device and a seed sequence. This ensures that the state of the engine
     * is different for each instanciation. Choose this constructor if you
     * need good statistical randomness.
     */
    Random();

    /**
     * @brief Constructor with simple initialization
     *
     * This constructor initializes the Mersenne Twister with a single seed.
     * This method is easy but not very good because it allows only @f$ 2^{64} @f$
     * possible states. Choose this constructor if you need reproducible
     * randomness as a same seed will always provide the same sequence.
     *
     * @param seed The seed for the engine
     */
    Random(uint64_t seed)
    : m_engine(seed)
    {

    }

    /**
     * @brief Compute an integer with a uniform distribution
     *
     * @param min The minimum value (inclusive)
     * @param max The maximum value (inclusive)
     * @return A value between `min` and `max`
     */
    template<typename T>
    T computeUniformInteger(T min, T max) {
      std::uniform_int_distribution<T> dist(min, max);
      return dist(m_engine);
    }

    /**
     * @brief Compute a float with a uniform distribution
     *
     * @param min The minimum value (inclusive)
     * @param max The maximum value (exclusive)
     * @return A value between `min` and `max`
     */
    template<typename T>
    T computeUniformFloat(T min, T max) {
      std::uniform_real_distribution<T> dist(min, max);
      return dist(m_engine);
    }

    /**
     * @brief Compute a float with a normal (Gaussian) distribution
     *
     * @param mean The mean of the distribution
     * @param stddev The standard deviation of the distribution
     * @return A value with a normal distribution
     */
    template<typename T>
    T computeNormalFloat(T mean, T stddev) {
      std::normal_distribution<T> dist(mean, stddev);
      return dist(m_engine);
    }

    /**
     * @brief Compute a boolean with a Bernoulli distribution
     *
     * @param p The probability of true
     * @return true with a probability of @f$ p @f$.
     */
    bool computeBernoulli(double p) {
      std::bernoulli_distribution dist(p);
      return dist(m_engine);
    }

    /**
     * @brief Compute a uniform position in a given area
     *
     * @param area The area
     * @returns A value in the area
     */
    Vector2f computePosition(const RectF& area);

    /**
     * @brief Compute a uniform position in a given area
     *
     * @param area The area
     * @returns A value in the area
     */
    Vector2i computePosition(const RectI& area);

    /**
     * @brief Compute a uniform position in a circle
     *
     * @param area The area
     * @returns A value in the circle
     */
    Vector2f computePosition(const CircF& area);

    /**
     * @brief Compute a uniform radius
     *
     * @param radiusMin The minimum radius
     * @param radiusMax The maximum radius
     * @returns A valid radius
     */
    float computeRadius(float radiusMin, float radiusMax);

    /**
     * @brief Compute a uniform angle in the range @f$ [ 0, 2 \pi ] @f$
     *
     * @returns A valid angle
     */
    float computeAngle();

    /**
     * @brief Compute an id
     *
     * @returns An id, except gf::InvalidId
     */
    gf::Id computeId();

    /**
     * @brief Get the underlying engine
     *
     * @return A reference to the engine
     */
    RandomEngine& getEngine() {
      return m_engine;
    }

  private:
    RandomEngine m_engine;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RANDOM_H
