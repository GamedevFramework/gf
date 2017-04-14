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
#ifndef GF_RECT_H
#define GF_RECT_H

#include <cstddef>
#include <algorithm>

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
   * A rectangle is defined by its top-left corner and its size.
   * It is a very simple class defined for convenience, so
   * its member variables (`left`, `top`, `width` and `height`) are public
   * and can be accessed directly. You can also access the `position`
   * and the `size` of the rectangle directly as gf::Vector.
   *
   * gf::Rect uses the usual rules for its boundaries:
   *
   * - The left and top edges are included in the rectangle's area
   * - The right (left + width) and bottom (top + height) edges are excluded from the rectangle's area
   *
   * This means that gf::RectI(0, 0, 1, 1) and gf::RectI(1, 1, 1, 1)
   * don't intersect.
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
  struct Rect {
    /**
     * An anonymous union to handle the various representations
     */
    union {
      struct {
        T left; ///< Left coordinate of the rectangle
        T top; ///< Top coordinate of the rectangle
        T width; ///< Width of the rectangle
        T height; ///< Height of the rectangle
      };
      struct {
        Vector<T, 2> position; ///< Position of the rectangle
        Vector<T, 2> size; ///< Size of the rectangle
      };
    };

    /**
     * @brief Default constructor
     *
     * Creates an empty rectangle (it is equivalent to calling
     * `Rect(0, 0, 0, 0)`).
     */
    constexpr Rect() noexcept
    : left{0}, top{0}, width{0}, height{0}
    {

    }

    /**
     * @brief Construct the rectangle from its coordinates
     *
     * Be careful, the last two parameters are the width
     * and height, not the right and bottom coordinates!
     *
     * @param rectLeft Left coordinate of the rectangle
     * @param rectTop Top coordinate of the rectangle
     * @param rectWidth Width of the rectangle
     * @param rectHeight Height of the rectangle
     */
    constexpr Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight) noexcept
    : left{rectLeft}, top{rectTop}, width{rectWidth}, height{rectHeight}
    {

    }

    /**
     * @brief Construct the rectangle from position and size
     *
     * Be careful, the last parameter is the size,
     * not the bottom-right corner!
     *
     * @param rectPosition Position of the top left corner of the rectangle
     * @param rectSize Size of the rectangle
     */
    Rect(const Vector<T, 2>& rectPosition, const Vector<T, 2>& rectSize) noexcept
    : position(rectPosition), size(rectSize)
    {

    }

    /**
     * @brief Get the position of the rectangle
     *
     * It is a synonym for the `position` member
     *
     * @return The position of the rectangle
     * @sa getSize()
     */
    constexpr Vector<T, 2> getPosition() const noexcept {
      return position;
    }

    /**
     * @brief Get the size of the rectangle
     *
     * It is a synonym for the `size` member
     *
     * @return The size of the rectangle
     * @sa getPosition()
     */
    constexpr Vector<T, 2> getSize() const noexcept {
      return size;
    }

    /**
     * @brief Check if the rectangle is empty
     *
     * An empty rectangle is a rectangle that has one of its size coordinates
     * that is zero.
     *
     * @return True if the rectangle is empty
     */
    constexpr bool isEmpty() const noexcept {
      return width == 0 || height == 0;
    }

    /**
     * @brief Get the center of the rectangle
     *
     * @return The center of the rectangle
     */
    constexpr Vector<T, 2> getCenter() const noexcept {
      return { left + width / 2, top + height / 2 };
    }

    /**
     * @brief Get the top left corner
     *
     * @return The top left corner
     */
    constexpr Vector<T, 2> getTopLeft() const noexcept {
      return { left, top };
    }

    /**
     * @brief Get the top right corner
     *
     * @return The top right corner
     */
    constexpr Vector<T, 2> getTopRight() const noexcept {
      return { left + width, top };
    }

    /**
     * @brief Get the bottom left corner
     *
     * @return The bottom left corner
     */
    constexpr Vector<T, 2> getBottomLeft() const noexcept {
      return { left, top + height };
    }

    /**
     * @brief Get the bottom right corner
     *
     * @return The bottom right corner
     */
    constexpr Vector<T, 2> getBottomRight() const noexcept {
      return { left + width, top + height };
    }


    /**
     * @brief Check if a point is inside the rectangle's area
     *
     * @param point Point to test
     * @return True if the point is inside, false otherwise
     * @sa intersects()
     */
    constexpr bool contains(const Vector<T, 2>& point) const noexcept {
      return left <= point.x && point.x < left + width && top <= point.y && point.y < top + height;
    }

    /**
     * @brief Check if a rectangle is inside the rectangle's area
     *
     * @param other Rectangle to test
     * @return True if the rectangle is inside, false otherwise
     * @sa intersects()
     */
    constexpr bool contains(const Rect<T>& other) const noexcept {
      return left <= other.left && other.left + other.width <= left + width
          && top <= other.top && other.top + other.height <= top + height;
    }

    /**
     * @brief Check the intersection between two rectangles
     *
     * @param other Rectangle to test
     * @return True if rectangles overlap, false otherwise
     * @sa contains()
     */
    constexpr bool intersects(const Rect<T>& other) const noexcept {
      return left + width > other.left && left < other.left + other.width
          && top + height > other.top && top < other.top + other.height;
    }

    /**
     * @brief Check the intersection between two rectangles
     *
     * This overload returns the overlapped rectangle in the
     * `result` parameter.
     *
     * @param other Rectangle to test
     * @param result Rectangle to be filled with the intersection
     * @return True if rectangles overlap, false otherwise
     * @sa contains
     */
    bool intersects(const Rect<T>& other, Rect<T>& result) const noexcept {
      if (!intersects(other)) {
        result = Rect<T>();
        return false;
      }

      T resultLeft = std::max(left, other.left);
      T resultTop = std::max(top, other.top);
      T resultRight = std::min(left + width, other.left + other.width);
      T resultBottom = std::min(top + height, other.top + other.height);

      result = Rect<T>(resultLeft, resultTop, resultRight - resultLeft, resultBottom - resultTop);
      return true;
    }

    /**
     * @brief Extend the rectangle
     *
     * @param value The amount to extend
     * @return A new extended rectangle
     * @sa shrink()
     */
    constexpr Rect<T> extend(T value) const noexcept {
      return Rect<T>(left - value, top - value, width + 2 * value, height + 2 * value);
    }

    /**
     * @brief Shrink the rectangle
     *
     * @param value The amount to shrink
     * @return A new shrinked rectangle
     * @sa extend()
     */
    constexpr Rect<T> shrink(T value) const noexcept {
      return Rect<T>(left + value, top + value, width - 2 * value, height - 2 * value);
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
  extern template struct Rect<float>;
  extern template struct Rect<int>;
  extern template struct Rect<unsigned>;
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
    return lhs.position == rhs.position && lhs.size == rhs.size;
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
    return lhs.position != rhs.position || lhs.size != rhs.size;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RECT_H
