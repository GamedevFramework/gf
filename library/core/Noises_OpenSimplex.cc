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
#include <gf/Noises.h>

#include <cassert>
#include <algorithm>
#include <numeric>

#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/Span.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

#define DUP(x) x, x

  namespace {

    constexpr std::size_t PSize = 2048;
    constexpr std::size_t PMask = PSize - 1;

    constexpr
    int fastFloor(double val) {
      int ival = static_cast<int>(val);
      return val < ival ? ival - 1 : ival;
    }

    void generatePermutation2048(Random& random, std::array<uint16_t, 2048>& perm) {
      std::iota(perm.begin(), perm.end(), 0);
      std::shuffle(perm.begin(), perm.end(), random.getEngine());
    }

  }

  /*
   * OpenSimplexNoise2D
   */

  namespace {

    struct LatticePoint2D {
      constexpr LatticePoint2D(int x, int y)
      : xsv(x)
      , ysv(y)
      , dx(0.0)
      , dy(0.0)
      {
        double ssv = (xsv + ysv) * -0.211324865405187;
        dx = -xsv - ssv;
        dy = -ysv - ssv;
      }

      int xsv, ysv;
      double dx, dy;
    };

    constexpr std::array<LatticePoint2D, 8 * 4> computeLookup2D_S() {
      std::array<LatticePoint2D, 8 * 4> result = {
        DUP(DUP(DUP(DUP(DUP(LatticePoint2D(0, 0))))))
      };

      for (int i = 0; i < 8; i++) {
        int i1 = 0;
        int j1 = 0;
        int i2 = 0;
        int j2 = 0;

        if ((i & 1) == 0) {
          if ((i & 2) == 0) { i1 = -1; j1 = 0; } else { i1 = 1; j1 = 0; }
          if ((i & 4) == 0) { i2 = 0; j2 = -1; } else { i2 = 0; j2 = 1; }
        } else {
          if ((i & 2) != 0) { i1 = 2; j1 = 1; } else { i1 = 0; j1 = 1; }
          if ((i & 4) != 0) { i2 = 1; j2 = 2; } else { i2 = 1; j2 = 0; }
        }

        result[i * 4 + 0] = LatticePoint2D(0, 0);
        result[i * 4 + 1] = LatticePoint2D(1, 1);
        result[i * 4 + 2] = LatticePoint2D(i1, j1);
        result[i * 4 + 3] = LatticePoint2D(i2, j2);
      }

      return result;
    }

    constexpr std::array<Vector2d, PSize> computeGradients2D(double N2) {
      std::array<Vector2d, PSize> result = {
        DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(gf::vec(0.0, 0.0))))))))))))
      };

      std::array<Vector2d, 24> grad2 = {
        gf::vec( 0.130526192220052,  0.99144486137381),
        gf::vec( 0.38268343236509,   0.923879532511287),
        gf::vec( 0.608761429008721,  0.793353340291235),
        gf::vec( 0.793353340291235,  0.608761429008721),
        gf::vec( 0.923879532511287,  0.38268343236509),
        gf::vec( 0.99144486137381,   0.130526192220051),
        gf::vec( 0.99144486137381,  -0.130526192220051),
        gf::vec( 0.923879532511287, -0.38268343236509),
        gf::vec( 0.793353340291235, -0.60876142900872),
        gf::vec( 0.608761429008721, -0.793353340291235),
        gf::vec( 0.38268343236509,  -0.923879532511287),
        gf::vec( 0.130526192220052, -0.99144486137381),
        gf::vec(-0.130526192220052, -0.99144486137381),
        gf::vec(-0.38268343236509,  -0.923879532511287),
        gf::vec(-0.608761429008721, -0.793353340291235),
        gf::vec(-0.793353340291235, -0.608761429008721),
        gf::vec(-0.923879532511287, -0.38268343236509),
        gf::vec(-0.99144486137381,  -0.130526192220052),
        gf::vec(-0.99144486137381,   0.130526192220051),
        gf::vec(-0.923879532511287,  0.38268343236509),
        gf::vec(-0.793353340291235,  0.608761429008721),
        gf::vec(-0.608761429008721,  0.793353340291235),
        gf::vec(-0.38268343236509,   0.923879532511287),
        gf::vec(-0.130526192220052,  0.99144486137381),
      };

      for (auto & v : grad2) {
        v /= N2;
      }

      for (std::size_t i = 0; i < result.size(); ++i) {
        result[i] = grad2[i % grad2.size()];
      }

      return result;
    }

    struct OpenSimplex2DConfig {
      Span<const LatticePoint2D> lookup;
      Span<const Vector2d> gradients;
      std::size_t points = 0;
      double attn = 0.0;
    };

    constexpr std::array<LatticePoint2D, 8 * 4> Lookup2D_S = computeLookup2D_S();
    constexpr double N2_S = 0.05481866495625118;
    constexpr std::array<Vector2d, PSize> Gradients2D_S = computeGradients2D(N2_S);

    constexpr OpenSimplex2DConfig Config2D_S = {
      gf::span(Lookup2D_S.data(), Lookup2D_S.size()),
      gf::span(Gradients2D_S.data(), Gradients2D_S.size()),
      4,
      2.0 / 3.0
    };

    constexpr std::array<LatticePoint2D, 4> Lookup2D_F = {
      LatticePoint2D(1, 0), LatticePoint2D(0, 0), LatticePoint2D(1, 1), LatticePoint2D(0, 1),
    };
    constexpr double N2_F = 0.01001634121365712;
    constexpr std::array<Vector2d, PSize> Gradients2D_F = computeGradients2D(N2_F);

    constexpr OpenSimplex2DConfig Config2D_F = {
      gf::span(Lookup2D_F.data(), Lookup2D_F.size()),
      gf::span(Gradients2D_F.data(), Gradients2D_F.size()),
      3,
      0.5
    };

    constexpr OpenSimplex2DConfig getConfig2D(OpenSimplexType type) {
      switch (type) {
        case OpenSimplexType::Super:
          return Config2D_S;
        case OpenSimplexType::Fast:
          return Config2D_F;
      }

      assert(false);
      return OpenSimplex2DConfig{};
    }

  }

  OpenSimplexNoise2D::OpenSimplexNoise2D(Random& random, OpenSimplexType type, OpenSimplex2DVariant variant)
  : m_type(type)
  , m_variant(variant)
  {
    auto config = getConfig2D(m_type);
    generatePermutation2048(random, m_perm);

    for (std::size_t i = 0; i < m_perm.size(); ++i) {
      m_gradients2D[i] = config.gradients[m_perm[i]];
    }
  }

  double OpenSimplexNoise2D::getValue(double x, double y) {
    switch (m_variant) {
      case OpenSimplex2DVariant::Classic: {
        double s = 0.366025403784439 * (x + y);
        double xs = x + s;
        double ys = y + s;
        return getValueBase(xs, ys);
      }

      case OpenSimplex2DVariant::XBeforeY: {
        double xx = x * 0.7071067811865476;
        double yy = y * 1.224744871380249;
        return getValueBase(yy + xx, yy - xx);
      }
    }

    return 0.0;
  }

  double OpenSimplexNoise2D::getValueBase(double xs, double ys) const {
    auto config = getConfig2D(m_type);
    double value = 0;

    // Get base points and offsets
    int xsb = fastFloor(xs);
    int ysb = fastFloor(ys);
    double xsi = xs - xsb;
    double ysi = ys - ysb;

    // Index to point list
    std::size_t index = 0;

    switch (m_type) {
      case OpenSimplexType::Super: {
        std::size_t a = static_cast<std::size_t>(xsi + ysi);
        index = (a << 2) |
          static_cast<std::size_t>(xsi - ysi / 2 + 1 - a / 2.0) << 3 |
          static_cast<std::size_t>(ysi - xsi / 2 + 1 - a / 2.0) << 4;
        break;
      }

      case OpenSimplexType::Fast:
        index = static_cast<std::size_t>((ysi - xsi) / 2 + 1);
        break;
    }

    double ssi = (xsi + ysi) * -0.211324865405187;
    double xi = xsi + ssi;
    double yi = ysi + ssi;

    // Point contributions
    for (std::size_t i = 0; i < config.points; i++) {
      assert(index + i < config.lookup.getSize());
      LatticePoint2D c = config.lookup[index + i];

      double dx = xi + c.dx;
      double dy = yi + c.dy;
      double attn = config.attn - dx * dx - dy * dy;

      if (attn <= 0) {
        continue;
      }

      std::size_t pxm = (xsb + c.xsv) & PMask;
      std::size_t pym = (ysb + c.ysv) & PMask;
      Vector2d grad = m_gradients2D[m_perm[pxm] ^ pym];
      double extrapolation = grad.x * dx + grad.y * dy;

      attn *= attn;
      value += attn * attn * extrapolation;
    }

    return value;
  }


  /*
   * OpenSimplexNoise3D
   */

  namespace {

    struct LatticePoint3D {
      constexpr LatticePoint3D(int x, int y, int z, int lattice)
      : xrv(x + lattice * 1024)
      , yrv(y + lattice * 1024)
      , zrv(z + lattice * 1024)
      , dxr(-x + lattice * 0.5)
      , dyr(-y + lattice * 0.5)
      , dzr(-z + lattice * 0.5)
      , nextOnFailure(-1)
      , nextOnSuccess(-1)
      {
      }

      int xrv, yrv, zrv;
      double dxr, dyr, dzr;
      std::ptrdiff_t nextOnFailure, nextOnSuccess;
    };

    constexpr std::array<LatticePoint3D, 14> computeLookup3DSequence_S(int i) {
      int i1 = (i >> 0) & 1;
      int j1 = (i >> 1) & 1;
      int k1 = (i >> 2) & 1;
      int i2 = i1 ^ 1;
      int j2 = j1 ^ 1;
      int k2 = k1 ^ 1;

      std::array<LatticePoint3D, 14> result = {
        LatticePoint3D(i1, j1, k1, 0),
        LatticePoint3D(i1 + i2, j1 + j2, k1 + k2, 1),

        LatticePoint3D(i1 ^ 1, j1, k1, 0),
        LatticePoint3D(i1, j1 ^ 1, k1 ^ 1, 0),

        LatticePoint3D(i1 + (i2 ^ 1), j1 + j2, k1 + k2, 1),
        LatticePoint3D(i1 + i2, j1 + (j2 ^ 1), k1 + (k2 ^ 1), 1),

        LatticePoint3D(i1, j1 ^ 1, k1, 0),
        LatticePoint3D(i1 ^ 1, j1, k1 ^ 1, 0),

        LatticePoint3D(i1 + i2, j1 + (j2 ^ 1), k1 + k2, 1),
        LatticePoint3D(i1 + (i2 ^ 1), j1 + j2, k1 + (k2 ^ 1), 1),

        LatticePoint3D(i1, j1, k1 ^ 1, 0),
        LatticePoint3D(i1 ^ 1, j1 ^ 1, k1, 0),

        LatticePoint3D(i1 + i2, j1 + j2, k1 + (k2 ^ 1), 1),
        LatticePoint3D(i1 + (i2 ^ 1), j1 + (j2 ^ 1), k1 + k2, 1),
      };

      result[0].nextOnFailure = result[0].nextOnSuccess = 1;
      result[1].nextOnFailure = result[1].nextOnSuccess = 2;

      result[2].nextOnFailure = 3; result[2].nextOnSuccess = 5;
      result[3].nextOnFailure = 4; result[3].nextOnSuccess = 4;

      result[4].nextOnFailure = 5; result[4].nextOnSuccess = 6;
      result[5].nextOnFailure = result[5].nextOnSuccess = 6;

      result[6].nextOnFailure = 7; result[6].nextOnSuccess = 9;
      result[7].nextOnFailure = 8; result[7].nextOnSuccess = 8;

      result[8].nextOnFailure = 9; result[8].nextOnSuccess = 10;
      result[9].nextOnFailure = result[9].nextOnSuccess = 10;

      result[10].nextOnFailure = 11; result[10].nextOnSuccess = 13;
      result[11].nextOnFailure = 12; result[11].nextOnSuccess = 12;

      result[12].nextOnFailure = 13; result[12].nextOnSuccess = -1;
      result[13].nextOnFailure = result[13].nextOnSuccess = -1;

      return result;
    }

    constexpr std::array<LatticePoint3D, 8> computeLookup3DSequence_F(int i) {
      int i1 = (i >> 0) & 1;
      int j1 = (i >> 1) & 1;
      int k1 = (i >> 2) & 1;
      int i2 = i1 ^ 1;
      int j2 = j1 ^ 1;
      int k2 = k1 ^ 1;

      std::array<LatticePoint3D, 8> result = {
        LatticePoint3D(i1, j1, k1, 0),
        LatticePoint3D(i1 + i2, j1 + j2, k1 + k2, 0),

        LatticePoint3D(i1 ^ 1, j1, k1, 0),
        LatticePoint3D(i1, j1 ^ 1, k1, 0),
        LatticePoint3D(i1, j1, k1 ^ 1, 0),

        LatticePoint3D(i1 + (i2 ^ 1), j1 + j2, k1 + k2, 1),
        LatticePoint3D(i1 + i2, j1 + (j2 ^ 1), k1 + k2, 1),
        LatticePoint3D(i1 + i2, j1 + j2, k1 + (k2 ^ 1), 1),
      };

      result[0].nextOnFailure = result[0].nextOnSuccess = 1;
      result[1].nextOnFailure = result[1].nextOnSuccess = 2;

      result[2].nextOnFailure = 3; result[2].nextOnSuccess = 6;
      result[3].nextOnFailure = 4; result[3].nextOnSuccess = 5;
      result[4].nextOnFailure = result[4].nextOnSuccess = 5;

      result[5].nextOnFailure = 6; result[5].nextOnSuccess = -1;
      result[6].nextOnFailure = 7; result[6].nextOnSuccess = -1;
      result[7].nextOnFailure = result[7].nextOnSuccess = -1;

      return result;
    }

    constexpr std::array<Vector3d, PSize> computeGradients3D(double N3) {
      std::array<Vector3d, PSize> result = {
        DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(DUP(gf::vec(0.0, 0.0, 0.0))))))))))))
      };

      std::array<Vector3d, 48> grad3 = {
        gf::vec(-2.22474487139,      -2.22474487139,      -1.0),
        gf::vec(-2.22474487139,      -2.22474487139,       1.0),
        gf::vec(-3.0862664687972017, -1.1721513422464978,  0.0),
        gf::vec(-1.1721513422464978, -3.0862664687972017,  0.0),
        gf::vec(-2.22474487139,      -1.0,                -2.22474487139),
        gf::vec(-2.22474487139,       1.0,                -2.22474487139),
        gf::vec(-1.1721513422464978,  0.0,                -3.0862664687972017),
        gf::vec(-3.0862664687972017,  0.0,                -1.1721513422464978),
        gf::vec(-2.22474487139,      -1.0,                 2.22474487139),
        gf::vec(-2.22474487139,       1.0,                 2.22474487139),
        gf::vec(-3.0862664687972017,  0.0,                 1.1721513422464978),
        gf::vec(-1.1721513422464978,  0.0,                 3.0862664687972017),
        gf::vec(-2.22474487139,       2.22474487139,      -1.0),
        gf::vec(-2.22474487139,       2.22474487139,       1.0),
        gf::vec(-1.1721513422464978,  3.0862664687972017,  0.0),
        gf::vec(-3.0862664687972017,  1.1721513422464978,  0.0),
        gf::vec(-1.0,                -2.22474487139,      -2.22474487139),
        gf::vec( 1.0,                -2.22474487139,      -2.22474487139),
        gf::vec( 0.0,                -3.0862664687972017, -1.1721513422464978),
        gf::vec( 0.0,                -1.1721513422464978, -3.0862664687972017),
        gf::vec(-1.0,                -2.22474487139,       2.22474487139),
        gf::vec( 1.0,                -2.22474487139,       2.22474487139),
        gf::vec( 0.0,                -1.1721513422464978,  3.0862664687972017),
        gf::vec( 0.0,                -3.0862664687972017,  1.1721513422464978),
        gf::vec(-1.0,                 2.22474487139,      -2.22474487139),
        gf::vec( 1.0,                 2.22474487139,      -2.22474487139),
        gf::vec( 0.0,                 1.1721513422464978, -3.0862664687972017),
        gf::vec( 0.0,                 3.0862664687972017, -1.1721513422464978),
        gf::vec(-1.0,                 2.22474487139,       2.22474487139),
        gf::vec( 1.0,                 2.22474487139,       2.22474487139),
        gf::vec( 0.0,                 3.0862664687972017,  1.1721513422464978),
        gf::vec( 0.0,                 1.1721513422464978,  3.0862664687972017),
        gf::vec( 2.22474487139,      -2.22474487139,      -1.0),
        gf::vec( 2.22474487139,      -2.22474487139,       1.0),
        gf::vec( 1.1721513422464978, -3.0862664687972017,  0.0),
        gf::vec( 3.0862664687972017, -1.1721513422464978,  0.0),
        gf::vec( 2.22474487139,      -1.0,                -2.22474487139),
        gf::vec( 2.22474487139,       1.0,                -2.22474487139),
        gf::vec( 3.0862664687972017,  0.0,                -1.1721513422464978),
        gf::vec( 1.1721513422464978,  0.0,                -3.0862664687972017),
        gf::vec( 2.22474487139,      -1.0,                 2.22474487139),
        gf::vec( 2.22474487139,       1.0,                 2.22474487139),
        gf::vec( 1.1721513422464978,  0.0,                 3.0862664687972017),
        gf::vec( 3.0862664687972017,  0.0,                 1.1721513422464978),
        gf::vec( 2.22474487139,       2.22474487139,      -1.0),
        gf::vec( 2.22474487139,       2.22474487139,       1.0),
        gf::vec( 3.0862664687972017,  1.1721513422464978,  0.0),
        gf::vec( 1.1721513422464978,  3.0862664687972017,  0.0)
      };

      for (auto & v : grad3) {
        v /= N3;
      }

      for (std::size_t i = 0; i < result.size(); ++i) {
        result[i] = grad3[i % grad3.size()];
      }

      return result;
    }


    struct OpenSimplex3DConfig {
      std::array<Span<const LatticePoint3D>, 8> lookup;
      Span<const Vector3d> gradients;
      double attn = 0.0;
    };

    constexpr std::array<std::array<LatticePoint3D, 14>, 8> Lookup3D_S = {
      computeLookup3DSequence_S(0),
      computeLookup3DSequence_S(1),
      computeLookup3DSequence_S(2),
      computeLookup3DSequence_S(3),
      computeLookup3DSequence_S(4),
      computeLookup3DSequence_S(5),
      computeLookup3DSequence_S(6),
      computeLookup3DSequence_S(7),
    };
    constexpr double N3_S = 0.2781926117527186;
    constexpr std::array<Vector3d, PSize> Gradients3D_S = computeGradients3D(N3_S);

    constexpr OpenSimplex3DConfig Config3D_S = {
      {
        gf::span(Lookup3D_S[0].data(), Lookup3D_S[0].size()),
        gf::span(Lookup3D_S[1].data(), Lookup3D_S[1].size()),
        gf::span(Lookup3D_S[2].data(), Lookup3D_S[2].size()),
        gf::span(Lookup3D_S[3].data(), Lookup3D_S[3].size()),
        gf::span(Lookup3D_S[4].data(), Lookup3D_S[4].size()),
        gf::span(Lookup3D_S[5].data(), Lookup3D_S[5].size()),
        gf::span(Lookup3D_S[6].data(), Lookup3D_S[6].size()),
        gf::span(Lookup3D_S[7].data(), Lookup3D_S[7].size()),
      },
      gf::span(Gradients3D_S.data(), Gradients3D_S.size()),
      0.75
    };

    constexpr std::array<std::array<LatticePoint3D, 8>, 8> Lookup3D_F = {
      computeLookup3DSequence_F(0),
      computeLookup3DSequence_F(1),
      computeLookup3DSequence_F(2),
      computeLookup3DSequence_F(3),
      computeLookup3DSequence_F(4),
      computeLookup3DSequence_F(5),
      computeLookup3DSequence_F(6),
      computeLookup3DSequence_F(7),
    };
    constexpr double N3_F = 0.030485933181293584;
    constexpr std::array<Vector3d, PSize> Gradients3D_F = computeGradients3D(N3_F);

    constexpr OpenSimplex3DConfig Config3D_F = {
      {
        gf::span(Lookup3D_F[0].data(), Lookup3D_F[0].size()),
        gf::span(Lookup3D_F[1].data(), Lookup3D_F[1].size()),
        gf::span(Lookup3D_F[2].data(), Lookup3D_F[2].size()),
        gf::span(Lookup3D_F[3].data(), Lookup3D_F[3].size()),
        gf::span(Lookup3D_F[4].data(), Lookup3D_F[4].size()),
        gf::span(Lookup3D_F[5].data(), Lookup3D_F[5].size()),
        gf::span(Lookup3D_F[6].data(), Lookup3D_F[6].size()),
        gf::span(Lookup3D_F[7].data(), Lookup3D_F[7].size()),
      },
      gf::span(Gradients3D_F.data(), Gradients3D_F.size()),
      0.5
    };

    constexpr OpenSimplex3DConfig getConfig3D(OpenSimplexType type) {
      switch (type) {
        case OpenSimplexType::Super:
          return Config3D_S;
        case OpenSimplexType::Fast:
          return Config3D_F;
      }

      assert(false);
      return OpenSimplex3DConfig{};
    }

  }


  OpenSimplexNoise3D::OpenSimplexNoise3D(Random& random, OpenSimplexType type, OpenSimplex3DVariant variant)
  : m_type(type)
  , m_variant(variant)
  {
    auto config = getConfig3D(m_type);
    generatePermutation2048(random, m_perm);

    for (std::size_t i = 0; i < m_perm.size(); ++i) {
      m_gradients3D[i] = config.gradients[m_perm[i]];
    }
  }

  double OpenSimplexNoise3D::getValue(double x, double y, double z) {
    switch (m_variant) {
      case OpenSimplex3DVariant::Classic: {
        double r = (2.0 / 3.0) * (x + y + z);
        double xr = r - x;
        double yr = r - y;
        double zr = r - z;
        return getValueBase(xr, yr, zr);
      }

      case OpenSimplex3DVariant::XYBeforeZ: {
        double xy = x + y;
        double s2 = xy * -0.211324865405187;
        double zz = z * 0.577350269189626;
        double xr = x + s2 - zz;
        double yr = y + s2 - zz;
        double zr = xy * 0.577350269189626 + zz;
        return getValueBase(xr, yr, zr);
      }

      case OpenSimplex3DVariant::XZBeforeY: {
        double xz = x + z;
        double s2 = xz * -0.211324865405187;
        double yy = y * 0.577350269189626;
        double xr = x + s2 - yy;
        double zr = z + s2 - yy;
        double yr = xz * 0.577350269189626 + yy;
        return getValueBase(xr, yr, zr);
      }
    }

    assert(false);
    return 0.0;
  }

  double OpenSimplexNoise3D::getValueBase(double xr, double yr, double zr) const {
    auto config = getConfig3D(m_type);

    int xrb = fastFloor(xr);
    int yrb = fastFloor(yr);
    int zrb = fastFloor(zr);
    double xri = xr - xrb;
    double yri = yr - yrb;
    double zri = zr - zrb;

    std::size_t xht = static_cast<std::size_t>(xri + 0.5);
    std::size_t yht = static_cast<std::size_t>(yri + 0.5);
    std::size_t zht = static_cast<std::size_t>(zri + 0.5);
    std::size_t index = (xht << 0) | (yht << 1) | (zht << 2);

    double value = 0.0;
    ptrdiff_t ci = 0;

    while (ci != -1) {
      LatticePoint3D c = config.lookup[index][ci];

      double dxr = xri + c.dxr;
      double dyr = yri + c.dyr;
      double dzr = zri + c.dzr;
      double attn = config.attn - dxr * dxr - dyr * dyr - dzr * dzr;

      if (attn < 0) {
        ci = c.nextOnFailure;
      } else {
        int pxm = (xrb + c.xrv) & PMask;
        int pym = (yrb + c.yrv) & PMask;
        int pzm = (zrb + c.zrv) & PMask;
        Vector3d grad = m_gradients3D[m_perm[m_perm[pxm] ^ pym] ^ pzm];
        double extrapolation = grad.x * dxr + grad.y * dyr + grad.z * dzr;

        attn *= attn;
        value += attn * attn * extrapolation;
        ci = c.nextOnSuccess;
      }
    }

    return value;
  }


#undef DUP


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
