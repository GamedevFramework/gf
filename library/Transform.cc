/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include <gf/Transform.h>

#include <algorithm>
#include <tuple>

#include <gf/Matrix.h>
#include <gf/Rect.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RectF transform(const Matrix3f& mat, const RectF& rect) {
    Vector2f pointTopLeft = transform(mat, rect.getTopLeft());
    Vector2f pointTopRight = transform(mat, rect.getTopRight());
    Vector2f pointBottomLeft = transform(mat, rect.getBottomLeft());
    Vector2f pointBottomRight = transform(mat, rect.getBottomRight());

    Vector2f min;
    Vector2f max;

    std::tie(min.x, max.x) = std::minmax({ pointTopLeft.x, pointTopRight.x, pointBottomLeft.x, pointBottomRight.x });
    std::tie(min.y, max.y) = std::minmax({ pointTopLeft.y, pointTopRight.y, pointBottomLeft.y, pointBottomRight.y });

    return RectF(min, max - min);
  }

  void translate(Matrix3f& mat, Vector2f offset) {
    mat *= translation(offset);
  }

  void rotate(Matrix3f& mat, float angle) {
    mat *= rotation(angle);
  }

  void rotate(Matrix3f& mat, float angle, Vector2f center) {
    mat *= rotation(angle, center);
  }

  void scale(Matrix3f& mat, Vector2f factor) {
    mat *= scaling(factor);
  }

  void scale(Matrix3f& mat, Vector2f factor, Vector2f center) {
    mat *= scaling(factor, center);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
