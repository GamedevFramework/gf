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
#ifndef GF_NOISE_H
#define GF_NOISE_H

#include <cstddef>
#include <cstdint>
#include <array>
#include <functional>

#include "Math.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Random;

  /**
   * @ingroup core
   * @brief Gradient noise
   *
   * [Gradient noise](https://en.wikipedia.org/wiki/Gradient_noise) is a
   * lattice-based noise based on gradients.
   */
  class GF_API GradientNoise {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param step A step
     *
     * @sa gf::Step
     */
    GradientNoise(Random& random, Step<double> step);

    /**
     * @brief Take a noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     */
    double operator()(double x, double y) const;

  private:
    Step<double> m_step;
    std::array<Vector2d, 256> m_gradients;
    std::array<uint8_t, 256> m_perm;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };


  /**
   * @ingroup core
   * @brief Fractal noise
   *
   * Fractal noise is based of fractional Brownian motion (fBm). It consists
   * in adding several octaves of a basic noise at different amplitudes.
   *
   */
  class GF_API FractalNoise {
  public:
    /**
     * @brief Constructor
     *
     * @param noise The basic noise function
     * @param scale The scale factor
     * @param octaves The number of octaves
     * @param lacunarity The factor applied to frequency
     * @param persistence The factor applied to amplitude
     * @param dimension The contrast between the layers
     */
    FractalNoise(std::function<double(double,double)> noise, double scale, std::size_t octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0)
    : m_noise(noise)
    , m_scale(scale)
    , m_octaves(octaves)
    , m_lacunarity(lacunarity)
    , m_persistence(persistence)
    , m_dimension(dimension)
    {

    }

    /**
     * @brief Take a noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     */
    double operator()(double x, double y) const;

  private:
    std::function<double(double,double)> m_noise;
    double m_scale;
    std::size_t m_octaves;
    double m_lacunarity;
    double m_persistence;
    double m_dimension;
  };



  /**
   * @ingroup core
   * @brief Perlin noise
   *
   * [Perlin noise](https://en.wikipedia.org/wiki/Perlin_noise) is the
   * combination of a fractal noise and a gradient noise.
   *
   * @sa gf::GradientNoise, gf::FractalNoise
   */
  class GF_API PerlinNoise {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param scale The scale factor
     * @param octaves The number of octaves
     */
    PerlinNoise(Random& random, double scale, std::size_t octaves = 8);

    /**
     * @brief Take a noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     */
    double operator()(double x, double y) const;

  private:
    GradientNoise m_gradient;
    FractalNoise m_fractal;
  };

  /**
   * @ingroup core
   * @brief Simplex noise
   *
   * [Simplex noise](https://en.wikipedia.org/wiki/Simplex_noise) is a lattice
   * noise based on gradients put on a simplex.
   *
   * This implementation is limited to 2D noise and is *not* submitted to the
   * patent that covers simplex noise.
   *
   * @sa gf::GradientNoise
   */
  class GF_API SimplexNoise {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     */
    SimplexNoise(Random& random);

    /**
     * @brief Take a noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     */
    double operator()(double x, double y) const;


  private:
    std::array<uint8_t, 256> m_perm;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };

  /**
   * @ingroup core
   * @brief OpenSimplex noise
   *
   * [OpenSimplex noise](https://en.wikipedia.org/wiki/OpenSimplex_noise) is a lattice
   * noise very similar to simplex noise.
   *
   * @sa gf::SimplexNoise
   */
  class GF_API OpenSimplexNoise {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     */
    OpenSimplexNoise(Random& random);

    /**
     * @brief Take a noise value
     *
     * @param x The x coordinate of the noise value
     * @param y The y coordinate of the noise value
     * @return The noise value
     */
    double operator()(double x, double y) const;


  private:
    std::array<uint8_t, 256> m_perm;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_NOISE_H
