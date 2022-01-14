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
 */
#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <cassert>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <vector>

#include "CoreApi.h"
#include "Math.h"
#include "Range.h"
#include "SerializationFwd.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_container
   * @brief A two-dimensional array with no data
   *
   * You probably want to use gf::Array2D
   *
   * @sa gf::Array2D
   */
  template<typename I>
  class Index2D {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty array.
     */
    Index2D()
    : m_size(0, 0)
    {
    }

    /**
     * @brief Constructor with a size
     *
     * @param size The size of the array
     */
    Index2D(Vector<I, 2> size)
    : m_size(size)
    {
    }

    /**
     * @brief Default copy constructor
     */
    Index2D(const Index2D&) = default;

    /**
     * @brief Default copy assignment
     */
    Index2D& operator=(const Index2D&) = default;

    /**
     * @brief Default move constructor
     */
    Index2D(Index2D&&) = default;

    /**
     * @brief Default move assignement
     */
    Index2D& operator=(Index2D&&) = default;

    /**
     * @brief Swap with another array
     *
     * @param other An other array
     */
    void swap(Index2D& other) {
      std::swap(m_size, other.m_size);
    }

    /**
     * @name Size and position
     * @{
     */

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
     * @brief Check if a position is valid
     *
     * A valid position is a position inside the array
     *
     * @return True if the position is valid
     */
    constexpr bool isValid(Vector<I, 2> pos) const noexcept {
      return pos.col < m_size.col && pos.row < m_size.row && (std::is_unsigned<I>::value || (0 <= pos.col && 0 <= pos.row));
    }

    /**
     * @brief Transform a 1D position into a 2D position
     *
     * @param index A 1D position
     * @return The corresponding 2D position
     */
    constexpr Vector<I, 2> toPosition(std::size_t index) const noexcept {
      return { static_cast<I>(index % m_size.col), static_cast<I>(index / m_size.col) };
    }

    /**
     * @brief Transform a 2D position into a 1D position
     *
     * @param pos A 2D position
     * @return The corresponding 1D position
     */
    constexpr std::size_t toIndex(Vector<I, 2> pos) const noexcept {
      return pos.row * m_size.col + pos.col;
    }

    /** @} */

    /**
     * @name Ranges
     * @{
     */

    /**
     * @brief Get the 1D index range of the array
     *
     * @return A range with all the 1D index in the array
     */
    constexpr RangeZ getIndexRange() const noexcept {
      return { 0, static_cast<std::size_t>(m_size.col) * static_cast<std::size_t>(m_size.row) };
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

    /**
     * @brief Get a range for 8 neighbors (at most)
     *
     * @param pos The base position
     * @return A range over neighbors (position not included)
     */
    NeighborSquareRange<I> get8NeighborsRange(Vector<I, 2> pos) const noexcept {
      return getNeighborSquareRange(pos, 1);
    }

    /**
     * @brief Get a range for 24 neighbors (at most)
     *
     * @param pos The base position
     * @return A range over neighbors (position not included)
     */
    NeighborSquareRange<I> get24NeighborsRange(Vector<I, 2> pos) const noexcept {
      return getNeighborSquareRange(pos, 2);
    }

    /**
     * @brief Get a range for 4 neighbors (at most)
     *
     * @param pos The base position
     * @return A range over neighbors (position not included)
     */
    NeighborDiamondRange<I> get4NeighborsRange(Vector<I, 2> pos) const noexcept {
      return getNeighborDiamondRange(pos, 1);
    }

    /**
     * @brief Get a range for 12 neighbors (at most)
     *
     * @param pos The base position
     * @return A range over neighbors (position not included)
     */
    NeighborDiamondRange<I> get12NeighborsRange(Vector<I, 2> pos) const noexcept {
      return getNeighborDiamondRange(pos, 2);
    }

    /** @} */

  private:
    NeighborSquareRange<I> getNeighborSquareRange(Vector<I, 2> pos, I n) const noexcept {
      assert(isValid(pos));

      auto colMin = pos.col - std::min(pos.col, n);
      auto colMax = pos.col + std::min(m_size.col - pos.col - 1, n);
      auto rowMin = pos.row - std::min(pos.row, n);
      auto rowMax = pos.row + std::min(m_size.row - pos.row - 1, n);

      return NeighborSquareRange<I>{ Range<I>{ colMin, colMax + 1 }, Range<I>{ rowMin, rowMax + 1 }, pos };
    }

    NeighborDiamondRange<I> getNeighborDiamondRange(Vector<I, 2> pos, I n) const noexcept {
      assert(isValid(pos));

      auto colMin = pos.col - std::min(pos.col, n);
      auto colMax = pos.col + std::min(m_size.col - pos.col - 1, n);
      auto rowMin = pos.row - std::min(pos.row, n);
      auto rowMax = pos.row + std::min(m_size.row - pos.row - 1, n);

      return NeighborDiamondRange<I>{ Range<I>{ colMin, colMax + 1 }, Range<I>{ rowMin, rowMax + 1 }, pos, n };
    }

  private:
    Vector<I, 2> m_size;
  };

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template class GF_CORE_API Index2D<unsigned>;
  extern template class GF_CORE_API Index2D<int>;
#endif

  /**
   * @ingroup core_container
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
  class Array2D : public Index2D<I> {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty array.
     */
    Array2D()
    : Index2D<I>()
    {
    }

    /**
     * @brief Constructor with a size
     *
     * @param size The size of the array
     */
    Array2D(Vector<I, 2> size)
    : Index2D<I>(size)
    , m_data(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height))
    {
    }

    /**
     * @brief Constructor with a size and a value
     *
     * @param size The size of the array
     * @param value The initial value in the array
     */
    Array2D(Vector<I, 2> size, const T& value)
    : Index2D<I>(size)
    , m_data(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height), value)
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
      Index2D<I>::swap(other);
      std::swap(m_data, other.m_data);
    }

    using Index2D<I>::getSize;
    using Index2D<I>::getCols;
    using Index2D<I>::getRows;
    using Index2D<I>::isValid;
    using Index2D<I>::toPosition;
    using Index2D<I>::toIndex;
    using Index2D<I>::getIndexRange;
    using Index2D<I>::getRowRange;
    using Index2D<I>::getColRange;
    using Index2D<I>::getPositionRange;
    using Index2D<I>::get8NeighborsRange;
    using Index2D<I>::get24NeighborsRange;

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

    /** @} */


    /**
     * @name Iterators
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

    /** @} */

  private:
    T& get(Vector<I, 2> pos) {
      return m_data[toIndex(pos)];
    }

    const T& get(Vector<I, 2> pos) const {
      return m_data[toIndex(pos)];
    }

  private:
    std::vector<T> m_data;
  };

  /**
   * @relates Array2D
   * @brief Equality operator for 2D array
   */
  template<typename T, typename I>
  bool operator==(const Array2D<T,I>& lhs, const Array2D<T,I>& rhs) {
    auto lhsSize = lhs.getSize();
    auto rhsSize = rhs.getSize();

    if (lhsSize.width != rhsSize.width || lhsSize.height != rhsSize.height) {
      return false;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  /**
   * @relates Array2D
   * @brief Serialize a 2D array
   */
  template<typename T, typename I>
  Serializer& operator|(Serializer& ar, const Array2D<T,I>& array) {
    auto size = array.getSize();
    ar | size.width | size.height;

    for (auto& item : array) {
      ar | const_cast<T&>(item);
    }

    return ar;
  }

  /**
   * @relates Array2D
   * @brief Deserialize a 2D array
   */
  template<typename T, typename I>
  Deserializer& operator|(Deserializer& ar, Array2D<T,I>& array) {
    Vector<I, 2> size;
    ar | size.width | size.height;

    Array2D<T,I> tmp(size);

    for (auto& item : tmp) {
      ar | item;
    }

    array = std::move(tmp);
    return ar;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // ARRAY2D_H
