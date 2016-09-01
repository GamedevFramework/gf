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
#include <gf/Noise.h>

#include <cassert>
#include <cmath>
#include <algorithm>
#include <random>

#include <gf/Math.h>
#include <gf/Random.h>

namespace gf {
inline namespace v1 {
  Noise::~Noise() {

  }

  static void generatePermutation(Random& random, std::array<uint8_t, 256>& perm) {
    // initialize permutation
    for (int i = 0; i < 256; ++i) {
      perm[i] = static_cast<uint8_t>(i);
    }

    // generate permutation
    std::uniform_int_distribution<std::size_t> dist(0, 255);

    for (unsigned i = 0; i < 2560; ++i) {
      auto j = dist(random.getEngine());
      auto k = dist(random.getEngine());
      std::swap(perm[j], perm[k]);
    }
  }

  GradientNoise::GradientNoise(Random& random, Step<double> step)
  : m_step(step)
  {
    // generate gradients
    std::uniform_real_distribution<double> distGradient(0.0f, 2.0f * Pi);

    for (auto& vec : m_gradients) {
      double angle = distGradient(random.getEngine());
      vec = gf::unit(angle);
    }

    // initialize permutation
    generatePermutation(random, m_perm);
  }

  double GradientNoise::getValue(double x, double y) {
    uint8_t qx = static_cast<uint8_t>(std::fmod(x, 256));
    double rx = std::fmod(x, 1);
    assert(rx >= 0.0 && rx <= 1.0);

    uint8_t qy = static_cast<uint8_t>(std::fmod(y, 256));
    double ry = std::fmod(y, 1);
    assert(ry >= 0.0 && ry <= 1.0);

    double nw = dot(at(qx    , qy    ), {rx       , ry      });
    double ne = dot(at(qx + 1, qy    ), {rx - 1.0f, ry      });
    double sw = dot(at(qx    , qy + 1), {rx       , ry - 1.0f});
    double se = dot(at(qx + 1, qy + 1), {rx - 1.0f, ry - 1.0f});

    double n = lerp(nw, ne, m_step(rx));
    double s = lerp(sw, se, m_step(rx));

    return lerp(n, s, m_step(ry));
  }

  const Vector2d& GradientNoise::at(uint8_t i, uint8_t j) const {
    uint8_t index = i + m_perm.at(j);
    return m_gradients.at(index);
  }


  double FractalNoise::getValue(double x, double y) {
    double value = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;

    x *= m_scale;
    y *= m_scale;

    for (std::size_t k = 0; k < m_octaves; ++k) {
      value += m_noise(x * frequency, y * frequency) * std::pow(amplitude, m_dimension);
      frequency *= m_lacunarity;
      amplitude *= m_persistence;
    }

    return value;
  }



  PerlinNoise::PerlinNoise(Random& random, double scale, std::size_t octaves)
  : m_gradient(random, gf::cubicStep)
  , m_fractal(m_gradient, scale, octaves)
  {

  }

  double PerlinNoise::getValue(double x, double y) {
    return m_fractal(x, y);
  }


  SimplexNoise::SimplexNoise(Random& random)
  {
    generatePermutation(random, m_perm);
  }


  double SimplexNoise::getValue(double x, double y) {
    static constexpr double K = .366025403784438646763723170752; // (sqrt(3) - 1) / 2
    static constexpr double C = .211324865405187117745425609748; // K / (1 + 2 * K)

    double s = (x + y) * K;
    double i = x + s;
    double j = y + s;

    i = std::floor(i);
    j = std::floor(j);

    double t = (i + j) * C;
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

    double x1 = x0 - i1 + C;
    double y1 = y0 - j1 + C;

    double x2 = x0 - 1 + 2 * C;
    double y2 = y0 - 1 + 2 * C;

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

  const Vector2d& SimplexNoise::at(uint8_t i, uint8_t j) const {
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

    uint8_t index = i + m_perm.at(j);
    return gradients[index % 8];
  }


  OpenSimplexNoise::OpenSimplexNoise(Random& random)
  {
    generatePermutation(random, m_perm);
  }

  double OpenSimplexNoise::getValue(double x, double y) {
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

  const Vector2d& OpenSimplexNoise::at(uint8_t i, uint8_t j) const {
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

    uint8_t index = m_perm.at(static_cast<uint8_t>(i + m_perm.at(j)));
    return gradients[index % 8];
  }


  WorleyNoise::WorleyNoise(Random& random, std::size_t count, Distance2<double> distance, std::vector<double> coeffs)
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

  double WorleyNoise::getValue(double x, double y) {
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

}
}
