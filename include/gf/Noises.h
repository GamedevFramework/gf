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
#ifndef GF_NOISES_H
#define GF_NOISES_H

#include <cstddef>
#include <cstdint>
#include <array>
#include <functional>
#include <vector>

#include "Math.h"
#include "Noise.h"
#include "Portability.h"
#include "Vector.h"


namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Random;


  /**
   * @ingroup core
   * @brief Gradient 2D noise
   *
   * [Gradient noise](https://en.wikipedia.org/wiki/Gradient_noise) is a
   * lattice-based noise based on gradients.
   */
  class GF_API GradientNoise2D : public Noise2D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param step A step
     *
     * @sa gf::Step
     */
    GradientNoise2D(Random& random, Step<double> step);

    virtual double getValue(double x, double y) override;

  private:
    Step<double> m_step;
    std::array<uint8_t, 256> m_perm;
    std::array<Vector2d, 256> m_gradients2D;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };

  /**
   * @ingroup core
   * @brief Gradient 3D noise
   *
   * [Gradient noise](https://en.wikipedia.org/wiki/Gradient_noise) is a
   * lattice-based noise based on gradients.
   */
  class GF_API GradientNoise3D : public Noise3D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param step A step
     *
     * @sa gf::Step
     */
    GradientNoise3D(Random& random, Step<double> step);

    virtual double getValue(double x, double y, double z) override;

  private:
    Step<double> m_step;
    std::array<uint8_t, 256> m_perm;
    std::array<Vector3d, 256> m_gradients3D;

    const Vector3d& at(uint8_t i, uint8_t j, uint8_t k) const;
  };

  /**
   * @ingroup core
   * @brief Better gradient 2D noise
   *
   * An implementation of the better gradient noise of Kensler et al.,
   * especially the new hash function and filter kernel. This noise is
   * slower than gradient noise but gives better results.
   *
   * @sa [Better Gradient Noise. A. Kensler, A. Knoll, P. Shirley. 2008](https://www.cs.utah.edu/~aek/research/noise.pdf)
   */
  class GF_API BetterGradientNoise2D : public Noise2D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     */
    BetterGradientNoise2D(Random& random);

    virtual double getValue(double x, double y) override;

  private:
    std::array<uint8_t, 256> m_permX;
    std::array<uint8_t, 256> m_permY;
    std::array<Vector2d, 256> m_gradients2D;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };

  /**
   * @ingroup core
   * @brief Fractal 2D noise
   *
   * Fractal noise is based of fractional Brownian motion (fBm). It consists
   * in adding several octaves of a basic noise at different amplitudes.
   *
   */
  class GF_API FractalNoise2D : public Noise2D {
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
    FractalNoise2D(Noise2D& noise, double scale, std::size_t octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double
    dimension = 1.0);

    virtual double getValue(double x, double y) override;

  private:
    Noise2D& m_noise;
    double m_scale;
    std::size_t m_octaves;
    double m_lacunarity;
    double m_persistence;
    double m_dimension;
  };


  /**
   * @ingroup core
   * @brief Fractal 3D noise
   *
   * Fractal noise is based of fractional Brownian motion (fBm). It consists
   * in adding several octaves of a basic noise at different amplitudes.
   *
   */
  class GF_API FractalNoise3D : public Noise3D {
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
    FractalNoise3D(Noise3D& noise, double scale, std::size_t octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    virtual double getValue(double x, double y, double z) override;

  private:
    Noise3D& m_noise;
    double m_scale;
    std::size_t m_octaves;
    double m_lacunarity;
    double m_persistence;
    double m_dimension;
  };


  /**
   * @ingroup core
   * @brief Perlin 2D noise
   *
   * [Perlin noise](https://en.wikipedia.org/wiki/Perlin_noise) is the
   * combination of a fractal noise and a gradient noise.
   *
   * @sa gf::GradientNoise2D, gf::FractalNoise2D
   */
  class GF_API PerlinNoise2D : public Noise2D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param scale The scale factor
     * @param octaves The number of octaves
     */
    PerlinNoise2D(Random& random, double scale, std::size_t octaves = 8);

    virtual double getValue(double x, double y) override;

  private:
    GradientNoise2D m_gradient;
    FractalNoise2D m_fractal;
  };

  /**
   * @ingroup core
   * @brief Perlin 3D noise
   *
   * [Perlin noise](https://en.wikipedia.org/wiki/Perlin_noise) is the
   * combination of a fractal noise and a gradient noise.
   *
   * @sa gf::GradientNoise3D, gf::FractalNoise3D
   */
  class GF_API PerlinNoise3D : public Noise3D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param scale The scale factor
     * @param octaves The number of octaves
     */
    PerlinNoise3D(Random& random, double scale, std::size_t octaves = 8);

    virtual double getValue(double x, double y, double z) override;

  private:
    GradientNoise3D m_gradient;
    FractalNoise3D m_fractal;
  };


  /**
   * @ingroup core
   * @brief Simplex 2D noise
   *
   * [Simplex noise](https://en.wikipedia.org/wiki/Simplex_noise) is a lattice
   * noise based on gradients put on a simplex.
   *
   * This implementation is limited to 2D noise and is *not* submitted to the
   * patent that covers simplex noise.
   *
   * @sa gf::GradientNoise2D
   */
  class GF_API SimplexNoise2D : public Noise2D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     */
    SimplexNoise2D(Random& random);

    virtual double getValue(double x, double y) override;

  private:
    std::array<uint8_t, 256> m_perm;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };


  /**
   * @ingroup core
   * @brief OpenSimplex 2D noise
   *
   * [OpenSimplex noise](https://en.wikipedia.org/wiki/OpenSimplex_noise) is a lattice
   * noise very similar to simplex noise.
   *
   * @sa gf::SimplexNoise2D
   */
  class GF_API OpenSimplexNoise2D : public Noise2D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     */
    OpenSimplexNoise2D(Random& random);

    virtual double getValue(double x, double y) override;

  private:
    std::array<uint8_t, 256> m_perm;

    const Vector2d& at(uint8_t i, uint8_t j) const;
  };

  /**
   * @ingroup core
   * @brief OpenSimplex3D noise
   *
   * [OpenSimplex noise](https://en.wikipedia.org/wiki/OpenSimplex_noise) is a lattice
   * noise very similar to simplex noise.
   *
   * @sa gf::SimplexNoise
   */
  class GF_API OpenSimplexNoise3D : public Noise3D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     */
    OpenSimplexNoise3D(Random& random);

    virtual double getValue(double x, double y, double z) override;

  private:
    std::array<uint8_t, 256> m_perm;

    const Vector3d& at(uint8_t i, uint8_t j, uint8_t k) const;
  };

  /**
   * @ingroup core
   * @brief Wavelet 3D noise
   *
   */
  class GF_API WaveletNoise3D : public Noise3D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param n Wavelet tile size
     */
    WaveletNoise3D(Random& random, std::ptrdiff_t n = 32);

    virtual double getValue(double x, double y, double z) override;

  private:
    std::ptrdiff_t m_n;
    std::vector<double> m_data;
  };

  /**
   * @ingroup core
   * @brief Worley 2D noise
   *
   * [Worley noise](https://en.wikipedia.org/wiki/Worley_noise) is a point
   * based noise. It is also known as Voronoi noise or cellular noise or
   * simply cell noise.
   */
  class GF_API WorleyNoise2D : public Noise2D {
  public:
    /**
     * @brief Constructor
     *
     * @param random A random engine
     * @param count The number of points
     * @param distance A distance function
     * @param coeffs The coefficients for the noise
     */
    WorleyNoise2D(Random& random, std::size_t count, Distance2<double> distance, std::vector<double> coeffs);

    virtual double getValue(double x, double y) override;

  private:
    std::size_t m_count;
    Distance2<double> m_distance;
    std::vector<double> m_coeffs;
    std::vector<Vector2d> m_cells;
  };


  /**
   * @ingroup core
   * @brief Hetero Terrain 2D noise
   *
   */
  class GF_API HeteroTerrain2D : public Noise2D {
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
    HeteroTerrain2D(Noise2D& noise, double scale, double offset = 0.0, std::size_t octaves = 8, double lacunarity = 2.0, double persistence = 0.5, double dimension = 1.0);

    virtual double getValue(double x, double y) override;

  private:
    Noise2D& m_noise;
    double m_scale;
    double m_offset;
    std::size_t m_octaves;
    double m_lacunarity;
    double m_persistence;
    double m_dimension;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_NOISES_H
