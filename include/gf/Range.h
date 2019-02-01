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
    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::bidirectional_iterator_tag;

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
       * @brief Swap the iterator with another iterator
       */
      void swap(Iterator& other) {
        using std::swap;
        swap(index, other.index);
      }

      /**
       * @brief Dereference operator
       *
       * @return The index
       */
      reference operator*() noexcept {
        return index;
      }

      /**
       * @brief Pointer operator
       *
       * @return The index
       */
      pointer operator->() noexcept {
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
       * @brief Decrement operator (prefix)
       *
       * @return The iterator
       */
      Iterator& operator--() noexcept {
        --index;
        return *this;
      }

      /**
       * @brief Decrement operator (postfix)
       *
       * @return The iterator
       */
      Iterator operator--(int) noexcept {
        Iterator copy = *this;
        --index;
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
   * @relates Range::Iterator
   * @brief Swap two range iterators
   */
  template<typename T>
  inline
  void swap(typename Range<T>::Iterator& lhs, typename Range<T>::Iterator& rhs) {
    lhs.swap(rhs);
  }

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
    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = Vector<T, 2>;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

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
       * @brief Swap the iterator with another iterator
       */
      void swap(Iterator& other) {
        using std::swap;
        swap(range, other.range);
        swap(position, other.position);
      }

      /**
       * @brief Dereference operator
       *
       * @returns The position
       */
      reference operator*() noexcept {
        return position;
      }

      /**
       * @brief Pointer operator
       *
       * @returns The position
       */
      pointer operator->() noexcept {
        return position;
      }

      /**
       * @brief Increment operator (prefix)
       *
       * @returns The iterator
       */
      Iterator& operator++() noexcept {
        step();
        return *this;
      }

      /**
       * @brief Increment operator (postfix)
       *
       * @returns The iterator
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

  /**
   * @relates PositionRange::Iterator
   * @brief Swap two range iterators
   */
  template<typename T>
  inline
  void swap(typename PositionRange<T>::Iterator& lhs, typename PositionRange<T>::Iterator& rhs) {
    lhs.swap(rhs);
  }


  /**
   * @ingroup core
   * @brief A 2D range
   *
   * gf::NeighborSquareRange represents a range accross a 2D area around an origin (not included).
   */
  template<typename T>
  struct NeighborSquareRange {
    Range<T> first;   ///< The range in the first dimension
    Range<T> second;  ///< The range in the second dimension
    Vector<T, 2> origin;

    /**
     * @brief An iterator for a 2D range
     */
    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = Vector<T, 2>;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      Range<T> range;
      Vector<T, 2> position;
      Vector<T, 2> origin;

      /**
       * @brief Constructor
       *
       * @param iteratorRange The range in the first dimension
       * @param iteratorPosition The current position in 2D
       * @param iteratorOrigin The position of the origin
       */
      constexpr Iterator(Range<T> iteratorRange, Vector<T, 2> iteratorPosition, Vector<T, 2> iteratorOrigin) noexcept
      : range(iteratorRange)
      , position(iteratorPosition)
      , origin(iteratorOrigin)
      {

      }

      /**
       * @brief Swap the iterator with another iterator
       */
      void swap(Iterator& other) {
        using std::swap;
        swap(range, other.range);
        swap(position, other.position);
        swap(origin, other.origin);
      }

      /**
       * @brief Dereference operator
       *
       * @returns The position
       */
      reference operator*() noexcept {
        return position;
      }

      /**
       * @brief Pointer operator
       *
       * @returns The position
       */
      pointer operator->() noexcept {
        return position;
      }

      /**
       * @brief Increment operator (prefix)
       *
       * @returns The iterator
       */
      Iterator& operator++() noexcept {
        step();
        return *this;
      }

      /**
       * @brief Increment operator (postfix)
       *
       * @returns The iterator
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
        do {
          ++position.x;

          if (position.x >= range.hi) {
            position.x = range.lo;
            ++position.y;
          }
        } while (position.x == origin.x && position.y == origin.y);
      }
    };

    /**
     * @brief Get a begin iterator
     *
     * @return A begin iterator
     * @sa end()
     */
    constexpr Iterator begin() const noexcept {
      return Iterator(first, { first.lo, second.lo }, origin);
    }

    /**
     * @brief Get a end iterator
     *
     * @return A end iterator
     * @sa begin()
     */
    constexpr Iterator end() const noexcept {
      return Iterator(first, { first.lo, second.hi }, origin);
    }

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RANGE_H
