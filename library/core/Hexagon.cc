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

  Vector2f HexagonHelper::getHexagonSize(float radius) const noexcept {
    Vector2f size;

    switch (m_axis) {
      case MapCellAxis::X:
        size = { radius * Sqrt3, radius * 2.0f };
        break;

      case MapCellAxis::Y:
        size = { radius * 2.0f, radius * Sqrt3 };
        break;
    }

    return size;
  }

  RectF HexagonHelper::computeBounds(Vector2i size, float radius) const noexcept {
    Vector2f bottomRightCorner(0.0f, 0.0f);
    Vector2f hexagonSize = getHexagonSize(radius);

    switch (m_axis) {
      case MapCellAxis::X:
        bottomRightCorner.x = hexagonSize.width * size.x;
        if (size.width > 1) {
          bottomRightCorner.x += 0.5f * hexagonSize.width;
        }
        bottomRightCorner.y = hexagonSize.height * (0.25f + 0.75f * size.y);
        break;

      case MapCellAxis::Y:
        bottomRightCorner.x = hexagonSize.width * (0.25f + 0.75f * size.x);
        bottomRightCorner.y = hexagonSize.height * size.y;
        if (size.height > 1) {
          bottomRightCorner.y += 0.5f * hexagonSize.height;
        }
        break;
    }

    return RectF::fromPositionSize({ 0.0f, 0.0f }, bottomRightCorner);
  }

  Vector2f HexagonHelper::computeCenter(Vector2i coords, float radius) const {
    Vector2f center;
    Vector2f hexagonSize = getHexagonSize(radius);

    switch (m_axis) {
      case MapCellAxis::X:
        center.x = hexagonSize.width * (0.5f + coords.x);
        switch (m_index) {
        case MapCellIndex::Odd:
          center.x += hexagonSize.width * 0.5f * (coords.y&1);
          break;

        case MapCellIndex::Even:
          center.x += hexagonSize.width * 0.5f * ((coords.y + 1)&1);
          break;
        }
        center.y = hexagonSize.height * (0.5f + 0.75f * coords.y);
        break;

      case MapCellAxis::Y:
        center.x = hexagonSize.width * (0.5f + 0.75f * coords.x);
        center.y = hexagonSize.height * (0.5f + coords.y);
        switch (m_index) {
        case MapCellIndex::Odd:
          center.y += hexagonSize.height * 0.5f * (coords.x&1);
          break;

        case MapCellIndex::Even:
          center.y += hexagonSize.height * 0.5f * ((coords.x + 1)&1);
          break;
        }
        break;
    }

    return center;
  }

  std::vector<Vector2f> HexagonHelper::computeCorners(Vector2i coords, float radius) const {
    std::vector<Vector2f> corners(6);
    Vector2f center = computeCenter(coords, radius);

    for (unsigned i = 0; i < corners.size(); ++i) {
      float angle = i * Pi3;
      if (m_axis == MapCellAxis::X) {
        angle -= Pi6;
      }

      corners[i].x = center.x + radius * std::cos(angle);
      corners[i].y = center.y + radius * std::sin(angle);
    }

    return corners;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
