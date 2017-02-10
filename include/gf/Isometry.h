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
#ifndef GF_ISOMETRY_H
#define GF_ISOMETRY_H

#include <cmath>

#include "Math.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @brief A rotation
   */
  struct GF_API Rotation {
    float cos;
    float sin;

    Rotation()
    : cos(1.0f), sin(0.0f)
    {

    }

    Rotation(float angle)
    : cos(std::cos(angle)), sin(std::sin(angle))
    {

    }

    void setAngle(float angle) {
      cos = std::cos(angle);
      sin = std::sin(angle);
    }

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
   */
  struct GF_API Translation {
    Vector2f offset;

    Translation()
    : offset(0.0f, 0.0f)
    {

    }

    Translation(Vector2f translationOffset)
    : offset(translationOffset)
    {

    }

    void setOffset(Vector2f newOffset) noexcept {
      offset = newOffset;
    }

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
   * @brief A simple isometry (rotation then translation)
   */
  struct GF_API Isometry {
    Rotation rotation; ///< Rotation of the isometry
    Translation translation; ///< Translation of the isometry

    Isometry()
    {

    }

    /**
     * @brief Constructor with a rotation and a translation
     *
     * @param angle The rotation angle
     * @param offset The translation offset
     */
    Isometry(float angle, Vector2f offset)
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
    Isometry(float angle)
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
    Isometry(Vector2f offset)
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
   * @relates Isometry
   * @brief Apply an isometry to a 2D point.
   *
   * @param iso The isometry
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f transform(const Isometry& iso, Vector2f point) {
    return transform(iso.translation, transform(iso.rotation, point));
  }

  /**
   * @relates Isometry
   * @brief Apply an inverse isometry to a 2D point.
   *
   * @param iso The isometry
   * @param point The point to transform
   * @return The transformed point
   */
  constexpr
  Vector2f inverseTransform(const Isometry& iso, Vector2f point) {
    return inverseTransform(iso.rotation, inverseTransform(iso.translation, point));
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ISOMETRY_H
