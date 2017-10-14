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
#ifndef GF_RANGE_H
#define GF_RANGE_H

#include <cstddef>
#include <iterator>

#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A half-open range of values
   *
   * gf::Range represents a half-open range of values.
   *
   */
  template<typename T>
  struct Range {
    T lo; ///< The lower endpoint (included)
    T hi; ///< The higher endpoint (excluded)

    /**
     * @ingroup core
     * @brief A range iterator
     */
    struct Iterator : public std::iterator<std::input_iterator_tag, T> {
      T index; ///< The index in the range

      /**
       * @brief Constructor
       *
       * @param iteratorIndex Index in the range
       */
      constexpr Iterator(T iteratorIndex) noexcept
      : index(iteratorIndex)
      {

      }

      /**
       * @brief Dereference operator
       *
       * @return The index
       */
      T operator*() noexcept {
        return index;
      }

      /**
       * @brief Increment operator (prefix)
       *
       * @return The iterator
       */
      Iterator& operator++() noexcept {
        ++index;
        return *this;
      }

      /**
       * @brief Increment operator (postfix)
       *
       * @return The iterator
       */
      Iterator operator++(int) noexcept {
        Iterator copy = *this;
        ++index;
        return copy;
      }

      /**
       * @brief Inequality operator
       *
       * @param other Another iterator
       * @return True if the iterator are different
       */
      constexpr bool operator!=(const Iterator& other) const noexcept {
        return index != other.index;
      }

      /**
       * @brief Equality operator
       *
       * @param other Another iterator
       * @return True if the iterator are the same
       */
      constexpr bool operator==(const Iterator& other) const noexcept {
        return index == other.index;
      }

    };

    /**
     * @brief Check if a value is in a range
     *
     * @param value The value to test
     * @return True if the value is in the range
     */
    constexpr bool contains(T value) const noexcept {
      return lo <= value && value < hi;
    }

    /**
     * @brief Get a begin iterator
     *
     * @return A begin iterator
     * @sa end()
     */
    constexpr Iterator begin() const noexcept {
      return Iterator(lo);
    }

    /**
     * @brief Get a end iterator
     *
     * @return A end iterator
     * @sa begin()
     */
    constexpr Iterator end() const noexcept {
      return Iterator(hi);
    }

    /**
     * @brief Get the length of the range
     *
     * @return The length of the range
     */
    constexpr T length() const noexcept {
      return hi - lo;
    }

    /**
     * @brief Check if the range is empty
     *
     * @return True if the range is empty
     * @sa isValid()
     */
    constexpr bool isEmpty() const noexcept {
      return lo >= hi;
    }

    /**
     * @brief Check is the range is valid
     *
     * @return True if the range is valid
     * @sa isEmpty()
     */
    constexpr bool isValid() const noexcept {
      return lo <= hi;
    }

  };

  /**
   * @ingroup core
   * @brief A float range
   *
   * @sa gf::Range
   */
  using RangeF = Range<float>;

  /**
   * @ingroup core
   * @brief A int range
   *
   * @sa gf::Range
   */
  using RangeI = Range<int>;

  /**
   * @ingroup core
   * @brief A unsigned range
   *
   * @sa gf::Range
   */
  using RangeU = Range<unsigned>;

  /**
   * @ingroup core
   * @brief A `std::size_t` range
   *
   * @sa gf::Range
   */
  using RangeZ = Range<std::size_t>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct Range<float>;
  extern template struct Range<int>;
  extern template struct Range<unsigned>;
#endif



  /**
   * @ingroup core
   * @brief A 2D range
   *
   * gf::PositionRange represents a range across a 2D area.
   */
  template<typename T>
  struct PositionRange {
    Range<T> first;   ///< The range in the first dimension
    Range<T> second;  ///< The range in the second dimension

    /**
     * @brief An iterator for a 2D range
     */
    struct Iterator : public std::iterator<std::input_iterator_tag, T> {
      Range<T> range;
      Vector<T, 2> position;

      /**
       * @brief Constructor
       *
       * @param iteratorRange The range in the first dimension
       * @param iteratorPosition The current position in 2D
       */
      constexpr Iterator(Range<T> iteratorRange, Vector<T, 2> iteratorPosition) noexcept
      : range(iteratorRange)
      , position(iteratorPosition)
      {

      }

      /**
       * @brief Dereference operator
       *
       * @return The position
       */
      Vector<T, 2> operator*() noexcept {
        return position;
      }

      /**
       * @brief Increment operator (prefix)
       *
       * @return The iterator
       */
      Iterator& operator++() noexcept {
        step();
        return *this;
      }

      /**
       * @brief Increment operator (postfix)
       *
       * @return The iterator
       */
      Iterator operator++(int) noexcept {
        Iterator copy = *this;
        step();
        return copy;
      }

      /**
       * @brief Inequality operator
       *
       * @param other Another iterator
       * @return True if the iterator are different
       */
      constexpr bool operator!=(const Iterator& other) const noexcept {
        return position.x != other.position.x || position.y != other.position.y;
      }

      /**
       * @brief Equality operator
       *
       * @param other Another iterator
       * @return True if the iterator are the same
       */
      constexpr bool operator==(const Iterator& other) const noexcept {
        return position.x == other.position.x && position.y == other.position.y;
      }

    private:
      void step() {
        ++position.x;

        if (position.x >= range.hi) {
          position.x = range.lo;
          ++position.y;
        }
      }
    };

    /**
     * @brief Get a begin iterator
     *
     * @return A begin iterator
     * @sa end()
     */
    constexpr Iterator begin() const noexcept {
      return Iterator(first, { first.lo, second.lo });
    }

    /**
     * @brief Get a end iterator
     *
     * @return A end iterator
     * @sa begin()
     */
    constexpr Iterator end() const noexcept {
      return Iterator(first, { first.lo, second.hi });
    }

  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RANGE_H
