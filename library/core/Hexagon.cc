/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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

  RectF HexagonHelper::computeBounds(Vector2i layerSize) const noexcept {
    Vector2f base = layerSize * m_tileSize;

    switch (m_axis) {
      case MapCellAxis::X:
        base.x /= 2;
        base.y += m_tileSize.height / 2;
        break;
      case MapCellAxis::Y:
        base.y /= 2;
        base.x += m_tileSize.width / 2;
        break;
    }

    return RectF::fromSize(base);
  }

  RectI HexagonHelper::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(computeCoordinates(local.min), computeCoordinates(local.max)).grow(2);
  }

  RectF HexagonHelper::computeCellBounds(Vector2i coords) const noexcept {
    Vector2f base = coords * m_tileSize;

    switch (m_axis) {
      case MapCellAxis::X:
        base.x += coords.x * m_sideLength;
        base.x /= 2;

        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.x % 2 != 0) {
              base.y += m_tileSize.height / 2;
            }
            break;
          case MapCellIndex::Even:
            if (coords.x % 2 == 0) {
              base.y += m_tileSize.height / 2;
            }
            break;
        }
        break;
      case MapCellAxis::Y:
        base.y += coords.y * m_sideLength;
        base.y /= 2;

        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.y % 2 != 0) {
              base.x += m_tileSize.width / 2;
            }
            break;
          case MapCellIndex::Even:
            if (coords.y % 2 == 0) {
              base.x += m_tileSize.width / 2;
            }
            break;
        }
        break;
    }

    return RectF::fromPositionSize(base, m_tileSize);
  }

  Vector2i HexagonHelper::computeCoordinates(Vector2f position) const noexcept {
    // TODO: quick approximation but not really good
    auto tileSize = m_tileSize;

    switch (m_axis) {
      case MapCellAxis::X:
        tileSize.x /= 2;
        break;
      case MapCellAxis::Y:
        tileSize.y /= 2;
        break;
    }

    return position / tileSize;
  }

  Polyline HexagonHelper::computePolyline(Vector2i coords) const {
    auto bounds = computeCellBounds(coords);
    Polyline polyline;

    switch (m_axis) {
      case MapCellAxis::X:
        polyline.addPoint((bounds.getTopLeft() + bounds.getTopRight()) / 2.0f);
        polyline.addPoint(0.75f * bounds.getTopLeft() + 0.25f * bounds.getBottomLeft());
        polyline.addPoint(0.25f * bounds.getTopLeft() + 0.75f * bounds.getBottomLeft());
        polyline.addPoint((bounds.getBottomLeft() + bounds.getBottomRight()) / 2.0f);
        polyline.addPoint(0.25f * bounds.getTopRight() + 0.75f * bounds.getBottomRight());
        polyline.addPoint(0.75f * bounds.getTopRight() + 0.25f * bounds.getBottomRight());
        break;

      case MapCellAxis::Y:
        polyline.addPoint((bounds.getTopLeft() + bounds.getBottomLeft()) / 2.0f);
        polyline.addPoint(0.75f * bounds.getTopLeft() + 0.25f * bounds.getTopRight());
        polyline.addPoint(0.25f * bounds.getTopLeft() + 0.75f * bounds.getTopRight());
        polyline.addPoint((bounds.getTopRight() + bounds.getBottomRight()) / 2.0f);
        polyline.addPoint(0.25f * bounds.getBottomLeft() + 0.75f * bounds.getBottomRight());
        polyline.addPoint(0.75f * bounds.getBottomLeft() + 0.25f * bounds.getBottomRight());
        break;
    }

    return polyline;
  }

  void HexagonHelper::forEachNeighbor(Vector2i coords, Vector2i layerSize, std::function<void(Vector2i)> func) const {
    // TODO
  }

  Vector2f HexagonHelper::computeRegularSize(MapCellAxis axis, float radius) {
    Vector2f size;

    switch (axis) {
      case MapCellAxis::X:
        size = { radius * Sqrt3, radius * 2.0f };
        break;

      case MapCellAxis::Y:
        size = { radius * 2.0f, radius * Sqrt3 };
        break;
    }

    return size;
  }


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

    return RectF::fromSize(bottomRightCorner);
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
