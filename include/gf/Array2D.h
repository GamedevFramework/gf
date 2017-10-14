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
#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <cassert>
#include <type_traits>
#include <utility>
#include <vector>

#include "Log.h"

#include "Math.h"
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
   * The array is templated with the type of the data and the type of the
   * indices (defaults to `unsigned`).
   *
   * Contrary to the usual way of accessing 2D arrays, the first coordinate is
   * the column and the second coordinate is the row. So that, if `size` is the
   * size of the array and `pos` is the position in the array:
   *
   * - @f$ 0 \leq \mathtt{pos.x} = \mathtt{pos.col} < \mathtt{size.width} = \mathtt{size.col} @f$
   * - @f$ 0 \leq \mathtt{pos.y} = \mathtt{pos.row} < \mathtt{size.height} = \mathtt{size.row} @f$
   *
   * Some convinient visitors are provided to visit the four neighbors (up,
   * down, left and right), or the eight neighbors.
   *
   * @sa gf::Matrix
   */
  template<typename T, typename I = unsigned>
  class Array2D {
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
    Array2D(Vector<I, 2> size)
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
    Array2D(Vector<I, 2> size, const T& value)
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
     * @brief Swap with another array
     *
     * @param other An other array
     */
    void swap(Array2D& other) {
      std::swap(m_data, other.m_data);
      std::swap(m_size, other.m_size);
    }

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
    constexpr Vector<I, 2> getSize() const noexcept {
      return m_size;
    }

    /**
     * @brief Get the number of columns
     *
     * @return The number of columns
     */
    constexpr I getCols() const noexcept {
      return m_size.col;
    }

    /**
     * @brief Get the number of rows
     *
     * @return The number of rows
     */
    constexpr I getRows() const noexcept {
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

    /**
     * @brief Check if a position is valid
     *
     * A valid position is a position inside the array
     *
     * @return True if the position is valid
     */
    constexpr bool isValid(Vector<I, 2> pos) const noexcept {
      return pos.col < m_size.col && pos.row < m_size.row && (std::is_unsigned<I>::value || (0 <= pos.col && 0 <= pos.row));
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
    T& operator()(Vector<I, 2> pos) {
      return get(pos);
    }

    /**
     * @brief Get the element at a given 1D index
     *
     * @param index The 1D index of the element
     * @sa getPositionRange()
     */
    T& operator()(std::size_t index) {
      return m_data[index];
    }

    /**
     * @brief Get the element at a given 2D position
     *
     * @param pos The 2D position of the element
     */
    const T& operator()(Vector<I, 2> pos) const {
      return get(pos);
    }

    /**
     * @brief Get the element at a given 1D index
     *
     * @param index The 1D index of the element
     * @sa getPositionRange()
     */
    const T& operator()(std::size_t index) const {
      return m_data[index];
    }

    /**
     * @brief Transform a 1D position into a 2D position
     *
     * @param pos A 1D position
     * @return The corresponding 2D position
     */
    constexpr Vector<I, 2> toPosition(std::size_t pos) const noexcept {
      return { pos % m_size.col, pos / m_size.col };
    }

    /** @} */

    /**
     * @name Visitors
     * @{
     */

    /**
     * @brief Visit the 4 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical and horizontal direction. The function checks if the neighbor
     * actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit4Neighbors(Vector<I, 2> pos, Func func) {
      visitNeighborsDiamond(pos, func, 1);
    }

    /**
     * @brief Visit the 4 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical and horizontal direction. The function checks if the neighbor
     * actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit4Neighbors(Vector<I, 2> pos, Func func) const {
      visitNeighborsDiamond(pos, func, 1);
    }

    /**
     * @brief Visit the 12 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical and horizontal direction. The function checks if the neighbor
     * actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit12Neighbors(Vector<I, 2> pos, Func func) {
      visitNeighborsDiamond(pos, func, 2);
    }

    /**
     * @brief Visit the 12 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical and horizontal direction. The function checks if the neighbor
     * actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit12Neighbors(Vector<I, 2> pos, Func func) const {
      visitNeighborsDiamond(pos, func, 2);
    }

    /**
     * @brief Visit the 8 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical, horizontal and diagonal direction. The function checks if the
     * neighbor actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit8Neighbors(Vector<I, 2> pos, Func func) {
      visitNeighborsSquare(pos, func, 1);
    }

    /**
     * @brief Visit the 8 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical, horizontal and diagonal direction. The function checks if the
     * neighbor actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */


    template<typename Func>
    void visit8Neighbors(Vector<I, 2> pos, Func func) const {
      visitNeighborsSquare(pos, func, 1);
    }

    /**
     * @brief Visit the 24 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical, horizontal and diagonal direction. The function checks if the
     * neighbor actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit24Neighbors(Vector<I, 2> pos, Func func) {
      visitNeighborsSquare(pos, func, 2);
    }

    /**
     * @brief Visit the 24 neighbors of a given position
     *
     * This function calls a callback function for every neighbor in the
     * vertical, horizontal and diagonal direction. The function checks if the
     * neighbor actually exists.
     *
     * The callback function has the following prototype:
     *
     * ~~~{.cc}
     * void callback(Vector<I, 2> pos, T value);
     * // pos is the position of the neighbor
     * // value is the value of the neighbor
     * ~~~
     *
     * The callback function can be a simple function but also a
     * [lambda expression](http://en.cppreference.com/w/cpp/language/lambda).
     *
     * @param pos The position
     * @param func A callback function
     */
    template<typename Func>
    void visit24Neighbors(Vector<I, 2> pos, Func func) const {
      visitNeighborsSquare(pos, func, 2);
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
     * @brief Get the 1D index range of the array
     *
     * @return A range with all the 1D index in the array
     */
    constexpr RangeZ getIndexRange() const noexcept {
      return { 0, m_size.col * m_size.row };
    }

    /**
     * @brief Get the row range
     *
     * @return A range with all the rows
     */
    constexpr Range<I> getRowRange() const noexcept {
      return { 0, m_size.row };
    }

    /**
     * @brief Get the column range
     *
     * @return A range with all the columns
     */
    constexpr Range<I> getColRange() const noexcept {
      return { 0, m_size.col };
    }

    /**
     * @brief Get the position range
     *
     * @return A range for iterating among the positions
     */
    constexpr PositionRange<I> getPositionRange() const noexcept {
      return { getColRange(), getRowRange() };
    }

    /** @} */

  private:
    T& get(Vector<I, 2> pos) {
      return m_data[pos.row * m_size.col + pos.col];
    }

    const T& get(Vector<I, 2> pos) const {
      return m_data[pos.row * m_size.col + pos.col];
    }

    template<typename Func>
    void visitNeighborsSquare(Vector<I, 2> pos, Func func, I n) const {
      assert(isValid(pos));

      auto colMin = pos.col - std::min(pos.col, n);
      auto colMax = pos.col + std::min(m_size.col - pos.col - 1, n);
      auto rowMin = pos.row - std::min(pos.row, n);
      auto rowMax = pos.row + std::min(m_size.row - pos.row - 1, n);

      for (auto row = rowMin; row <= rowMax; ++row) {
        for (auto col = colMin; col <= colMax; ++col) {
          if (col == pos.col && row == pos.row) { // avoid to include VectorOps.h
            continue;
          }

          func({ col, row }, get({ col, row }));
        }
      }
    }

    template<typename Func>
    void visitNeighborsSquare(Vector<I, 2> pos, Func func, I n) {
      assert(isValid(pos));

      auto colMin = pos.col - std::min(pos.col, n);
      auto colMax = pos.col + std::min(m_size.col - pos.col - 1, n);
      auto rowMin = pos.row - std::min(pos.row, n);
      auto rowMax = pos.row + std::min(m_size.row - pos.row - 1, n);

      for (auto row = rowMin; row <= rowMax; ++row) {
        for (auto col = colMin; col <= colMax; ++col) {
          if (col == pos.col && row == pos.row) { // avoid to include VectorOps.h
            continue;
          }

          func({ col, row }, get({ col, row }));
        }
      }
    }


    template<typename Func>
    void visitNeighborsDiamond(Vector<I, 2> pos, Func func, I n) const {
      assert(isValid(pos));

      auto colMin = pos.col - std::min(pos.col, n);
      auto colMax = pos.col + std::min(m_size.col - pos.col - 1, n);
      auto rowMin = pos.row - std::min(pos.row, n);
      auto rowMax = pos.row + std::min(m_size.row - pos.row - 1, n);

      for (auto row = rowMin; row <= rowMax; ++row) {
        for (auto col = colMin; col <= colMax; ++col) {
          if (col == pos.col && row == pos.row) { // avoid to include VectorOps.h
            continue;
          }

          if (gf::absdiff(col, pos.col) + gf::absdiff(row, pos.row) > n) {
            continue;
          }

          func({ col, row }, get({ col, row }));
        }
      }
    }

    template<typename Func>
    void visitNeighborsDiamond(Vector<I, 2> pos, Func func, I n) {
      assert(isValid(pos));

      auto colMin = pos.col - std::min(pos.col, n);
      auto colMax = pos.col + std::min(m_size.col - pos.col - 1, n);
      auto rowMin = pos.row - std::min(pos.row, n);
      auto rowMax = pos.row + std::min(m_size.row - pos.row - 1, n);

      for (auto row = rowMin; row <= rowMax; ++row) {
        for (auto col = colMin; col <= colMax; ++col) {
          if (col == pos.col && row == pos.row) { // avoid to include VectorOps.h
            continue;
          }

          if (gf::absdiff(col, pos.col) + gf::absdiff(row, pos.row) > n) {
            continue;
          }

          func({ col, row }, get({ col, row }));
        }
      }
    }

  private:
    Vector<I, 2> m_size;
    std::vector<T> m_data;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // ARRAY2D_H
