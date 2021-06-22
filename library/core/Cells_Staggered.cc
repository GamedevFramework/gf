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
#include <gf/Cells.h>

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
    // TODO: quick approximation but not really good
    auto tileSize = m_tileSize;

    switch (m_axis) {
      case CellAxis::Y:
        tileSize.y /= 2;
        break;
      case CellAxis::X:
        tileSize.x /= 2;
        break;
    }

    return position / tileSize;
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

  std::vector<Vector2i> StaggeredCells::computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags) const {
    // TODO
    return { };
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
