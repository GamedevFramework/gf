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
#ifndef GF_RECT_H
#define GF_RECT_H

#include <cstddef>
#include <algorithm>
#include <limits>
#include <tuple>

#include "Anchor.h"
#include "CoreApi.h"
#include "Quarter.h"
#include "Vector.h"
#include "VectorOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_geometry
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
   * @snippet snippets/doc_class_rect.cc rect
   *
   * @sa gf::Circ
   */
  template<typename T>
  struct Rect {
    /**
     * @brief Default constructor
     *
     * Creates a rectangle of size zero located at the origin.
     */
    constexpr Rect() noexcept
    : min(gf::Zero)
    , max(gf::Zero)
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
     * @brief Create a rectangle from a size
     *
     * The position of the rectangle is assumed to be at @f$ (0, 0) @f$
     *
     * @param size The size of the rectangle
     * @returns A new rectangle
     */
    static constexpr Rect<T> fromSize(Vector<T, 2> size) noexcept {
      return Rect<T>({ T(0), T(0) }, size);
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
      return Rect<T>(Vector<T, 2>(std::numeric_limits<T>::max()), Vector<T, 2>(std::numeric_limits<T>::lowest()));
    }

    /**
     * @brief Get the position of the rectangle
     *
     * This is the same as `min`.
     *
     * @returns The position of the rectangle
     */
    constexpr Vector<T, 2> getPosition() const noexcept {
      return min;
    }

    /**
     * @brief Get the size of the rectangle
     *
     * @returns The size of the rectangle
     */
    constexpr Vector<T, 2> getSize() const noexcept {
      return max - min;
    }

    /**
     * @brief Check if the rectangle is empty
     *
     * @returns True if the rectangle is empty
     */
    constexpr bool isEmpty() const noexcept {
      return min.x >= max.x || min.y >= max.y;
    }

    /**
     * @brief Get the center of the rectangle
     *
     * @returns The center of the rectangle
     */
    constexpr Vector<T, 2> getCenter() const noexcept {
      return min + (max - min) / 2;
    }

    /**
     * @brief Check if a point is inside the rectangle
     *
     * @param point The point to test
     * @returns True if the point is inside the rectangle
     */
    constexpr bool contains(Vector<T, 2> point) const noexcept {
      return min.x <= point.x && point.x < max.x && min.y <= point.y && point.y < max.y;
    }

    /**
     * @brief Check if a rectangle is totally inside the rectangle
     *
     * @param other Another rectangle to test
     * @returns True if the tested rectangle is inside the current rectangle
     */
    constexpr bool contains(const Rect& other) const noexcept {
      return min.x <= other.min.x && other.max.x <= max.x && min.y <= other.min.y && other.max.y <= max.y;
    }

    /**
     * @brief Check if two rectangles interset
     *
     * @param other Another rectangle to test
     * @returns True if the two rectangles interset
     */
    constexpr bool intersects(const Rect& other) const noexcept {
      return min.x < other.max.x && other.min.x < max.x && min.y < other.max.y && other.min.y < max.y;
    }

    /**
     * @brief Check if two rectangles interset and get the intersetion rectangle
     *
     * @param other Another rectangle to test
     * @param result A reference to put the result of the intersection (in case there is any)
     * @returns True if the two rectangles interset
     */
    constexpr bool intersects(const Rect& other, Rect& result) const noexcept {
      if (!intersects(other)) {
        return false;
      }

      result = getIntersection(other);
      return true;
    }

    /**
     * @brief Compute the intersection of two rectangles
     *
     * @param other Another rectangle to test
     * @returns The result of the intersection
     */
    constexpr Rect getIntersection(const Rect& other) const noexcept {
      Rect res;

      res.min.x = std::max(min.x, other.min.x);
      res.min.y = std::max(min.y, other.min.y);
      res.max.x = std::min(max.x, other.max.x);
      res.max.y = std::min(max.y, other.max.y);

      return res;
    }

    /**
     * @brief Get the volume of the intersection
     *
     * @param other Another rectangle to test
     * @returns The volume of the intersection or 0 in case there is no intersection
     */
    constexpr T getIntersectionVolume(const Rect& other) const noexcept {
      auto xMin = std::max(min.x, other.min.x);
      auto xMax = std::min(max.x, other.max.x);

      if (xMin >= xMax) {
        return T(0);
      }

      auto yMin = std::max(min.y, other.min.y);
      auto yMax = std::min(max.y, other.max.y);

      if (yMin >= yMax) {
        return T(0);
      }

      return (xMax - xMin) * (yMax - yMin);
    }

    /**
     * @brief Get the extent length of the intersection
     *
     * @param other Another rectangle to test
     * @returns The extent length of the intersection or 0 in case there is no intersection
     */
    constexpr T getIntersectionExtentLength(const Rect& other) const noexcept {
      auto xMin = std::max(min.x, other.min.x);
      auto xMax = std::min(max.x, other.max.x);

      if (xMin >= xMax) {
        return T(0);
      }

      auto yMin = std::max(min.y, other.min.y);
      auto yMax = std::min(max.y, other.max.y);

      if (yMin >= yMax) {
        return T(0);
      }

      return (xMax - xMin) + (yMax - yMin);
    }

    /**
     * @brief Extend the rectangle with a point (as array)
     *
     * @param point The point of extension
     */
    constexpr void extend(const T (&point)[2]) noexcept {
      min.x = std::min(min.x, point[0]);
      min.y = std::min(min.y, point[1]);
      max.x = std::max(max.x, point[0]);
      max.y = std::max(max.y, point[1]);
    }

    /**
     * @brief Extend the rectangle with a point
     *
     * @param point The point of extension
     */
    constexpr void extend(Vector<T, 2> point) noexcept {
      min.x = std::min(min.x, point.x);
      min.y = std::min(min.y, point.y);
      max.x = std::max(max.x, point.x);
      max.y = std::max(max.y, point.y);
    }

    /**
     * @brief Extend the rectangle with a rectangle
     *
     * @param other The rectangle of extension
     */
    constexpr void extend(const Rect& other) noexcept {
      min.x = std::min(min.x, other.min.x);
      min.y = std::min(min.y, other.min.y);
      max.x = std::max(max.x, other.max.x);
      max.y = std::max(max.y, other.max.y);
    }

    /**
     * @brief Get the rectangle extended by another rectangle
     *
     * @param other The rectangle of extension
     */
    constexpr Rect getExtended(const Rect& other) const noexcept {
      Rect res(*this);
      res.extend(other);
      return res;
    }

    /**
     * @brief Get the volume of the rectangle
     *
     * @returns The volume of the rectangle
     */
    constexpr T getVolume() const noexcept {
      return (max.x - min.x) * (max.y - min.y);
    }

    /**
     * @brief Get the extent length of the rectangle
     *
     * @returns The extent length of the rectangle
     */
    constexpr T getExtentLength() const noexcept {
      return (max.x - min.x) + (max.y - min.y);
    }

    /**
     * @brief Get the minimum edge of the rectangle
     *
     * @returns The length of the minimum edge of the rectangle
     */
    constexpr T getMinimumEdge() const noexcept {
      return std::min(max.x - min.x, max.y - min.y);
    }

    /**
     * @brief Grow the rectangle
     *
     * @param value The amount to grow
     * @return A new extended rectangle
     * @sa shrink()
     */
    constexpr Rect grow(T value) const noexcept {
      return Rect(min - value, max + value);
    }

    /**
     * @brief Shrink the rectangle
     *
     * @param value The amount to shrink
     * @return A new shrinked rectangle
     * @sa grow()
     */
    constexpr Rect shrink(T value) const noexcept {
      return Rect(min + value, max - value);
    }

    /**
     * @brief Ensures that min coordinates are less than max coordinates
     */
    constexpr void normalize() noexcept {
      if (min.x > max.x) { std::swap(min.x, max.x); }
      if (min.y > max.y) { std::swap(min.y, max.y); }
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

    Vector<T, 2> min; ///< The minimum point of the rectangle
    Vector<T, 2> max; ///< The maximum point of the rectangle

  private:
    constexpr Rect(Vector<T, 2> min, Vector<T, 2> max) noexcept
    : min(min)
    , max(max)
    {
    }
  };

  /**
   * @ingroup core_geometry
   * @brief A `float` rectangle
   *
   * @sa gf::Rect
   */
  using RectF = Rect<float>;

  /**
   * @ingroup core_geometry
   * @brief A `double` rectangle
   *
   * @sa gf::Rect
   */
  using RectD = Rect<double>;

  /**
   * @ingroup core_geometry
   * @brief A `int` rectangle
   *
   * @sa gf::Rect
   */
  using RectI = Rect<int>;

  /**
   * @ingroup core_geometry
   * @brief A `unsigned` rectangle
   *
   * @sa gf::Rect
   */
  using RectU = Rect<unsigned>;

  /**
   * @ingroup core_geometry
   * @brief A `std::size_t` rectangle
   *
   * @sa gf::Rect
   */
  using RectZ = Rect<std::size_t>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Rect<float>;
  extern template struct GF_CORE_API Rect<double>;
  extern template struct GF_CORE_API Rect<int>;
  extern template struct GF_CORE_API Rect<unsigned>;
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
   * @ingroup core_geometry
   * @brief Divide a rectangle in quarters
   *
   * @param other The rectangle to divide
   * @param quarter The quarter to compute
   */
  template<typename T>
  constexpr
  Rect<T> computeBoxQuarter(const Rect<T>& other, Quarter quarter) {
    Vector<T, 2> size = (other.max - other.min) / 2;

    switch (quarter) {
      case Quarter::UpperLeft:
        return Rect<T>::fromMinMax(other.min, other.max - size);
      case Quarter::UpperRight:
        return Rect<T>::fromMinMax({ other.min.x + size.width, other.min.y }, { other.max.x, other.max.y - size.height });
      case Quarter::LowerRight:
        return Rect<T>::fromMinMax(other.min + size, other.max);
      case Quarter::LowerLeft:
        return Rect<T>::fromMinMax({ other.min.x, other.min.y + size.height }, { other.max.x - size.width, other.max.y });
    }

    return other;
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
