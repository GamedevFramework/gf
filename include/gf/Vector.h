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
#ifndef GF_VECTOR_H
#define GF_VECTOR_H

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <initializer_list>
#include <type_traits>

#include "CoreApi.h"
#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_maths
   * @brief General purpose math vector
   *
   * gf::Vector is a class that represents an element of a `N`-dimensional
   * space. It is used throughout the library for different purposes.
   *
   * The template parameter `T` is the type of coordinates. . It can be any
   * type that supports arithmetic operations (+, -, *, /) and relational
   * operators (==, !=, <, >).
   *
   * Several specializations are defined for common use cases:
   *
   * - For dimension 2: gf::Vector<T, 2>
   * - For dimension 3: gf::Vector<T, 3>
   * - For dimension 4: gf::Vector<T, 4>
   *
   * This class was designed according to the article
   * [On Vector Math Libraries](http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/)
   * by Nathan Reed.
   */
  template<typename T, std::size_t N>
  struct Vector {
    #ifndef DOXYGEN_SHOULD_SKIP_THIS
    static_assert(N > 0, "N must be strictly positive");
    #endif

    /**
     * @brief Default constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector() = default;

    /**
     * @brief Constructor that zero the vector out
     */
    constexpr Vector(ZeroType) noexcept
    {
      zero();
    }

    /**
     * @brief Constructor that fills the vector with a value.
     *
     * This constructor takes a value and fills the entire vector with this
     * value. Care must be taken when calling this constructor:
     *
     * ~~~{.cc}
     * gf::Vector<int, 5> vecOK(42); // OK, vector is filled with 42
     * gf::Vector<int, 5> vecKO{42}; // KO, vector is initialized with a 42 in the first coordinate
     * ~~~
     *
     * @param val The value to fill the vector with
     */
    explicit Vector(T val) noexcept
    {
      std::fill_n(data, N, val);
    }

    /**
     * @brief Constructor that takes an array
     *
     * This constructor can ease conversion from other math libraries. The
     * array must contain enough data for `N` dimensions.
     *
     * ~~~{.cc}
     * float array[5] = { 1.0f, -1.0f, 0.5f, -2.0f, 0.0f };
     * gf::Vector<float, 5> vec(array);
     * ~~~
     *
     * @param array An array with the values of the vector
     */
    explicit Vector(const T *array)
    {
      std::copy_n(array, N, data);
    }

    /**
     * @brief Constructor that takes an initializer list
     *
     * This constructor allows to use an initializer list to define the
     * coordinates of the vector.
     *
     * ~~~{.cc}
     * gf::Vector<bool, 5> vec1{ true, true, false, true, false };
     * gf::Vector<bool, 5> vec2 = { false, true, true, false, true };
     * ~~~
     *
     * @param list An initializer list.
     */
    Vector(std::initializer_list<T> list) noexcept
    {
      std::copy_n(list.begin(), std::min(list.size(), N), data);
    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     *
     * @param other The vector to copy from
     */
    Vector(const Vector& other) = default;


    /**
     * @brief Converting copy constructor
     *
     * @param other The vector to copy from
     */
    template<typename U>
    Vector(const Vector<U, N>& other) noexcept
    {
      static_assert(std::is_convertible<U,T>::value, "Non-convertible types");
      std::transform(data, data + N, other.data, [](U val) { return static_cast<T>(val); });
    }

    /**
     * @brief Default copy assignment
     *
     * This operator is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector& operator=(const Vector& other) = default;

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 5> vec = { 1, 3, 5, 7, 9 };
     * std::printf("%i", vec[1]); // prints 3
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T operator[](std::size_t i) const {
      return data[i];
    }

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 5> vec;
     * vec[0] = vec[1] = vec[2] = vec[3] = vec[4] = 0;
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T& operator[](std::size_t i) {
      return data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    T *begin() {
      return &data[0];
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    T *end() {
      return &data[N];
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *begin() const {
      return &data[0];
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *end() const {
      return &data[N];
    }

    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *cbegin() const {
      return &data[0];
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *cend() const {
      return &data[N];
    }

    /**
     * @brief Zero out the vector
     */
    constexpr void zero() noexcept {
      for (std::size_t i = 0; i < N; ++i) {
        data[i] = T{};
      }
    }

    /**
     * @brief The internal representation of the vector
     *
     * A vector is represented with an array of `N` values of type `T`. It
     * can be accessed directly, like an array or like a pointer, which can
     * ease interoperability with other libraries.
     */
    T data[N];
  };

  /**
   * @ingroup core_maths
   * @brief A 2D vector
   *
   * This specialization of gf::Vector handles the 2-dimension spaces. It can
   * be accessed with various representations:
   *
   * - the `(x,y)` representation, used for generic coordinates in the 2D space
   * - the `(u,v)` representation, used for texture coordinates (see [UV mapping](https://en.wikipedia.org/wiki/UV_mapping))
   * - the `(s,t)` representation, used for texture coordinates
   * - the size representation with a `width` member and a `height` member, used to represent a 2-dimensional size
   * - the indices representation with a `col` member and a `row` member, used to access a 2-dimensional array (gf::Array2D)
   *
   * Several common typedef are defined:
   *
   * - gf::Vector2i with `int` as `T`
   * - gf::Vector2u with `unsigned` as `T`
   * - gf::Vector2z with `std::size_t` as `T`
   * - gf::Vector2f with `float` as `T`
   * - gf::Vector2d with `double` as `T`
   * - gf::Vector2b with `bool` as `T`
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::Vector2f v1(16.5f, 24.0f);
   * v1.x = 18.2f;
   *
   * gf::Vector2f v2 = v1 * 5.0f;
   * gf::Vector2f v3 = v1 + v2;
   *
   * bool different = (v2 != v3);
   *
   * gf::Vector2u size;
   * size.width = 1920;
   * size.height = 1080;
   *
   * gf::Vector2f texCoords;
   * texCoords.u = 0.0f;
   * texCoords.v = 0.3f;
   * ~~~
   */
  template <typename T>
  struct Vector<T, 2> {
    /**
     * @brief Default constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector() = default;

    /**
     * @brief Constructor that zero the vector out
     */
    constexpr Vector(ZeroType) noexcept
    : x(T(0))
    , y(T(0))
    {
    }

    /**
     * @brief Constructor that fills the vector with a value.
     *
     * This constructor takes a value and fills the entire vector with this
     * value. Care must be taken when calling this constructor:
     *
     * ~~~{.cc}
     * gf::Vector<int, 2> vecOK(42); // OK, vector is filled with 42
     * gf::Vector<int, 2> vecKO{42}; // KO, vector is initialized with a 42 in the first coordinate
     * ~~~
     *
     * @param val The value to fill the vector with
     */
    explicit constexpr Vector(T val) noexcept
    : x(val)
    , y(val)
    {
    }

    /**
     * @brief Constructor that takes an array
     *
     * This constructor can ease conversion from other math libraries. The
     * array must contain enough data for 2 dimensions.
     *
     * ~~~{.cc}
     * float array[2] = { 1.0f, -1.0f };
     * gf::Vector<float, 2> vec(array);
     * ~~~
     *
     * @param array An array with the values of the vector
     */
    explicit constexpr Vector(const T *array)
    : x(array[0])
    , y(array[1])
    {
    }

    /**
     * @brief Constructor that takes 2 components
     *
     * @param first The first component
     * @param second The second component
     */
    constexpr Vector(T first, T second) noexcept
    : x(first)
    , y(second)
    {
    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     *
     * @param other The vector to copy from
     */
    Vector(const Vector& other) = default;

    /**
     * @brief Converting copy constructor
     *
     * @param other The vector to copy from
     */
    template<typename U>
    Vector(const Vector<U, 2>& other) noexcept
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
    {
      static_assert(std::is_convertible<U,T>::value, "Non-convertible types");
    }

    /**
     * @brief Default copy assignment
     *
     * This operator is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector& operator=(const Vector& other) = default;

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 2> vec = { 1, 3 };
     * std::printf("%i", vec[1]); // prints 3
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T operator[](std::size_t i) const {
      const T *data[] = { &x, &y };
      return *data[i];
    }

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 2> vec;
     * vec[0] = vec[1] = 0;
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T& operator[](std::size_t i) {
      T *data[] = { &x, &y };
      return *data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    T *begin() {
      return &x;
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    T *end() {
      return &x + 2;
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *begin() const {
      return &x;
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *end() const {
      return &x + 2;
    }
    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *cbegin() const {
      return &x;
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *cend() const {
      return &x + 2;
    }

    /**
     * @brief Zero out the vector
     */
    constexpr void zero() noexcept {
      x = y = T{};
    }

    /**
     * An anonymous union to handle the first coordinate
     */
    union {
      T x; ///< First coordinate in the `(x,y)` representation. @sa y
      T u; ///< First coordinate in the `(u,v)` representation. @sa v
      T s; ///< First coordinate in the `(s,t)` representation. @sa t
      T width; ///< First coordinate in the size representation. @sa height
      T col; ///< First coordinate in the indices representation @sa row
    };

    /**
     * An anonymous union to handle the second coordinate
     */
    union {
      T y; ///< Second coordinate in the `(x,y)` representation. @sa x
      T v; ///< Second coordinate in the `(u,v)` representation. @sa u
      T t; ///< Second coordinate in the `(s,t)` representation. @sa s
      T height; ///< Second coordinate in the size representation. @sa width
      T row; ///< Second coordinate in the indices representation @sa col
    };
  };

  /**
   * @ingroup core_maths
   * @brief A 3D vector
   *
   * This specialization of gf::Vector handles the 3-dimension spaces. It can
   * be accessed with various representations:
   *
   * - the `(x,y,z)` representation, used for generic coordinates in the 3D space
   * - the `(r,g,b)` representation, used for RGB colors
   *
   * Several common typedef are defined:
   *
   * - gf::Vector3i with `int` as `T`
   * - gf::Vector3u with `unsigned` as `T`
   * - gf::Vector3z with `std::size_t` as `T`
   * - gf::Vector3f with `float` as `T`
   * - gf::Vector3d with `double` as `T`
   * - gf::Vector3b with `bool` as `T`
   *
   * For colors, some additional typedef are defined:
   *
   * - gf::Color3f with `float` as `T`
   * - gf::Color3u with `uint8_t` as `T`
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::Vector3f v1(58.0f, 96.0f, 63.0f);
   * v1.x = 94.0f;
   *
   * gf::Vector3f v2 = v1 * 5.0f;
   * gf::Vector3f v3 = v1 + v2;
   *
   * gf::Color3u green(0x00, 0xFF, 0x00);
   * ~~~
   */
  template <typename T>
  struct Vector<T, 3> {
    /**
     * @brief Default constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector() = default;

    /**
     * @brief Constructor that zero the vector out
     */
    constexpr Vector(ZeroType) noexcept
    {
      zero();
    }

    /**
     * @brief Constructor that fills the vector with a value.
     *
     * This constructor takes a value and fills the entire vector with this
     * value. Care must be taken when calling this constructor:
     *
     * ~~~{.cc}
     * gf::Vector<int, 3> vecOK(42); // OK, vector is filled with 42
     * gf::Vector<int, 3> vecKO{42}; // KO, vector is initialized with a 42 in the first coordinate
     * ~~~
     *
     * @param val The value to fill the vector with
     */
    explicit constexpr Vector(T val) noexcept
    : x(val)
    , y(val)
    , z(val)
    {
    }

    /**
     * @brief Constructor that takes an array
     *
     * This constructor can ease conversion from other math libraries. The
     * array must contain enough data for 3 dimensions.
     *
     * ~~~{.cc}
     * float array[3] = { 1.0f, -1.0f, 0.0f };
     * gf::Vector<float, 3> vec(array);
     * ~~~
     *
     * @param array An array with the values of the vector
     */
    explicit constexpr Vector(const T *array)
    : x(array[0])
    , y(array[1])
    , z(array[2])
    {
    }

    /**
     * @brief Constructor that takes 3 components
     *
     * @param first The first component
     * @param second The second component
     * @param third The third component
     */
    constexpr Vector(T first, T second, T third) noexcept
    : x(first)
    , y(second)
    , z(third)
    {
    }

    /**
     * @brief Constructor that takes a 2D vector and a z component
     *
     * @param xy The first 2 component, x and y
     * @param third The z component
     */
    constexpr Vector(Vector<T, 2> xy, T third)
    : x(xy.x)
    , y(xy.y)
    , z(third)
    {
    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     *
     * @param other The vector to copy from
     */
    Vector(const Vector& other) = default;

    /**
     * @brief Converting copy constructor
     *
     * @param other The vector to copy from
     */
    template<typename U>
    Vector(const Vector<U, 3>& other) noexcept
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
    , z(static_cast<T>(other.z))
    {
      static_assert(std::is_convertible<U,T>::value, "Non-convertible types");
    }

    /**
     * @brief Default copy assignment
     *
     * This operator is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector& operator=(const Vector& other) = default;

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 3> vec = { 1, 3, 5 };
     * std::printf("%i", vec[1]); // prints 3
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T operator[](std::size_t i) const {
      const T *data[] = { &x, &y, &z };
      return *data[i];
    }

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 3> vec;
     * vec[0] = vec[1] = vec[2] = 0;
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T& operator[](std::size_t i) {
      T *data[] = { &x, &y, &z };
      return *data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    T *begin() {
      return &x;
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    T *end() {
      return &x + 3;
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *begin() const {
      return &x;
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *end() const {
      return &x + 3;
    }

    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *cbegin() const {
      return &x;
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *cend() const {
      return &x + 3;
    }

    /**
     * @brief Zero out the vector
     */
    constexpr void zero() noexcept {
      x = y = z = T{};
    }

    /**
     * @brief Swizzle to get the first two coordinates as a 2D vector
     */
    constexpr Vector<T, 2> xy() const {
      return { x, y };
    }


    /**
     * An anonymous union to handle the first coordinate
     */
    union {
      T x; ///< First coordinate in the `(x,y,z)` representation. @sa y, z
      T r; ///< First coordinate in the `(r,g,b)` representation. @sa g, b
    };

    /**
     * An anonymous union to handle the second coordinate
     */
    union {
      T y; ///< Second coordinate in the `(x,y,z)` representation. @sa x, z
      T g; ///< Second coordinate in the `(r,g,b)` representation. @sa r, b
    };

    /**
     * An anonymous union to handle the third coordinate
     */
    union {
      T z; ///< Third coordinate in the `(x,y,z)` representation. @sa x, y
      T b; ///< Third coordinate in the `(r,g,b)` representation. @sa r, g
    };
  };

  /**
   * @ingroup core_maths
   * @brief A 4D vector
   *
   * This specialization of gf::Vector handles the 4-dimension spaces. It can
   * be accessed with various representations:
   *
   * - the `(x,y,z,w)` representation, used for generic coordinates in the 4D space
   * - the `(r,g,b,a)` representation, used for RGBA colors
   *
   * Several common typedef are defined:
   *
   * - gf::Vector4i with `int` as `T`
   * - gf::Vector4u with `unsigned` as `T`
   * - gf::Vector4z with `std::size_t` as `T`
   * - gf::Vector4f with `float` as `T`
   * - gf::Vector4d with `double` as `T`
   * - gf::Vector4b with `bool` as `T`
   *
   * For colors, some additional typedef are defined:
   *
   * - gf::Color4f with `float` as `T`
   * - gf::Color4u with `uint8_t` as `T`
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::Vector4f v1(98.0f, 75.0f, 23.0f);
   * v1.x = 57.0f;
   *
   * gf::Vector4f v2 = v1 * 5.0f;
   * gf::Vector4f v3 = v1 + v2;
   *
   * gf::Color4u opaqueGreen(0x00, 0xFF, 0x00, 0xFF);
   * ~~~
   */
  template <typename T>
  struct Vector<T, 4> {
    /**
     * @brief Default constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector() = default;

    /**
     * @brief Constructor that zero the vector out
     */
    constexpr Vector(ZeroType) noexcept
    {
      zero();
    }

    /**
     * @brief Constructor that fills the vector with a value.
     *
     * This constructor takes a value and fills the entire vector with this
     * value. Care must be taken when calling this constructor:
     *
     * ~~~{.cc}
     * gf::Vector<int, 4> vecOK(42); // OK, vector is filled with 42
     * gf::Vector<int, 4> vecKO{42}; // KO, vector is initialized with a 42 in the first coordinate
     * ~~~
     *
     * @param val The value to fill the vector with
     */
    explicit constexpr Vector(T val) noexcept
    : x(val)
    , y(val)
    , z(val)
    , w(val)
    {
    }

    /**
     * @brief Constructor that takes an array
     *
     * This constructor can ease conversion from other math libraries. The
     * array must contain enough data for 4 dimensions.
     *
     * ~~~{.cc}
     * float array[4] = { 1.0f, -1.0f, 0.5f, 0.0f };
     * gf::Vector<float, 4> vec(array);
     * ~~~
     *
     * @param array An array with the values of the vector
     */
    explicit constexpr Vector(const T *array)
    : x(array[0])
    , y(array[1])
    , z(array[2])
    , w(array[3])
    {
    }

    /**
     * @brief Constructor that takes 4 components
     *
     * @param first The first component
     * @param second The second component
     * @param third The third component
     * @param fourth The fourth component
     */
    constexpr Vector(T first, T second, T third, T fourth) noexcept
    : x(first)
    , y(second)
    , z(third)
    , w(fourth)
    {
    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     *
     * @param other The vector to copy from
     */
    Vector(const Vector& other) = default;

    /**
     * @brief Converting copy constructor
     *
     * @param other The vector to copy from
     */
    template<typename U>
    Vector(const Vector<U, 4>& other) noexcept
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
    , z(static_cast<T>(other.z))
    , w(static_cast<T>(other.w))
    {
      static_assert(std::is_convertible<U,T>::value, "Non-convertible types");
    }

    /**
     * @brief Default copy assignment
     *
     * This operator is defaulted so that this type is
     * [trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType).
     */
    Vector& operator=(const Vector& other) = default;

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 4> vec = { 1, 3, 5, 7 };
     * std::printf("%i", vec[1]); // prints 3
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T operator[](std::size_t i) const {
      const T *data[] = { &x, &y, &z, &w };
      return *data[i];
    }

    /**
     * @brief Access to the @f$ i @f$-th coordinate.
     *
     * ~~~{.cc}
     * gf::Vector<int, 5> vec;
     * vec[0] = vec[1] = vec[2] = vec[3] = vec[4] = 0;
     * ~~~
     *
     * @param i the coordinate number
     * @return The @f$ i @f$-th coordinate of the vector
     */
    constexpr T& operator[](std::size_t i) {
      T *data[] = { &x, &y, &z, &w };
      return *data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    T *begin() {
      return &x;
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    T *end() {
      return &x + 4;
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *begin() const {
      return &x;
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *end() const {
      return &x + 4;
    }

    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    const T *cbegin() const {
      return &x;
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    const T *cend() const {
      return &x + 4;
    }

    /**
     * @brief Zero out the vector
     */
    constexpr void zero() noexcept {
      x = y = z = w = T{};
    }

    /**
     * @brief Swizzle to get the first two coordinates as a 2D vector
     */
    constexpr Vector<T, 2> xy() const {
      return { x, y };
    }

    /**
     * @brief Swizzle to get the first three coordinates as a 3D vector
     */
    constexpr Vector<T, 3> xyz() const {
      return { x, y, z };
    }

    /**
     * @brief Swizzle to get the first three coordinates as a RGB color
     */
    constexpr Vector<T, 3> rgb() const {
      return { r, g, b };
    }


    /**
     * An anonymous union to handle the first coordinate
     */
    union {
      T x; ///< First coordinate in the `(x,y,z,w)` representation. @sa y, z, w
      T r; ///< First coordinate in the `(r,g,b,a)` representation. @sa g, b, a
    };

    /**
     * An anonymous union to handle the second coordinate
     */
    union {
      T y; ///< Second coordinate in the `(x,y,z,w)` representation. @sa x, z, w
      T g; ///< Second coordinate in the `(r,g,b,a)` representation. @sa r, b, a
    };

    /**
     * An anonymous union to handle the third coordinate
     */
    union {
      T z; ///< Third coordinate in the `(x,y,z,w)` representation. @sa x, y, w
      T b; ///< Third coordinate in the `(r,g,b,a)` representation. @sa r, g, a
    };

    /**
     * An anonymous union to handle the fourth coordinate
     */
    union {
      T w; ///< Fourth coordinate in the `(x,y,z,w)` representation. @sa x, y, z
      T a; ///< Fourth coordinate in the `(r,g,b,a)` representation. @sa r, g, b
    };
  };

  /**
   * @ingroup core_maths
   * @brief A float vector with 2 components
   *
   * @sa gf::Vector, gf::Vector<T, 2>
   */
  using Vector2f = Vector<float, 2>;

  /**
   * @ingroup core_maths
   * @brief A float vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Vector3f = Vector<float, 3>;

  /**
   * @ingroup core_maths
   * @brief A float vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Vector4f = Vector<float, 4>;

  /**
   * @ingroup core_maths
   * @brief A double vector with 2 components
   *
   * @sa gf::Vector, gf::Vector<T, 2>
   */
  using Vector2d = Vector<double, 2>;

  /**
   * @ingroup core_maths
   * @brief A double vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Vector3d = Vector<double, 3>;

  /**
   * @ingroup core_maths
   * @brief A double vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Vector4d = Vector<double, 4>;

  /**
   * @ingroup core_maths
   * @brief A int vector with 2 components
   *
   * @sa gf::Vector, gf::Vector<T, 2>
   */
  using Vector2i = Vector<int, 2>;

  /**
   * @ingroup core_maths
   * @brief A int vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Vector3i = Vector<int, 3>;

  /**
   * @ingroup core_maths
   * @brief A int vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Vector4i = Vector<int, 4>;

  /**
   * @ingroup core_maths
   * @brief A unsigned vector with 2 components
   *
   * @sa gf::Vector, gf::Vector<T, 2>
   */
  using Vector2u = Vector<unsigned, 2>;

  /**
   * @ingroup core_maths
   * @brief A unsigned vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Vector3u = Vector<unsigned, 3>;

  /**
   * @ingroup core_maths
   * @brief A unsigned vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Vector4u = Vector<unsigned, 4>;

  /**
   * @ingroup core_maths
   * @brief A std::size_t vector with 2 components
   *
   * @sa gf::Vector, gf::Vector<T, 2>
   */
  using Vector2z = Vector<std::size_t, 2>;

  /**
   * @ingroup core_maths
   * @brief A std::size_t vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Vector3z = Vector<std::size_t, 3>;

  /**
   * @ingroup core_maths
   * @brief A std::size_t vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Vector4z = Vector<std::size_t, 4>;

  /**
   * @ingroup core_maths
   * @brief A bool vector with 2 components
   *
   * @sa gf::Vector, gf::Vector<T, 2>
   */
  using Vector2b = Vector<bool, 2>;

  /**
   * @ingroup core_maths
   * @brief A bool vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Vector3b = Vector<bool, 3>;

  /**
   * @ingroup core_maths
   * @brief A bool vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Vector4b = Vector<bool, 4>;

  /**
   * @ingroup core_maths
   * @brief A color vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  template<typename T>
  using Color3 = Vector<T, 3>;

  /**
   * @ingroup core_maths
   * @brief A color vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  template<typename T>
  using Color4 = Vector<T, 4>;

  /**
   * @ingroup core_maths
   * @brief A float color vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Color3f = Color3<float>;

  /**
   * @ingroup core_maths
   * @brief A float color vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Color4f = Color4<float>;

  /**
   * @ingroup core_maths
   * @brief A double color vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Color3d = Color3<double>;

  /**
   * @ingroup core_maths
   * @brief A double color vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Color4d = Color4<double>;

  /**
   * @ingroup core_maths
   * @brief A uint8_t color vector with 3 components
   *
   * @sa gf::Vector, gf::Vector<T, 3>
   */
  using Color3u = Color3<uint8_t>;

  /**
   * @ingroup core_maths
   * @brief A uint8_t color vector with 4 components
   *
   * @sa gf::Vector, gf::Vector<T, 4>
   */
  using Color4u = Color4<uint8_t>;



// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Vector<float, 2>;
  extern template struct GF_CORE_API Vector<float, 3>;
  extern template struct GF_CORE_API Vector<float, 4>;

  extern template struct GF_CORE_API Vector<double, 2>;
  extern template struct GF_CORE_API Vector<double, 3>;
  extern template struct GF_CORE_API Vector<double, 4>;

  extern template struct GF_CORE_API Vector<int, 2>;
  extern template struct GF_CORE_API Vector<int, 3>;
  extern template struct GF_CORE_API Vector<int, 4>;

  extern template struct GF_CORE_API Vector<unsigned, 2>;
  extern template struct GF_CORE_API Vector<unsigned, 3>;
  extern template struct GF_CORE_API Vector<unsigned, 4>;

  extern template struct GF_CORE_API Vector<bool, 2>;
  extern template struct GF_CORE_API Vector<bool, 3>;
  extern template struct GF_CORE_API Vector<bool, 4>;

  extern template struct GF_CORE_API Vector<uint8_t, 3>;
  extern template struct GF_CORE_API Vector<uint8_t, 4>;
#endif

  /**
   * @ingroup core_maths
   * @brief A distance function
   *
   * A distance function is a function that gives the distance between two vectors.
   *
   * ~~~
   * gf::Distance<float, 3> distFn = gf::manhattanDistance<float, 3>;
   * Vector3f vec1 = ...;
   * Vector3f vec2 = ...;
   *
   * float distance = distFn(vec1, vec2);
   * ~~~
   *
   * @sa manhattanDistance(), squareDistance(), euclideanDistance(), chebyshevDistance()
   * @sa gf::Distance2, gf::Distance3
   */
  template<typename T, std::size_t N>
  using Distance = T (*)(Vector<T, N>, Vector<T, N>);

  /**
   * @ingroup core_maths
   * @brief A distance function for 2D vectors
   *
   * @sa gf::Distance
   */
  template<typename T>
  using Distance2 = Distance<T, 2>;

  /**
   * @ingroup core_maths
   * @brief A distance function for 3D vectors
   *
   * @sa gf::Distance
   */
  template<typename T>
  using Distance3 = Distance<T, 3>;


  /**
   * @relates Vector
   * @brief Swap two vectors
   */
  template<typename T, std::size_t N>
  inline
  void swap(Vector<T,N>& lhs, Vector<T,N>& rhs) noexcept {
    std::swap_ranges(lhs.begin(), lhs.end(), rhs.begin());
  }

  /**
   * @relates Vector
   * @brief Universal vector factory
   *
   * @param values The values for the vector
   * @returns A vector with a deduced type and initialized with the values provided
   */
  template<typename ... Types>
  constexpr
  auto vec(Types... values) -> Vector<std::common_type_t<Types...>, sizeof...(Types)> {
    static_assert(sizeof...(Types) > 0, "Vectors must have at least one coordinate");
    return { static_cast<std::common_type_t<Types...>>(values)... };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GAME_VECTOR_H
