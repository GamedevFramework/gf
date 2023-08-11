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

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RectF OrthogonalCells::computeBounds(Vector2i layerSize) const noexcept {
    return RectF::fromSize(layerSize * m_tileSize);
  }

  RectI OrthogonalCells::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(local.min / m_tileSize, local.max / m_tileSize);
  }

  RectF OrthogonalCells::computeCellBounds(Vector2i coords) const noexcept {
    return RectF::fromPositionSize(coords * m_tileSize, m_tileSize);
  }

  Vector2i OrthogonalCells::computeCoordinates(Vector2f position) const noexcept {
    return vec(static_cast<int>(std::floor(position.x / m_tileSize.width)), static_cast<int>(std::floor(position.y / m_tileSize.height)));
  }

  Polyline OrthogonalCells::computePolyline(Vector2i coords) const {
    RectF rect = computeCellBounds(coords);
    Polyline line(Polyline::Loop);
    line.addPoint(rect.getTopRight());
    line.addPoint(rect.getTopLeft());
    line.addPoint(rect.getBottomLeft());
    line.addPoint(rect.getBottomRight());
    return line;
  }

  std::vector<Vector2i> OrthogonalCells::computeNeighbors(Vector2i coords, Vector2i layerSize, Flags<CellNeighborQuery> flags) const {
    std::vector<Vector2i> neighbors;

    neighbors.push_back(coords + gf::vec(-1,  0));
    neighbors.push_back(coords + gf::vec( 1,  0));
    neighbors.push_back(coords + gf::vec( 0, -1));
    neighbors.push_back(coords + gf::vec( 0,  1));

    if (flags.test(CellNeighborQuery::Diagonal)) {
      neighbors.push_back(coords + gf::vec(-1, -1));
      neighbors.push_back(coords + gf::vec( 1, -1));
      neighbors.push_back(coords + gf::vec(-1,  1));
      neighbors.push_back(coords + gf::vec( 1,  1));
    }

    if (flags.test(CellNeighborQuery::Valid)) {
      RectI bounds = RectI::fromSize(layerSize);
      neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [bounds](Vector2i neighbor) {
        return !bounds.contains(neighbor);
      }), neighbors.end());
    }

    return neighbors;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
