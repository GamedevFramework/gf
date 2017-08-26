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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
