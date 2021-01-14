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
#include <gf/Orthogonal.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RectF OrthogonalHelper::computeBounds(Vector2i layerSize, Vector2f tileSize) const {
    return RectF::fromPositionSize(gf::vec(0.0f, 0.0f), layerSize * tileSize);
  }

  RectI OrthogonalHelper::computeVisibleArea(const RectF& local, Vector2f tileSize) const {
    return RectI::fromMinMax(local.min / tileSize, local.max / tileSize);
  }

  RectF OrthogonalHelper::computeCellBounds(Vector2i coords, Vector2f tileSize) const {
    return RectF::fromPositionSize(coords * tileSize, tileSize);
  }

  Vector2i OrthogonalHelper::computeCoordinates(Vector2f position, Vector2f tileSize) const {
    return position / tileSize;
  }

  Polyline OrthogonalHelper::computePolyline(Vector2i coords, Vector2f tileSize) const {
    RectF rect = computeCellBounds(coords, tileSize);
    Polyline line(Polyline::Loop);
    line.addPoint(rect.getTopRight());
    line.addPoint(rect.getTopLeft());
    line.addPoint(rect.getBottomLeft());
    line.addPoint(rect.getBottomRight());
    return line;
  }

  void OrthogonalHelper::forEachNeighbor(Vector2i coords, Vector2i layerSize, std::function<void(Vector2i)> func) const {
    if (coords.x > 0) {
      func(coords + gf::dirx(-1));
    }

    if (coords.x < layerSize.width - 1) {
      func(coords + gf::dirx(1));
    }

    if (coords.y > 0) {
      func(coords + gf::diry(-1));
    }

    if (coords.y < layerSize.height - 1) {
      func(coords + gf::diry(1));
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
