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
#ifndef GF_RECT_H
#define GF_RECT_H

#include <cstddef>
#include <algorithm>

#include "Anchor.h"
#include "Box.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Utility class for manipulating 2D axis aligned rectangles
   *
   * A rectangle is defined by its top-left corner (called `min`) and
   * its bottom-right corner (called `max').
   *
   * gf::Rect uses the usual rules for its boundaries:
   *
   * - The left and top edges are included in the rectangle's area
   * - The right and bottom edges are excluded from the rectangle's area
   *
   * gf::Rect is a template and may be used with any numeric type, but
   * for simplicity, some common typedef are defined:
   *
   * - gf::RectI with `int` as `T`
   * - gf::RectU with `unsigned` as `T`
   * - gf::RectZ with `std::size_t` as `T`
   * - gf::RectF with `float` as `T`
   *
   * So that you don't have to care about the template syntax.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * // Define a rectangle, located at (0, 0) with a size of 20x5
   * gf::RectI r1(0, 0, 20, 5);
   *
   * // Define another rectangle, located at (4, 2) with a size of 18x10
   * gf::Vector2i position(4, 2);
   * gf::Vector2i size(18, 10);
   * gf::RectI r2(position, size);
   *
   * // Test intersections with the point (3, 1)
   * bool b1 = r1.contains({ 3, 1 }); // true
   * bool b2 = r2.contains({ 3, 1 }); // false
   *
   * // Test the intersection between r1 and r2
   * gf::RectI result;
   * bool b3 = r1.intersects(r2, result); // true
   * // result == (4, 2, 16, 3)
   * ~~~
   */
  template<typename T>
  struct Rect : Box<T, 2> {
    /**
     * @brief Default constructor
     *
     * Creates an empty rectangle.
     */
    constexpr Rect() noexcept
    : Box<T, 2>({ T(0), T(0) })
    {
    }

    /**
     * @brief Constructor from a box
     *
     * @param box The orignal box
     */
    constexpr Rect(const Box<T, 2>& box)
    : Box<T, 2>(box)
    {
    }

    /**
     * @brief Create a rectangle from a min point and a max point
     *
     * @param min The minimum point in the rectangle
     * @param max The maximum point in the rectangle
     * @returns A new rectangle
     */
    static constexpr Rect<T> fromMinMax(Vector<T, 2> min, Vector<T, 2> max) noexcept {
      return Rect<T>(min, max);
    }

    /**
     * @brief Create a rectangle from a position (top-left) and a size
     *
     * @param position The top-left position of the rectangle
     * @param size The size of the rectangle
     * @returns A new rectangle
     */
    static constexpr Rect<T> fromPositionSize(Vector<T, 2> position, Vector<T, 2> size) noexcept {
      return Rect<T>(position, position + size);
    }

    /**
     * @brief Create a rectangle from a center and a size
     *
     * @param center The center of the rectangle
     * @param size The size of the rectangle
     * @returns A new rectangle
     */
    static constexpr Rect<T> fromCenterSize(Vector<T, 2> center, Vector<T, 2> size) noexcept {
      return Rect<T>(center - size / T(2), center + size / T(2));
    }

    /**
     * @brief Create an empty rectangle
     *
     * @returns An empty rectangle
     */
    static constexpr Rect<T> empty() noexcept {
      return Rect<T>(Box<T,2>());
    }

    /**
     * @brief Get a position from the rectangle and an anchor
     *
     * @param anchor An anchor
     * @returns A position in the rectangle
     */
    constexpr Vector<T, 2> getPositionFromAnchor(Anchor anchor) const noexcept {
      switch (anchor) {
        case Anchor::TopLeft:
          return this->min;
        case Anchor::TopCenter:
          return { (this->min.x + this->max.x) / T(2), this->min.y };
        case Anchor::TopRight:
          return { this->max.x, this->min.y };
        case Anchor::CenterLeft:
          return { this->min.x, (this->min.y + this->max.y) / T(2) };
        case Anchor::Center:
          return this->getCenter();
        case Anchor::CenterRight:
          return { this->max.x, (this->min.y + this->max.y) / T(2) };
        case Anchor::BottomLeft:
          return { this->min.x, this->max.y };
        case Anchor::BottomCenter:
          return { (this->min.x + this->max.x) / T(2), this->max.y };
        case Anchor::BottomRight:
          return this->max;
      }

      return this->min;
    }

    /**
     * @brief Get the top left corner
     *
     * @return The top left corner
     */
    constexpr Vector<T, 2> getTopLeft() const noexcept {
      return getPositionFromAnchor(Anchor::TopLeft);
    }

    /**
     * @brief Get the top right corner
     *
     * @return The top right corner
     */
    constexpr Vector<T, 2> getTopRight() const noexcept {
      return getPositionFromAnchor(Anchor::TopRight);
    }

    /**
     * @brief Get the bottom left corner
     *
     * @return The bottom left corner
     */
    constexpr Vector<T, 2> getBottomLeft() const noexcept {
      return getPositionFromAnchor(Anchor::BottomLeft);
    }

    /**
     * @brief Get the bottom right corner
     *
     * @return The bottom right corner
     */
    constexpr Vector<T, 2> getBottomRight() const noexcept {
      return getPositionFromAnchor(Anchor::BottomRight);
    }

    /**
     * @brief Get the width of the rectangle
     *
     * @returns The width of the rectangle
     */
    constexpr T getWidth() const noexcept {
      return this->max.x - this->min.x;
    }

    /**
     * @brief Get the height of the rectangle
     *
     * @returns The height of the rectangle
     */
    constexpr T getHeight() const noexcept {
      return this->max.y - this->min.y;
    }

  private:
    constexpr Rect(Vector<T, 2> min, Vector<T, 2> max) noexcept
    : Box<T, 2>(min, max)
    {
    }
  };

  /**
   * @ingroup core
   * @brief A `float` rectangle
   *
   * @sa gf::Rect
   */
  using RectF = Rect<float>;

  /**
   * @ingroup core
   * @brief A `double` rectangle
   *
   * @sa gf::Rect
   */
  using RectD = Rect<double>;

  /**
   * @ingroup core
   * @brief A `int` rectangle
   *
   * @sa gf::Rect
   */
  using RectI = Rect<int>;

  /**
   * @ingroup core
   * @brief A `unsigned` rectangle
   *
   * @sa gf::Rect
   */
  using RectU = Rect<unsigned>;

  /**
   * @ingroup core
   * @brief A `std::size_t` rectangle
   *
   * @sa gf::Rect
   */
  using RectZ = Rect<std::size_t>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_API Rect<float>;
  extern template struct GF_API Rect<double>;
  extern template struct GF_API Rect<int>;
  extern template struct GF_API Rect<unsigned>;
#endif

  /**
   * @relates Rect
   * @brief Equality operator
   *
   * @param lhs First rectangle
   * @param rhs Second rectangle
   * @return True if the two rectangles are the same
   */
  template<typename T>
  inline
  bool operator==(const Rect<T>& lhs, const Rect<T>& rhs) {
    return lhs.min == rhs.min && lhs.max == rhs.max;
  }

  /**
   * @relates Rect
   * @brief Inequality operator
   *
   * @param lhs First rectangle
   * @param rhs Second rectangle
   * @return True if the two rectangles are different
   */
  template<typename T>
  inline
  bool operator!=(const Rect<T>& lhs, const Rect<T>& rhs) {
    return !(lhs == rhs);
  }

  /**
   * @relates Rect
   * @brief Serialize a rectangle
   *
   * @param ar The archive
   * @param rect The rectangle to serialize
   * @sa gf::Serialize, gf::Deserialiser
   */
  template<typename Archive, typename T>
  Archive& operator|(Archive& ar, Rect<T>& rect) {
    return ar | rect.min.x | rect.min.y | rect.max.x | rect.max.y;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RECT_H
