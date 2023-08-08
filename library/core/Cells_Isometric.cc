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
inline namespace v2 {
#endif

  RectF IsometricCells::computeBounds() const noexcept {

    return RectF::fromSize((m_layerSize.width + m_layerSize.height) * m_tileSize / 2);
  }

  RectI IsometricCells::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(local.min / m_tileSize, local.max / m_tileSize);
  }

  RectF IsometricCells::computeCellBounds(Vector2i coords) const noexcept {
    Vector2i transformed = { coords.x - coords.y + m_layerSize.height - 1, coords.x + coords.y };
    return RectF::fromPositionSize(transformed * m_tileSize / 2, m_tileSize);
  }

  Vector2i IsometricCells::computeCoordinates(Vector2f position) const noexcept {
    position.x -= m_layerSize.height * m_tileSize.width / 2;
    const float u = position.x / (m_tileSize.width / 2);
    const float v = position.y / (m_tileSize.height / 2);

    return gf::vec(std::floor((u + v) / 2), std::floor((v - u) / 2));
  }

  Polyline IsometricCells::computePolyline(Vector2i coords) const {
    RectF bounds = computeCellBounds(coords);
    Polyline line(Polyline::Loop);
    line.addPoint(bounds.getPositionFromAnchor(Anchor::TopCenter));
    line.addPoint(bounds.getPositionFromAnchor(Anchor::CenterRight));
    line.addPoint(bounds.getPositionFromAnchor(Anchor::BottomCenter));
    line.addPoint(bounds.getPositionFromAnchor(Anchor::CenterLeft));
    return line;
  }

  std::vector<Vector2i> IsometricCells::computeNeighbors(Vector2i coords, Flags<CellNeighborQuery> flags) const {
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
      RectI bounds = RectI::fromSize(m_layerSize);
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
