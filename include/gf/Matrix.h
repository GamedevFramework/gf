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
#ifndef GF_MATRIX_H
#define GF_MATRIX_H

#include <cstddef>
#include <algorithm>

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
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

    /**
     * @brief Constructor that fills the matrix with a value.
     */
    explicit Matrix(T val) {
      std::fill_n(data, ROWS * COLS, val);
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
   * @ingroup core
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

    /**
     * @brief Constructor that takes all the elements
     *
     * @param xx The element in the first row and first column
     * @param xy The element in the first row and second column
     * @param yx The element in the second row and first column
     * @param yy The element in the second row and second column
     */
    constexpr Matrix(T xx, T xy, T yx, T yy)
    : data{xx, xy, yx, yy}
    {

    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T grid[2][2]; ///< Grid representation
      T data[4]; ///< Array representation
      struct {
        T xx, xy;
        T yx, yy;
      };
    };
  };

  /**
   * @ingroup core
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

    /**
     * @brief Constructor that takes all the elements
     *
     * @param xx The element in the first row and first column
     * @param xy The element in the first row and second column
     * @param xz The element in the first row and third column
     * @param yx The element in the second row and first column
     * @param yy The element in the second row and second column
     * @param yz The element in the second row and third column
     * @param zx The element in the third row and first column
     * @param zy The element in the third row and second column
     * @param zz The element in the third row and third column
     */
    constexpr Matrix(T xx, T xy, T xz, T yx, T yy, T yz, T zx, T zy, T zz)
    : data{xx, xy, xz, yx, yy, yz, zx, zy, zz}
    {

    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T grid[3][3]; ///< Grid representation
      T data[9]; ///< Array representation
      struct {
        T xx, xy, xz;
        T yx, yy, yz;
        T zx, zy, zz;
      };
    };
  };

  /**
   * @ingroup core
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
     * @brief Constructor that takes all the elements
     *
     * @param xx The element in the first row and first column
     * @param xy The element in the first row and second column
     * @param xz The element in the first row and third column
     * @param xw The element in the first row and fourth column
     * @param yx The element in the second row and first column
     * @param yy The element in the second row and second column
     * @param yz The element in the second row and third column
     * @param yw The element in the second row and fourth column
     * @param zx The element in the third row and first column
     * @param zy The element in the third row and second column
     * @param zz The element in the third row and third column
     * @param zw The element in the third row and fourth column
     * @param wx The element in the fourth row and first column
     * @param wy The element in the fourth row and second column
     * @param wz The element in the fourth row and third column
     * @param ww The element in the fourth row and fourth column
     */
    constexpr Matrix(T xx, T xy, T xz, T xw, T yx, T yy, T yz, T yw, T zx, T zy, T zz, T zw, T wx, T wy, T wz, T ww)
    : data{xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww}
    {

    }

    /**
     * An anonymous union to handle the various representations
     */
    union {
      T grid[4][4]; ///< Grid representation
      T data[16]; ///< Array representation
      struct {
        T xx, xy, xz, xw;
        T yx, yy, yz, yw;
        T zx, zy, zz, zw;
        T wx, wy, wz, ww;
      };
    };
  };

  /**
   * @ingroup core
   * @brief A float square matrix of size 2
   *
   * @sa gf::Matrix, gf::Matrix<T, 2, 2>
   */
  using Matrix2f = Matrix<float, 2, 2>;

  /**
   * @ingroup core
   * @brief A float square matrix of size 3
   *
   * @sa gf::Matrix, gf::Matrix<T, 3, 3>
   */
  using Matrix3f = Matrix<float, 3, 3>;

  /**
   * @ingroup core
   * @brief A float square matrix of size 4
   *
   * @sa gf::Matrix, gf::Matrix<T, 4, 4>
   */
  using Matrix4f = Matrix<float, 4, 4>;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct Matrix<float, 2, 2>;
  extern template struct Matrix<float, 3, 3>;
  extern template struct Matrix<float, 4, 4>;
#endif

  /*
   * equality operations
   */

  /**
   * @relates Matrix
   * @brief Equality operator between two matrices
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  bool operator==(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        if (lhs.grid[i][j] != rhs.grid[i][j]) {
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
  inline
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
  inline
  Matrix<T, ROWS, COLS> operator-(const Matrix<T, ROWS, COLS>& val) {
    Matrix<T, ROWS, COLS> out;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out.grid[i][j] = - val.grid[i][j];
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
  inline
  Matrix<T, ROWS, COLS> operator+(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    Matrix<T, ROWS, COLS> out;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out.grid[i][j] = lhs.grid[i][j] + rhs.grid[i][j];
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Component-wise addition and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS>& operator+=(Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs.grid[i][j] += rhs.grid[i][j];
      }
    }

    return lhs;
  }


  /**
   * @relates Matrix
   * @brief Component-wise substraction
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS> operator-(const Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    Matrix<T, ROWS, COLS> out;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out.grid[i][j] = lhs.grid[i][j] - rhs.grid[i][j];
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Component-wise substraction and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS>& operator-=(Matrix<T, ROWS, COLS>& lhs, const Matrix<T, ROWS, COLS>& rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs.grid[i][j] -= rhs.grid[i][j];
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
  inline
  Matrix<T, ROWS, COLS> operator*(const Matrix<T, ROWS, COLS>& lhs, T rhs) {
    Matrix<T, ROWS, COLS> out;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out.grid[i][j] = lhs.grid[i][j] * rhs;
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Right scalar multiplication and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS>& operator*=(Matrix<T, ROWS, COLS>& lhs, T rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs.grid[i][j] *= rhs;
      }
    }

    return lhs;
  }

  /**
   * @relates Matrix
   * @brief Left scalar multiplication
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS> operator*(T lhs, const Matrix<T, ROWS, COLS>& rhs) {
    Matrix<T, ROWS, COLS> out;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out.grid[i][j] = lhs * rhs.grid[i][j];
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Right scalar division
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS> operator/(const Matrix<T, ROWS, COLS>& lhs, T rhs) {
    Matrix<T, ROWS, COLS> out;

    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        out.grid[i][j] = lhs.grid[i][j] / rhs;
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Right scalar division and assignment
   */
  template<typename T, std::size_t ROWS, std::size_t COLS>
  inline
  Matrix<T, ROWS, COLS>& operator/=(Matrix<T, ROWS, COLS>& lhs, T rhs) {
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLS; ++j) {
        lhs.grid[i][j] /= rhs;
      }
    }

    return lhs;
  }

  /**
   * @relates Matrix
   * @brief Matrix-vector multiplication
   */
  template<typename T, std::size_t S1, std::size_t S2>
  inline
  Vector<T, S1> operator*(const Matrix<T, S1, S2>& lhs, const Vector<T, S2>& rhs) {
    Vector<T, S1> out;

    for (std::size_t i = 0; i < S1; ++i) {
      T val{0};

      for (std::size_t j = 0; j < S2; ++j) {
        val += lhs.grid[i][j] * rhs.data[j];
      }

      out.data[i] = val;
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Vector-matrix multiplication
   */
  template<typename T, std::size_t S1, std::size_t S2>
  inline
  Vector<T, S2> operator*(const Vector<T, S1>& lhs, const Matrix<T, S1, S2>& rhs) {
    Vector<T, S2> out;

    for (std::size_t j = 0; j < S2; ++j) {
      T val{0};

      for (std::size_t i = 0; i < S1; ++i) {
        val += lhs.data[i] * rhs.grid[i][j];
      }

      out.data[j] = val;
    }

    return out;
  }


  /**
   * @relates Matrix
   * @brief Matrix-matrix multiplication
   */
  template<typename T, std::size_t S1, std::size_t S2, std::size_t S3>
  inline
  Matrix<T, S1, S3> operator*(const Matrix<T, S1, S2>& lhs, const Matrix<T, S2, S3>& rhs) {
    Matrix<T, S1, S3> out;

    for (std::size_t i = 0; i < S1; ++i) {
      for (std::size_t j = 0; j < S3; ++j) {
        T val{0};

        for (std::size_t k = 0; k < S2; ++k) {
          val += lhs.grid[i][k] * rhs.grid[k][j];
        }

        out.grid[i][j] = val;
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
  inline
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
  inline
  MatrixType identity() {
    static_assert(MatrixType::Rows == MatrixType::Cols, "identity() is only defined for square matrices.");

    MatrixType out;

    typedef typename MatrixType::value_type value_type;

    for (std::size_t i = 0; i < MatrixType::Rows; ++i) {
      for (std::size_t j = 0; j < MatrixType::Cols; ++j) {
        out.grid[i][j] = (i == j) ? value_type{1} : value_type{0};
      }
    }

    return out;
  }

  /**
   * @relates Matrix
   * @brief Transposition of a matrix
   */
  template<typename T, std::size_t S1, std::size_t S2>
  inline
  Matrix<T, S2, S1> transpose(const Matrix<T, S1, S2>& mat) {
    Matrix<T, S2, S1> out;

    for (std::size_t i = 0; i < S1; ++i) {
      for (std::size_t j = 0; j < S2; ++j) {
        out.grid[j][i] = mat.grid[i][j];
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
  inline
  Matrix<T, 2, 2> invert(const Matrix<T, 2, 2>& mat) {
    Matrix<T, 2, 2> out;

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
  inline
  Matrix<T, 3, 3> invert(const Matrix<T, 3, 3>& mat) {
    Matrix<T, 3, 3> out;

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
