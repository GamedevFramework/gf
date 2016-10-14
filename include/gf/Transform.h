/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef GF_TRANSFORM_H
#define GF_TRANSFORM_H

#include <cmath>

#include "Matrix.h"
#include "Portability.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  // https://en.wikipedia.org/wiki/Homogeneous_coordinates

  /**
   * @ingroup core
   * @brief Apply an affine transformation to a 2D point.
   *
   * @param mat The transformation matrix
   * @param point The point to transform
   * @return The transformed point
   */
  inline
  Vector2f transform(const Matrix3f& mat, Vector2f point) {
    Vector3f homogeneousPoint{point, 1.0f};
    Vector3f result = mat * homogeneousPoint;
    return result.xy;
  }

  /**
   * @ingroup core
   * @brief Apply an affine transformaton to a rectangle
   *
   * The resulting rectangle is axis-aligned
   *
   * @param mat The transformation matrix
   * @param rect The rectangle to transform
   * @return The transformed rectangle
   */
  GF_API RectF transform(const Matrix3f& mat, const RectF& rect);


  /**
   * @ingroup core
   * @brief Identity transform
   *
   * @return The identity matrix
   */
  constexpr Matrix3f identityTransform() {
    return Matrix3f(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  }

  /**
   * @ingroup core
   * @brief Get a translation matrix
   *
   * @param offset The offset of the translation
   * @return A translation by `offset`
   */
  inline
  Matrix3f translation(Vector2f offset) {
    return Matrix3f{
      1.0f, 0.0f, offset.x,
      0.0f, 1.0f, offset.y,
      0.0f, 0.0f, 1.0f
    };
  }

  /**
   * @ingroup core
   * @brief Combine the current transform with a translation
   *
   * @param mat The current transform
   * @param offset The offset of the translation
   */
  GF_API void translate(Matrix3f& mat, Vector2f offset);

  /**
   * @ingroup core
   * @brief Get a rotation matrix
   *
   * @param angle The angle of the rotation (in radians)
   * @return A rotation of `angle` radians
   */
  inline
  Matrix3f rotation(float angle) {
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    return Matrix3f{
       cos, -sin, 0.0f,
       sin,  cos, 0.0f,
      0.0f, 0.0f, 1.0f
    };
  }

  /**
   * @ingroup core
   * @brief Get a rotation matrix
   *
   * @param angle The angle of the rotation (in radians)
   * @param center The center of the rotation
   * @return A rotation of `angle` radians of origin `center`
   */
  inline
  Matrix3f rotation(float angle, Vector2f center) {
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    return Matrix3f{
       cos, -sin, center.x * (1 - cos) + center.y * sin,
       sin,  cos, center.y * (1 - cos) - center.x * sin,
      0.0f, 0.0f, 1.0f
    };
  }

  /**
   * @ingroup core
   * @brief Combine the current transform with a rotation
   *
   * @param mat The current transform
   * @param angle The angle of the rotation (in radians)
   */
  GF_API void rotate(Matrix3f& mat, float angle);

  /**
   * @ingroup core
   * @brief Combine the current transform with a rotation
   *
   * @param mat The current transform
   * @param angle The angle of the rotation (in radians)
   * @param center The center of the rotation
   */
  GF_API void rotate(Matrix3f& mat, float angle, Vector2f center);

  /**
   * @ingroup core
   * @brief Get a scaling matrix
   *
   * @param factor The scaling factor
   * @return A scaling by `factor`
   */
  inline
  Matrix3f scaling(Vector2f factor) {
    return Matrix3f{
      factor.x,     0.0f, 0.0f,
          0.0f, factor.y, 0.0f,
          0.0f,     0.0f, 1.0f
    };
  }

  /**
   * @ingroup core
   * @brief Get a scaling matrix
   *
   * @param factor The scaling factor
   * @param center The center of the scaling
   * @return A scaling by `factor` of origin `center`
   */
  inline
  Matrix3f scaling(Vector2f factor, Vector2f center) {
    return Matrix3f{
      factor.x,     0.0f, center.x * (1.0f - factor.x),
          0.0f, factor.y, center.y * (1.0f - factor.y),
          0.0f,     0.0f, 1.0f
    };
  }

  /**
   * @ingroup core
   * @brief Combine the current transform with a scaling
   *
   * @param mat The current transform
   * @param factor The scaling factor
   */
  GF_API void scale(Matrix3f& mat, Vector2f factor);

  /**
   * @ingroup core
   * @brief Combine the current transform with a scaling
   *
   * @param mat The current transform
   * @param factor The scaling factor
   * @param center The center of the scaling
   */
  GF_API void scale(Matrix3f& mat, Vector2f factor, Vector2f center);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TRANSFORM_H
