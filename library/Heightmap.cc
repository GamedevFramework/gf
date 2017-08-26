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
#include <gf/Heightmap.h>

#include <algorithm>

#include <gf/Color.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Heightmap::Heightmap(Vector2i size)
  : m_data(size, 0.0)
  {

  }

  void Heightmap::reset() {
    std::fill(m_data.begin(), m_data.end(), 0.0);
  }

  std::pair<double, double> Heightmap::getMinMax() const {
    auto p = std::minmax_element(m_data.begin(), m_data.end());
    return { *p.first, *p.second };
  }

  void Heightmap::normalize(double min, double max) {
    if (min > max) {
      std::swap(min, max);
    }

    double currMin, currMax;
    std::tie(currMin, currMax) = getMinMax();

    double factor = 0.0f;

    if (!gf::almostEquals(currMin, currMax)) {
      factor = (max - min) / (currMax - currMin);
    }

    for (auto& value : m_data) {
      value = min + (value - currMin) * factor;
    }
  }

  void Heightmap::addNoise(Noise2D& noise, double scale)  {
    for (auto row : m_data.getRowRange()) {
      double y = static_cast<double>(row) / m_data.getRows() * scale;

      for (auto col : m_data.getColRange()) {
        double x = static_cast<double>(col) / m_data.getCols() * scale;
        m_data({ col, row }) = noise(x, y);
      }
    }
  }

  Heightmap Heightmap::subMap(RectI area) const {
    if (area.left + area.width > m_data.getCols()) {
      area.width = m_data.getCols() - area.left;
    }

    if (area.top + area.height > m_data.getRows()) {
      area.height = m_data.getRows() - area.top;
    }

    Heightmap out(area.size);

    for (int j = 0; j < area.height; ++j) {
      for (int i = 0; i < area.width; ++i) {
        out.m_data({ i, j }) = m_data({ area.left + i, area.top + j });
      }
    }

    return out;
  }

  Image Heightmap::copyToGrayscaleImage() const {
    Image image;
    image.create(m_data.getSize());

    for (auto pos : m_data.getPositionRange()) {
      uint8_t value = static_cast<uint8_t>(m_data(pos) * 255);
      image.setPixel(pos, { value, value, value, 0xFF });
    }

    return image;
  }

  static double valueWithWaterLevel(double value, double waterLevel) {
    if (value < waterLevel) {
      return value / waterLevel * 0.5;
    }

    return (value - waterLevel) / (1.0 - waterLevel) * 0.5 + 0.5;
  }

  Image Heightmap::copyToColoredImage(const ColorRamp& ramp, double waterLevel, Render render) const {
    Image image;
    image.create(m_data.getSize());

    for (auto pos : m_data.getPositionRange()) {
      double value = valueWithWaterLevel(m_data(pos), waterLevel);
      Color4f color = ramp.computeColor(value);
      image.setPixel(pos, Color::toRgba32(color));
    }

    if (render == Render::Shaded) {
      static constexpr Vector3d Light = { -1, -1, 0 };

      for (auto pos : m_data.getPositionRange()) {
        if (m_data(pos) < waterLevel) {
          continue;
        }

        double x = pos.col;
        double y = pos.row;

        // compute the normal vector
        Vector3d normal(0, 0, 0);
        unsigned count = 0;

        Vector3d p{x, y, m_data(pos)};

        if (pos.col > 0 && pos.row > 0) {
          Vector3d pn{x    , y - 1, m_data({ pos.col    , pos.row - 1 })};
          Vector3d pw{x - 1, y    , m_data({ pos.col - 1, pos.row     })};

          Vector3d v3 = cross(p - pw, p - pn);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (pos.col > 0 && pos.row < m_data.getRows() - 1) {
          Vector3d pw{x - 1, y    , m_data({ pos.col - 1, pos.row     })};
          Vector3d ps{x    , y + 1, m_data({ pos.col    , pos.row + 1 })};

          Vector3d v3 = cross(p - ps, p - pw);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (pos.col < m_data.getCols() - 1 && pos.row > 0) {
          Vector3d pe{x + 1, y    , m_data({ pos.col + 1, pos.row     })};
          Vector3d pn{x    , y - 1, m_data({ pos.col    , pos.row - 1 })};

          Vector3d v3 = cross(p - pn, p - pe);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (pos.col < m_data.getCols() - 1 && pos.row < m_data.getRows() - 1) {
          Vector3d pe{x + 1, y    , m_data({ pos.col + 1, pos.row     })};
          Vector3d ps{x    , y + 1, m_data({ pos.col    , pos.row + 1 })};

          Vector3d v3 = cross(p - pe, p - ps);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        normal = gf::normalize(normal / count);
        double d = gf::dot(Light, normal);
        d = gf::clamp(0.5 + 35 * d, 0.0, 1.0);

        Color4u pixel = image.getPixel(pos);

        Color4u lo = gf::lerp(pixel, Color4u(0x33, 0x11, 0x33, 0xFF), 0.7);
        Color4u hi = gf::lerp(pixel, Color4u(0xFF, 0xFF, 0xCC, 0xFF), 0.3);

        if (d < 0.5) {
          image.setPixel(pos, gf::lerp(lo, pixel, 2 * d));
        } else {
          image.setPixel(pos, gf::lerp(pixel, hi, 2 * d - 1));
        }
      }
    }

    return image;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
