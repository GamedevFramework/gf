/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
#ifndef GF_CIRC_H
#define GF_CIRC_H

#include "CoreApi.h"
#include "Vector.h"
#include "VectorOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_geometry
   * @brief Utility class for manipulating circles
   *
   * A circle is defined by its center and its radius.
   * It is a very simple class defined for convenience, so
   * its member variables (`center` and `radius`) are public
   * and can be accessed directly.
   *
   * gf::Circ is a template and may be used with any numeric type, but
   * for simplicity, some common typedef are defined:
   *
   * - gf::CircI with `int` as `T`
   * - gf::CircU with `unsigned` as `T`
   * - gf::CircZ with `std::size_t` as `T`
   * - gf::CircF with `float` as `T`
   * - gf::CircD with `double` as `T`
   *
   * So that you don't have to care about the template syntax.
   *
   * Usage example:
   *
   * @snippet snippets/doc_struct_circ.cc circ
   */
  template<typename T>
  struct Circ {
    Vector<T, 2> center; ///< Center of the circle
    T radius; ///< Radius of the circle

    /**
     * @brief Default constructor
     *
     * Creates an empty circle (it is equivalent to calling
     * `Circ({ 0, 0 }, 0`).
     */
    constexpr Circ() noexcept
    : center(gf::Zero)
    , radius(T(0))
    {
    }

    /**
     * @brief Construct the circle from center and radius
     *
     * @param circCenter Center of the circle
     * @param circRadius Radius of the circle
     */
    constexpr Circ(const Vector<T, 2>& circCenter, T circRadius) noexcept
    : center(circCenter)
    , radius(circRadius)
    {
    }

    /**
     * @brief Get the center of the circle
     *
     * It is a synonym for the `center` member
     *
     * @return The center of the circle
     * @sa getRadius()
     */
    constexpr Vector<T, 2> getCenter() const noexcept {
      return center;
    }

    /**
     * @brief Get the radius of the circle
     *
     * It is a synonym for the `radius` member
     *
     * @return The radius of the circle
     * @sa getCenter()
     */
    constexpr T getRadius() const noexcept {
      return radius;
    }

    /**
     * @brief Check if the circle is empty
     *
     * An empty circle is a circle with a zero radius.
     *
     * @return True if the circle is empty
     */
    constexpr bool isEmpty() const noexcept {
      return radius == T(0);
    }

    /**
     * @brief Check if a point is insied a circle's area
     *
     * @param point The point to test
     * @return True if the point is inside, false otherwise
     * @sa intersects()
     */
    inline bool contains(const Vector<T, 2>& point) const noexcept {
      return gf::squareDistance(center, point) <= gf::square(radius);
    }

    /**
     * @brief Check the intersection between two circles
     *
     * @param other The circle to test
     * @return True if circles overlap, false otherwise
     * @sa contains()
     */
    inline bool intersects(const Circ& other) const noexcept {
      return gf::squareDistance(center, other.center) <= gf::square(radius + other.radius);
    }

    /**
     * @brief Get the top of the circle
     *
     * @return The top of the circle
     */
    constexpr Vector<T, 2> getTop() const noexcept {
      return this->center - gf::diry(this->radius);
    }

    /**
     * @brief Get the bottom of the circle
     *
     * @return The bottom of the circle
     */
    constexpr Vector<T, 2> getBottom() const noexcept {
      return this->center + gf::diry(this->radius);
    }

    /**
     * @brief Get the left of the circle
     *
     * @return The left of the circle
     */
    constexpr Vector<T, 2> getLeft() const noexcept {
      return this->center - gf::dirx(this->radius);
    }

    /**
     * @brief Get the right of the circle
     *
     * @return The right of the circle
     */
    constexpr Vector<T, 2> getRight() const noexcept {
      return this->center + gf::dirx(this->radius);
    }


  };


  /**
   * @ingroup core_geometry
   * @brief A `float` circle
   *
   * @sa gf::Circ
   */
  using CircF = Circ<float>;

  /**
   * @ingroup core_geometry
   * @brief A `double` circle
   *
   * @sa gf::Circ
   */
  using CircD = Circ<double>;

  /**
   * @ingroup core_geometry
   * @brief A `int` circle
   *
   * @sa gf::Circ
   */
  using CircI = Circ<int>;

  /**
   * @ingroup core_geometry
   * @brief A `unsigned` circle
   *
   * @sa gf::Circ
   */
  using CircU = Circ<unsigned>;

  /**
   * @ingroup core_geometry
   * @brief A `std::size_t` circle
   *
   * @sa gf::Circ
   */
  using CircZ = Circ<std::size_t>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Circ<float>;
  extern template struct GF_CORE_API Circ<double>;
  extern template struct GF_CORE_API Circ<int>;
  extern template struct GF_CORE_API Circ<unsigned>;
#endif

  /**
   * @relates Circ
   * @brief Equality operator
   *
   * @param lhs First circle
   * @param rhs Second circle
   * @return True if the two circles are the same
   */
  template<typename T>
  inline
  bool operator==(const Circ<T>& lhs, const Circ<T>& rhs) {
    return lhs.center == rhs.center && lhs.radius == rhs.radius;
  }

  /**
   * @relates Circ
   * @brief Inequality operator
   *
   * @param lhs First circle
   * @param rhs Second circle
   * @return True if the two circles are different
   */
  template<typename T>
  inline
  bool operator!=(const Circ<T>& lhs, const Circ<T>& rhs) {
    return lhs.center != rhs.center || lhs.radius != rhs.radius;
  }

  /**
   * @relates Circ
   * @brief Serialize a circle
   *
   * @param ar The archive
   * @param circ The circle to serialize
   * @sa gf::Serializer, gf::Deserializer
   */
  template<typename Archive, typename T>
  Archive& operator|(Archive& ar, Circ<T>& circ) {
    return ar | circ.center | circ.radius;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_CIRC_H
