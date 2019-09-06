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
#ifndef GF_CIRC_H
#define GF_CIRC_H

#include "Ball.h"
#include "Portability.h"
#include "Vector.h"
#include "VectorOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
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
   * So that you don't have to care about the temlate syntax.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * // Define a circle, with a center at  (10, 10) and a radius of 20
   * gf::CircI c1({ 10, 10 }, 20);
   *
   * // Define a circle with a center at (0, 0) and a radius of 2
   * gf::CircI c2({ 0, 0 }, 2);
   *
   * // Test intersections with the point (3, 1)
   * bool b1 = c1.contains({ 3, 1 }); // true
   * bool b2 = c2.contains({ 3, 1 }); // false
   *
   * // Test the intersection between c1 and c2
   * bool b3 = c1.intersects(c2); // true
   * ~~~
   */
  template<typename T>
  struct Circ : Ball<T, 2> {
    /**
     * @brief Default constructor
     *
     * Creates an empty circle (it is equivalent to calling
     * `Circ({ 0, 0 }, 0`).
     */
    constexpr Circ() noexcept
    : Ball<T, 2>()
    {
    }

    /**
     * @brief Construct the circle from center and radius
     *
     * @param circCenter Center of the circle
     * @param circRadius Radius of the circle
     */
    constexpr Circ(const Vector<T, 2>& circCenter, T circRadius) noexcept
    : Ball<T, 2>(circCenter, circRadius)
    {
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
   * @ingroup core
   * @brief A `float` circle
   *
   * @sa gf::Circ
   */
  using CircF = Circ<float>;

  /**
   * @ingroup core
   * @brief A `double` circle
   *
   * @sa gf::Circ
   */
  using CircD = Circ<double>;

  /**
   * @ingroup core
   * @brief A `int` circle
   *
   * @sa gf::Circ
   */
  using CircI = Circ<int>;

  /**
   * @ingroup core
   * @brief A `unsigned` circle
   *
   * @sa gf::Circ
   */
  using CircU = Circ<unsigned>;

  /**
   * @ingroup core
   * @brief A `std::size_t` circle
   *
   * @sa gf::Circ
   */
  using CircZ = Circ<std::size_t>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_API Circ<float>;
  extern template struct GF_API Circ<double>;
  extern template struct GF_API Circ<int>;
  extern template struct GF_API Circ<unsigned>;
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
