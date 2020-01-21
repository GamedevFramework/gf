/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <gf/Hexagon.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void HexagonHelper::setCoordinateUnitLength(float coordinateUnitLength) {
    m_coordinateUnitLength = coordinateUnitLength;
    m_hexagonSize = { coordinateUnitLength * 2.0f, coordinateUnitLength * Sqrt3 };
  }

  Vector2f HexagonHelper::computeCenter(Vector2i coords) const {
    Vector2f base = { 1.5f * m_hexagonSize.width * coords.x, 0.5f * m_hexagonSize.height * coords.y };
    if (coords.y % 2 == 1) {
      base.x += m_hexagonSize.width * 0.75f;
    }

    base.x += m_hexagonSize.width / 2.0f;
    base.y += m_hexagonSize.height / 2.0f;

    return base;
  }

  std::vector<Vector2f> HexagonHelper::computeCorners(Vector2i coords) const {
    std::vector<Vector2f> corners(6);
    Vector2f center = computeCenter(coords);

    corners[0] = { center.x - 0.25f * m_hexagonSize.width, center.y - 0.50f * m_hexagonSize.height };
    corners[1] = { center.x + 0.25f * m_hexagonSize.width, center.y - 0.50f * m_hexagonSize.height };
    corners[2] = { center.x + 0.50f * m_hexagonSize.width, center.y - 0.00f * m_hexagonSize.height };
    corners[3] = { center.x + 0.25f * m_hexagonSize.width, center.y + 0.50f * m_hexagonSize.height };
    corners[4] = { center.x - 0.25f * m_hexagonSize.width, center.y + 0.50f * m_hexagonSize.height };
    corners[5] = { center.x - 0.50f * m_hexagonSize.width, center.y + 0.00f * m_hexagonSize.height };

    return corners;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
