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

  /**
   * @brief A rotation
   *
   * @sa gf::Translation, gf::Transform
   */
  struct GF_API Rotation {
    float cos; ///< The cosine of the rotation angle
    float sin; ///< The sine of the rotation angle

    /**
     * @brief Default constructor
     *
     * The default rotation is a rotation of angle @f$ 0 @f$.
     */
    Rotation()
    : cos(1.0f), sin(0.0f)
    {

    }

    /**
     * @brief Constructor with an angle
     *
     * @param angle The rotation angle
     */
    Rotation(float angle)
    : cos(std::cos(angle)), sin(std::sin(angle))
    {

    }

    /**
     * @brief Set the rotation angle
     *
     * @param angle The new rotation angle
     */
    void setAngle(float angle) {
      cos = std::cos(angle);
      sin = std::sin(angle);
    }

    /**
     * @brief Get the rotation angle
     *
     * @returns The current rotation angle
     */
    float getAngle() const {
      return std::atan2(sin, cos);
    }
  };

  /**
   * @relates Rotation
   * @brief Apply a rotation to a 2D point.
   *
   * @param rotation The rotation
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f transform(const Rotation& rotation, Vector2f point) {
    return {
      rotation.cos * point.x - rotation.sin * point.y,
      rotation.sin * point.x + rotation.cos * point.y
    };
  }

  /**
   * @relates Rotation
   * @brief Apply an inverse rotation to a 2D point.
   *
   * @param rotation The rotation
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f inverseTransform(const Rotation& rotation, Vector2f point) {
    return {
       rotation.cos * point.x + rotation.sin * point.y,
      -rotation.sin * point.x + rotation.cos * point.y
    };
  }

  /**
   * @brief A translation
   *
   * @sa gf::Rotation, gf::Transform
   */
  struct GF_API Translation {
    Vector2f offset; ///< The offset of the translation

    /**
     * @brief Default constructor
     *
     * The default translation has a null offset
     */
    Translation()
    : offset(0.0f, 0.0f)
    {

    }

    /**
     * @brief Constructor with an offset
     *
     * @param translationOffset The translation offset
     */
    Translation(Vector2f translationOffset)
    : offset(translationOffset)
    {

    }

    /**
     * @brief Set the translation offset
     *
     * @param newOffset The new offset
     */
    void setOffset(Vector2f newOffset) noexcept {
      offset = newOffset;
    }

    /**
     * @brief Get the translation offset
     *
     * @return The current translation offset
     */
    Vector2f getOffset() const noexcept {
      return offset;
    }
  };

  /**
   * @relates Translation
   * @brief Apply a translation to a 2D point.
   *
   * @param translation The translation
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f transform(const Translation& translation, Vector2f point) {
    return { point.x + translation.offset.x, point.y + translation.offset.y };
  }

  /**
   * @relates Translation
   * @brief Apply an inverse translation to a 2D point.
   *
   * @param translation The translation
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f inverseTransform(const Translation& translation, Vector2f point) {
    return { point.x - translation.offset.x, point.y - translation.offset.y };
  }

  /**
   * @brief A simple transformation (rotation then translation)
   *
   * This class is meant for simple transformation as can be seen in physics.
   * It is the composition of a rotation and a translation.
   *
   * For more complex affine transformation, you can use gf::Matrix3f.
   *
   * @sa gf::Matrix3f, gf::Rotation, gf::Translation
   */
  struct GF_API Transform {
    Rotation rotation; ///< The rotation of the transformation
    Translation translation; ///< The translation of the transformation

    /**
     * @brief Default constructor
     *
     * The default transform is the identity.
     */
    Transform()
    {

    }

    /**
     * @brief Constructor with a rotation and a translation
     *
     * @param angle The rotation angle
     * @param offset The translation offset
     */
    Transform(float angle, Vector2f offset)
    : rotation(angle), translation(offset)
    {

    }

    /**
     * @brief Constructor with a rotation
     *
     * There is no translation.
     *
     * @param angle The rotation angle
     */
    Transform(float angle)
    : rotation(angle)
    {

    }

    /**
     * @brief Constructor with a translation
     *
     * There is no rotation.
     *
     * @param offset The translation offset
     */
    Transform(Vector2f offset)
    : translation(offset)
    {

    }

    /**
     * @brief Set the rotation angle
     *
     * @param angle The rotation angle
     */
    void setAngle(float angle) {
      rotation.setAngle(angle);
    }

    /**
     * @brief Get the rotation angle
     *
     * @returns The rotation angle
     */
    float getAngle() const {
      return rotation.getAngle();
    }

    /**
     * @brief Set the translation offset
     *
     * @param offset The translation offset
     */
    void setOffset(Vector2f offset) noexcept {
      translation.setOffset(offset);
    }

    /**
     * @brief Get the translation offset
     *
     * @returns The translation offset
     */
    Vector2f getOffset() const noexcept {
      return translation.getOffset();
    }

  };

  /**
   * @relates Transform
   * @brief Apply a transformation to a 2D point.
   *
   * @param trans The transformation
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f transform(const Transform& trans, Vector2f point) {
    return transform(trans.translation, transform(trans.rotation, point));
  }

  /**
   * @relates Transform
   * @brief Apply an inverse transformation to a 2D point.
   *
   * @param trans The transformation
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f inverseTransform(const Transform& trans, Vector2f point) {
    return inverseTransform(trans.rotation, inverseTransform(trans.translation, point));
  }

  // https://en.wikipedia.org/wiki/Homogeneous_coordinates

  /**
   * @ingroup core
   * @brief Apply an affine transformation to a 2D point.
   *
   * @param mat The transformation matrix
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f transform(const Matrix3f& mat, Vector2f point) {
    return { mat.xx * point.x + mat.xy * point.y + mat.xz, mat.yx * point.x + mat.yy * point.y + mat.yz };
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
