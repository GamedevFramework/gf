/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <cassert>
#include <vector>

#include "Portability.h"
#include "Range.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A two-dimensional array
   *
   * gf::Array represents a two-dimensional array, organized in row-major order.
   *
   * Contrary to the usual way of accessing 2D arrays, the first coordinate is
   * the column and the second coordinate is the row. So that, if `size` is the
   * size of the array and `pos` if the position in the array:
   *
   * - @f$ 0 \leq \mathtt{pos.x} = \mathtt{pos.col} < \mathtt{size.width} = \mathtt{size.col} @f$
   * - @f$ 0 \leq \mathtt{pos.y} = \mathtt{pos.row} < \mathtt{size.height} = \mathtt{size.row} @f$
   *
   * Some convinient visitors are provided to visit the four neighbours (up,
   * down, left and right), or the eight neighbours.
   *
   * @sa gf::Matrix
   */
  template<class T>
  class GF_API Array2D {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty array.
     */
    Array2D()
    : m_size(0, 0)
    {

    }

    /**
     * @brief Constructor with a size
     *
     * @param size The size of the array
     */
    Array2D(Vector2u size)
    : m_size(size)
    , m_data(size.width * size.height)
    {

    }

    /**
     * @brief Constructor with a size and a value
     *
     * @param size The size of the array
     * @param value The initial value in the array
     */
    Array2D(Vector2u size, const T& value)
    : m_size(size)
    , m_data(size.width * size.height, value)
    {

    }

    /**
     * @brief Default copy constructor
     */
    Array2D(const Array2D&) = default;

    /**
     * @brief Default copy assignment
     */
    Array2D& operator=(const Array2D&) = default;

    /**
     * @brief Default move constructor
     */
    Array2D(Array2D&&) = default;

    /**
     * @brief Default move assignement
     */
    Array2D& operator=(Array2D&&) = default;

    /**
     * @name Raw data access
     * @{
     */

    /**
     * @brief Get the pointer to raw data
     *
     * The returned pointer is `const` so you can not modify the array with
     * this function.
     *
     * @return The pointer to raw data
     */
    const T *getDataPtr() const noexcept {
      return m_data.data();
    }

    /**
     * @brief Get the raw data size
     *
     * @return The total number of elements in the array
     */
    std::size_t getDataSize() const noexcept {
      return m_data.size();
    }

    /**
     * @brief Get the size of the array
     *
     * @return The size of the array
     */
    constexpr Vector2u getSize() const noexcept {
      return m_size;
    }

    /**
     * @brief Get the number of columns
     *
     * @return The number of columns
     */
    constexpr unsigned getCols() const noexcept {
      return m_size.col;
    }

    /**
     * @brief Get the number of rows
     *
     * @return The number of rows
     */
    constexpr unsigned getRows() const noexcept {
      return m_size.row;
    }

    /**
     * @brief Check if the array is empty
     *
     * An empty array is an array with @f$ 0 @f$ elements, i.e. either the
     * number of columns is @f$ 0 @f$ or the number of rows is @f$ 0 @f$.
     *
     * @return True if the array is empty
     */
    constexpr bool isEmpty() const noexcept {
      return m_data.empty();
    }

    /** @} */

    /**
     * @name Elements access
     * @{
     */

    /**
     * @brief Get the element at a given 2D position
     *
     * @param pos The 2D position of the element
     */
    T& operator()(Vector2u pos) {
      return get(pos);
    }

    /**
     * @brief Get the element at a given 1D position
     *
     * @param pos The 1D position of the element
     * @sa getPositionRange()
     */
    T& operator()(std::size_t pos) {
      return m_data[pos];
    }

    /**
     * @brief Get the element at a given 2D position
     *
     * @param pos The 2D position of the element
     */
    const T& operator()(Vector2u pos) const {
      return get(pos);
    }

    /**
     * @brief Get the element at a given 1D position
     *
     * @param pos The 1D position of the element
     * @sa getPositionRange()
     */
    const T& operator()(std::size_t pos) const {
      return m_data[pos];
    }

    /**
     * @brief Transform a 1D position into a 2D position
     *
     * @param pos A 1D position
     * @return The corresponding 2D position
     */
    constexpr Vector2u toPosition(std::size_t pos) const noexcept {
      return { pos % m_size.col, pos / m_size.col };
    }

    /** @} */

    /**
     * @name Visitors
     * @{
     */

    /**
     * @brief Visit the 4 neighbours of a given position
     *
     * This function calls a callback function for every neighbour in the
     * vertical and horizontal direction. The function checks if the neighbour
     * actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector2u pos, T value);
     * // pos is the position of the neighbour
     * // value is the value of the neighbour
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit4Neighbours(Vector2u pos, Func func) {
      if (pos.row > 0) {
        Vector2u neighbour{ pos.col, pos.row - 1 };
        func(neighbour, get(neighbour));
      }

      if (pos.row < m_size.row - 1) {
        Vector2u neighbour{ pos.col, pos.row + 1 };
        func(neighbour, get(neighbour));
      }

      if (pos.col > 0) {
        Vector2u neighbour{ pos.col - 1, pos.row };
        func(neighbour, get(neighbour));
      }

      if (pos.col < m_size.col - 1) {
        Vector2u neighbour{ pos.col + 1, pos.row };
        func(neighbour, get(neighbour));
      }
    }

    /**
     * @brief Visit the 4 neighbours of a given position
     *
     * This function calls a callback function for every neighbour in the
     * vertical and horizontal direction. The function checks if the neighbour
     * actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector2u pos, T value);
     * // pos is the position of the neighbour
     * // value is the value of the neighbour
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit4Neighbours(Vector2u pos, Func func) const {
      if (pos.row > 0) {
        Vector2u neighbour{ pos.col, pos.row - 1 };
        func(neighbour, get(neighbour));
      }

      if (pos.row < m_size.row - 1) {
        Vector2u neighbour{ pos.col, pos.row + 1 };
        func(neighbour, get(neighbour));
      }

      if (pos.col > 0) {
        Vector2u neighbour{ pos.col - 1, pos.row };
        func(neighbour, get(neighbour));
      }

      if (pos.col < m_size.col - 1) {
        Vector2u neighbour{ pos.col + 1, pos.row };
        func(neighbour, get(neighbour));
      }
    }

    /**
     * @brief Visit the 8 neighbours of a given position
     *
     * This function calls a callback function for every neighbour in the
     * vertical, horizontal and diagonal direction. The function checks if the
     * neighbour actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector2u pos, T value);
     * // pos is the position of the neighbour
     * // value is the value of the neighbour
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit8Neighbours(const Vector2u& pos, Func func) {
      for (int i = -1; i <= 1; ++i) {
        if (pos.row == 0 && i == -1) {
          continue;
        }

        if (pos.row + 1 == m_size.row && i == 1) {
          continue;
        }

        for (int j = -1; j <= 1; ++j) {
          if (pos.col == 0 && j == -1) {
            continue;
          }

          if (pos.col + 1 == m_size.col && j == 1) {
            continue;
          }

          if (i != 0 || j != 0) {
            Vector2u neighbour{ pos.col + i, pos.row + j };
            func(neighbour, get(neighbour));
          }
        }
      }
    }

    /**
     * @brief Visit the 8 neighbours of a given position
     *
     * This function calls a callback function for every neighbour in the
     * vertical, horizontal and diagonal direction. The function checks if the
     * neighbour actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector2u pos, T value);
     * // pos is the position of the neighbour
     * // value is the value of the neighbour
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit8Neighbours(Vector2u pos, Func func) const {
      for (int i = -1; i <= 1; ++i) {
        if (pos.row == 0 && i == -1) {
          continue;
        }

        if (pos.row + 1 == m_size.row && i == 1) {
          continue;
        }

        for (int j = -1; j <= 1; ++j) {
          if (pos.col == 0 && j == -1) {
            continue;
          }

          if (pos.col + 1 == m_size.col && j == 1) {
            continue;
          }

          if (i != 0 || j != 0) {
            Vector2u neighbour{ pos.col + i, pos.row + j };
            func(neighbour, get(neighbour));
          }
        }
      }
    }

    /** @} */

    /**
     * @name Iterators and ranges
     * @{
     */

    /**
     * @brief Get an iterator to the first element of the array
     *
     * @return A `begin` iterator to the array
     * @sa end()
     */
    const T *begin() const noexcept {
      return m_data.data();
    }

    /**
     * @brief Get an iterator to the element following the last element of the array
     *
     * @return An `end` iterator to the array
     * @sa begin()
     */
    const T *end() const noexcept {
      return m_data.data() + m_data.size();
    }

    /**
     * @brief Get an iterator to the first element of the array
     *
     * @return A `begin` iterator to the array
     * @sa end()
     */
    T *begin() noexcept {
      return m_data.data();
    }

    /**
     * @brief Get an iterator to the element following the last element of the array
     *
     * @return An `end` iterator to the array
     * @sa begin()
     */
    T *end() noexcept {
      return m_data.data() + m_data.size();
    }

    /**
     * @brief Get the 1D position range of the array
     *
     * @return A range with all the 1D positions in the array
     */
    constexpr RangeZ getPositionRange() const noexcept {
      return { 0, m_size.col * m_size.row };
    }

    /**
     * @brief Get the row range
     *
     * @return A range with all the rows
     */
    constexpr RangeU getRowRange() const noexcept {
      return { 0, m_size.row };
    }

    /**
     * @brief Get the column range
     *
     * @return A range with all the columns
     */
    constexpr RangeU getColRange() const noexcept {
      return { 0, m_size.col };
    }

    /** @} */

  private:
    T& get(Vector2u pos) {
      return m_data[pos.row * m_size.col + pos.col];
    }

    const T& get(Vector2u pos) const {
      return m_data[pos.row * m_size.col + pos.col];
    }

  private:
    Vector2u m_size;
    std::vector<T> m_data;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // ARRAY2D_H
