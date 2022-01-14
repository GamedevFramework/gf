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
#ifndef GF_MATRIX_H
#define GF_MATRIX_H

#include <cstddef>
#include <algorithm>

#include "CoreApi.h"
#include "Vector.h"
#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_maths
   * @brief General purpose math matrix
   *
   * gf::Matrix represents a matrix with `ROWS` rows and `COLS` columns. The
   * internal representation uses a [row-major order](https://en.wikipedia.org/wiki/Row-major_order).
   *
   * The template parameter `T` is the type of coordinates. It can be any
   * type that supports arithmetic operations (+, -, *, /).
   *
   * Several specializations are defined for common use cases, especially for
   * [square matrices](https://en.wikipedia.org/wiki/Square_matrix):
   *
   * - For dimension 2: gf::Matrix<T, 2, 2>
   * - For dimension 3: gf::Matrix<T, 3, 3>
   * - For dimension 4: gf::Matrix<T, 4, 4>
   *
   * This class was designed according to the article
   * [On Vector Math Libraries](http://www.reedbeta.com/blog/2013/12/28/on-vector-math-libraries/)
   * by Nathan Reed.
   *
   * @sa gf::Array2D
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  struct Matrix {
    #ifndef DOXYGEN_SHOULD_SKIP_THIS
    static_assert(ROWS > 0, "ROWS must be strictly positive");
    static_assert(COLS > 0, "COLS must be strictly positive");
    #endif

    /**
     * @brief The value type of the elements of the matrix
     */
    using value_type = T;

    /**
     * @brief The number of rows in the matrix
     */
    static constexpr std::size_t Rows = ROWS;

    /**
     * @brief The number of columns in the matrix
     */
    static constexpr std::size_t Cols = COLS;

    /**
     * @brief Default constructor
     */
    Matrix() = default;

    constexpr Matrix(ZeroType) noexcept
    {
      zero();
    }

    /**
     * @brief Constructor that fills the matrix with a value.
     */
    explicit Matrix(T val) noexcept
    {
      std::fill_n(data, ROWS * COLS, val);
    }

    explicit Matrix(const T *array)
    {
      std::copy_n(array, ROWS * COLS, data);
    }

    T operator()(std::size_t row, std::size_t col) const {
      return grid[row][col];
    }

    T& operator()(std::size_t row, std::size_t col) {
      return grid[row][col];
    }

    /**
     * @brief Zero out the matrix
     */
    constexpr void zero() noexcept {
      for (std::size_t i = 0; i < ROWS * COLS; ++i) {
        data[i] = T{};
      }
    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T grid[ROWS][COLS]; ///< Grid representation
      T data[ROWS * COLS]; ///< Array representation
    };
  };

  /**
   * @ingroup core_maths
   * @brief A 2x2 matrix
   *
   * This specialization of gf::Matrix handles linear transformation of 2D
   * vectors. As translation is not a linear transformation but an affine
   * transformation, this specialization is not used in the library. Instead
   * use a gf::Matrix<T, 3, 3>. It is provided for convinience.
   *
   * Each element of the matrix can be accessed through a name with a double
   * letter indicating the column and row of the element (e.g. `xy` indicates
   * the first row and second column).
   *
   * A common typedef is defined:
   *
   * - gf::Matrix2f with `float` as `T`
   */
  template<typename T>
  struct Matrix<T, 2, 2> {
    /**
     * @brief The value type of the elements of the matrix
     */
    using value_type = T;

    /**
     * @brief The number of rows in the matrix
     */
    static constexpr std::size_t Rows = 2;

    /**
     * @brief The number of columns in the matrix
     */
    static constexpr std::size_t Cols = 2;

    /**
     * @brief Default constructor
     */
    Matrix() = default;

    constexpr Matrix(ZeroType) noexcept
    {
      zero();
    }

    explicit constexpr Matrix(T val) noexcept
    : m11(val), m12(val)
    , m21(val), m22(val)
    {
    }

    explicit constexpr Matrix(const T *array)
    : m11(array[0]), m12(array[1])
    , m21(array[2]), m22(array[3])
    {
    }

    /**
     * @brief Constructor that takes all the elements
     *
     * @param e11 The element in the first row and first column
     * @param e12 The element in the first row and second column
     * @param e21 The element in the second row and first column
     * @param e22 The element in the second row and second column
     */
    constexpr Matrix(T e11, T e12, T e21, T e22) noexcept
    : m11(e11), m12(e12)
    , m21(e21), m22(e22)
    {
    }

    const T *getData() const noexcept {
      return &m11;
    }

    T *getData() noexcept {
      return &m11;
    }

    T operator()(std::size_t row, std::size_t col) const {
      const T *grid[Rows][Cols] = {
        { &m11, &m12 },
        { &m21, &m22 }
      };
      return *grid[row][col];
    }

    T& operator()(std::size_t row, std::size_t col) {
      T *grid[Rows][Cols] = {
        { &m11, &m12 },
        { &m21, &m22 }
      };
      return *grid[row][col];
    }

    constexpr void zero() noexcept {
      m11 = m12 = T{};
      m21 = m22 = T{};
    }

    union {
      T xx;
      T m11;
    };

    union {
      T xy;
      T m12;
    };

    union {
      T yx;
      T m21;
    };

    union {
      T yy;
      T m22;
    };
  };

  /**
   * @ingroup core_maths
   * @brief A 3x3 matrix
   *
   * This specialization of gf::Matrix handles affine transformation of 2D
   * vectors (using homogeneous coordinates).
   *
   * Each element of the matrix can be accessed through a name with a double
   * letter indicating the column and row of the element (e.g. `xy` indicates
   * the first row and second column).
   *
   * A common typedef is defined:
   *
   * - gf::Matrix3f with `float` as `T`
   *
   * @sa transform, translate, translation, rotate, rotation, scale, scaling
   */
  template<typename T>
  struct Matrix<T, 3, 3> {
    /**
     * @brief The value type of the elements of the matrix
     */
    using value_type = T;

    /**
     * @brief The number of rows in the matrix
     */
    static constexpr std::size_t Rows = 3;

    /**
     * @brief The number of columns in the matrix
     */
    static constexpr std::size_t Cols = 3;

    /**
     * @brief Default constructor
     */
    Matrix() = default;

    constexpr Matrix(ZeroType) noexcept
    {
      zero();
    }

    explicit constexpr Matrix(T val) noexcept
    : m11(val), m12(val), m13(val)
    , m21(val), m22(val), m23(val)
    , m31(val), m32(val), m33(val)
    {
    }

    explicit constexpr Matrix(const T *array)
    : m11(array[0]), m12(array[1]), m13(array[2])
    , m21(array[3]), m22(array[4]), m23(array[5])
    , m31(array[6]), m32(array[7]), m33(array[8])
    {
    }

    /**
     * @brief Constructor that takes all the elements
     *
     * @param e11 The element in the first row and first column
     * @param e12 The element in the first row and second column
     * @param e13 The element in the first row and third column
     * @param e21 The element in the second row and first column
     * @param e22 The element in the second row and second column
     * @param e23 The element in the second row and third column
     * @param e31 The element in the third row and first column
     * @param e32 The element in the third row and second column
     * @param e33 The element in the third row and third column
     */
    constexpr Matrix(T e11, T e12, T e13, T e21, T e22, T e23, T e31, T e32, T e33) noexcept
    : m11(e11), m12(e12), m13(e13)
    , m21(e21), m22(e22), m23(e23)
    , m31(e31), m32(e32), m33(e33)
    {
    }

    const T *getData() const noexcept {
      return &m11;
    }

    T *getData() noexcept {
      return &m11;
    }

    T operator()(std::size_t row, std::size_t col) const {
      const T *grid[Rows][Cols] = {
        { &m11, &m12, &m13 },
        { &m21, &m22, &m23 },
        { &m31, &m32, &m33 }
      };
      return *grid[row][col];
    }

    T& operator()(std::size_t row, std::size_t col) {
      T *grid[Rows][Cols] = {
        { &m11, &m12, &m13 },
        { &m21, &m22, &m23 },
        { &m31, &m32, &m33 }
      };
      return *grid[row][col];
    }

    constexpr void zero() noexcept {
      m11 = m12 = m13 = T{};
      m21 = m22 = m23 = T{};
      m31 = m32 = m33 = T{};
    }

    union {
      T xx;
      T m11;
    };

    union {
      T xy;
      T m12;
    };

    union {
      T xz;
      T m13;
    };

    union {
      T yx;
      T m21;
    };

    union {
      T yy;
      T m22;
    };

    union {
      T yz;
      T m23;
    };

    union {
      T zx;
      T m31;
    };

    union {
      T zy;
      T m32;
    };

    union {
      T zz;
      T m33;
    };
  };

  /**
   * @ingroup core_maths
   * @brief A 4x4 matrix
   *
   * This specialization of gf::Matrix handles affine transformation of 3D
   * vectors (using homogeneous coordinates).
   *
   * Each element of the matrix can be accessed through a name with a double
   * letter indicating the column and row of the element (e.g. `xy` indicates
   * the first row and second column).
   *
   * A common typedef is defined:
   *
   * - gf::Matrix4f with `float` as `T`
   */
  template<typename T>
  struct Matrix<T, 4, 4> {
    /**
     * @brief The value type of the elements of the matrix
     */
    using value_type = T;

    /**
     * @brief The number of rows in the matrix
     */
    static constexpr std::size_t Rows = 4;

    /**
     * @brief The number of columns in the matrix
     */
    static constexpr std::size_t Cols = 4;

    /**
     * @brief Default constructor
     */
    Matrix() = default;

    constexpr Matrix(ZeroType) noexcept
    {
      zero();
    }

    explicit constexpr Matrix(T val) noexcept
    : m11(val), m12(val), m13(val), m14(val)
    , m21(val), m22(val), m23(val), m24(val)
    , m31(val), m32(val), m33(val), m34(val)
    , m41(val), m42(val), m43(val), m44(val)
    {
    }

    explicit constexpr Matrix(const T *array)
    : m11(array[ 0]), m12(array[ 1]), m13(array[ 2]), m14(array[ 3])
    , m21(array[ 4]), m22(array[ 5]), m23(array[ 6]), m24(array[ 7])
    , m31(array[ 8]), m32(array[ 9]), m33(array[10]), m34(array[11])
    , m41(array[12]), m42(array[13]), m43(array[14]), m44(array[15])
    {
    }

    /**
     * @brief Constructor that takes all the elements
     *
     * @param e11 The element in the first row and first column
     * @param e12 The element in the first row and second column
     * @param e13 The element in the first row and third column
     * @param e14 The element in the first row and fourth column
     * @param e21 The element in the second row and first column
     * @param e22 The element in the second row and second column
     * @param e23 The element in the second row and third column
     * @param e24 The element in the second row and fourth column
     * @param e31 The element in the third row and first column
     * @param e32 The element in the third row and second column
     * @param e33 The element in the third row and third column
     * @param e34 The element in the third row and fourth column
     * @param e41 The element in the fourth row and first column
     * @param e42 The element in the fourth row and second column
     * @param e43 The element in the fourth row and third column
     * @param e44 The element in the fourth row and fourth column
     */
    constexpr Matrix(T e11, T e12, T e13, T e14, T e21, T e22, T e23, T e24, T e31, T e32, T e33, T e34, T e41, T e42, T e43, T e44) noexcept
    : m11(e11), m12(e12), m13(e13), m14(e14)
    , m21(e21), m22(e22), m23(e23), m24(e24)
    , m31(e31), m32(e32), m33(e33), m34(e34)
    , m41(e41), m42(e42), m43(e43), m44(e44)
    {
    }

    const T *getData() const noexcept {
      return &m11;
    }

    T *getData() noexcept {
      return &m11;
    }

    T operator()(std::size_t row, std::size_t col) const {
      const T *grid[Rows][Cols] = {
        { &m11, &m12, &m13, &m14 },
        { &m21, &m22, &m23, &m24 },
        { &m31, &m32, &m33, &m34 },
        { &m41, &m42, &m43, &m44 }
      };
      return *grid[row][col];
    }

    T& operator()(std::size_t row, std::size_t col) {
      T *grid[Rows][Cols] = {
        { &m11, &m12, &m13, &m14 },
        { &m21, &m22, &m23, &m24 },
        { &m31, &m32, &m33, &m34 },
        { &m41, &m42, &m43, &m44 }
      };
      return *grid[row][col];
    }

    constexpr void zero() noexcept {
      m11 = m12 = m13 = m14 = T{};
      m21 = m22 = m23 = m24 = T{};
      m31 = m32 = m33 = m34 = T{};
      m41 = m42 = m43 = m44 = T{};
    }

    union {
      T xx;
      T m11;
    };

    union {
      T xy;
      T m12;
    };

    union {
      T xz;
      T m13;
    };

    union {
      T xw;
      T m14;
    };

    union {
      T yx;
      T m21;
    };

    union {
      T yy;
      T m22;
    };

    union {
      T yz;
      T m23;
    };

    union {
      T yw;
      T m24;
    };

    union {
      T zx;
      T m31;
    };

    union {
      T zy;
      T m32;
    };

    union {
      T zz;
      T m33;
    };

    union {
      T zw;
      T m34;
    };

    union {
      T wx;
      T m41;
    };

    union {
      T wy;
      T m42;
    };

    union {
      T wz;
      T m43;
    };

    union {
      T ww;
      T m44;
    };
  };

  /**
   * @ingroup core_maths
   * @brief A float square matrix of size 2
   *
   * @sa gf::Matrix, gf::Matrix<T, 2, 2>
   */
  using Matrix2f = Matrix<float, 2, 2>;

  /**
   * @ingroup core_maths
   * @brief A double square matrix of size 2
   *
   * @sa gf::Matrix, gf::Matrix<T, 2, 2>
   */
  using Matrix2d = Matrix<double, 2, 2>;

  /**
   * @ingroup core_maths
   * @brief A float square matrix of size 3
   *
   * @sa gf::Matrix, gf::Matrix<T, 3, 3>
   */
  using Matrix3f = Matrix<float, 3, 3>;

  /**
   * @ingroup core_maths
   * @brief A double square matrix of size 3
   *
   * @sa gf::Matrix, gf::Matrix<T, 3, 3>
   */
  using Matrix3d = Matrix<double, 3, 3>;

  /**
   * @ingroup core_maths
   * @brief A float square matrix of size 4
   *
   * @sa gf::Matrix, gf::Matrix<T, 4, 4>
   */
  using Matrix4f = Matrix<float, 4, 4>;

  /**
   * @ingroup core_maths
   * @brief A double square matrix of size 4
   *
   * @sa gf::Matrix, gf::Matrix<T, 4, 4>
   */
  using Matrix4d = Matrix<double, 4, 4>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API Matrix<float, 2, 2>;
  extern template struct GF_CORE_API Matrix<float, 3, 3>;
  extern template struct GF_CORE_API Matrix<float, 4, 4>;

  extern template struct GF_CORE_API Matrix<double, 2, 2>;
  extern template struct GF_CORE_API Matrix<double, 3, 3>;
  extern template struct GF_CORE_API Matrix<double, 4, 4>;
#endif

  /*
   * equality operations
   */

  /**
   * @relates Matrix
   * @brief Equality operator between two matrices
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  bool operator==(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        if (lhs(i,j) != rhs(i,j)) {
          return false;
        }
      }
    }

    return true;
  }

  /**
   * @relates Matrix
   * @brief Inequality operator between two matrices
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  bool operator!=(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    return !(lhs == rhs);
  }

  /*
   * unary operators
   */

  /**
   * @relates Matrix
   * @brief Component-wise unary minus
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS> operator-(const Matrix<T, ROWS, COLS>& val) {
    Matrix<T, ROWS, COLS> out = gf::Zero;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out(i,j) = - val(i,j);
      }
    }

    return out;
  }

  /*
   * arithmetic component-wise operations
   */

  /**
   * @relates Matrix
   * @brief Component-wise addition
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS> operator+(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    Matrix<T, ROWS, COLS> out = gf::Zero;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out(i,j) = lhs(i,j) + rhs(i,j);
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Component-wise addition and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS>& operator+=(Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs(i,j) += rhs(i,j);
      }
    }

    return lhs;
  }


  /**
   * @relates Matrix
   * @brief Component-wise substraction
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS> operator-(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    Matrix<T, ROWS, COLS> out = gf::Zero;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out(i,j) = lhs(i,j) - rhs(i,j);
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Component-wise substraction and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS>& operator-=(Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs(i,j) -= rhs(i,j);
      }
    }

    return lhs;
  }

  /*
   * multiplication and division
   */

  /**
   * @relates Matrix
   * @brief Right scalar multiplication
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS> operator*(const Matrix<T, ROWS, COLS>& lhs, T rhs) {
    Matrix<T, ROWS, COLS> out = gf::Zero;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out(i,j) = lhs(i,j) * rhs;
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Right scalar multiplication and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS>& operator*=(Matrix<T, ROWS, COLS>& lhs, T rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs(i,j) *= rhs;
      }
    }

    return lhs;
  }

  /**
   * @relates Matrix
   * @brief Left scalar multiplication
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS> operator*(T lhs, const Matrix<T, ROWS, COLS>& rhs) {
    Matrix<T, ROWS, COLS> out = gf::Zero;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out(i,j) = lhs * rhs(i,j);
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Right scalar division
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS> operator/(const Matrix<T, ROWS, COLS>& lhs, T rhs) {
    Matrix<T, ROWS, COLS> out = gf::Zero;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out(i,j) = lhs(i,j) / rhs;
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Right scalar division and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  constexpr
  Matrix<T, ROWS, COLS>& operator/=(Matrix<T, ROWS, COLS>& lhs, T rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs(i,j) /= rhs;
      }
    }

    return lhs;
  }

  /**
   * @relates Matrix
   * @brief Matrix-vector multiplication
   */
  template<typename T, std::size_t S1, std::size_t S2>
  constexpr
  Vector<T, S1> operator*(const Matrix<T, S1, S2>& lhs, const Vector<T, S2>& rhs) {
    Vector<T, S1> out = gf::Zero;

    for (std::size_t i = 0; i < S1; ++i) {
      T val{0};

      for (std::size_t j = 0; j < S2; ++j) {
        val += lhs(i,j) * rhs[j];
      }

      out[i] = val;
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Vector-matrix multiplication
   */
  template<typename T, std::size_t S1, std::size_t S2>
  constexpr
  Vector<T, S2> operator*(const Vector<T, S1>& lhs, const Matrix<T, S1, S2>& rhs) {
    Vector<T, S2> out = gf::Zero;

    for (std::size_t j = 0; j < S2; ++j) {
      T val{0};

      for (std::size_t i = 0; i < S1; ++i) {
        val += lhs[i] * rhs(i,j);
      }

      out[j] = val;
    }

    return out;
  }


  /**
   * @relates Matrix
   * @brief Matrix-matrix multiplication
   */
  template<typename T, std::size_t S1, std::size_t S2, std::size_t S3>
  constexpr
  Matrix<T, S1, S3> operator*(const Matrix<T, S1, S2>& lhs, const Matrix<T, S2, S3>& rhs) {
    Matrix<T, S1, S3> out = gf::Zero;

    for (std::size_t i = 0; i < S1; ++i) {
      for (std::size_t j = 0; j < S3; ++j) {
        T val{0};

        for (std::size_t k = 0; k < S2; ++k) {
          val += lhs(i,k) * rhs(k,j);
        }

        out(i,j) = val;
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Matrix-matrix multiplication and assignment
   *
   * This operation is only available for square matrices.
   */
  template<typename T, std::size_t N>
  constexpr
  Matrix<T, N, N>& operator*=(Matrix<T, N, N>& lhs, const Matrix<T, N, N>& rhs) {
    lhs = lhs * rhs;
    return lhs;
  }

  /*
   * usual operations
   */

  /**
   * @relates Matrix
   * @brief Identity matrix
   *
   * @sa gf::identityTransform()
   */
  template<typename MatrixType>
  constexpr
  MatrixType identity() {
    static_assert(MatrixType::Rows == MatrixType::Cols, "identity() is only defined for square matrices.");

    MatrixType out = gf::Zero;

    typedef typename MatrixType::value_type value_type;

    for (std::size_t i = 0; i < MatrixType::Rows; ++i) {
      for (std::size_t j = 0; j < MatrixType::Cols; ++j) {
        out(i,j) = (i == j) ? value_type{1} : value_type{0};
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Transposition of a matrix
   */
  template<typename T, std::size_t S1, std::size_t S2>
  constexpr
  Matrix<T, S2, S1> transpose(const Matrix<T, S1, S2>& mat) {
    Matrix<T, S2, S1> out = gf::Zero;

    for (std::size_t i = 0; i < S1; ++i) {
      for (std::size_t j = 0; j < S2; ++j) {
        out(j,i) = mat(i,j);
      }
    }

    return out;
  }

  // https://en.wikipedia.org/wiki/Invertible_matrix

  /**
   * @relates Matrix
   * @brief Inversion of a 2x2 matrix
   */
  template<typename T>
  constexpr
  Matrix<T, 2, 2> invert(const Matrix<T, 2, 2>& mat) {
    Matrix<T, 2, 2> out = gf::Zero;

    out.xx = mat.yy;
    out.xy = - mat.xy;
    out.yx = - mat.yx;
    out.yy = mat.xx;

    T det = mat.xx * mat.yy - mat.yx * mat.xy;
    out /= det;
    return out;
  }

  /**
   * @relates Matrix
   * @brief Inversion of a 3x3 matrix
   */
  template<typename T>
  constexpr
  Matrix<T, 3, 3> invert(const Matrix<T, 3, 3>& mat) {
    Matrix<T, 3, 3> out = gf::Zero;

    out.xx = mat.yy * mat.zz - mat.zy * mat.yz;
    out.xy = - (mat.xy * mat.zz - mat.zy * mat.xz);
    out.xz = mat.xy * mat.yz - mat.yy * mat.xz;
    out.yx = - (mat.yx * mat.zz - mat.zx * mat.yz);
    out.yy = mat.xx * mat.zz - mat.zx * mat.xz;
    out.yz = - (mat.xx * mat.yz - mat.yx * mat.xz);
    out.zx = mat.yx * mat.zy - mat.zx * mat.yy;
    out.zy = - (mat.xx * mat.zy - mat.zx * mat.xy);
    out.zz = mat.xx * mat.yy - mat.yx * mat.xy;

    T det = mat.xx * out.xx + mat.xy * out.yx + mat.xz * out.zx;
    out /= det;
    return out;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GAME_MATRIX_H
