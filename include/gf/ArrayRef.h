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
 */
#ifndef GF_ARRAY_REF_H
#define GF_ARRAY_REF_H

#include <cassert>
#include <array>
#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A constant reference to an array and its size
   *
   * This class stores a pointer to some data and its size. It can be built
   * from various inputs: `std::vector`, static array, pointer and size.
   *
   * @sa gf::BufferRef, gf::StringRef, gf::array()
   */
  template<typename T>
  class ArrayRef {
  public:
    /**
     * @brief Default constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr ArrayRef()
    : m_data(nullptr)
    , m_size(0)
    {
    }

    /**
     * @brief Null constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr ArrayRef(std::nullptr_t)
    : m_data(nullptr)
    , m_size(0)
    {
    }

    /**
     * @brief Constructor from a pointer and a size
     *
     * @param data A pointer to the elements in the array
     * @param size The number of elements in the array
     */
    constexpr ArrayRef(const T *data, std::size_t size)
    : m_data(data)
    , m_size(size)
    {
    }

    /**
     * @brief Constructor from a `std::vector`
     *
     * @param values The vector of elements
     */
    ArrayRef(const std::vector<T>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    /**
     * @brief Constructor from a `std::array`
     *
     * @param values The array of elements
     */
    template<std::size_t N>
    ArrayRef(const std::array<T,N>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    /**
     * @brief Constructor from a static array
     *
     * The size is computed at compile-time.
     *
     * @param data The static array
     */
    template<std::size_t N>
    constexpr ArrayRef(const T (&data)[N])
    : m_data(data)
    , m_size(N)
    {
    }

    /**
     * @brief Get a pointer to the elements
     *
     * @returns A pointer to the first element in the array
     */
    constexpr const T *getData() const noexcept {
      return m_data;
    }

    /**
     * @brief Get the number of elements
     *
     * @returns The number of elements in the array
     */
    constexpr std::size_t getSize() const noexcept {
      return m_size;
    }

    /**
     * @brief Check if the array is empty
     *
     * @returns True if the array has no elements
     */
    constexpr bool isEmpty() const noexcept {
      return m_size == 0;
    }

    /**
     * @brief Create an array starting at a specified index
     *
     * @param index The starting index of the new array
     * @returns A sub-array starting at the new index
     */
    constexpr ArrayRef<T> sub(std::size_t index) {
      assert(index <= m_size);
      return ArrayRef<T>(m_data + index, m_size - index);
    }

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    constexpr const T *begin() const noexcept {
      return m_data;
    }

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    constexpr const T *end() const noexcept {
      return m_data + m_size;
    }

    /**
     * @brief Get an element at a given index
     *
     * No verification is done on the index.
     *
     * @param index The index of the element
     * @returns The element at the given index
     */
    constexpr const T& operator[](std::size_t index) const {
      return m_data[index];
    }

  private:
    const T *m_data;
    std::size_t m_size;
  };


  /**
   * @ingroup core
   * @brief Create a constant reference to an array
   *
   * @param data A pointer to the elements in the array
   * @param size The number of elements in the array
   * @returns A constant reference to an array
   */
  template<typename T>
  constexpr
  ArrayRef<T> array(const T *data, std::size_t size) {
    return ArrayRef<T>(data, size);
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ARRAY_REF_H
