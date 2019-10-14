/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#ifndef GF_BALL_H
#define GF_BALL_H

#include "Portability.h"
#include "Vector.h"
#include "VectorOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A n-dimension ball
   *
   * A ball is defined by its center and its radius.
   * It is a very simple class defined for convenience, so
   * its member variables (`center` and `radius`) are public
   * and can be accessed directly.
   */
  template<typename T, std::size_t N>
  struct Ball {
    Vector<T, N> center; ///< Center of the ball
    T radius; ///< Radius of the ball

    /**
     * @brief Default constructor
     *
     * Creates an empty ball.
     */
    constexpr Ball() noexcept
    : center(gf::Zero)
    , radius(T(0))
    {
    }

    /**
     * @brief Construct the ball from center and radius
     *
     * @param ballCenter Center of the ball
     * @param ballRadius Radius of the ball
     */
    constexpr Ball(const Vector<T, N>& ballCenter, T ballRadius) noexcept
    : center(ballCenter)
    , radius(ballRadius)
    {

    }

    /**
     * @brief Get the center of the ball
     *
     * It is a synonym for the `center` member
     *
     * @return The center of the ball
     * @sa getRadius()
     */
    constexpr Vector<T, N> getCenter() const noexcept {
      return center;
    }

    /**
     * @brief Get the radius of the ball
     *
     * It is a synonym for the `radius` member
     *
     * @return The radius of the ball
     * @sa getCenter()
     */
    constexpr T getRadius() const noexcept {
      return radius;
    }

    /**
     * @brief Check if the ball is empty
     *
     * An empty ball is a ball with a zero radius.
     *
     * @return True if the ball is empty
     */
    constexpr bool isEmpty() const noexcept {
      return radius == T(0);
    }

    /**
     * @brief Check if a point is insied a ball's area
     *
     * @param point The point to test
     * @return True if the point is inside, false otherwise
     * @sa intersects()
     */
    inline bool contains(const Vector<T, N>& point) const noexcept {
      return gf::squareDistance(center, point) <= gf::square(radius);
    }

    /**
     * @brief Check the intersection between two balls
     *
     * @param other The ball to test
     * @return True if balls overlap, false otherwise
     * @sa contains()
     */
    inline bool intersects(const Ball<T, N>& other) const noexcept {
      return gf::squareDistance(center, other.center) <= gf::square(radius + other.radius);
    }

  };

  /**
   * @relates Ball
   * @brief Equality operator
   *
   * @param lhs First ball
   * @param rhs Second ball
   * @return True if the two balls are the same
   */
  template<typename T, std::size_t N>
  constexpr
  bool operator==(const Ball<T, N>& lhs, const Ball<T, N>& rhs) {
    return lhs.center == rhs.center && lhs.radius == rhs.radius;
  }

  /**
   * @relates Ball
   * @brief Inequality operator
   *
   * @param lhs First ball
   * @param rhs Second ball
   * @return True if the two balls are different
   */
  template<typename T, std::size_t N>
  constexpr
  bool operator!=(const Ball<T, N>& lhs, const Ball<T, N>& rhs) {
    return lhs.center != rhs.center || lhs.radius != rhs.radius;
  }

  /**
   * @relates Ball
   * @brief Serialize a ball
   *
   * @param ar The archive
   * @param ball The ball to serialize
   * @sa gf::Serializer, gf::Deserializer
   */
  template<typename Archive, typename T, std::size_t N>
  Archive& operator|(Archive& ar, Ball<T, N>& ball) {
    return ar | ball.center | ball.radius;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_BALL_H
