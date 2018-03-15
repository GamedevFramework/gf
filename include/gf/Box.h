/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A multi-dimensional box
   *
   * @sa gf::Rect
   */
  template<typename T, std::size_t N>
  struct Box {
    Vector<T, N> min;
    Vector<T, N> max;

    constexpr Box() noexcept
    {
      for (std::size_t i = 0; i < N; ++i) {
        min[i] = std::numeric_limits<T>::max();
        max[i] = std::numeric_limits<T>::lowest();
      }
    }

    constexpr Box(Vector<T, N> p0, Vector<T, N> p1)
    {
      for (std::size_t i = 0; i < N; ++i) {
        std::tie(min[i], max[i]) = std::minmax(p0[i], p1[i]);
      }
    }

    constexpr Box(const T (&p0)[N], const T (&p1)[N]) {
      for (std::size_t i = 0; i < N; ++i) {
        std::tie(min[i], max[i]) = std::minmax(p0[i], p1[i]);
      }
    }

    constexpr Vector<T, N> getSize() const noexcept {
      return max - min;
    }

    constexpr bool isEmpty() const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (min[i] >= max[i]) {
          return true;
        }
      }

      return false;
    }

    constexpr Vector<T, N> getCenter() const noexcept {
      return (min + max) / 2;
    }

    constexpr bool contains(Vector<T, N> point) const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (!(min[i] <= point[i] && point[i] <= max[i])) {
          return false;
        }
      }

      return true;
    }

    constexpr bool contains(const Box<T, N>& other) const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (!(min[i] <= other.min[i] && other.max[i] <= max[i])) {
          return false;
        }
      }

      return true;
    }

    constexpr bool intersects(const Box<T, N>& other) const noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        if (!(min[i] <= other.max[i] && other.min[i] <= max[i])) {
          return false;
        }
      }

      return true;
    }

    constexpr void extend(const T (&point)[N]) {
      for (std::size_t i = 0; i < N; ++i) {
        std::tie(min[i], max[i]) = std::minmax({ min[i], max[i], point[i] });
      }
    }

    constexpr void extend(Vector<T, N> point) {
      for (std::size_t i = 0; i < N; ++i) {
        std::tie(min[i], max[i]) = std::minmax({ min[i], max[i], point[i] });
      }
    }

    constexpr void extend(const Box<T, N>& other) {
      for (std::size_t i = 0; i < N; ++i) {
        min[i] = std::min(min[i], other.min[i]);
        max[i] = std::max(max[i], other.max[i]);
      }
    }

    Box<T, N> getExtended(const Box<T, N>& other) {
      Box<T, N> res(*this);
      res.extend(other);
      return res;
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

  using Box2f = Box<float, 2>;
  using Box3f = Box<float, 3>;

  using Box2i = Box<int, 2>;
  using Box3i = Box<int, 3>;

  using Box2u = Box<unsigned, 2>;
  using Box3u = Box<unsigned, 3>;


  enum Quadrant {
    UpperLeft,
    UpperRight,
    LowerRight,
    LowerLeft,
  };

  template<typename T>
  constexpr Box<T, 2> computeBoxQuadrant(const Box<T, 2>& box, Quadrant quadrant) {
    constexpr Vector<T, 2> size = (box.max - box.min) / 2;

    switch (quadrant) {
      case Quadrant::UpperLeft:
        return Box<T,2>(box.min, box.max - size);
      case Quadrant::UpperRight:
        return Box<T,2>({ box.min.x + size.w, box.min.y }, { box.max.x, box.max.y - size.h });
      case Quadrant::LowerRight:
        return Box<T,2>(box.min + size, box.max);
      case Quadrant::LowerLeft:
        return Box<T,2>({ box.min.x, box.min.y + size.h }, { box.max.x - size.w, box.max.y });
    }

    return box;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BOX_H
