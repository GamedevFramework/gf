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
#ifndef GF_VECTOR_H
#define GF_VECTOR_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <initializer_list>
#include <type_traits>

#include "Math.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
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
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
     */
    Vector() = default;

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
    explicit Vector(T val)
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
    explicit Vector(T *array)
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
    Vector(std::initializer_list<T> list)
    {
      std::copy_n(list.begin(), std::min(list.size(), N), data);
    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
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
    Vector(const Vector<U, N>& other)
    {
      static_assert(std::is_convertible<U,T>::value, "");
      std::transform(data, data + N, other.data, [](U val) { return static_cast<T>(val); });
    }

    /**
     * @brief Default copy assignment
     *
     * This operator is defaulted so that this type is
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
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
    T operator[](std::size_t i) const {
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
    T& operator[](std::size_t i) {
      return data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    inline
    T *begin(void) {
      return &(data[0]);
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    inline
    T *end(void) {
      return &(data[N]);
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *begin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *end(void) const {
      return &(data[N]);
    }
    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *cbegin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *cend(void) const {
      return &(data[N]);
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
   * @ingroup core
   * @brief A 2D vector
   *
   * This specialization of gf::Vector handles the 2-dimension spaces. It can
   * be accessed with various representations:
   *
   * - the generic representation with the `data` member
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
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
     */
    Vector() = default;

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
    explicit Vector(T val) {
      std::fill_n(data, 2, val);
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
    explicit Vector(T *array)
    {
      std::copy_n(array, 2, data);
    }

    /**
     * @brief Constructor that takes 2 components
     *
     * @param x The first component
     * @param y The second component
     */
    constexpr Vector(T x, T y)
    : data{ x, y }
    {

    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
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
    Vector(const Vector<U, 2>& other)
    : x(other.x)
    , y(other.y)
    {

    }

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
    T operator[](std::size_t i) const {
      return data[i];
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
    T& operator[](std::size_t i) {
      return data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    inline
    T *begin(void) {
      return &(data[0]);
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    inline
    T *end(void) {
      return &(data[2]);
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *begin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *end(void) const {
      return &(data[2]);
    }
    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *cbegin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *cend(void) const {
      return &(data[2]);
    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T data[2]; ///< Generic representation
      struct {
        T x; ///< First coordinate in the `(x,y)` representation. @sa y
        T y; ///< Second coordinate in the `(x,y)` representation. @sa x
      };
      struct {
        T u; ///< First coordinate in the `(u,v)` representation. @sa v
        T v; ///< Second coordinate in the `(u,v)` representation. @sa u
      };
      struct {
        T s; ///< First coordinate in the `(s,t)` representation. @sa t
        T t; ///< Second coordinate in the `(s,t)` representation. @sa s
      };
      struct {
        T width; ///< First coordinate in the size representation. @sa height
        T height; ///< Second coordinate in the size representation. @sa width
      };
      struct {
        T col; ///< First coordinate in the indices representation @sa row
        T row; ///< Second coordinate in the indices representation @sa col
      };
    };
  };

  /**
   * @ingroup core
   * @brief A 3D vector
   *
   * This specialization of gf::Vector handles the 3-dimension spaces. It can
   * be accessed with various representations:
   *
   * - the generic representation with the `data` member
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
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
     */
    Vector() = default;

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
    explicit Vector(T val) {
      std::fill_n(data, 3, val);
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
    explicit Vector(T *array)
    {
      std::copy_n(array, 3, data);
    }

    /**
     * @brief Constructor that takes 3 components
     *
     * @param x The first component
     * @param y The second component
     * @param z The third component
     */
    constexpr Vector(T x, T y, T z)
    : data{ x, y, z }
    {

    }

    /**
     * @brief Constructor that takes a 2D vector and a z component
     *
     * @param xy The first 2 component, x and y
     * @param z The z component
     */
    constexpr Vector(Vector<T, 2> xy, T z)
    : data{ xy.x, xy.y, z }
    {

    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
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
    Vector(const Vector<U, 3>& other)
    : x(other.x)
    , y(other.y)
    , z(other.z)
    {

    }

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
    T operator[](std::size_t i) const {
      return data[i];
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
    T& operator[](std::size_t i) {
      return data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    inline
    T *begin(void) {
      return &(data[0]);
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    inline
    T *end(void) {
      return &(data[3]);
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *begin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *end(void) const {
      return &(data[3]);
    }
    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *cbegin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *cend(void) const {
      return &(data[3]);
    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T data[3]; ///< Generic representation
      struct {
        T x; ///< First coordinate in the `(x,y,z)` representation. @sa y, z
        T y; ///< Second coordinate in the `(x,y,z)` representation. @sa x, z
        T z; ///< Third coordinate in the `(x,y,z)` representation. @sa x, y
      };
      struct {
        T r; ///< First coordinate in the `(r,g,b)` representation. @sa g, b
        T g; ///< Second coordinate in the `(r,g,b)` representation. @sa r, b
        T b; ///< Third coordinate in the `(r,g,b)` representation. @sa r, g
      };
      Vector<T, 2> xy; ///< Swizzle to get the first two coordinates as a 2D vector
    };
  };

  /**
   * @ingroup core
   * @brief A 4D vector
   *
   * This specialization of gf::Vector handles the 4-dimension spaces. It can
   * be accessed with various representations:
   *
   * - the generic representation with the `data` member
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
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
     */
    Vector() = default;

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
    explicit Vector(T val) {
      std::fill_n(data, 4, val);
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
    explicit Vector(T *array)
    {
      std::copy_n(array, 4, data);
    }

    /**
     * @brief Constructor that takes 4 components
     *
     * @param x The first component
     * @param y The second component
     * @param z The third component
     * @param w The fourth component
     */
    constexpr Vector(T x, T y, T z, T w)
    : data{ x, y, z, w }
    {

    }

    /**
     * @brief Default copy constructor
     *
     * This constructor is defaulted so that this type is
     * [trivial](http://en.cppreference.com/w/cpp/concept/TrivialType).
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
    Vector(const Vector<U, 4>& other)
    : x(other.x)
    , y(other.y)
    , z(other.z)
    , w(other.w)
    {

    }

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
    T operator[](std::size_t i) const {
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
    T& operator[](std::size_t i) {
      return data[i];
    }


    /**
     * @brief Iterator.to the first element.
     *
     * @return A pointer to the first element.
     */
    inline
    T *begin(void) {
      return &(data[0]);
    }

    /**
     * @brief Iterator to the element after the last one.
     *
     * @return An invalid pointer that is the adress after the last element.
     */
    inline
    T *end(void) {
      return &(data[4]);
    }

    /**
     * @brief Iterator.to the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *begin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *end(void) const {
      return &(data[4]);
    }
    /**
     * @brief Iterator.on the first element (const version).
     *
     * @return A pointer on the first const element.
     */
    inline
    T const *cbegin(void) const {
      return &(data[0]);
    }

    /**
     * @brief Iterator on the element after the last one (const version).
     *
     * @return An invalid pointer that is the adress after the last const
     * element.
     */
    inline
    T const *cend(void) const {
      return &(data[4]);
    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T data[4]; ///< Generic representation
      struct {
        T x; ///< First coordinate in the `(x,y,z,w)` representation. @sa y, z, w
        T y; ///< Second coordinate in the `(x,y,z,w)` representation. @sa x, z, w
        T z; ///< Third coordinate in the `(x,y,z,w)` representation. @sa x, y, w
        T w; ///< Fourth coordinate in the `(x,y,z,w)` representation. @sa x, y, z
      };
      struct {
        T r; ///< First coordinate in the `(r,g,b,a)` representation. @sa g, b, a
        T g; ///< Second coordinate in the `(r,g,b,a)` representation. @sa r, b, a
        T b; ///< Third coordinate in the `(r,g,b,a)` representation. @sa r, g, a
        T a; ///< Fourth coordinate in the `(r,g,b,a)` representation. @sa r, g, b
      };
      Vector<T, 2> xy; ///< Swizzle to get the first two coordinates as a 2D vector
      Vector<T, 3> xyz; ///< Swizzle to get the first three coordinates as a 3D vector
      Vector<T, 3> rgb; ///< Swizzle to get the first three coordinates as a RGB color
    };
  };

  /**
   * @ingroup core
   * @brief A float vector with 2 components
   */
  using Vector2f = Vector<float, 2>;

  /**
   * @ingroup core
   * @brief A float vector with 3 components
   */
  using Vector3f = Vector<float, 3>;

  /**
   * @ingroup core
   * @brief A float vector with 4 components
   */
  using Vector4f = Vector<float, 4>;

  /**
   * @ingroup core
   * @brief A double vector with 2 components
   */
  using Vector2d = Vector<double, 2>;

  /**
   * @ingroup core
   * @brief A double vector with 3 components
   */
  using Vector3d = Vector<double, 3>;

  /**
   * @ingroup core
   * @brief A double vector with 4 components
   */
  using Vector4d = Vector<double, 4>;

  /**
   * @ingroup core
   * @brief A int vector with 2 components
   */
  using Vector2i = Vector<int, 2>;

  /**
   * @ingroup core
   * @brief A int vector with 3 components
   */
  using Vector3i = Vector<int, 3>;

  /**
   * @ingroup core
   * @brief A int vector with 4 components
   */
  using Vector4i = Vector<int, 4>;

  /**
   * @ingroup core
   * @brief A unsigned vector with 2 components
   */
  using Vector2u = Vector<unsigned, 2>;

  /**
   * @ingroup core
   * @brief A unsigned vector with 3 components
   */
  using Vector3u = Vector<unsigned, 3>;

  /**
   * @ingroup core
   * @brief A unsigned vector with 4 components
   */
  using Vector4u = Vector<unsigned, 4>;

  /**
   * @ingroup core
   * @brief A std::size_t vector with 2 components
   */
  using Vector2z = Vector<std::size_t, 2>;

  /**
   * @ingroup core
   * @brief A std::size_t vector with 3 components
   */
  using Vector3z = Vector<std::size_t, 3>;

  /**
   * @ingroup core
   * @brief A std::size_t vector with 4 components
   */
  using Vector4z = Vector<std::size_t, 4>;

  /**
   * @ingroup core
   * @brief A bool vector with 2 components
   */
  using Vector2b = Vector<bool, 2>;

  /**
   * @ingroup core
   * @brief A bool vector with 3 components
   */
  using Vector3b = Vector<bool, 3>;

  /**
   * @ingroup core
   * @brief A bool vector with 4 components
   */
  using Vector4b = Vector<bool, 4>;

  /**
   * @ingroup core
   * @brief A float color vector with 3 components
   */
  using Color3f = Vector<float, 3>;

  /**
   * @ingroup core
   * @brief A float color vector with 4 components
   */
  using Color4f = Vector<float, 4>;

  /**
   * @ingroup core
   * @brief A uint8_t color vector with 3 components
   */
  using Color3u = Vector<uint8_t, 3>;

  /**
   * @ingroup core
   * @brief A uint8_t color vector with 4 components
   */
  using Color4u = Vector<uint8_t, 4>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct Vector<float, 2>;
  extern template struct Vector<float, 3>;
  extern template struct Vector<float, 4>;

  extern template struct Vector<double, 2>;
  extern template struct Vector<double, 3>;
  extern template struct Vector<double, 4>;

  extern template struct Vector<int, 2>;
  extern template struct Vector<int, 3>;
  extern template struct Vector<int, 4>;

  extern template struct Vector<unsigned, 2>;
  extern template struct Vector<unsigned, 3>;
  extern template struct Vector<unsigned, 4>;

  extern template struct Vector<bool, 2>;
  extern template struct Vector<bool, 3>;
  extern template struct Vector<bool, 4>;

  extern template struct Vector<uint8_t, 3>;
  extern template struct Vector<uint8_t, 4>;
#endif

  /**
   * @relates Vector
   * @brief Equality operator between two vectors
   */
  template<typename T, std::size_t N>
  inline
  bool operator==(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      if (lhs.data[i] != rhs.data[i]) {
        return false;
      }
    }

    return true;
  }

  /**
   * @relates Vector
   * @brief Inequality operator between two vectors
   */
  template<typename T, std::size_t N>
  inline
  bool operator!=(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    return !(lhs == rhs);
  }

  /**
   * @relates Vector
   * @brief Component-wise unary minus
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> operator-(const Vector<T, N>& val) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = - val.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise addition
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator+(const Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] + rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise addition and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator+=(Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] += rhs.data[i];
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Right scalar addition
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator+(const Vector<T, N>& lhs, U rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] + rhs;
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Left scalar addition
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator+(T lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs + rhs.data[i];
    }

    return out;
  }


  /**
   * @relates Vector
   * @brief Component-wise substraction
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator-(const Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] - rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise substraction and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator-=(Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] -= rhs.data[i];
    }

    return lhs;
  }


  /**
   * @relates Vector
   * @brief Right scalar substraction
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator-(const Vector<T, N>& lhs, U rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] - rhs;
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Left scalar substraction
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator-(T lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs - rhs.data[i];
    }

    return out;
  }


  /**
   * @relates Vector
   * @brief Component-wise multiplication
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator*(const Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] * rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise multiplication and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator*=(Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] *= rhs.data[i];
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Right scalar multiplication
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator*(const Vector<T, N>& lhs, U rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] * rhs;
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Right scalar multiplication and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator*=(Vector<T, N>& lhs, U rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] *= rhs;
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Left scalar multiplication
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator*(T lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs * rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise division
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator/(const Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] / rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise division and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator/=(Vector<T, N>& lhs, const Vector<U, N>& rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] /= rhs.data[i];
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Right scalar division
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator/(const Vector<T, N>& lhs, U rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] / rhs;
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Right scalar division and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator/=(Vector<T, N>& lhs, U rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] /= rhs;
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Left scalar division
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator/(T lhs, const Vector<U, N>& rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs / rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise logical or operator
   */
  template<std::size_t N>
  inline
  Vector<bool, N> operator||(const Vector<bool, N>& lhs, const Vector<bool, N>& rhs) {
    Vector<bool, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] || rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise logical and operator
   */
  template<std::size_t N>
  inline
  Vector<bool, N> operator&&(const Vector<bool, N>& lhs, const Vector<bool, N>& rhs) {
    Vector<bool, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] && rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Scalar product
   */
  template<typename T, std::size_t N>
  inline
  T dot(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    T out{0};

    for (std::size_t i = 0; i < N; ++i) {
      out += lhs.data[i] * rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise minimum
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> min(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = std::min(lhs.data[i], rhs.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise maximum
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> max(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = std::max(lhs.data[i], rhs.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise absolute value
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> abs(const Vector<T, N>& val) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = std::abs(val.data[i]);
    }

    return out;
  }


  /**
   * @relates Vector
   * @brief Component-wise equality operator
   */
  template<typename T, std::size_t N>
  inline
  Vector<bool, N> equals(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    Vector<bool, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = (lhs.data[i] == rhs.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise comparison operator
   */
  template<typename T, std::size_t N>
  inline
  Vector<bool, N> lessThan(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    Vector<bool, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = (lhs.data[i] < rhs.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise comparison operator
   */
  template<typename T, std::size_t N>
  inline
  Vector<bool, N> greaterThan(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    Vector<bool, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = (lhs.data[i] > rhs.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise selection operator
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> select(const Vector<bool, N>& cond, const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = (cond.data[i] ? lhs.data[i] : rhs.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise clamp function
   *
   * Relative to two other vectors.
   *
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> clamp(const Vector<T, N>& val, const Vector<T, N>& lo, const Vector<T, N>& hi) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = clamp(val.data[i], lo.data[i], hi.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise clamp function
   *
   * Relative to two values.
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> clamp(const Vector<T, N>& val, T lo, T hi) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = clamp(val.data[i], lo, hi);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise lerp function
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<typename std::common_type<T,U>::type, N> lerp(const Vector<T, N>& lhs, const Vector<T, N>& rhs, U t) {
    return t * lhs + (1 - t) * rhs;
  }

  /**
   * @relates Vector
   * @brief Manhattan length of a vector
   *
   * The Manhattan length @f$ l @f$ of a vector @f$ (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ l = \sum_{i = 1}^{N} |u_i| @f]
   *
   * The Manhattan length is also called the 1-norm.
   *
   * @param vec A vector.
   * @returns The Manhattan length of the vector
   *
   * @sa manhattanDistance
   */
  template<typename T, std::size_t N>
  inline
  T manhattanLength(const Vector<T, N>& vec) {
    T out{0};

    for (std::size_t i = 0; i < N; ++i) {
      out += std::abs(vec.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Square Euclidean length of a vector
   *
   * The square Euclidean length @f$ l @f$ of a vector @f$ (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ l = \sum_{i = 1}^{N} u_i^2 @f]
   *
   * @param vec A vector.
   * @returns The square Euclidean length of the vector
   *
   * @sa euclideanLength, squareDistance
   */
  template<typename T, std::size_t N>
  inline
  T squareLength(const Vector<T, N>& vec) {
    T out{0};

    for (std::size_t i = 0; i < N; ++i) {
      out += square(vec.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Euclidean length of a vector
   *
   * The Euclidean length @f$ l @f$ of a vector @f$ (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ l = \sqrt{\sum_{i = 1}^{N} u_i^2} @f]
   *
   * The Euclidean length is also called the 2-norm.
   *
   * @param vec A vector.
   * @returns The Euclidean length of the vector
   *
   * @sa euclideanDistance
   */
  template<typename T, std::size_t N>
  inline
  T euclideanLength(const Vector<T, N>& vec) {
    return std::sqrt(squareLength(vec));
  }

  /**
   * @relates Vector
   * @brief Chebyshev length of a vector
   *
   * The Chebyshev length @f$ l @f$ of a vector @f$ (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ l = \max_{i = 1}^{N} |u_i| @f]
   *
   * The Chebyshev length is also called the infinity norm or maximum norm.
   *
   * @param vec A vector.
   * @returns The Chebyshev length of the vector
   *
   * @sa chebyshevDistance
   */
  template<typename T, std::size_t N>
  inline
  T chebyshevLength(const Vector<T, N>& vec) {
    T out = std::abs(vec.data[0]);

    for (std::size_t i = 1; i < N; ++i) {
      out = std::max(out, std::abs(vec.data[i]));
    }

    return out;
  }


  /**
   * @relates Vector
   * @brief Manhattan distance between two vectors
   *
   * The Manhattan distance between two vectors is the Manhattan length of the
   * difference of the two vectors.
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @returns The Manhattan distance between the two vectors
   *
   * @sa manhattanLength
   */
  template<typename T, std::size_t N>
  inline
  T manhattanDistance(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    return manhattanLength(lhs - rhs);
  }

  /**
   * @relates Vector
   * @brief Square Euclidean distance between two vectors
   *
   * The square Euclidean distance between two vectors is the square Euclidean
   * length of the difference of the two vectors.
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @returns The square Euclidean distance between the two vectors
   *
   * @sa squareLength, euclideanDistance
   */
  template<typename T, std::size_t N>
  inline
  T squareDistance(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    return squareLength(lhs - rhs);
  }

  /**
   * @relates Vector
   * @brief Euclidean distance between two vectors
   *
   * The Euclidean distance between two vectors is the Euclidean length of the
   * difference of the two vectors.
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @returns The Euclidean distance between the two vectors
   *
   * @sa euclideanLength
   */
  template<typename T, std::size_t N>
  inline
  T euclideanDistance(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    return euclideanLength(lhs - rhs);
  }

  /**
   * @relates Vector
   * @brief Chebyshev distance between two vectors
   *
   * The Chebyshev distance between two vectors is the Chebyshev length of the
   * difference of the two vectors.
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @returns The Chebyshev distance between the two vectors
   *
   * @sa chebyshevLength
   */
  template<typename T, std::size_t N>
  inline
  T chebyshevDistance(const Vector<T, N>& lhs, const Vector<T, N>& rhs) {
    return chebyshevDistance(lhs - rhs);
  }

  /**
   * @relates Vector
   * @brief Normalize a vector
   *
   * The normalized vector of @f$ v @f$ is a vector in the same direction but
   * with a (euclidean) length of 1. A normalized vector is also called a
   * [unit vector](https://en.wikipedia.org/wiki/Unit_vector).
   *
   * @param vec A vector
   * @returns A normalized vector
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> normalize(const Vector<T, N>& vec) {
    T length = euclideanLength(vec);
    return vec / length;
  }

  /**
   * @relates Vector
   * @brief Unit vector in a specified direction
   *
   * @param angle The angle of the direction
   * @return A unit vector
   */
  template<typename T>
  inline
  Vector<T, 2> unit(T angle) {
    return { std::cos(angle), std::sin(angle) };
  }

  /**
   * @relates Vector
   * @brief Perpendicular vector
   *
   * @param vec A vector
   * @returns A perpendicular vector
   */
  template<typename T>
  constexpr
  Vector<T, 2> perp(const Vector<T, 2>& vec) {
    return { -vec.y, vec.x };
  }

  /**
   * @relates Vector
   * @brief Cross product
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @return The cross product of the two vectors
   */
  template<typename T>
  constexpr
  Vector<T, 3> cross(const Vector<T, 3>& lhs, const Vector<T, 3>& rhs) {
    return {
      lhs.y * rhs.z - lhs.z * rhs.y,
      lhs.z * rhs.x - lhs.x * rhs.z,
      lhs.x * rhs.y - lhs.y * rhs.x
    };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GAME_VECTOR_H
