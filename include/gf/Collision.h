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
#ifndef GF_COLLISION_H
#define GF_COLLISION_H

#include "Circ.h"
#include "Polygon.h"
#include "Portability.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct Transform;

  /**
   * @ingroup game
   * @brief Data about the collision between two objects
   *
   * @sa [How to Create a Custom Physics Engine](http://gamedevelopment.tutsplus.com/series/how-to-create-a-custom-physics-engine--gamedev-12715)
   */
  struct GF_API Penetration {
    Vector2f normal; ///< Collision normal
    float depth; ///< Penetration depth
  };

  /**
   * @relates Penetration
   * @brief Check if two circles collides
   *
   * @param lhs First circle
   * @param lhsTrans Transformation of the first circle
   * @param rhs Second circle
   * @param rhsTrans Transformation of the second circle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const CircF& lhs, const Transform& lhsTrans, const CircF& rhs, const Transform& rhsTrans, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if two circles collides
   *
   * @param lhs First circle
   * @param rhs Second circle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const CircF& lhs, const CircF& rhs, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if a rectangle collides with a circle
   *
   * @param lhs The rectangle
   * @param rhs The circle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const RectF& lhs, const CircF& rhs, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if a circle collides with a rectangle
   *
   * @param lhs The circle
   * @param rhs The rectangle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const CircF& lhs, const RectF& rhs, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if two rectangles collides
   *
   * @param lhs First rectangle
   * @param rhs Second rectangle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const RectF& lhs, const RectF& rhs, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if a circle collides with a polygon
   *
   * @param lhs The circle
   * @param lhsTrans Transformation of the circle
   * @param rhs The polygon
   * @param rhsTrans Transformation of the polygon
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const CircF& lhs, const Transform& lhsTrans, const Polygon& rhs, const Transform& rhsTrans, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if a circle collides with a polygon
   *
   * @param lhs The circle
   * @param rhs The polygon
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const CircF& lhs, const Polygon& rhs, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if a polygon collides with a circle
   *
   * @param lhs The polygon
   * @param lhsTrans Transformation of the polygon
   * @param rhs The circle
   * @param rhsTrans Transformation of the circle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const Polygon& lhs, const Transform& lhsTrans, const CircF& rhs, const Transform& rhsTrans, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if a polygon collides with a circle
   *
   * @param lhs The polygon
   * @param rhs The circle
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const Polygon& lhs, const CircF& rhs, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if two polygons collides
   *
   * @param lhs First polygon
   * @param lhsTrans Transformation of the first polygon
   * @param rhs Second polygon
   * @param rhsTrans Transformation of the second polygon
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const Polygon& lhs, const Transform& lhsTrans, const Polygon& rhs, const Transform& rhsTrans, Penetration& p);

  /**
   * @relates Penetration
   * @brief Check if two polygons collides
   *
   * @param lhs First polygon
   * @param rhs Second polygon
   * @param p Data to fill if there is a collision
   * @return True if there is a collision
   */
  GF_API bool collides(const Polygon& lhs, const Polygon& rhs, Penetration& p);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_COLLISION_H
