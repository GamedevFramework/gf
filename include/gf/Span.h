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
#ifndef GF_SPAN_H
#define GF_SPAN_H

#include <cassert>
#include <array>
#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  template<typename T>
  class Span;

  /**
   * @ingroup core_vocabulary
   * @brief A static span
   *
   * A span is a non-owning reference to a contiguous sequence of objects. A
   * static span knows the number of objects at compile-time.
   *
   * This class stores a pointer to some data. It can be built from various
   * inputs: `std::array`, static array, pointer and size.
   *
   * @sa gf::Span
   */
  template<typename T, std::size_t N>
  class StaticSpan {
  public:
    /**
     * @brief The size of the span
     */
    static constexpr std::size_t Size = N;


    /**
     * @brief Default constructor
     *
     * Data is `nullptr`.
     */
    constexpr
    StaticSpan() noexcept
    : m_data(nullptr)
    {
    }

    /**
     * @brief Null constructor
     *
     * Data is `nullptr`.
     */
    constexpr
    StaticSpan(std::nullptr_t) noexcept
    : m_data(nullptr)
    {
    }

    /**
     * @brief Constructor from a pointer and a size
     *
     * The size must match the constant size of the span.
     *
     * @param data A pointer to the elements in the array
     * @param size The number of elements in the array
     */
    constexpr
    StaticSpan(T *data, [[maybe_unused]] std::size_t size) noexcept
    : m_data(data)
    {
      assert(size == Size);
    }

    /**
     * @brief Constructor from a static array
     *
     * The size must match the constant size of the span.
     *
     * @param data The static array
     */
    template<typename U, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    constexpr
    StaticSpan(U (&data)[N]) noexcept
    : m_data(data)
    {
    }

    /**
     * @brief Constructor from a `std::array`
     *
     * The size must match the constant size of the span.
     *
     * @param array The array of elements
     */
    template<typename U, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    constexpr
    StaticSpan(std::array<U, N>& array) noexcept
    : m_data(array.data())
    {
    }

    /**
     * @brief Constructor from another span
     *
     * @param other The other span
     */
    template<typename U, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    constexpr
    StaticSpan(StaticSpan<U, N> other) noexcept
    : m_data(other.getData())
    {
    }

    /**
     * @brief Get a pointer to the elements
     *
     * @returns A pointer to the first element in the array
     */
    constexpr
    T *getData() noexcept {
      return m_data;
    }

    /**
     * @brief Get a pointer to the elements
     *
     * @returns A pointer to the first element in the array
     */
    constexpr
    const T *getData() const noexcept {
      return m_data;
    }

    /**
     * @brief Get the number of elements
     *
     * @returns The number of elements in the array
     */
    constexpr
    std::size_t getSize() const noexcept {
      return Size;
    }

    /**
     * @brief Check if the array is empty
     *
     * @returns True if the array has no elements
     */
    constexpr
    bool isEmpty() const noexcept {
      return m_data == nullptr || Size == 0;
    }

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    constexpr
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
    constexpr
    T *end() noexcept {
      return m_data == nullptr ? nullptr : m_data + Size;
    }

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    constexpr
    const T *begin() const noexcept {
      return m_data;
    }

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    constexpr
    const T *end() const noexcept {
      return m_data == nullptr ? nullptr : m_data + Size;
    }

    /**
     * @brief Get an element at a given index
     *
     * @param index The index of the element
     * @returns The element at the given index
     */
    constexpr
    T& operator[](std::size_t index) noexcept {
      assert(m_data != nullptr && index < Size);
      return m_data[index];
    }

    /**
     * @brief Get an element at a given index
     *
     * @param index The index of the element
     * @returns The element at the given index
     */
    constexpr
    const T& operator[](std::size_t index) const noexcept {
      assert(m_data != nullptr && index < Size);
      return m_data[index];
    }

    /**
     * @brief Get an element at a constant index
     *
     * @returns The element at the given index
     */
    template<std::size_t I>
    constexpr
    T& get() noexcept {
      static_assert(I < N, "Out of bounds.");
      assert(m_data != nullptr);
      return m_data[I];
    }

    /**
     * @brief Get an element at a constant index
     *
     * @returns The element at the given index
     */
    template<std::size_t I>
    constexpr
    const T& get() const noexcept {
      static_assert(I < N, "Out of bounds.");
      assert(m_data != nullptr);
      return m_data[I];
    }

    /**
     * @brief Take a sub-span
     *
     * @param b The begin index
     * @param e The end index, past the last
     * @returns A sub-span
     */
    constexpr
    Span<T> slice(std::size_t b, std::size_t e) const noexcept {
      assert(b <= e && e <= Size);
      return Span<T>(m_data + b, e - b);
    }

    /**
     * @brief Take a constant sub-span
     *
     * @tparam B The begin index
     * @tparam E The end index, past the last
     * @returns A sub-span
     */
    template<std::size_t B, std::size_t E>
    constexpr
    StaticSpan<T, E - B> slice() const noexcept {
      static_assert(B <= E && E <= Size, "Out of bounds.");
      return StaticSpan<T, E - B>(m_data + B);
    }

    /**
     * @brief Take a span on the first objects
     *
     * @param count The number of objects in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> first(std::size_t count) const noexcept {
      return slice(0, count);
    }

    /**
     * @brief Take a constant span on the first objects
     *
     * @tparam C The number of objects in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    StaticSpan<T, C> first() const noexcept {
      static_assert(C <= Size, "Out of bounds.");
      return slice<0, C>();
    }

    /**
     * @brief Take a span on the last objects
     *
     * @param count The number of objects in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> last(std::size_t count) const noexcept {
      assert(count <= Size);
      return slice(Size - count, Size);
    }

    /**
     * @brief Take a constant span on the last objects
     *
     * @tparam C The number of objects in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    StaticSpan<T, C> last() const noexcept {
      static_assert(C <= Size, "Out of bounds.");
      return slice<Size - C, Size>();
    }

    /**
     * @brief Take a span on the first objects
     *
     * @param count The number of objects *not* in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> firstExcept(std::size_t count) const noexcept {
      assert(count <= Size);
      return slice(0, Size - count);
    }

    /**
     * @brief Take a constant span on the first objects
     *
     * @tparam C The number of objects *not* in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    StaticSpan<T, Size - C> firstExcept() const noexcept {
      static_assert(C <= Size, "Out of bounds.");
      return slice<0, Size - C>();
    }

    /**
     * @brief Take a span on the last objects
     *
     * @param count The number of objects *not* in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> lastExcept(std::size_t count) const noexcept {
      return slice(count, Size);
    }

    /**
     * @brief Take a constant span on the last objects
     *
     * @tparam C The number of objects *not* in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    StaticSpan<T, Size - C> lastExcept() const noexcept {
      static_assert(C <= Size, "Out of bounds.");
      return slice<C, Size>();
    }

  private:
    T *m_data;
  };


  /**
   * @ingroup core_vocabulary
   * @brief A span
   *
   * A span is a non-owning reference to a contiguous sequence of objects.
   *
   * This class stores a pointer to some data and its size. It can be built
   * from various inputs: `std::vector`, static array, pointer and size.
   *
   * @sa gf::StaticSpan
   */
  template<typename T>
  class Span {
  public:


    /**
     * @brief Default constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr
    Span()
    : m_data(nullptr)
    , m_size(0)
    {
    }

    /**
     * @brief Null constructor
     *
     * Data is `nullptr` and size is 0.
     */
    constexpr
    Span(std::nullptr_t)
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
    constexpr
    Span(T *data, std::size_t size)
    : m_data(data)
    , m_size(size)
    {
    }

    /**
     * @brief Constructor from a `std::vector`
     *
     * @param values The vector of elements
     */
    template<typename U, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    Span(std::vector<U>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    /**
     * @brief Constructor from a `std::vector`
     *
     * @param values The vector of elements
     */
    template<typename U, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value && std::is_const<T>::value>>
    Span(const std::vector<U>& values)
    : m_data(values.data())
    , m_size(values.size())
    {
    }

    /**
     * @brief Constructor from a `std::array`
     *
     * @param values The array of elements
     */
    template<typename U, std::size_t N, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    Span(std::array<U,N>& values)
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
    template<typename U, std::size_t N, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    constexpr Span(U (&data)[N])
    : m_data(data)
    , m_size(N)
    {
    }

    /**
     * @brief Constructor from another span
     *
     * @param other The other span
     */
    template<typename U, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    constexpr Span(Span<U> other)
    : m_data(other.getData())
    , m_size(other.getSize())
    {
    }

    /**
     * @brief Constructor from another static span
     *
     * @param other The other span
     */
    template<typename U, std::size_t N, typename = std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value>>
    constexpr Span(StaticSpan<U, N> other)
    : m_data(other.getData())
    , m_size(other.getSize())
    {
    }

    /**
     * @brief Get a pointer to the elements
     *
     * @returns A pointer to the first element in the array
     */
    constexpr T *getData() noexcept {
      return m_data;
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
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    constexpr T *begin() noexcept {
      return m_data;
    }

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    constexpr T *end() noexcept {
      return m_data + m_size;
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
    constexpr T& operator[](std::size_t index) {
      return m_data[index];
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

    /**
     * @brief Take a sub-span
     *
     * @param b The begin index
     * @param e The end index, past the last
     * @returns A sub-span
     */
    constexpr
    Span<T> slice(std::size_t b, std::size_t e) const noexcept {
      assert(b <= e && e <= m_size);
      return Span<T>(m_data + b, e - b);
    }

    /**
     * @brief Take a constant sub-span
     *
     * @tparam B The begin index
     * @tparam E The end index, past the last
     * @returns A sub-span
     */
    template<std::size_t B, std::size_t E>
    constexpr
    StaticSpan<T, E - B> slice() const noexcept {
      static_assert(B <= E, "Out of bounds.");
      assert(E <= m_size);
      return StaticSpan<T, E - B>(m_data + B);
    }

    /**
     * @brief Take a span on the first objects
     *
     * @param count The number of objects in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> first(std::size_t count) const noexcept {
      return slice(0, count);
    }

    /**
     * @brief Take a constant span on the first objects
     *
     * @tparam C The number of objects in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    StaticSpan<T, C> first() const noexcept {
      assert(C <= m_size);
      return StaticSpan<T, C>(m_data, C);
    }

    /**
     * @brief Take a span on the last objects
     *
     * @param count The number of objects in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> last(std::size_t count) const noexcept {
      assert(count <= m_size);
      return slice(m_size - count, m_size);
    }

    /**
     * @brief Take a constant span on the last objects
     *
     * @tparam C The number of objects in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    StaticSpan<T, C> last() const noexcept {
      assert(C <= m_size);
      return StaticSpan<T, C>(m_data + m_size - C, C);
    }

    /**
     * @brief Take a span on the first objects
     *
     * @param count The number of objects *not* in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> firstExcept(std::size_t count) const noexcept {
      assert(count <= m_size);
      return slice(0, m_size - count);
    }

    /**
     * @brief Take a span on the first objects
     *
     * @tparam C The number of objects *not* in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    Span<T> firstExcept() const noexcept {
      assert(C <= m_size);
      return slice(0, m_size - C);
    }

    /**
     * @brief Take a span on the last objects
     *
     * @param count The number of objects *not* in the new span
     * @returns The sub-span
     */
    constexpr
    Span<T> lastExcept(std::size_t count) const noexcept {
      return slice(count, m_size);
    }

    /**
     * @brief Take a span on the last objects
     *
     * @tparam C The number of objects *not* in the new span
     * @returns The sub-span
     */
    template<std::size_t C>
    constexpr
    Span<T> lastExcept() const noexcept {
      assert(C <= m_size);
      return slice(C, m_size);
    }

  private:
    T *m_data;
    std::size_t m_size;
  };


  template<typename T>
  constexpr
  Span<T> span(T *data, std::size_t size) {
    return Span<T>(data, size);
  }

  template<typename T, std::size_t N>
  constexpr
  StaticSpan<T, N> span(T (&data)[N]) {
    return StaticSpan<T, N>(data);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPAN_H
