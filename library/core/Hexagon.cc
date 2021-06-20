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

#include <cassert>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    inline
    float computeOffset(Vector2f tileSize, float sideLength, MapCellAxis axis) {
      switch (axis) {
        case MapCellAxis::X:
          return (tileSize.width - sideLength) / 2;
        case MapCellAxis::Y:
          return (tileSize.height - sideLength) / 2;
      }

      assert(false);
      return 0.0f;
    }

  }

  RectF HexagonHelper::computeBounds(Vector2i layerSize) const noexcept {
    Vector2f size;
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);

    switch (m_axis) {
      case MapCellAxis::X:
        size.height = layerSize.height * m_tileSize.height + m_tileSize.height / 2;
        size.width = layerSize.width * (m_tileSize.width - offset) + offset;
        break;

      case MapCellAxis::Y:
        size.width = layerSize.width * m_tileSize.width + m_tileSize.width / 2;
        size.height = layerSize.height * (m_tileSize.height - offset) + offset;
        break;
    }

    return RectF::fromSize(size);
  }

  RectI HexagonHelper::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(computeCoordinates(local.min), computeCoordinates(local.max)).grow(2);
  }

  RectF HexagonHelper::computeCellBounds(Vector2i coords) const noexcept {
    Vector2f base;
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);

    switch (m_axis) {
      case MapCellAxis::X:
        base.x = coords.x * (m_tileSize.width - offset);
        base.y = coords.y * m_tileSize.height;

        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.x % 2 == 1) {
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
        base.y = coords.y * (m_tileSize.height - offset);
        base.x = coords.x * m_tileSize.width;

        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.y % 2 == 1) {
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
    // good approximation but would need some tweaking

    Vector2i coords;
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);

    switch (m_axis) {
      case MapCellAxis::X:
        coords.x = position.x / (m_tileSize.width - offset);
        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.x % 2 == 0) {
              coords.y = position.y / m_tileSize.height;
            } else {
              coords.y = (position.y - m_tileSize.height / 2) / m_tileSize.height;
            }
            break;
          case MapCellIndex::Even:
            if (coords.x % 2 != 0) {
              coords.y = position.y / m_tileSize.height;
            } else {
              coords.y = (position.y - m_tileSize.height / 2) / m_tileSize.height;
            }
            break;
        }
        break;
      case MapCellAxis::Y:
        coords.y = position.y / (m_tileSize.height - offset);
        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.y % 2 == 0) {
              coords.x = position.x / m_tileSize.width;
            } else {
              coords.x = (position.x - m_tileSize.width / 2) / m_tileSize.width;
            }
            break;
          case MapCellIndex::Even:
            if (coords.y % 2 != 0) {
              coords.x = position.x / m_tileSize.width;
            } else {
              coords.x = (position.x - m_tileSize.width / 2) / m_tileSize.width;
            }
            break;
        }
        break;
    }

    return coords;
  }

  Polyline HexagonHelper::computePolyline(Vector2i coords) const {
    auto bounds = computeCellBounds(coords);
    float xmin = bounds.min.x;
    float ymin = bounds.min.y;
    float xmax = bounds.max.x;
    float ymax = bounds.max.y;
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);;

    Polyline polyline(Polyline::Loop);

    switch (m_axis) {
      case MapCellAxis::X:
        polyline.addPoint({ xmin,           (ymin + ymax) /2 });
        polyline.addPoint({ xmin + offset,  ymin });
        polyline.addPoint({ xmax - offset,  ymin });
        polyline.addPoint({ xmax,           (ymin + ymax) /2 });
        polyline.addPoint({ xmax - offset,  ymax });
        polyline.addPoint({ xmin + offset,  ymax });
        break;

      case MapCellAxis::Y:
        polyline.addPoint({ (xmin + xmax) / 2,  ymin });
        polyline.addPoint({ xmin,               ymin + offset });
        polyline.addPoint({ xmin,               ymax - offset });
        polyline.addPoint({ (xmin + xmax) / 2,  ymax });
        polyline.addPoint({ xmax,               ymax - offset });
        polyline.addPoint({ xmax,               ymin + offset });
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
        size = { radius * 2.0f, radius * Sqrt3 };
        break;

      case MapCellAxis::Y:
        size = { radius * Sqrt3, radius * 2.0f };
        break;
    }

    return size;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
