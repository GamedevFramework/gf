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
#include <gf/Cells.h>

#include <cassert>

#include <gf/Log.h>
#include <gf/Span.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    inline
    float computeOffset(Vector2f tileSize, float sideLength, CellAxis axis) {
      switch (axis) {
        case CellAxis::X:
          return (tileSize.width - sideLength) / 2;
        case CellAxis::Y:
          return (tileSize.height - sideLength) / 2;
      }

      assert(false);
      return 0.0f;
    }

  }

  RectF HexagonalCells::computeBounds(Vector2i layerSize) const noexcept {
    Vector2f size = gf::vec(0.0f, 0.0f);
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);

    switch (m_axis) {
      case CellAxis::X:
        size.height = layerSize.height * m_tileSize.height + m_tileSize.height / 2;
        size.width = layerSize.width * (m_tileSize.width - offset) + offset;
        break;

      case CellAxis::Y:
        size.width = layerSize.width * m_tileSize.width + m_tileSize.width / 2;
        size.height = layerSize.height * (m_tileSize.height - offset) + offset;
        break;
    }

    return RectF::fromSize(size);
  }

  RectI HexagonalCells::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(computeCoordinates(local.min), computeCoordinates(local.max)).grow(2);
  }

  RectF HexagonalCells::computeCellBounds(Vector2i coords) const noexcept {
    Vector2f base = gf::vec(0.0f, 0.0f);
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);

    switch (m_axis) {
      case CellAxis::X:
        base.x = coords.x * (m_tileSize.width - offset);
        base.y = coords.y * m_tileSize.height;

        switch (m_index) {
          case CellIndex::Odd:
            if (coords.x % 2 == 1) {
              base.y += m_tileSize.height / 2;
            }
            break;
          case CellIndex::Even:
            if (coords.x % 2 == 0) {
              base.y += m_tileSize.height / 2;
            }
            break;
        }
        break;

      case CellAxis::Y:
        base.y = coords.y * (m_tileSize.height - offset);
        base.x = coords.x * m_tileSize.width;

        switch (m_index) {
          case CellIndex::Odd:
            if (coords.y % 2 == 1) {
              base.x += m_tileSize.width / 2;
            }
            break;
          case CellIndex::Even:
            if (coords.y % 2 == 0) {
              base.x += m_tileSize.width / 2;
            }
            break;
        }
        break;
    }

    return RectF::fromPositionSize(base, m_tileSize);
  }

  Vector2i HexagonalCells::computeCoordinates(Vector2f position) const noexcept {
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);
    Vector2i coords = {};

    switch (m_axis) {
      case CellAxis::X: {
        const float lx = m_tileSize.width - offset;

        const float qx = std::floor(position.x / lx);
        const float rx = position.x - qx * lx;
        const float nrx = rx / offset;

        const float halfy = m_tileSize.height / 2.0f;
        const float qy = std::floor(position.y / halfy);
        const float ry = position.y - qy * halfy;
        const float nry = ry / halfy;

        const int x = static_cast<int>(qx);
        const int y = static_cast<int>(qy);

        coords = vec(x, y);

        if ((m_index == CellIndex::Even) == (parity(x) == 0)) {
          --coords.y;
        }

        coords.y = static_cast<int>(std::floor(coords.y / 2.0f));

        if (rx < offset) {
          if ((m_index == CellIndex::Even) == (parity(x) == 0)) {
            if (parity(y) == 0) {
              if (nrx < nry) {
                --coords.x;
                ++coords.y;
              }
            } else {
              if (nrx + nry < 1) {
                --coords.x;
              }
            }
          } else {
            if (parity(y) == 0) {
              if (nrx + nry < 1) {
                --coords.x;
                --coords.y;
              }
            } else {
              if (nrx < nry) {
                --coords.x;
              }
            }
          }
        }

        break;
      }

      case CellAxis::Y: {
        const float ly = m_tileSize.height - offset;

        const float qy = std::floor(position.y / ly);
        const float ry = position.y - qy * ly;
        const float nry = ry / offset;

        const float halfx = m_tileSize.width / 2.0f;
        const float qx = std::floor(position.x / halfx);
        const float rx = position.x - qx * halfx;
        const float nrx = rx / halfx;

        const int x = static_cast<int>(qx);
        const int y = static_cast<int>(qy);

        coords = vec(x, y);

        if ((m_index == CellIndex::Even) == (parity(y) == 0)) {
          --coords.x;
        }

        coords.x = static_cast<int>(std::floor(coords.x / 2.0f));

        if (ry < offset) {
          if ((m_index == CellIndex::Even) == (parity(y) == 0)) {
            if (parity(x) == 0) {
              if (nrx > nry) {
                --coords.y;
                ++coords.x;
              }
            } else {
              if (nrx + nry < 1) {
                --coords.y;
              }
            }
          } else {
            if (parity(x) == 0) {
              if (nrx + nry < 1) {
                --coords.y;
                --coords.x;
              }
            } else {
              if (nrx > nry) {
                --coords.y;
              }
            }
          }
        }

        break;
      }
    }

    return coords;
  }

  Polyline HexagonalCells::computePolyline(Vector2i coords) const {
    auto bounds = computeCellBounds(coords);
    float xmin = bounds.min.x;
    float ymin = bounds.min.y;
    float xmax = bounds.max.x;
    float ymax = bounds.max.y;
    float offset = computeOffset(m_tileSize, m_sideLength, m_axis);;

    Polyline polyline(Polyline::Loop);

    switch (m_axis) {
      case CellAxis::X:
        polyline.addPoint({ xmin,           (ymin + ymax) /2 });
        polyline.addPoint({ xmin + offset,  ymin });
        polyline.addPoint({ xmax - offset,  ymin });
        polyline.addPoint({ xmax,           (ymin + ymax) /2 });
        polyline.addPoint({ xmax - offset,  ymax });
        polyline.addPoint({ xmin + offset,  ymax });
        break;

      case CellAxis::Y:
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

  std::vector<Vector2i> HexagonalCells::computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags) const {
    static constexpr Vector2i XOffsets[2][6] = {
      { { +1,  0 }, { +1, -1 }, { 0, -1 }, { -1, -1 }, { -1,  0 }, {  0, +1 } },
      { { +1, +1 }, { +1,  0 }, { 0, -1 }, { -1,  0 }, { -1, +1 }, {  0, +1 } }
    };

    static constexpr Vector2i YOffsets[2][6] = {
      { { +1,  0 }, {  0, -1 }, { -1, -1 }, { -1,  0 }, { -1, +1 }, {  0, +1 } },
      { { +1,  0 }, { +1, -1 }, {  0, -1 }, { -1,  0 }, {  0, +1 }, { +1, +1 } }
    };

    StaticSpan<const Vector2i, 6> relative;

    switch (m_axis) {
      case CellAxis::X:
        if ((m_index == CellIndex::Odd && coords.x % 2 == 0) || (m_index == CellIndex::Even && coords.x % 2 == 1)) {
          relative = XOffsets[0];
        } else {
          relative = XOffsets[1];
        }
        break;
      case CellAxis::Y:
        if ((m_index == CellIndex::Odd && coords.y % 2 == 0) || (m_index == CellIndex::Even && coords.y % 2 == 1)) {
          relative = YOffsets[0];
        } else {
          relative = YOffsets[1];
        }
        break;
    }

    std::vector<Vector2i> neighbors;

    for (auto offset : relative) {
      neighbors.push_back(coords + offset);
    }

    assert(neighbors.size() == 6);

    if (flags.test(CellNeighborQuery::Valid)) {
      RectI bounds = RectI::fromSize(layerSize);
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vector2i neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
    }

    return neighbors;
  }

  Vector2f HexagonalCells::computeRegularSize(CellAxis axis, float radius) {
    Vector2f size = gf::vec(0.0f, 0.0f);

    switch (axis) {
      case CellAxis::X:
        size = { radius * 2.0f, radius * Sqrt3 };
        break;

      case CellAxis::Y:
        size = { radius * Sqrt3, radius * 2.0f };
        break;
    }

    return size;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
