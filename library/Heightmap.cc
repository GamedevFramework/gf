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
#include <gf/Unused.h>

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

  std::tuple<double, double> Heightmap::getMinMax() const {
    auto p = std::minmax_element(m_data.begin(), m_data.end());
    return std::make_tuple(*p.first, *p.second);
  }

  void Heightmap::normalize(double min, double max) {
    if (min > max) {
      std::swap(min, max);
    }

    double currMin, currMax;
    std::tie(currMin, currMax) = getMinMax();

    double factor = 0.0;

    if (!gf::almostEquals(currMin, currMax)) {
      factor = (max - min) / (currMax - currMin);
    }

    for (auto& value : m_data) {
      value = min + (value - currMin) * factor;
    }
  }

  void Heightmap::addHill(Vector2d center, double radius, double height) {
    Vector2i size = m_data.getSize();
    double radiusSquare = gf::square(radius);
    double coeff = height / radiusSquare;
    int minX = std::max(0, static_cast<int>(center.x - radius));
    int maxX = std::min(size.width, static_cast<int>(center.x + radius));
    int minY = std::max(0, static_cast<int>(center.y - radius));
    int maxY = std::min(size.height, static_cast<int>(center.y + radius));

    for (int y = minY; y < maxY; ++y) {
      double yDistSquare = gf::square(y - center.y);

      for (int x = minX; x < maxX; ++x) {
        double xDistSquare = gf::square(x - center.x);
        double z = radiusSquare - (yDistSquare + xDistSquare);

        if (z > 0.0) {
          m_data({ x, y }) += z * coeff;
        }
      }
    }
  }

  void Heightmap::digHill(Vector2d center, double radius, double height) {
    Vector2i size = m_data.getSize();
    double radiusSquare = gf::square(radius);
    double coeff = height / radiusSquare;
    int minX = std::max(0, static_cast<int>(center.x - radius));
    int maxX = std::min(size.width, static_cast<int>(center.x + radius));
    int minY = std::max(0, static_cast<int>(center.y - radius));
    int maxY = std::min(size.height, static_cast<int>(center.y + radius));

    for (int y = minY; y < maxY; ++y) {
      double yDistSquare = gf::square(y - center.y);

      for (int x = minX; x < maxX; ++x) {
        double xDistSquare = gf::square(x - center.x);
        double distSquare = yDistSquare + xDistSquare;

        if (distSquare < radiusSquare) {
          double z = (radiusSquare - distSquare) * coeff;

          if (height > 0.0) {
            if (m_data( { x, y }) < z) {
              m_data( { x, y }) = z;
            }
          } else {
            if (m_data( { x, y }) > z) {
              m_data( { x, y }) = z;
            }
          }
        }

      }
    }
  }

  void Heightmap::addNoise(Noise2D& noise, double scale)  {
    for (auto row : m_data.getRowRange()) {
      double y = static_cast<double>(row) / m_data.getRows() * scale;

      for (auto col : m_data.getColRange()) {
        double x = static_cast<double>(col) / m_data.getCols() * scale;
        m_data({ col, row }) += noise(x, y);
      }
    }
  }

  void Heightmap::addValue(double value) {
    for (auto& currentValue : m_data) {
      currentValue += value;
    }
  }

  void Heightmap::scale(double value) {
    for (auto& currentValue : m_data) {
      currentValue *= value;
    }
  }

  void Heightmap::clamp(double min, double max) {
    for (auto& value : m_data) {
      value = gf::clamp(value, min, max);
    }
  }

  double Heightmap::getSlope(Vector2i position) const {
    const double altitudeHere = m_data(position);
    double altitudeDifferenceMax = 0.0;

    m_data.visit4Neighbors(position, [altitudeHere, &altitudeDifferenceMax](Vector2i positionThere, double altitudeThere) {
      gf::unused(positionThere);
      double altitudeDifference = std::abs(altitudeHere - altitudeThere);

      if (altitudeDifference > altitudeDifferenceMax) {
        altitudeDifferenceMax = altitudeDifference;
      }
    });

    return altitudeDifferenceMax;
  }

  void Heightmap::thermalErosion(unsigned iterations, double talus, double fraction) {
    double d[3][3];

    Array2D<double, int> material(m_data.getSize());

    for (unsigned k = 0; k < iterations; ++k) {

      // initialize material map
      std::fill(material.begin(), material.end(), 0.0);

      // compute material map
      for (int y = 1; y < m_data.getRows() - 1; ++y) {
        for (int x = 1; x < m_data.getCols() - 1; ++x) {
          double diffTotal = 0.0;
          double diffMax = 0.0;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = m_data({ x, y }) - m_data({ x+i, y+j });
              d[1+i][1+j] = diff;

              if (diff > talus) {
                diffTotal += diff;

                if (diff > diffMax) {
                  diffMax = diff;
                }
              }
            }
          }

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = d[1+i][1+j];

              if (diff > talus) {
                material({ x+i, y+j }) += fraction * (diffMax - talus) * (diff / diffTotal);
              }
            }
          }
        }
      }

      // add material map to the heightmap
      for (int y = 1; y < m_data.getRows() - 1; ++y) {
        for (int x = 1; x < m_data.getCols() - 1; ++x) {
          m_data({ x, y }) += material({ x, y });
        }
      }
    }
  }

  void Heightmap::hydraulicErosion(unsigned iterations, double rainAmount, double solubility, double evaporation, double capacity) {
    Array2D<double, int> waterMap(m_data.getSize(), 0.0);
    Array2D<double, int> waterDiff(m_data.getSize(), 0.0);

    Array2D<double, int> materialMap(m_data.getSize(), 0.0);
    Array2D<double, int> materialDiff(m_data.getSize(), 0.0);

    double d[3][3];

    for (unsigned k = 0; k < iterations; ++k) {

      // 1. appearance of new water
      for (auto& water : waterMap) {
        water += rainAmount;
      }

      // 2. water erosion of the terrain
      for (auto pos : waterMap.getPositionRange()) {
        double material = solubility * waterMap(pos);
        m_data(pos) -= material;
        materialMap(pos) += material;
      }

      // 3. transportation of water
      std::fill(waterDiff.begin(), waterDiff.end(), 0.0);
      std::fill(materialDiff.begin(), materialDiff.end(), 0.0);

      for (int y = 1; y < m_data.getRows() - 1; ++y) {
        for (int x = 1; x < m_data.getCols() - 1; ++x) {
          double diffTotal = 0.0;
          double altitudeTotal = 0.0;
          double altitude = m_data({ x, y }) + waterMap({ x, y });
          int n = 0;

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double altitudeLocal = m_data({ x+i, y+j }) + waterMap({ x+i, y+j });
              double diff = altitude - altitudeLocal;
              d[1+i][1+j] = diff;

              if (diff > 0.0) {
                diffTotal += diff;
                altitudeTotal += altitudeLocal;
                n++;
              }
            }
          }

          if (n == 0) {
            continue;
          }

          double altitudeAverage = altitudeTotal / n;
          double diffAltitude = std::min(waterMap({ x, y }), altitude - altitudeAverage);

          for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
              double diff = d[1+i][1+j];

              if (diff > 0.0) {
                double diffWater = diffAltitude * (diff / diffTotal);
                waterDiff({ x + i, y + j }) += diffWater;
                waterDiff({ x, y }) -= diffWater;

                double diffMaterial = materialMap({ x, y }) * (diffWater / waterMap({ x, y }));
                materialDiff({ x + i, y + j }) += diffMaterial;
                materialDiff({ x, y }) -= diffMaterial;
              }
            }
          }
        }
      }

      for (auto pos : waterMap.getPositionRange()) {
        waterMap(pos) += waterDiff(pos);
      }

      for (auto pos : materialMap.getPositionRange()) {
        materialMap(pos) += materialDiff(pos);
      }

      // 4. evaporation of water
      for (auto pos : waterMap.getPositionRange()) {
        double water = waterMap(pos) * (1 - evaporation);

        waterMap(pos) = water;

        double materialMax = capacity * water;
        double diffMaterial = std::max(double(0), materialMap(pos) - materialMax);
        materialMap(pos) -= diffMaterial;
        m_data(pos) += diffMaterial;
      }

    }

  }

  void Heightmap::fastErosion(unsigned iterations, double talus, double fraction) {
    Array2D<double, int> material(m_data.getSize());

    for (unsigned k = 0; k < iterations; ++k) {

      // initialize material map
      std::fill(material.begin(), material.end(), 0.0);

      // compute material map
      for (auto position : m_data.getPositionRange()) {
        double altitudeDifferenceMax = 0.0;
        Vector2i positionMax = position;

        const double altitudeHere = m_data(position);

        m_data.visit8Neighbors(position, [altitudeHere, &altitudeDifferenceMax, &positionMax](Vector2i positionThere, double altitudeThere) {
          double altitudeDifference = altitudeHere - altitudeThere;
          if (altitudeDifference > altitudeDifferenceMax) {
            altitudeDifferenceMax = altitudeDifference;
            positionMax = positionThere;
          }
        });

        if (0 < altitudeDifferenceMax && altitudeDifferenceMax <= talus) {
          material(position) -= fraction * altitudeDifferenceMax;
          material(positionMax) += fraction * altitudeDifferenceMax;
        }
      }

      // add material map to the map
      for (auto position : m_data.getPositionRange()) {
        m_data(position) += material(position);
      }
    }
  }

  double Heightmap::getErosionScore() const {
    double total = 0.0;
    double totalSquare = 0.0;
    int n = 0;

    for (auto position : m_data.getPositionRange()) {
      double value = getSlope(position);
      total += value;
      totalSquare += gf::square(value);
      n++;
    }

    double average = total / n;
    double averageSquare = totalSquare / n;
    double stdDev = std::sqrt(averageSquare - gf::square(average));
    return stdDev / average;
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
