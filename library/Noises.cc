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
#include <gf/Noises.h>

#include <cassert>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>

#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static void generatePermutation(Random& random, std::array<uint8_t, 256>& perm) {
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), random.getEngine());
  }

  /*
   * Value
   */
  ValueNoise2D::ValueNoise2D(Random& random, Step<double> step)
  : m_step(step)
  {
    // initialize permutation

    generatePermutation(random, m_perm);

    // generate values

    std::uniform_real_distribution<double> distValue(0.0, 1.0);

    for (auto& value : m_values) {
      value = distValue(random.getEngine());
    }

  }

  double ValueNoise2D::getValue(double x, double y) {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double nw = at(qx    , qy    );
    double ne = at(qx + 1, qy    );
    double sw = at(qx    , qy + 1);
    double se = at(qx + 1, qy + 1);

    double n = gf::lerp(nw, ne, m_step(rx));
    double s = gf::lerp(sw, se, m_step(rx));

    return gf::lerp(n, s, m_step(ry));
  }

  double ValueNoise2D::at(uint8_t i, uint8_t j) const {
    uint8_t index = i;
    index = m_perm.at(index) + j;
    return m_values.at(m_perm.at(index));
  }


  /*
   * Gradient
   */

  GradientNoise2D::GradientNoise2D(Random& random, Step<double> step)
  : m_step(step)
  {
    // initialize permutation

    generatePermutation(random, m_perm);

    // distributions

    std::uniform_real_distribution<double> distAngle(0.0, 2.0 * Pi);

    // generate 2D gradients

    for (auto& vec : m_gradients2D) {
      double angle = distAngle(random.getEngine());
      vec = gf::unit(angle);
    }
  }

  double GradientNoise2D::getValue(double x, double y) {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double p00 = dot(at(qx    , qy    ), {rx      , ry      });
    double p10 = dot(at(qx + 1, qy    ), {rx - 1.0, ry      });
    double p01 = dot(at(qx    , qy + 1), {rx      , ry - 1.0});
    double p11 = dot(at(qx + 1, qy + 1), {rx - 1.0, ry - 1.0});

    double u = m_step(rx);

    double p0 = gf::lerp(p00, p10, u);
    double p1 = gf::lerp(p01, p11, u);

    double v = m_step(ry);

    return gf::lerp(p0, p1, v);
  }

  const Vector2d& GradientNoise2D::at(uint8_t i, uint8_t j) const {
    uint8_t index = i;
    index = m_perm.at(index) + j;
    return m_gradients2D.at(m_perm.at(index));
  }


  GradientNoise3D::GradientNoise3D(Random& random, Step<double> step)
  : m_step(step)
  {
    // initialize permutation

    generatePermutation(random, m_perm);

    // distributions

    std::uniform_real_distribution<double> distAngle(0.0, 2.0 * Pi);
    std::uniform_real_distribution<double> distHalfAngle(0.0, Pi);

    // generate 3D gradients

    for (auto& vec : m_gradients3D) {
      double phi = distAngle(random.getEngine());
      double theta = distHalfAngle(random.getEngine());
      vec = { std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta) };
    }
  }

  double GradientNoise3D::getValue(double x, double y, double z) {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    uint8_t qz = static_cast<uint8_t>(std::fmod(z, 256));
    double rz = std::fmod(z, 1);
    assert(rz >= 0.0 && rz <= 1.0);

    double p000 = dot(at(qx    , qy    , qz    ), {rx      , ry      , rz      });
    double p100 = dot(at(qx + 1, qy    , qz    ), {rx - 1.0, ry      , rz      });
    double p010 = dot(at(qx    , qy + 1, qz    ), {rx      , ry - 1.0, rz      });
    double p110 = dot(at(qx + 1, qy + 1, qz    ), {rx - 1.0, ry - 1.0, rz      });
    double p001 = dot(at(qx    , qy    , qz + 1), {rx      , ry      , rz - 1.0});
    double p101 = dot(at(qx + 1, qy    , qz + 1), {rx - 1.0, ry      , rz - 1.0});
    double p011 = dot(at(qx    , qy + 1, qz + 1), {rx      , ry - 1.0, rz - 1.0});
    double p111 = dot(at(qx + 1, qy + 1, qz + 1), {rx - 1.0, ry - 1.0, rz - 1.0});

    double u = m_step(rx);

    double p00 = gf::lerp(p000, p100, u);
    double p10 = gf::lerp(p010, p110, u);
    double p01 = gf::lerp(p001, p101, u);
    double p11 = gf::lerp(p011, p111, u);

    double v = m_step(ry);

    double p0 = gf::lerp(p00, p10, v);
    double p1 = gf::lerp(p01, p11, v);

    double w = m_step(rz);

    return gf::lerp(p0, p1, w);
  }

  const Vector3d& GradientNoise3D::at(uint8_t i, uint8_t j, uint8_t k) const {
    uint8_t index = i;
    index = m_perm.at(index) + j;
    index = m_perm.at(index) + k;
    return m_gradients3D.at(m_perm.at(index));
  }


  /*
   * BetterGradient
   */

  BetterGradientNoise2D::BetterGradientNoise2D(Random& random)
  {
    // initialize permutation

    generatePermutation(random, m_permX);
    generatePermutation(random, m_permY);

    // distributions

    std::uniform_real_distribution<double> distAngle(0.0, 2.0 * Pi);

    // generate 2D gradients

    for (auto& vec : m_gradients2D) {
      double angle = distAngle(random.getEngine());
      vec = gf::unit(angle);
    }
  }

  double BetterGradientNoise2D::getValue(double x, double y) {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double value = 0.0f;

    for (int j = -1; j <= 2; ++j) {
      for (int i = -1; i <= 2; ++i) {
        Vector2d r(rx - i, ry - j);
        double d = gf::squareLength(r);

        if (d < 4) {
          double t = 1 - d / 4;
          value += dot(at(qx + i, qy + j), r) * t * t * t * t * (4 * t - 3);
        }
      }
    }

    return value;
  }

  const Vector2d& BetterGradientNoise2D::at(uint8_t i, uint8_t j) const {
    uint8_t index = m_permX.at(i) ^ m_permY.at(j);
    return m_gradients2D.at(index);
  }



  /*
   * Fractal (fBm)
   */

  FractalNoise2D::FractalNoise2D(Noise2D& noise, double scale, std::size_t octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {

  }

  double FractalNoise2D::getValue(double x, double y) {
    double value = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    for (std::size_t k = 0; k < m_octaves; ++k) {
      value += m_noise.getValue(x * frequency, y * frequency) * std::pow(amplitude, m_dimension);

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  FractalNoise3D::FractalNoise3D(Noise3D& noise, double scale, std::size_t octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {

  }

  double FractalNoise3D::getValue(double x, double y, double z) {
    double value = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;
    z *= m_scale;

    for (std::size_t k = 0; k < m_octaves; ++k) {
      value += m_noise.getValue(x * frequency, y * frequency, z * frequency) * std::pow(amplitude, m_dimension);

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }


  /*
   * Perlin
   */

  PerlinNoise2D::PerlinNoise2D(Random& random, double scale, std::size_t octaves)
  : m_gradient(random, gf::quinticStep)
  , m_fractal(m_gradient, scale, octaves)
  {

  }

  double PerlinNoise2D::getValue(double x, double y) {
    return m_fractal(x, y);
  }

  PerlinNoise3D::PerlinNoise3D(Random& random, double scale, std::size_t octaves)
  : m_gradient(random, gf::quinticStep)
  , m_fractal(m_gradient, scale, octaves)
  {

  }

  double PerlinNoise3D::getValue(double x, double y, double z) {
    return m_fractal(x, y, z);
  }


  /*
   * Simplex
   */

  SimplexNoise2D::SimplexNoise2D(Random& random)
  {
    generatePermutation(random, m_perm);
  }

  double SimplexNoise2D::getValue(double x, double y) {
    static constexpr double F2 = .366025403784438646763723170752; // (sqrt(3) - 1) / 2
    static constexpr double G2 = .211324865405187117745425609748; // K / (1 + 2 * K)

    double s = (x + y) * F2;
    double i = x + s;
    double j = y + s;

    i = std::floor(i);
    j = std::floor(j);

    double t = (i + j) * G2;
    double X0 = i - t;
    double Y0 = j - t;

    double x0 = x - X0;
    double y0 = y - Y0;

    uint8_t i1 = 0;
    uint8_t j1 = 0;

    if (x0 > y0) {
      i1 = 1;
    } else {
      j1 = 1;
    }

    double x1 = x0 - i1 + G2;
    double y1 = y0 - j1 + G2;

    double x2 = x0 - 1 + 2 * G2;
    double y2 = y0 - 1 + 2 * G2;

    uint8_t ii = static_cast<uint8_t>(i);
    uint8_t jj = static_cast<uint8_t>(j);

    double res = 0.0;

    double d0 = 0.5 - x0*x0 - y0*y0;

    if (d0 > 0) {
      d0 *= d0;
      res += d0 * d0 * dot(at(ii, jj), { x0, y0 });
    }

    double d1 = 0.5 - x1*x1 - y1*y1;

    if (d1 > 0) {
      d1 *= d1;
      res += d1 * d1 * dot(at(ii + i1, jj + j1), { x1, y1 });
    }

    double d2 = 0.5 - x2*x2 - y2*y2;

    if (d2 > 0) {
      d2 *= d2;
      res += d2 * d2 * dot(at(ii + 1, jj + 1), { x2, y2 });
    }

    return 45.23065 * res;
  }

  /*
   *         |
   *      1  -  0
   *         |
   *   6     -     4
   *         |
   * --|--|--+--|--|--
   *         |
   *   7     -     5
   *         |
   *      3  -  2
   *         |
   */

  const Vector2d& SimplexNoise2D::at(uint8_t i, uint8_t j) const {
    static constexpr Vector2d gradients[8] = {
      {  1.0,  2.0 },
      { -1.0,  2.0 },
      {  1.0, -2.0 },
      { -1.0, -2.0 },
      {  2.0,  1.0 },
      {  2.0, -1.0 },
      { -2.0,  1.0 },
      { -2.0, -1.0 }
    };

    uint8_t index = i;
    index = m_perm.at(index) + j;
    return gradients[m_perm.at(index) % 8];
  }


  /*
   * OpenSimplex
   */

  OpenSimplexNoise2D::OpenSimplexNoise2D(Random& random)
  {
    generatePermutation(random, m_perm);
  }

  double OpenSimplexNoise2D::getValue(double x, double y) {
    static constexpr double StretchConstant2D = -0.211324865405187117745425609748; // (1 / sqrt(2 + 1) - 1) / 2;
    static constexpr double SquishConstant2D = 0.366025403784438646763723170752;   // (sqrt(2 + 1) - 1) / 2;

    static constexpr double NormConstant2D = 47;

    // Place input coordinates onto grid.
    double stretchOffset = (x + y) * StretchConstant2D;
    double xs = x + stretchOffset;
    double ys = y + stretchOffset;

    // Floor to get grid coordinates of rhombus (stretched square) super-cell origin.
    double xsb = std::floor(xs);
    double ysb = std::floor(ys);

    // Skew out to get actual coordinates of rhombus origin. We'll need these later.
    double squishOffset = (xsb + ysb) * SquishConstant2D;
    double xb = xsb + squishOffset;
    double yb = ysb + squishOffset;

    // Compute grid coordinates relative to rhombus origin.
    double xins = xs - xsb;
    double yins = ys - ysb;

    // Sum those together to get a value that determines which region we're in.
    double inSum = xins + yins;

    // Positions relative to origin point.
    double dx0 = x - xb;
    double dy0 = y - yb;

    // We'll be defining these inside the next block and using them afterwards.
    double dxExt, dyExt;
    int xsvExt, ysvExt;

    double value = 0;

    // Contribution (1,0)
    double dx1 = dx0 - 1 - SquishConstant2D;
    double dy1 = dy0 - 0 - SquishConstant2D;
    double attn1 = 2 - dx1 * dx1 - dy1 * dy1;

    if (attn1 > 0) {
      attn1 *= attn1;
      value += attn1 * attn1 * dot(at(xsb + 1, ysb + 0), { dx1, dy1 });
    }

    // Contribution (0,1)
    double dx2 = dx0 - 0 - SquishConstant2D;
    double dy2 = dy0 - 1 - SquishConstant2D;
    double attn2 = 2 - dx2 * dx2 - dy2 * dy2;

    if (attn2 > 0) {
      attn2 *= attn2;
      value += attn2 * attn2 * dot(at(xsb + 0, ysb + 1), { dx2, dy2 });
    }

    if (inSum <= 1) { // We're inside the triangle (2-Simplex) at (0,0)
      double zins = 1 - inSum;
      if (zins > xins || zins > yins) { // (0,0) is one of the closest two triangular vertices
        if (xins > yins) {
          xsvExt = xsb + 1;
          ysvExt = ysb - 1;
          dxExt = dx0 - 1;
          dyExt = dy0 + 1;
        } else {
          xsvExt = xsb - 1;
          ysvExt = ysb + 1;
          dxExt = dx0 + 1;
          dyExt = dy0 - 1;
        }
      } else { // (1,0) and (0,1) are the closest two vertices.
        xsvExt = xsb + 1;
        ysvExt = ysb + 1;
        dxExt = dx0 - 1 - 2 * SquishConstant2D;
        dyExt = dy0 - 1 - 2 * SquishConstant2D;
      }
    } else { // We're inside the triangle (2-Simplex) at (1,1)
      double zins = 2 - inSum;
      if (zins < xins || zins < yins) { // (0,0) is one of the closest two triangular vertices
        if (xins > yins) {
          xsvExt = xsb + 2;
          ysvExt = ysb + 0;
          dxExt = dx0 - 2 - 2 * SquishConstant2D;
          dyExt = dy0 + 0 - 2 * SquishConstant2D;
        } else {
          xsvExt = xsb + 0;
          ysvExt = ysb + 2;
          dxExt = dx0 + 0 - 2 * SquishConstant2D;
          dyExt = dy0 - 2 - 2 * SquishConstant2D;
        }
      } else { // (1,0) and (0,1) are the closest two vertices.
        dxExt = dx0;
        dyExt = dy0;
        xsvExt = xsb;
        ysvExt = ysb;
      }

      xsb += 1;
      ysb += 1;
      dx0 = dx0 - 1 - 2 * SquishConstant2D;
      dy0 = dy0 - 1 - 2 * SquishConstant2D;
    }

    // Contribution (0,0) or (1,1)
    double attn0 = 2 - dx0 * dx0 - dy0 * dy0;

    if (attn0 > 0) {
      attn0 *= attn0;
      value += attn0 * attn0 * dot(at(xsb, ysb), { dx0, dy0 });
    }

    // Extra Vertex
    double attnExt = 2 - dxExt * dxExt - dyExt * dyExt;
    if (attnExt > 0) {
      attnExt *= attnExt;
      value += attnExt * attnExt * dot(at(xsvExt, ysvExt), { dxExt, dyExt });
    }

    return value / NormConstant2D;
  }

  OpenSimplexNoise3D::OpenSimplexNoise3D(Random& random)
  {
    generatePermutation(random, m_perm);
  }

  const Vector2d& OpenSimplexNoise2D::at(uint8_t i, uint8_t j) const {
    static constexpr Vector2d gradients[8] = {
      {  5.0,  2.0 },
      {  2.0,  5.0 },
      { -5.0,  2.0 },
      { -2.0,  5.0 },
      {  5.0, -2.0 },
      {  2.0, -5.0 },
      { -5.0, -2.0 },
      { -2.0, -5.0 }
    };

    uint8_t index = i;
    index = m_perm.at(index) + j;
    return gradients[m_perm.at(index) % 8];
  }

  double OpenSimplexNoise3D::getValue(double x, double y, double z) {
    static constexpr double StretchConstant3D = -1.0 / 6; // (1 / sqrt(3 + 1) - 1) / 3;
    static constexpr double SquishConstant3D = 1.0 / 3; // (sqrt(3 + 1) - 1) / 3;

    static constexpr double NormConstant3D = 103;


    //Place input coordinates on simplectic honeycomb.
    double stretchOffset = (x + y + z) * StretchConstant3D;
    double xs = x + stretchOffset;
    double ys = y + stretchOffset;
    double zs = z + stretchOffset;

    //Floor to get simplectic honeycomb coordinates of rhombohedron (stretched cube) super-cell origin.
    double xsb = std::floor(xs);
    double ysb = std::floor(ys);
    double zsb = std::floor(zs);

    //Skew out to get actual coordinates of rhombohedron origin. We'll need these later.
    double squishOffset = (xsb + ysb + zsb) * SquishConstant3D;
    double xb = xsb + squishOffset;
    double yb = ysb + squishOffset;
    double zb = zsb + squishOffset;

    //Compute simplectic honeycomb coordinates relative to rhombohedral origin.
    double xins = xs - xsb;
    double yins = ys - ysb;
    double zins = zs - zsb;

    //Sum those together to get a value that determines which region we're in.
    double inSum = xins + yins + zins;

    //Positions relative to origin point.
    double dx0 = x - xb;
    double dy0 = y - yb;
    double dz0 = z - zb;

    //We'll be defining these inside the next block and using them afterwards.
    double dxExt0, dyExt0, dzExt0;
    double dxExt1, dyExt1, dzExt1;
    int xsvExt0, ysvExt0, zsvExt0;
    int xsvExt1, ysvExt1, zsvExt1;

    double value = 0;

    if (inSum <= 1) { //We're inside the tetrahedron (3-Simplex) at (0,0,0)

      //Determine which two of (0,0,1), (0,1,0), (1,0,0) are closest.
      uint8_t aPoint = 0x01;
      double aScore = xins;
      uint8_t bPoint = 0x02;
      double bScore = yins;
      if (aScore >= bScore && zins > bScore) {
        bScore = zins;
        bPoint = 0x04;
      } else if (aScore < bScore && zins > aScore) {
        aScore = zins;
        aPoint = 0x04;
      }

      //Now we determine the two lattice points not part of the tetrahedron that may contribute.
      //This depends on the closest two tetrahedral vertices, including (0,0,0)
      double wins = 1 - inSum;

      if (wins > aScore || wins > bScore) { //(0,0,0) is one of the closest two tetrahedral vertices.
        uint8_t c = (bScore > aScore ? bPoint : aPoint); //Our other closest vertex is the closest out of a and b.

        if ((c & 0x01) == 0) {
          xsvExt0 = xsb - 1;
          xsvExt1 = xsb;
          dxExt0 = dx0 + 1;
          dxExt1 = dx0;
        } else {
          xsvExt0 = xsvExt1 = xsb + 1;
          dxExt0 = dxExt1 = dx0 - 1;
        }

        if ((c & 0x02) == 0) {
          ysvExt0 = ysvExt1 = ysb;
          dyExt0 = dyExt1 = dy0;

          if ((c & 0x01) == 0) {
            ysvExt1 -= 1;
            dyExt1 += 1;
          } else {
            ysvExt0 -= 1;
            dyExt0 += 1;
          }
        } else {
          ysvExt0 = ysvExt1 = ysb + 1;
          dyExt0 = dyExt1 = dy0 - 1;
        }

        if ((c & 0x04) == 0) {
          zsvExt0 = zsb;
          zsvExt1 = zsb - 1;
          dzExt0 = dz0;
          dzExt1 = dz0 + 1;
        } else {
          zsvExt0 = zsvExt1 = zsb + 1;
          dzExt0 = dzExt1 = dz0 - 1;
        }
      } else { //(0,0,0) is not one of the closest two tetrahedral vertices.
        uint8_t c = (uint8_t)(aPoint | bPoint); //Our two extra vertices are determined by the closest two.

        if ((c & 0x01) == 0) {
          xsvExt0 = xsb;
          xsvExt1 = xsb - 1;
          dxExt0 = dx0 - 2 * SquishConstant3D;
          dxExt1 = dx0 + 1 - SquishConstant3D;
        } else {
          xsvExt0 = xsvExt1 = xsb + 1;
          dxExt0 = dx0 - 1 - 2 * SquishConstant3D;
          dxExt1 = dx0 - 1 - SquishConstant3D;
        }

        if ((c & 0x02) == 0) {
          ysvExt0 = ysb;
          ysvExt1 = ysb - 1;
          dyExt0 = dy0 - 2 * SquishConstant3D;
          dyExt1 = dy0 + 1 - SquishConstant3D;
        } else {
          ysvExt0 = ysvExt1 = ysb + 1;
          dyExt0 = dy0 - 1 - 2 * SquishConstant3D;
          dyExt1 = dy0 - 1 - SquishConstant3D;
        }

        if ((c & 0x04) == 0) {
          zsvExt0 = zsb;
          zsvExt1 = zsb - 1;
          dzExt0 = dz0 - 2 * SquishConstant3D;
          dzExt1 = dz0 + 1 - SquishConstant3D;
        } else {
          zsvExt0 = zsvExt1 = zsb + 1;
          dzExt0 = dz0 - 1 - 2 * SquishConstant3D;
          dzExt1 = dz0 - 1 - SquishConstant3D;
        }
      }

      //Contribution (0,0,0)
      double attn0 = 2 - dx0 * dx0 - dy0 * dy0 - dz0 * dz0;

      if (attn0 > 0) {
        attn0 *= attn0;
        value += attn0 * attn0 * dot(at(xsb + 0, ysb + 0, zsb + 0), { dx0, dy0, dz0 });
      }

      //Contribution (1,0,0)
      double dx1 = dx0 - 1 - SquishConstant3D;
      double dy1 = dy0 - 0 - SquishConstant3D;
      double dz1 = dz0 - 0 - SquishConstant3D;
      double attn1 = 2 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;

      if (attn1 > 0) {
        attn1 *= attn1;
        value += attn1 * attn1 * dot(at(xsb + 1, ysb + 0, zsb + 0), { dx1, dy1, dz1 });
      }

      //Contribution (0,1,0)
      double dx2 = dx0 - 0 - SquishConstant3D;
      double dy2 = dy0 - 1 - SquishConstant3D;
      double dz2 = dz1;
      double attn2 = 2 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;

      if (attn2 > 0) {
        attn2 *= attn2;
        value += attn2 * attn2 * dot(at(xsb + 0, ysb + 1, zsb + 0), { dx2, dy2, dz2 });
      }

      //Contribution (0,0,1)
      double dx3 = dx2;
      double dy3 = dy1;
      double dz3 = dz0 - 1 - SquishConstant3D;
      double attn3 = 2 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;

      if (attn3 > 0) {
        attn3 *= attn3;
        value += attn3 * attn3 * dot(at(xsb + 0, ysb + 0, zsb + 1), { dx3, dy3, dz3 });
      }
    } else if (inSum >= 2) { //We're inside the tetrahedron (3-Simplex) at (1,1,1)

      //Determine which two tetrahedral vertices are the closest, out of (1,1,0), (1,0,1), (0,1,1) but not (1,1,1).
      uint8_t aPoint = 0x06;
      double aScore = xins;
      uint8_t bPoint = 0x05;
      double bScore = yins;

      if (aScore <= bScore && zins < bScore) {
        bScore = zins;
        bPoint = 0x03;
      } else if (aScore > bScore && zins < aScore) {
        aScore = zins;
        aPoint = 0x03;
      }

      //Now we determine the two lattice points not part of the tetrahedron that may contribute.
      //This depends on the closest two tetrahedral vertices, including (1,1,1)
      double wins = 3 - inSum;

      if (wins < aScore || wins < bScore) { //(1,1,1) is one of the closest two tetrahedral vertices.
        uint8_t c = (bScore < aScore ? bPoint : aPoint); //Our other closest vertex is the closest out of a and b.

        if ((c & 0x01) != 0) {
          xsvExt0 = xsb + 2;
          xsvExt1 = xsb + 1;
          dxExt0 = dx0 - 2 - 3 * SquishConstant3D;
          dxExt1 = dx0 - 1 - 3 * SquishConstant3D;
        } else {
          xsvExt0 = xsvExt1 = xsb;
          dxExt0 = dxExt1 = dx0 - 3 * SquishConstant3D;
        }

        if ((c & 0x02) != 0) {
          ysvExt0 = ysvExt1 = ysb + 1;
          dyExt0 = dyExt1 = dy0 - 1 - 3 * SquishConstant3D;

          if ((c & 0x01) != 0) {
            ysvExt1 += 1;
            dyExt1 -= 1;
          } else {
            ysvExt0 += 1;
            dyExt0 -= 1;
          }
        } else {
          ysvExt0 = ysvExt1 = ysb;
          dyExt0 = dyExt1 = dy0 - 3 * SquishConstant3D;
        }

        if ((c & 0x04) != 0) {
          zsvExt0 = zsb + 1;
          zsvExt1 = zsb + 2;
          dzExt0 = dz0 - 1 - 3 * SquishConstant3D;
          dzExt1 = dz0 - 2 - 3 * SquishConstant3D;
        } else {
          zsvExt0 = zsvExt1 = zsb;
          dzExt0 = dzExt1 = dz0 - 3 * SquishConstant3D;
        }
      } else { //(1,1,1) is not one of the closest two tetrahedral vertices.
        uint8_t c = (uint8_t)(aPoint & bPoint); //Our two extra vertices are determined by the closest two.

        if ((c & 0x01) != 0) {
          xsvExt0 = xsb + 1;
          xsvExt1 = xsb + 2;
          dxExt0 = dx0 - 1 - SquishConstant3D;
          dxExt1 = dx0 - 2 - 2 * SquishConstant3D;
        } else {
          xsvExt0 = xsvExt1 = xsb;
          dxExt0 = dx0 - SquishConstant3D;
          dxExt1 = dx0 - 2 * SquishConstant3D;
        }

        if ((c & 0x02) != 0) {
          ysvExt0 = ysb + 1;
          ysvExt1 = ysb + 2;
          dyExt0 = dy0 - 1 - SquishConstant3D;
          dyExt1 = dy0 - 2 - 2 * SquishConstant3D;
        } else {
          ysvExt0 = ysvExt1 = ysb;
          dyExt0 = dy0 - SquishConstant3D;
          dyExt1 = dy0 - 2 * SquishConstant3D;
        }

        if ((c & 0x04) != 0) {
          zsvExt0 = zsb + 1;
          zsvExt1 = zsb + 2;
          dzExt0 = dz0 - 1 - SquishConstant3D;
          dzExt1 = dz0 - 2 - 2 * SquishConstant3D;
        } else {
          zsvExt0 = zsvExt1 = zsb;
          dzExt0 = dz0 - SquishConstant3D;
          dzExt1 = dz0 - 2 * SquishConstant3D;
        }
      }

      //Contribution (1,1,0)
      double dx3 = dx0 - 1 - 2 * SquishConstant3D;
      double dy3 = dy0 - 1 - 2 * SquishConstant3D;
      double dz3 = dz0 - 0 - 2 * SquishConstant3D;
      double attn3 = 2 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;

      if (attn3 > 0) {
        attn3 *= attn3;
        value += attn3 * attn3 * dot(at(xsb + 1, ysb + 1, zsb + 0), { dx3, dy3, dz3 });
      }

      //Contribution (1,0,1)
      double dx2 = dx3;
      double dy2 = dy0 - 0 - 2 * SquishConstant3D;
      double dz2 = dz0 - 1 - 2 * SquishConstant3D;
      double attn2 = 2 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;

      if (attn2 > 0) {
        attn2 *= attn2;
        value += attn2 * attn2 * dot(at(xsb + 1, ysb + 0, zsb + 1), { dx2, dy2, dz2 });
      }

      //Contribution (0,1,1)
      double dx1 = dx0 - 0 - 2 * SquishConstant3D;
      double dy1 = dy3;
      double dz1 = dz2;
      double attn1 = 2 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;

      if (attn1 > 0) {
        attn1 *= attn1;
        value += attn1 * attn1 * dot(at(xsb + 0, ysb + 1, zsb + 1), { dx1, dy1, dz1 });
      }

      //Contribution (1,1,1)
      dx0 = dx0 - 1 - 3 * SquishConstant3D;
      dy0 = dy0 - 1 - 3 * SquishConstant3D;
      dz0 = dz0 - 1 - 3 * SquishConstant3D;
      double attn0 = 2 - dx0 * dx0 - dy0 * dy0 - dz0 * dz0;

      if (attn0 > 0) {
        attn0 *= attn0;
        value += attn0 * attn0 * dot(at(xsb + 1, ysb + 1, zsb + 1), { dx0, dy0, dz0 });
      }
    } else { //We're inside the octahedron (Rectified 3-Simplex) in between.
      double aScore;
      uint8_t aPoint;
      bool aIsFurtherSide;
      double bScore;
      uint8_t bPoint;
      bool bIsFurtherSide;

      //Decide between point (0,0,1) and (1,1,0) as closest
      double p1 = xins + yins;

      if (p1 > 1) {
        aScore = p1 - 1;
        aPoint = 0x03;
        aIsFurtherSide = true;
      } else {
        aScore = 1 - p1;
        aPoint = 0x04;
        aIsFurtherSide = false;
      }

      //Decide between point (0,1,0) and (1,0,1) as closest
      double p2 = xins + zins;

      if (p2 > 1) {
        bScore = p2 - 1;
        bPoint = 0x05;
        bIsFurtherSide = true;
      } else {
        bScore = 1 - p2;
        bPoint = 0x02;
        bIsFurtherSide = false;
      }

      //The closest out of the two (1,0,0) and (0,1,1) will replace the furthest out of the two decided above, if closer.
      double p3 = yins + zins;

      if (p3 > 1) {
        double score = p3 - 1;

        if (aScore <= bScore && aScore < score) {
          aScore = score;
          aPoint = 0x06;
          aIsFurtherSide = true;
        } else if (aScore > bScore && bScore < score) {
          bScore = score;
          bPoint = 0x06;
          bIsFurtherSide = true;
        }
      } else {
        double score = 1 - p3;

        if (aScore <= bScore && aScore < score) {
          aScore = score;
          aPoint = 0x01;
          aIsFurtherSide = false;
        } else if (aScore > bScore && bScore < score) {
          bScore = score;
          bPoint = 0x01;
          bIsFurtherSide = false;
        }
      }

      //Where each of the two closest points are determines how the extra two vertices are calculated.
      if (aIsFurtherSide == bIsFurtherSide) {
        if (aIsFurtherSide) { //Both closest points on (1,1,1) side

          //One of the two extra points is (1,1,1)
          dxExt0 = dx0 - 1 - 3 * SquishConstant3D;
          dyExt0 = dy0 - 1 - 3 * SquishConstant3D;
          dzExt0 = dz0 - 1 - 3 * SquishConstant3D;
          xsvExt0 = xsb + 1;
          ysvExt0 = ysb + 1;
          zsvExt0 = zsb + 1;

          //Other extra point is based on the shared axis.
          uint8_t c = (uint8_t)(aPoint & bPoint);

          if ((c & 0x01) != 0) {
            dxExt1 = dx0 - 2 - 2 * SquishConstant3D;
            dyExt1 = dy0 - 2 * SquishConstant3D;
            dzExt1 = dz0 - 2 * SquishConstant3D;
            xsvExt1 = xsb + 2;
            ysvExt1 = ysb;
            zsvExt1 = zsb;
          } else if ((c & 0x02) != 0) {
            dxExt1 = dx0 - 2 * SquishConstant3D;
            dyExt1 = dy0 - 2 - 2 * SquishConstant3D;
            dzExt1 = dz0 - 2 * SquishConstant3D;
            xsvExt1 = xsb;
            ysvExt1 = ysb + 2;
            zsvExt1 = zsb;
          } else {
            dxExt1 = dx0 - 2 * SquishConstant3D;
            dyExt1 = dy0 - 2 * SquishConstant3D;
            dzExt1 = dz0 - 2 - 2 * SquishConstant3D;
            xsvExt1 = xsb;
            ysvExt1 = ysb;
            zsvExt1 = zsb + 2;
          }
        } else {//Both closest points on (0,0,0) side

          //One of the two extra points is (0,0,0)
          dxExt0 = dx0;
          dyExt0 = dy0;
          dzExt0 = dz0;
          xsvExt0 = xsb;
          ysvExt0 = ysb;
          zsvExt0 = zsb;

          //Other extra point is based on the omitted axis.
          uint8_t c = (uint8_t)(aPoint | bPoint);

          if ((c & 0x01) == 0) {
            dxExt1 = dx0 + 1 - SquishConstant3D;
            dyExt1 = dy0 - 1 - SquishConstant3D;
            dzExt1 = dz0 - 1 - SquishConstant3D;
            xsvExt1 = xsb - 1;
            ysvExt1 = ysb + 1;
            zsvExt1 = zsb + 1;
          } else if ((c & 0x02) == 0) {
            dxExt1 = dx0 - 1 - SquishConstant3D;
            dyExt1 = dy0 + 1 - SquishConstant3D;
            dzExt1 = dz0 - 1 - SquishConstant3D;
            xsvExt1 = xsb + 1;
            ysvExt1 = ysb - 1;
            zsvExt1 = zsb + 1;
          } else {
            dxExt1 = dx0 - 1 - SquishConstant3D;
            dyExt1 = dy0 - 1 - SquishConstant3D;
            dzExt1 = dz0 + 1 - SquishConstant3D;
            xsvExt1 = xsb + 1;
            ysvExt1 = ysb + 1;
            zsvExt1 = zsb - 1;
          }
        }
      } else { //One point on (0,0,0) side, one point on (1,1,1) side
        uint8_t c1, c2;

        if (aIsFurtherSide) {
          c1 = aPoint;
          c2 = bPoint;
        } else {
          c1 = bPoint;
          c2 = aPoint;
        }

        //One contribution is a permutation of (1,1,-1)
        if ((c1 & 0x01) == 0) {
          dxExt0 = dx0 + 1 - SquishConstant3D;
          dyExt0 = dy0 - 1 - SquishConstant3D;
          dzExt0 = dz0 - 1 - SquishConstant3D;
          xsvExt0 = xsb - 1;
          ysvExt0 = ysb + 1;
          zsvExt0 = zsb + 1;
        } else if ((c1 & 0x02) == 0) {
          dxExt0 = dx0 - 1 - SquishConstant3D;
          dyExt0 = dy0 + 1 - SquishConstant3D;
          dzExt0 = dz0 - 1 - SquishConstant3D;
          xsvExt0 = xsb + 1;
          ysvExt0 = ysb - 1;
          zsvExt0 = zsb + 1;
        } else {
          dxExt0 = dx0 - 1 - SquishConstant3D;
          dyExt0 = dy0 - 1 - SquishConstant3D;
          dzExt0 = dz0 + 1 - SquishConstant3D;
          xsvExt0 = xsb + 1;
          ysvExt0 = ysb + 1;
          zsvExt0 = zsb - 1;
        }

        //One contribution is a permutation of (0,0,2)
        dxExt1 = dx0 - 2 * SquishConstant3D;
        dyExt1 = dy0 - 2 * SquishConstant3D;
        dzExt1 = dz0 - 2 * SquishConstant3D;
        xsvExt1 = xsb;
        ysvExt1 = ysb;
        zsvExt1 = zsb;

        if ((c2 & 0x01) != 0) {
          dxExt1 -= 2;
          xsvExt1 += 2;
        } else if ((c2 & 0x02) != 0) {
          dyExt1 -= 2;
          ysvExt1 += 2;
        } else {
          dzExt1 -= 2;
          zsvExt1 += 2;
        }
      }

      //Contribution (1,0,0)
      double dx1 = dx0 - 1 - SquishConstant3D;
      double dy1 = dy0 - 0 - SquishConstant3D;
      double dz1 = dz0 - 0 - SquishConstant3D;
      double attn1 = 2 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;

      if (attn1 > 0) {
        attn1 *= attn1;
        value += attn1 * attn1 * dot(at(xsb + 1, ysb + 0, zsb + 0), { dx1, dy1, dz1 });
      }

      //Contribution (0,1,0)
      double dx2 = dx0 - 0 - SquishConstant3D;
      double dy2 = dy0 - 1 - SquishConstant3D;
      double dz2 = dz1;
      double attn2 = 2 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;

      if (attn2 > 0) {
        attn2 *= attn2;
        value += attn2 * attn2 * dot(at(xsb + 0, ysb + 1, zsb + 0), { dx2, dy2, dz2 });
      }

      //Contribution (0,0,1)
      double dx3 = dx2;
      double dy3 = dy1;
      double dz3 = dz0 - 1 - SquishConstant3D;
      double attn3 = 2 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;

      if (attn3 > 0) {
        attn3 *= attn3;
        value += attn3 * attn3 * dot(at(xsb + 0, ysb + 0, zsb + 1), { dx3, dy3, dz3 });
      }

      //Contribution (1,1,0)
      double dx4 = dx0 - 1 - 2 * SquishConstant3D;
      double dy4 = dy0 - 1 - 2 * SquishConstant3D;
      double dz4 = dz0 - 0 - 2 * SquishConstant3D;
      double attn4 = 2 - dx4 * dx4 - dy4 * dy4 - dz4 * dz4;

      if (attn4 > 0) {
        attn4 *= attn4;
        value += attn4 * attn4 * dot(at(xsb + 1, ysb + 1, zsb + 0), { dx4, dy4, dz4 });
      }

      //Contribution (1,0,1)
      double dx5 = dx4;
      double dy5 = dy0 - 0 - 2 * SquishConstant3D;
      double dz5 = dz0 - 1 - 2 * SquishConstant3D;
      double attn5 = 2 - dx5 * dx5 - dy5 * dy5 - dz5 * dz5;

      if (attn5 > 0) {
        attn5 *= attn5;
        value += attn5 * attn5 * dot(at(xsb + 1, ysb + 0, zsb + 1), { dx5, dy5, dz5 });
      }

      //Contribution (0,1,1)
      double dx6 = dx0 - 0 - 2 * SquishConstant3D;
      double dy6 = dy4;
      double dz6 = dz5;
      double attn6 = 2 - dx6 * dx6 - dy6 * dy6 - dz6 * dz6;

      if (attn6 > 0) {
        attn6 *= attn6;
        value += attn6 * attn6 * dot(at(xsb + 0, ysb + 1, zsb + 1), { dx6, dy6, dz6 });
      }
    }

    //First extra vertex
    double attnExt0 = 2 - dxExt0 * dxExt0 - dyExt0 * dyExt0 - dzExt0 * dzExt0;

    if (attnExt0 > 0) {
      attnExt0 *= attnExt0;
      value += attnExt0 * attnExt0 * dot(at(xsvExt0, ysvExt0, zsvExt0), { dxExt0, dyExt0, dzExt0 });
    }

    //Second extra vertex
    double attnExt1 = 2 - dxExt1 * dxExt1 - dyExt1 * dyExt1 - dzExt1 * dzExt1;

    if (attnExt1 > 0) {
      attnExt1 *= attnExt1;
      value += attnExt1 * attnExt1 * dot(at(xsvExt1, ysvExt1, zsvExt1), { dxExt1, dyExt1, dzExt1 });
    }

    return value / NormConstant3D;
  }

  const Vector3d& OpenSimplexNoise3D::at(uint8_t i, uint8_t j, uint8_t k) const {
    static constexpr Vector3d gradients[24] = {
      { -11,   4,   4 },
      {  -4,  11,   4 },
      {  -4,   4,  11 },
      {  11,   4,   4 },
      {   4,  11,   4 },
      {   4,   4,  11 },
      { -11,  -4,   4 },
      {  -4, -11,   4 },
      {  -4,  -4,  11 },
      {  11,  -4,   4 },
      {   4, -11,   4 },
      {   4,  -4,  11 },
      { -11,   4,  -4 },
      {  -4,  11,  -4 },
      {  -4,   4, -11 },
      {  11,   4,  -4 },
      {   4,  11,  -4 },
      {   4,   4, -11 },
      { -11,  -4,  -4 },
      {  -4, -11,  -4 },
      {  -4,  -4, -11 },
      {  11,  -4,  -4 },
      {   4, -11,  -4 },
      {   4,  -4, -11 }
    };

    uint8_t index = i;
    index = m_perm.at(index) + j;
    index = m_perm.at(index) + k;
    return gradients[m_perm.at(index) % 24];
  }


  /*
   * Wavelet
   */

  static std::ptrdiff_t positiveMod(std::ptrdiff_t x, std::ptrdiff_t n) {
    std::ptrdiff_t r = x % n;
    return r < 0 ? r + n : r;
  }

  static void waveletDownsample(const double *from, double *to, std::ptrdiff_t n, std::ptrdiff_t stride) {
    static constexpr std::ptrdiff_t DownCoeffsCount = 16;
    static constexpr double DownCoeffs[2 * DownCoeffsCount] = {
      0.000334f, -0.001528f,  0.000410f,  0.003545f, -0.000938f, -0.008233f,  0.002172f,  0.019120f,
     -0.005040f, -0.044412f,  0.011655f,  0.103311f, -0.025936f, -0.243780f,  0.033979f,  0.655340f,
      0.655340f,  0.033979f, -0.243780f, -0.025936f,  0.103311f,  0.011655f, -0.044412f, -0.005040f,
      0.019120f,  0.002172f, -0.008233f, -0.000938f,  0.003546f,  0.000410f, -0.001528f,  0.000334f,
    };

    const double *coeffs = &DownCoeffs[DownCoeffsCount];

    for (std::ptrdiff_t i = 0; i < n/2; ++i) {
      double value = 0;

      for (std::ptrdiff_t k = 2 * i - DownCoeffsCount; k <=  2 * i - DownCoeffsCount; ++k) {
        std::ptrdiff_t index = k - 2 * i;
        assert(-DownCoeffsCount <= index && index < DownCoeffsCount);
        value += coeffs[index] * from[positiveMod(k, n) * stride];
      }

      to[i * stride] = value;
    }
  }

  static void waveletUpsample(const double *from, double *to, std::ptrdiff_t n, std::ptrdiff_t stride) {
    static constexpr std::ptrdiff_t UpCoeffsCount = 2;
    static constexpr double UpCoeff[2 * UpCoeffsCount] = {
      0.25, 0.75, 0.75, 0.25
    };

    const double *coeffs = &UpCoeff[UpCoeffsCount];

    for (std::ptrdiff_t i = 0; i < n; ++i) {
      double value = 0;

      for (std::ptrdiff_t k = i/2; k <= i/2 + 1; ++k) {
        std::ptrdiff_t index = i - 2 * k;
        assert(-UpCoeffsCount <= index && index < UpCoeffsCount);
        value += coeffs[index] * from[positiveMod(k, n/2) * stride];
      }

      to[i * stride] = value;
    }
  }

  WaveletNoise3D::WaveletNoise3D(Random& random, std::ptrdiff_t n)
  : m_n(n + n % 2)
  {
    std::size_t sz = m_n * m_n * m_n;
    m_data.resize(sz);

    std::vector<double> tmp1(sz);
    std::vector<double> tmp2(sz);

    // step 1: fill the tile with numbers in the range -1 to 1

    for (auto& value : m_data) {
      value = random.computeUniformFloat(-1.0, 1.0);
    }

    // step 2 and 3: downsample and upsample the tile

    for (std::ptrdiff_t iy = 0; iy < m_n; ++iy) {
      for (std::ptrdiff_t iz = 0; iz < m_n; ++iz) {
        // each x row
        std::ptrdiff_t i = iy * m_n + iz * m_n * m_n;
        waveletDownsample(&m_data[i], &tmp1[i], m_n, 1);
        waveletUpsample(&tmp1[i], &tmp2[i], m_n, 1);
      }
    }

    for (std::ptrdiff_t ix = 0; ix < m_n; ++ix) {
      for (std::ptrdiff_t iz = 0; iz < m_n; ++iz) {
        // each y row
        std::ptrdiff_t i = ix + iz * m_n * m_n;
        waveletDownsample(&tmp2[i], &tmp1[i], m_n, m_n);
        waveletUpsample(&tmp1[i], &tmp2[i], m_n, m_n);
      }
    }

    for (std::ptrdiff_t ix = 0; ix < m_n; ++ix) {
      for (std::ptrdiff_t iy = 0; iy < m_n; ++iy) {
        // each z row
        std::ptrdiff_t i = ix + iy * m_n;
        waveletDownsample(&tmp2[i], &tmp1[i], m_n, m_n * m_n);
        waveletUpsample(&tmp1[i], &tmp2[i], m_n, m_n * m_n);
      }
    }

    // step 4: substract out the coarse-scale contribution

    for (std::ptrdiff_t i = 0; i < m_n * m_n * m_n; ++i) {
      m_data[i] -= tmp2[i];
    }

    // avoid event/odd variance difference by adding off-offset version of noise to itself

    std::ptrdiff_t offset = m_n / 2;

    if (offset % 2 == 0) {
      ++offset;
    }

    std::ptrdiff_t k = 0;

    for (std::ptrdiff_t ix = 0; ix < m_n; ++ix) {
      for (std::ptrdiff_t iy = 0; iy < m_n; ++iy) {
        for (std::ptrdiff_t iz = 0; iz < m_n; ++iz) {
          std::ptrdiff_t index = positiveMod(ix + offset, m_n)
                               + positiveMod(iy + offset, m_n) * m_n
                               + positiveMod(iz + offset, m_n) * m_n * m_n;
          assert(0 <= index && index < m_n * m_n * m_n);
          tmp1[k++] = m_data[index];
        }
      }
    }

    for (std::ptrdiff_t i = 0; i < m_n * m_n * m_n; ++i) {
      m_data[i] += tmp1[i];
    }
  }

  double WaveletNoise3D::getValue(double x, double y, double z) {
    double p[3] = { x, y, z };

    std::ptrdiff_t f[3];
    std::ptrdiff_t c[3];
    std::ptrdiff_t mid[3];

    double w[3][3];
    double value = 0;

    // evaluate quadratic B-spline basis functions

    for (std::ptrdiff_t i = 0; i < 3; ++i) {
      mid[i] = std::ceil(p[i] - 0.5);
      double t = mid[i] - (p[i] - 0.5);
      w[i][0] = t * t / 2;
      w[i][2] = (1 - t) * (1 - t) / 2;
      w[i][1] = 1 - w[i][0] - w[i][2];
    }

    // evaluate noise by weighting noise coefficients by basis function values

    for (f[2] = -1; f[2] <= 1; ++f[2]) {
      for (f[1] = -1; f[1] <= 1; ++f[1]) {
        for (f[0] = -1; f[0] <= 1; ++f[0]) {
          double weight = 1;

          for (std::ptrdiff_t i = 0; i < 3; ++i) {
            c[i] = positiveMod(mid[i] + f[i], m_n);
            weight *= w[i][f[i] + 1];
          }

          value += weight * m_data[c[0] + c[1] * m_n + c[2] * m_n * m_n];
        }
      }
    }

    return value;
  }

  /*
   * Worley
   */

  WorleyNoise2D::WorleyNoise2D(Random& random, std::size_t count, Distance2<double> distance, std::vector<double> coeffs)
  : m_count(count)
  , m_distance(distance)
  , m_coeffs(std::move(coeffs))
  {
    // generate cells

    m_cells.reserve(m_count * 4);

    for (std::size_t i = 0; i < m_count; ++i) {
      auto x = random.computeUniformFloat(0.0, 1.0);
      auto y = random.computeUniformFloat(0.0, 1.0);

      m_cells.push_back({x, y});

      if (x < 0.5) {
        if (y < 0.5) {
          m_cells.push_back({x + 1.0, y      });
          m_cells.push_back({x      , y + 1.0});
          m_cells.push_back({x + 1.0, y + 1.0});
        } else {
          m_cells.push_back({x + 1.0, y      });
          m_cells.push_back({x      , y - 1.0});
          m_cells.push_back({x + 1.0, y - 1.0});
        }
      } else {
        if (y < 0.5) {
          m_cells.push_back({x - 1.0, y      });
          m_cells.push_back({x      , y + 1.0});
          m_cells.push_back({x - 1.0, y + 1.0});
        } else {
          m_cells.push_back({x - 1.0, y      });
          m_cells.push_back({x      , y - 1.0});
          m_cells.push_back({x - 1.0, y - 1.0});
        }
      }
    }

    // some sanity checks

    if (m_coeffs.empty()) {
      m_coeffs.push_back(1.0);
    }

    if (m_coeffs.size() > m_cells.size()) {
      m_coeffs.resize(m_cells.size());
    }

  }

  double WorleyNoise2D::getValue(double x, double y) {
    double rx = std::fmod(x, 1);
    double ry = std::fmod(y, 1);

    auto size = m_coeffs.size();

    Vector2d here{rx, ry};

    std::partial_sort(m_cells.begin(), m_cells.begin() + size, m_cells.end(), [&here, this](const Vector2d& lhs, const Vector2d& rhs) {
      return m_distance(here, lhs) < m_distance(here, rhs);
    });

    double value = 0.0;

    for (decltype(size) i = 0; i < size; ++i) {
      value += m_coeffs[i] * m_distance(here, m_cells[i]);
    }

    return value;
  }

  /*
   * Multifractal
   */

  Multifractal2D::Multifractal2D(Noise2D& noise, double scale, std::size_t octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {

  }

  double Multifractal2D::getValue(double x, double y) {
    double value = 1.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    /*
     * There seems to be an error in the original source code from
     * Musgrave. Blender source code provides an alternative implementation
     * that does not use the offset parameter. gf provides the Blender version.
     *
     * original: https://engineering.purdue.edu/~ebertd/texture/1stEdition/musgrave/musgrave.c
     * blender: https://developer.blender.org/diffusion/B/browse/master/source/blender/blenlib/intern/noise.c
     */

    for (std::size_t k = 0; k < m_octaves; ++k) {
      value *= m_noise.getValue(x * frequency, y * frequency) * std::pow(amplitude, m_dimension) + 1.0;

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * Hetero Terrain
   */

  HeteroTerrain2D::HeteroTerrain2D(Noise2D& noise, double scale, double offset, std::size_t octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_offset(offset)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {

  }

  double HeteroTerrain2D::getValue(double x, double y) {
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    double value = m_offset + m_noise(x, y);

    frequency *= m_lacunarity;
    amplitude *= m_persistence;

    for (std::size_t k = 1; k < m_octaves; ++k) {
      auto increment = m_noise.getValue(x * frequency, y * frequency) + m_offset;
      increment *= std::pow(amplitude, m_dimension);
      increment *= value;
      value += increment;

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }

  /*
   * Hybrid Multifractal
   */

  HybridMultifractal2D::HybridMultifractal2D(Noise2D& noise, double scale, double offset, std::size_t octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_offset(offset)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {

  }

  double HybridMultifractal2D::getValue(double x, double y) {
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    double value = m_noise(x, y) + m_offset;
    double weight = value;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;

    for (std::size_t k = 1; k < m_octaves; ++k) {
      if (weight > 1.0) {
        weight = 1.0;
      }

      double signal = (m_noise.getValue(x * frequency, y * frequency) + m_offset) * std::pow(amplitude, m_dimension);
      value += weight * signal;
      weight *= signal;

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }


  /*
   * Ridged Multifractal
   */

  RidgedMultifractal2D::RidgedMultifractal2D(Noise2D& noise, double scale, double offset, double gain, std::size_t octaves, double lacunarity, double persistence, double dimension)
  : m_noise(noise)
  , m_scale(scale)
  , m_offset(offset)
  , m_gain(gain)
  , m_octaves(octaves)
  , m_lacunarity(lacunarity)
  , m_persistence(persistence)
  , m_dimension(dimension)
  {

  }

  double RidgedMultifractal2D::getValue(double x, double y) {
    double value;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    double signal = m_noise(x, y);
    signal = std::abs(signal);
    signal = m_offset - signal;
    signal *= signal;

    value = signal;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;

    for (std::size_t k = 1; k < m_octaves; ++k) {
      double weight = signal * m_gain;
      weight = gf::clamp(weight, 0.0, 1.0);

      signal = m_noise.getValue(x * frequency, y * frequency);
      signal = std::abs(signal);
      signal = m_offset - signal;
      signal *= signal;

      signal *= weight;
      value += signal * std::pow(amplitude, m_dimension);

      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }


  Noise3DTo2DAdapter::Noise3DTo2DAdapter(Noise3D& noise, Vector3d normal, Vector3d point)
  : m_noise(noise)
  , m_normal(normal)
  , m_point(point)
  {

  }

  double Noise3DTo2DAdapter::getValue(double x, double y) {
    double z = 0.0;

    if (std::abs(m_normal.z) > gf::Epsilon) {
       z = m_point.z + (m_normal.x * (m_point.x - x) + m_normal.y * (m_point.y - y)) / m_normal.z;
    }

    return m_noise(x, y, z);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
