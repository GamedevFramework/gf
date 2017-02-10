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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_TRANSFORMABLE_H
#define GF_TRANSFORMABLE_H

#include "Anchor.h"
#include "Drawable.h"
#include "Matrix.h"
#include "Portability.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Decomposed transform defined by a position, a rotation and a scale
   *
   * gf::Matrix3f, as a low-level class, offers a great level of
   * flexibility but it is not always convenient to manage. Indeed,
   * one can easily combine any kind of operation, such as a translation
   * followed by a rotation followed by a scaling, but once the result
   * transform is built, there's no way to go backward and, let's say,
   * change only the rotation without modifying the translation and scaling.
   * The entire transform must be recomputed, which means that you
   * need to retrieve the initial translation and scale factors as
   * well, and combine them the same way you did before updating the
   * rotation. This is a tedious operation, and it requires to store
   * all the individual components of the final transform.
   *
   * That's exactly what gf::Transformable was written for: it hides
   * these variables and the composed transform behind an easy to use
   * interface. You can set or get any of the individual components
   * without worrying about the others. It also provides the composed
   * transform (as a gf::Matrix3f), and keeps it up-to-date.
   *
   * In addition to the position, rotation and scale, gf::Transformable
   * provides an "origin" component, which represents the local origin
   * of the three other components. Let's take an example with a 10x10
   * pixels sprite. By default, the sprite is positioned/rotated/scaled
   * relatively to its top-left corner, because it is the local point
   * @f$(0, 0)@f$. But if we change the origin to be @f$(5, 5)@f$, the
   * sprite will be positioned/rotated/scaled around its center instead.
   * And if we set the origin to @f$(10, 10)@f$, it will be transformed
   * around its bottom-right corner.
   *
   * To keep the gf::Transformable class simple, there's only one
   * origin for all the components. You cannot position the sprite
   * relatively to its top-left corner while rotating it around its
   * center, for example. To do such things, use gf::Matrix3f directly.
   *
   * gf::Transformable can be used as a base class. That's what
   * gf's sprites, texts, curves and shapes do.
   *
   * ~~~{.cc}
   * class MyEntity : public gf::Transformable {
   *   virtual void draw(gf::RenderTarget& target, gf::RenderStates states) override {
   *     states.transform *= getTransform();
   *     target.draw(..., states);
   *   }
   * };
   *
   * MyEntity entity;
   * entity.setPosition(10, 20);
   * entity.setRotation(45);
   * renderer.draw(entity);
   * ~~~
   *
   * @sa gf::Matrix3f
   */
  class GF_API Transformable : public Drawable {
  public:
    /**
     * @brief Default constructor
     *
     * By default:
     *
     * - the origin is set to @f$(0,0)@f$
     * - the position is set to @f$(0,0)@f$
     * - the rotation is set to @f$ 0 @f$
     * - the scale is set to @f$(1,1)@f$
     */
    Transformable();

    /**
     * @brief Set the local origin of the object
     *
     * The origin of an object defines the center point for
     * all transformations (position, scale, rotation).
     * The coordinates of this point must be relative to the
     * top-left corner of the object, and ignore all
     * transformations (position, scale, rotation).
     *
     * The default origin of a transformable object is @f$(0, 0)@f$.
     *
     * @param origin New origin
     *
     * @sa getOrigin()
     */
    void setOrigin(Vector2f origin);

    /**
     * @brief Get the local origin of the object
     *
     * @return Current origin
     *
     * @sa setOrigin()
     */
    Vector2f getOrigin() const {
      return m_origin;
    }

    /**
     * @brief Set the position of the object
     *
     * This function completely overwrites the previous position.
     * See the move() function to apply an offset based on the previous
     * position instead.
     *
     * The default position of a transformable object is @f$(0, 0)@f$.
     *
     * @param position New position
     *
     * @sa move(), getPosition()
     */
    void setPosition(Vector2f position);

    /**
     * @brief Get the position of the object
     *
     * @return Current position
     *
     * @sa setPosition
     */
    Vector2f getPosition() const {
      return m_position;
    }

    /**
     * @brief Move the object by a given offset
     *
     * This function adds to the current position of the object,
     * unlike setPosition() which overwrites it.
     * Thus, it is equivalent to the following code:
     *
     * ~~~{.cc}
     * object.setPosition(object.getPosition() + offset);
     * ~~~
     *
     * @param offset Offset
     *
     * @sa setPosition()
     */
    void move(Vector2f offset);

    /**
     * @brief Set the orientation of the object
     *
     * This function completely overwrites the previous rotation.
     * See the rotate() function to add an angle based on the previous
     * rotation instead.
     *
     * The default rotation of a transformable object is @f$ 0 @f$.
     *
     * @param angle New rotation, in radians
     *
     * @sa rotate(), getRotation()
     */
    void setRotation(float angle);

    /**
     * @brief Get the orientation of the object
     *
     * @return Current rotation, in radians
     *
     * @sa setRotation()
     */
    float getRotation() const {
      return m_rotation;
    }

    /**
     * @brief Rotate the object
     *
     * This function adds to the current rotation of the object,
     * unlike setRotation() which overwrites it.
     * Thus, it is equivalent to the following code:
     *
     * ~~~{.cc}
     * object.setRotation(object.getRotation() + angle);
     * ~~~
     *
     * @param angle Angle of rotation, in radians
     */
    void rotate(float angle);

    /**
     * @brief Set the scale factors of the object
     *
     * This function completely overwrites the previous scale.
     * See the scale() function to add a factor based on the previous
     * scale instead.
     *
     * The default scale of a transformable object is @f$(1, 1)@f$.
     *
     * @param factors New scale factors
     *
     * @sa scale(), getScale()
     */
    void setScale(Vector2f factors);


    /**
     * @brief Set the scale factor of the object
     *
     * This function is a shortcut when the scale factors for @f$ x @f$
     * and @f$ y @f$ are the same. It is equivalent to:
     *
     * ~~~{.cc}
     * object.setScale({ factor, factor });
     * ~~~
     *
     * @param factor New scale factor
     *
     * @sa scale(), getScale()
     */
    void setScale(float factor) {
      setScale({ factor, factor });
    }

    /**
     * @brief Get the current scale of the object
     *
     * @return Current scale factors
     *
     * @sa setScale()
     */
    Vector2f getScale() const {
      return m_scale;
    }

    /**
     * @brief Scale the object
     *
     * This function multiplies the current scale of the object,
     * unlike setScale() which overwrites it.
     * Thus, it is equivalent to the following code:
     *
     * ~~~{.cc}
     * object.setScale(object.getScale() * factors);
     * ~~~
     *
     * @param factors Scale factors
     *
     * @sa setScale()
     */
    void scale(Vector2f factors);

    /**
     * @brief Scale the object
     *
     * This function is a shortcut when the scale factors for @f$ x @f$
     * and @f$ y @f$ are the same. It is equivalent to:
     *
     * ~~~{.cc}
     * object.scale({ factor, factor });
     * ~~~
     *
     * @param factor Scale factor
     *
     * @sa setScale()
     */
    void scale(float factor) {
      scale({ factor, factor });
    }

    /**
     * @brief Get the combined transform of the object
     *
     * The combined transform of the object is (in this order):
     *
     * - a translation of -origin
     * - a scaling of the defined scaling factors
     * - a rotation of the defined angle
     * - a translation of the defined position
     *
     * @return Transform combining the position/rotation/scale/origin of the object
     *
     * @sa getInverseTransform()
     */
    Matrix3f getTransform() const;

    /**
     * @brief Get the inverse of the combined transform of the object
     *
     * @return Inverse of the combined transformations applied to the object
     *
     * @sa getTransform()
     */
    Matrix3f getInverseTransform() const;

  protected:
    /**
     * @brief Set the origin from an anchor and bounds
     *
     * This function can be called from derived classes for setting the origin
     * properly thanks to an anchor and the bounds they computed.
     *
     * @param anchor An anchor
     * @param bounds The bounds of the entity
     */
    void setOriginFromAnchorAndBounds(Anchor anchor, const RectF& bounds);

  private:
    Vector2f m_origin;
    Vector2f m_position;
    float m_rotation;
    Vector2f m_scale;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TRANSFORMABLE_H
