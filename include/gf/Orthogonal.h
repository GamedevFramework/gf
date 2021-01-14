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
#ifndef GF_ORTHOGONAL_H
#define GF_ORTHOGONAL_H

#include <functional>

#include "CoreApi.h"
#include "Polyline.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_utilities
   * @brief A helper for computing coordinates in an orthogonal map
   */
  class GF_CORE_API OrthogonalHelper {
  public:
    RectF computeBounds(Vector2i layerSize, Vector2f tileSize) const;

    RectI computeVisibleArea(const RectF& local, Vector2f tileSize) const;

    RectF computeCellBounds(Vector2i coords, Vector2f tileSize) const;

    Vector2i computeCoordinates(Vector2f position, Vector2f tileSize) const;

    Polyline computePolyline(Vector2i coords, Vector2f tileSize) const;

    void forEachNeighbor(Vector2i coords, Vector2i layerSize, std::function<void(Vector2i)> func) const;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ORTHOGONAL_H
