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
#ifndef GF_STRING_REF_H
#define GF_STRING_REF_H

#include <cstring>
#include <string>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A constant reference to a string and its size
   *
   * This class stores a pointer to a string and its size. It can be built
   * from various inputs: `std::string`, C string.
   *
   * @sa gf::ArrayRef, gf::BufferRef
   */
  class StringRef {
  public:
    /**
     * @brief Default constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr StringRef()
    : m_data(nullptr)
    , m_size(0)
    {

    }

    /**
     * @brief Null constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr StringRef(std::nullptr_t)
    : m_data(nullptr)
    , m_size(0)
    {

    }

    /**
     * @brief Constructor from a pointer and a size
     *
     * @param str A pointer to a string
     * @param size The size of the string
     */
    constexpr StringRef(const char *str, std::size_t size)
    : m_data(str)
    , m_size(size)
    {

    }

    /**
     * @brief Constructor from a null-terminated string
     *
     * The size is computed with `std::strlen`.
     *
     * @param str A null-terminated string
     */
    StringRef(const char *str)
    : m_data(str)
    , m_size(std::strlen(str))
    {

    }

    /**
     * @brief Constructor from a `std::string`
     *
     * @param str A C++ string
     */
    StringRef(const std::string& str)
    : m_data(str.data())
    , m_size(str.size())
    {

    }

    /**
     * @brief Get a pointer to the string
     *
     * @returns A pointer to the beginning of the string
     */
    constexpr const char *getData() const noexcept {
      return m_data;
    }

    /**
     * @brief Get the size of the string
     *
     * @returns The size of the string
     */
    constexpr std::size_t getSize() const noexcept {
      return m_size;
    }

    /**
     * @brief Check if the string is empty
     *
     * An empty string is a string with size 0. So the null string is empty.
     *
     * @returns True if the string is empty
     */
    constexpr bool isEmpty() const noexcept {
      return m_size == 0;
    }

    /**
     * @brief Get an iterator to the beginning of the string
     *
     * @returns A pointer to the first character
     *
     * @sa end()
     */
    constexpr const char *begin() const noexcept {
      return m_data;
    }

    /**
     * @brief Get an iterator past the end of the string
     *
     * @returns A pointer past the last character
     *
     * @sa begin()
     */
    constexpr const char *end() const noexcept {
      return m_data + m_size;
    }

    /**
     * @brief Get a character at a given index
     *
     * No verification is done on the index.
     *
     * @param index The index of character
     * @returns The character at the given index
     */
    constexpr char operator[](std::size_t index) const {
      return m_data[index];
    }

  private:
    const char *m_data;
    std::size_t m_size;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STRING_REF_H
