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
#include <gf/Stagger.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RectF StaggerHelper::computeBounds(Vector2i layerSize) const noexcept {
    Vector2f base = layerSize * m_tileSize;

    switch (m_axis) {
      case MapCellAxis::Y:
        base.y /= 2;
        base.x += m_tileSize.width / 2;
        break;
      case MapCellAxis::X:
        base.x /= 2;
        base.y += m_tileSize.height / 2;
        break;
    }

    return RectF::fromSize(base);
  }

  RectI StaggerHelper::computeVisibleArea(const RectF& local) const noexcept {
    return RectI::fromMinMax(computeCoordinates(local.min), computeCoordinates(local.max)).grow(2);
  }

  RectF StaggerHelper::computeCellBounds(Vector2i coords) const noexcept {
    Vector2f base = coords * m_tileSize;

    switch (m_axis) {
      case MapCellAxis::Y:
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
      case MapCellAxis::X:
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
    }

    return RectF::fromPositionSize(base, m_tileSize);
  }

  Vector2i StaggerHelper::computeCoordinates(Vector2f position) const noexcept {
    // TODO: quick approximation but not really good
    auto tileSize = m_tileSize;

    switch (m_axis) {
      case MapCellAxis::Y:
        tileSize.y /= 2;
        break;
      case MapCellAxis::X:
        tileSize.x /= 2;
        break;
    }

    return position / tileSize;
  }

  Polyline StaggerHelper::computePolyline(Vector2i coords) const {
    Polyline line;
    // TODO
    return line;
  }

  void StaggerHelper::forEachNeighbor(Vector2i coords, Vector2i layerSize, std::function<void(Vector2i)> func) const {
    // TODO
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
