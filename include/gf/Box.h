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
#ifndef GF_BOX_H
#define GF_BOX_H

#include <cstddef>
#include <algorithm>
#include <limits>
#include <tuple>

#include "Portability.h"
#include "Vector.h"
#include "VectorOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A multi-dimensional box
   *
   * A box is defined by its minimum coordinates (included) and its
   * maximum coordinates (excluded).
   *
   * @sa gf::Rect
   */
  template<typename T, std::size_t N>
  struct Box {
    Vector<T, N> min; ///< The minimum point of the box
    Vector<T, N> max; ///< The maximum point of the box

    /**
     * @brief Default constructor
     */
    constexpr Box() noexcept
    {
      for (std::size_t i = 0; i < N; ++i) {
        min[i] = std::numeric_limits<T>::max();
        max[i] = std::numeric_limits<T>::lowest();
      }
    }

    /**
     * @brief Constructor with two points
     *
     * @param p0 The first point in the box
     * @param p1 The second point in the box
     */
    constexpr Box(Vector<T, N> p0, Vector<T, N> p1) noexcept
    : min(gf::Zero)
    , max(gf::Zero)
    {
      for (std::size_t i = 0; i < N; ++i) {
        if (p0[i] < p1[i]) {
          min[i] = p0[i];
          max[i] = p1[i];
        } else {
          min[i] = p1[i];
          max[i] = p0[i];
        }
      }
    }

    /**
     * @brief Constructor with two points (as arrays)
     *
     * @param p0 The first point in the box
     * @param p1 The second point in the box
     */
    constexpr Box(const T (&p0)[N], const T (&p1)[N]) noexcept
    : min(gf::Zero)
    , max(gf::Zero)
    {
      for (std::size_t i = 0; i < N; ++i) {
        if (p0[i] < p1[i]) {
          min[i] = p0[i];
          max[i] = p1[i];
        } else {
          min[i] = p1[i];
          max[i] = p0[i];
        }
      }
    }

    /**
     * @brief Constructor with one point
     *
     * @param p The point in the box
     */
    constexpr explicit Box(Vector<T, N> p) noexcept
    : min(p)
    , max(p)
    {

    }

    /**
     * @brief Get the position of the box
     *
     * This is the same as `min`.
     *
     * @returns The position of the box
     */
    constexpr Vector<T, N> getPosition() const noexcept {
      return min;
    }

    /**
     * @brief Get the size of the box
     *
     * @returns The size of the box
     */
    constexpr Vector<T, N> getSize() const noexcept {
      return max - min;
    }

    /**
     * @brief Check if the box is empty
     *
     * @returns True if the box is empty
     */
    constexpr bool isEmpty() const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (min[i] >= max[i]) {
          return true;
        }
      }

      return false;
    }

    /**
     * @brief Get the center of the box
     *
     * @returns The center of the box
     */
    constexpr Vector<T, N> getCenter() const noexcept {
      return min + (max - min) / 2;
    }

    /**
     * @brief Check if a point is inside the box
     *
     * @param point The point to test
     * @returns True if the point is inside the box
     */
    constexpr bool contains(Vector<T, N> point) const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (!(min[i] <= point[i] && point[i] < max[i])) {
          return false;
        }
      }

      return true;
    }

    /**
     * @brief Check if a box is totally inside the box
     *
     * @param other Another box to test
     * @returns True if the tested box is inside the current box
     */
    constexpr bool contains(const Box<T, N>& other) const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (!(min[i] <= other.min[i] && other.max[i] <= max[i])) {
          return false;
        }
      }

      return true;
    }

    /**
     * @brief Check if two boxes interset
     *
     * @param other Another box to test
     * @returns True if the two boxes interset
     */
    constexpr bool intersects(const Box<T, N>& other) const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (!(min[i] < other.max[i] && other.min[i] < max[i])) {
          return false;
        }
      }

      return true;
    }

    /**
     * @brief Check if two boxes interset and get the intersetion box
     *
     * @param other Another box to test
     * @param result A reference to put the result of the intersection (in case there is any)
     * @returns True if the two boxes interset
     */
    constexpr bool intersects(const Box<T, N>& other, Box<T, N>& result) const noexcept {
      if (!intersects(other)) {
        return false;
      }

      result = getIntersection(other);
      return true;
    }

    /**
     * @brief Compute the intersection of two boxes
     *
     * @param other Another box to test
     * @returns The result of the intersection
     */
    constexpr Box<T, N> getIntersection(const Box<T, N>& other) const noexcept {
      Box<T, N> res;

      for (std::size_t i = 0; i < N; ++i) {
        res.min[i] = std::max(min[i], other.min[i]);
        res.max[i] = std::min(max[i], other.max[i]);
      }

      return res;
    }

    /**
     * @brief Get the volume of the intersection
     *
     * @param other Another box to test
     * @returns The volume of the intersection or 0 in case there is no intersection
     */
    constexpr T getIntersectionVolume(const Box<T, N>& other) const noexcept {
      T res = 1;

      for (std::size_t i = 0; i < N; ++i) {
        auto axisMin = std::max(min[i], other.min[i]);
        auto axisMax = std::min(max[i], other.max[i]);

        if (axisMin >= axisMax) {
          return T();
        }

        res *= (axisMax - axisMin);
      }

      return res;
    }

    /**
     * @brief Get the extent length of the intersection
     *
     * @param other Another box to test
     * @returns The extent length of the intersection or 0 in case there is no intersection
     */
    constexpr T getIntersectionExtentLength(const Box<T, N>& other) const noexcept {
      T res = 0;

      for (std::size_t i = 0; i < N; ++i) {
        auto axisMin = std::max(min[i], other.min[i]);
        auto axisMax = std::min(max[i], other.max[i]);

        if (axisMin >= axisMax) {
          return T();
        }

        res += (axisMax - axisMin);
      }

      return res;
    }

    /**
     * @brief Extend the box with a point (as array)
     *
     * @param point The point of extension
     */
    constexpr void extend(const T (&point)[N]) noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        T axisMin = min[i];
        T axisMax = max[i];
        std::tie(min[i], max[i]) = std::minmax({ axisMin, axisMax, point[i] });
      }
    }

    /**
     * @brief Extend the box with a point
     *
     * @param point The point of extension
     */
    constexpr void extend(Vector<T, N> point) noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        T axisMin = min[i];
        T axisMax = max[i];
        std::tie(min[i], max[i]) = std::minmax({ axisMin, axisMax, point[i] });
      }
    }

    /**
     * @brief Extend the box with a box
     *
     * @param other The box of extension
     */
    constexpr void extend(const Box<T, N>& other) noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        min[i] = std::min(min[i], other.min[i]);
        max[i] = std::max(max[i], other.max[i]);
      }
    }

    /**
     * @brief Get the box extended by another box
     *
     * @param other The box of extension
     */
    constexpr Box<T, N> getExtended(const Box<T, N>& other) const noexcept {
      Box<T, N> res(*this);
      res.extend(other);
      return res;
    }

    /**
     * @brief Get the volume of the box
     *
     * @returns The volume of the box
     */
    constexpr T getVolume() const noexcept {
      T volume = 1;

      for (std::size_t i = 0; i < N; ++i) {
        volume *= (max[i] - min[i]);
      }

      return volume;
    }

    /**
     * @brief Get the extent length of the box
     *
     * @returns The extent length of the box
     */
    constexpr T getExtentLength() const noexcept {
      T distance = 0;

      for (std::size_t i = 0; i < N; ++i) {
        distance += (max[i] - min[i]);
      }

      return distance;
    }

    /**
     * @brief Get the minimum edge of the box
     *
     * @returns The length of the minimum edge of the box
     */
    constexpr T getMinimumEdge() const noexcept {
      T minimum = max[0] - min[0];

      for (std::size_t i = 1; i < N; ++i) {
        minimum = std::min(minimum, max[i] - min[i]);
      }

      return minimum;
    }

    /**
     * @brief Grow the box
     *
     * @param value The amount to grow
     * @return A new extended box
     * @sa shrink()
     */
    constexpr Box<T, 2> grow(T value) const noexcept {
      return Box<T, 2>(min - value, max + value);
    }

    /**
     * @brief Shrink the box
     *
     * @param value The amount to shrink
     * @return A new shrinked box
     * @sa grow()
     */
    constexpr Box<T, 2> shrink(T value) const noexcept {
      return Box<T, 2>(min + value, max - value);
    }

    /**
     * @brief Ensures that min coordinates are less than max coordinates
     */
    constexpr void normalize() noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (min[i] > max[i]) {
          std::swap(min[i], max[i]);
        }
      }
    }
  };

  /**
   * @ingroup core
   * @brief A box of dimension 2 with `float`
   */
  using Box2f = Box<float, 2>;

  /**
   * @ingroup core
   * @brief A box of dimension 3 with `float`
   */
  using Box3f = Box<float, 3>;

  /**
   * @ingroup core
   * @brief A box of dimension 2 with `int`
   */
  using Box2i = Box<int, 2>;

  /**
   * @ingroup core
   * @brief A box of dimension 3 with `int`
   */
  using Box3i = Box<int, 3>;

  /**
   * @ingroup core
   * @brief A box of dimension 2 with `unsigned`
   */
  using Box2u = Box<unsigned, 2>;

  /**
   * @ingroup core
   * @brief A box of dimension 3 with `unsigned`
   */
  using Box3u = Box<unsigned, 3>;


  /**
   * @relates Box
   * @brief Equality operator
   */
  template<typename T, std::size_t N>
  constexpr
  bool operator==(const Box<T, N>& lhs, const Box<T, N>& rhs) {
    return lhs.min == rhs.min && lhs.max == rhs.max;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BOX_H
