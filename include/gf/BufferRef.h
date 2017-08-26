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
#ifndef GF_BUFFER_REF_H
#define GF_BUFFER_REF_H

#include <array>
#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A reference to a modifiable buffer and its size
   *
   * This class stores a pointer to a buffer and its size. It can be built
   * from various inputs: `std::vector`, `std::array`, static array, pointer
   * and size.
   *
   * @sa gf::ArrayRef, gf::StringRef
   */
  template<typename T>
  class BufferRef {
  public:
    /**
     * @brief Default constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr BufferRef()
    : m_data(nullptr)
    , m_size(0)
    {

    }

    /**
     * @brief Null constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr BufferRef(std::nullptr_t)
    : m_data(nullptr)
    , m_size(0)
    {

    }

    /**
     * @brief Constructor from a pointer and a size
     *
     * @param data A pointer to a buffer
     * @param size The number of elements in the buffer
     */
    constexpr BufferRef(T *data, std::size_t size)
    : m_data(data)
    , m_size(size)
    {

    }

    /**
     * @brief Constructor from a `std::vector`
     *
     * @param values The vector of elements
     */
    BufferRef(std::vector<T>& values)
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
    BufferRef(std::array<T,N>& values)
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
    constexpr BufferRef(T (&data)[N])
    : m_data(data)
    , m_size(N)
    {

    }

    /**
     * @brief Get a pointer to the elements
     *
     * @returns A pointer to the first element in the array
     */
    T *getData() noexcept {
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
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    T *begin() noexcept {
      return m_data;
    }

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    T *end() noexcept {
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
    T& operator[](std::size_t index) {
      return m_data[index];
    }

  private:
    T *m_data;
    std::size_t m_size;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BUFFER_REF_H
