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

#include <gf/Log.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RectF StaggeredCells::computeBounds(Vector2i layerSize) const noexcept {
    Vector2f base = layerSize * m_tileSize;

    switch (m_axis) {
      case CellAxis::X:
        base.x /= 2;
        base.x += m_tileSize.width / 2;
        base.y += m_tileSize.height / 2;
        break;
      case CellAxis::Y:
        base.y /= 2;
        base.y += m_tileSize.height / 2;
        base.x += m_tileSize.width / 2;
        break;
    }

    return RectF::fromSize(base);
  }

  RectI StaggeredCells::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(computeCoordinates(local.min), computeCoordinates(local.max)).grow(2);
  }

  RectF StaggeredCells::computeCellBounds(Vector2i coords) const noexcept {
    Vector2f base = coords * m_tileSize;

    switch (m_axis) {
      case CellAxis::Y:
        base.y /= 2;

        switch (m_index) {
          case CellIndex::Odd:
            if (coords.y % 2 != 0) {
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
      case CellAxis::X:
        base.x /= 2;

        switch (m_index) {
          case CellIndex::Odd:
            if (coords.x % 2 != 0) {
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
    }

    return RectF::fromPositionSize(base, m_tileSize);
  }

  Vector2i StaggeredCells::computeCoordinates(Vector2f position) const noexcept {
    const Vector2f half = m_tileSize / 2.0f;

    const float qx = std::floor(position.x / half.x);
    const float rx = (position.x - qx * half.x) / half.x;
    assert(0 <= rx && rx < 1.0f);

    const float qy = std::floor(position.y / half.y);
    const float ry = (position.y - qy * half.y) / half.y;
    assert(0 <= ry && ry < 1.0f);

    const int x = static_cast<int>(qx);
    const int y = static_cast<int>(qy);

    gf::Vector2i coords = vec(x, y);

    const bool isDiagonallySplit = (m_index == CellIndex::Even) == (parity(x) == parity(y));

    if (m_axis == CellAxis::X) {
      if ((isDiagonallySplit && rx < ry) || (!isDiagonallySplit && (rx + ry) < 1)) {
        --coords.x;
      }

      coords.y = static_cast<int>(std::floor(qy / 2));

      if (parity(y) == 0 && ((isDiagonallySplit && rx > ry) || (!isDiagonallySplit && (rx + ry) < 1))) {
        --coords.y;
      }

      // Log::info("position: %g %g\tq: %g %g\tr: %g %g\tcoords: %d %d\n", position.x, position.y, qx, qy, rx, ry, coords.x, coords.y);
    } else {
      if ((isDiagonallySplit && rx > ry) || (!isDiagonallySplit && (rx + ry) < 1)) {
        --coords.y;
      }

      coords.x = static_cast<int>(std::floor(qx / 2));

      if (parity(x) == 0 && ((isDiagonallySplit && rx < ry) || (!isDiagonallySplit && (rx + ry) < 1))) {
        --coords.x;
      }
    }

    return coords;
  }

  Polyline StaggeredCells::computePolyline(Vector2i coords) const {
    auto bounds = computeCellBounds(coords);
    float xmin = bounds.min.x;
    float ymin = bounds.min.y;
    float xmax = bounds.max.x;
    float ymax = bounds.max.y;

    Polyline line(gf::Polyline::Loop);
    line.addPoint({ (xmin + xmax) / 2,  ymin });
    line.addPoint({ xmax,               (ymin + ymax) / 2 });
    line.addPoint({ (xmin + xmax) / 2,  ymax });
    line.addPoint({ xmin,               (ymin + ymax) / 2 });
    return line;
  }

  std::vector<Vector2i> StaggeredCells::computeNeighbors([[maybe_unused]] Vector2i coords, [[maybe_unused]] Vector2i layerSize, [[maybe_unused]] Flags<CellNeighborQuery> flags) const {
    // TODO
    return { };
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
