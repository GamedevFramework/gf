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
#ifndef GF_RANDOM_H
#define GF_RANDOM_H

#include <cstdint>
#include <random>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A random engine
   *
   * gf::Random is a wrapper around [C++11 standard random features](http://en.cppreference.com/w/cpp/numeric/random).
   * It embeds a Mersenne Twister engine and provides several distributions
   * above this engine.
   *
   */
  class GF_API Random {
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
     * This method is easy but not very good because it allows only @f$ 2^{32} @f$
     * possible states. Choose this constructor if you need reproducible
     * randomness as a same seed will always provide the same sequence.
     *
     * @param seed The seed for the engine
     */
    Random(std::uint_fast32_t seed)
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
     * @brief Get the underlying engine
     *
     * @return A reference to the engine
     */
    std::mt19937& getEngine() {
      return m_engine;
    }

  private:
    std::mt19937 m_engine;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RANDOM_H
