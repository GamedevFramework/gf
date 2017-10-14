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
#ifndef GF_VECTOR_OPS_H
#define GF_VECTOR_OPS_H

#include <cmath>
#include <algorithm>
#include <type_traits>

#include "Math.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @relates Vector
   * @brief Equality operator between two vectors
   */
  template<typename T, std::size_t N>
  inline
  bool operator==(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  bool operator!=(Vector<T, N> lhs, Vector<T, N> rhs) {
    return !(lhs == rhs);
  }

  /**
   * @relates Vector
   * @brief Component-wise unary minus
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> operator-(Vector<T, N> val) {
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
  Vector<typename std::common_type<T,U>::type, N> operator+(Vector<T, N> lhs, Vector<U, N> rhs) {
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
  Vector<T, N>& operator+=(Vector<T, N>& lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator+(Vector<T, N> lhs, U rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] + rhs;
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Right scalar addition and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator+=(Vector<T, N>& lhs, U rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] += rhs;
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Left scalar addition
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator+(T lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator-(Vector<T, N> lhs, Vector<U, N> rhs) {
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
  Vector<T, N>& operator-=(Vector<T, N>& lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator-(Vector<T, N> lhs, U rhs) {
    Vector<typename std::common_type<T,U>::type, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] - rhs;
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Right scalar substraction and assignment
   */
  template<typename T, typename U, std::size_t N>
  inline
  Vector<T, N>& operator-=(Vector<T, N>& lhs, U rhs) {
    for (std::size_t i = 0; i < N; ++i) {
      lhs.data[i] -= rhs;
    }

    return lhs;
  }

  /**
   * @relates Vector
   * @brief Left scalar substraction
   */
  template<typename T, typename U, std::size_t N, typename E = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  inline
  Vector<typename std::common_type<T,U>::type, N> operator-(T lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator*(Vector<T, N> lhs, Vector<U, N> rhs) {
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
  Vector<T, N>& operator*=(Vector<T, N>& lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator*(Vector<T, N> lhs, U rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator*(T lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator/(Vector<T, N> lhs, Vector<U, N> rhs) {
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
  Vector<T, N>& operator/=(Vector<T, N>& lhs, Vector<U, N> rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator/(Vector<T, N> lhs, U rhs) {
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
  Vector<typename std::common_type<T,U>::type, N> operator/(T lhs, Vector<U, N> rhs) {
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
  Vector<bool, N> operator||(Vector<bool, N> lhs, Vector<bool, N> rhs) {
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
  Vector<bool, N> operator&&(Vector<bool, N> lhs, Vector<bool, N> rhs) {
    Vector<bool, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lhs.data[i] && rhs.data[i];
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Scalar product
   *
   * The [scalar product](https://en.wikipedia.org/wiki/Dot_product) or dot
   * product of two vectors @f$ \mathbf{a} = (a_1, \ldots, a_N) @f$ and
   * @f$ \mathbf{b} = (b_1, \ldots, b_N) @f$ is:
   *
   * @f[ \mathbf{a} \cdot \mathbf{b} = \sum_{i=1}^{N} a_i b_i
   * = a_1 b_1 + \ldots + a_N b_N @f]
   *
   */
  template<typename T, std::size_t N>
  inline
  T dot(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<T, N> min(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<T, N> max(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<T, N> abs(Vector<T, N> val) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = std::abs(val.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise sign value
   */
  template<typename T, std::size_t N>
  inline
  Vector<int, N> sign(Vector<T, N> val) {
    Vector<int, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = gf::sign(val.data[i]);
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Component-wise equality operator
   */
  template<typename T, std::size_t N>
  inline
  Vector<bool, N> equals(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<bool, N> lessThan(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<bool, N> greaterThan(Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<T, N> select(Vector<bool, N> cond, Vector<T, N> lhs, Vector<T, N> rhs) {
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
  Vector<T, N> clamp(Vector<T, N> val, Vector<T, N> lo, Vector<T, N> hi) {
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
  Vector<T, N> clamp(Vector<T, N> val, T lo, T hi) {
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
  Vector<T, N> lerp(Vector<T, N> lhs, Vector<T, N> rhs, U t) {
    Vector<T, N> out;

    for (std::size_t i = 0; i < N; ++i) {
      out.data[i] = lerp(lhs.data[i], rhs.data[i], t);
    }

    return out;
    // return (1 - t) * lhs + t * rhs;
  }

  /**
   * @relates Vector
   * @brief Manhattan length of a vector
   *
   * The Manhattan length @f$ \|\mathbf{u}\|_1 @f$ of a vector
   * @f$ \mathbf{u} = (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ \|\mathbf{u}\|_1 = \sum_{i = 1}^{N} |u_i| @f]
   *
   * The Manhattan length is also called the 1-norm.
   *
   * @param vec A vector.
   * @returns The Manhattan length of the vector
   *
   * @sa manhattanDistance()
   */
  template<typename T, std::size_t N>
  inline
  T manhattanLength(Vector<T, N> vec) {
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
   * The square Euclidean length @f$ \|\mathbf{u}\|_2^2 @f$ of a vector
   * @f$ \mathbf{u} = (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ \|\mathbf{u}\|_2^2 = \sum_{i = 1}^{N} u_i^2 @f]
   *
   * @param vec A vector.
   * @returns The square Euclidean length of the vector
   *
   * @sa euclideanLength(), squareDistance()
   */
  template<typename T, std::size_t N>
  inline
  T squareLength(Vector<T, N> vec) {
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
   * The Euclidean length @f$ \|\mathbf{u}\|_2 @f$ of a vector
   * @f$ \mathbf{u} = (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ \|\mathbf{u}\|_2 = \sqrt{\sum_{i = 1}^{N} u_i^2} @f]
   *
   * The Euclidean length is also called the 2-norm.
   *
   * @param vec A vector.
   * @returns The Euclidean length of the vector
   *
   * @sa euclideanDistance()
   */
  template<typename T, std::size_t N>
  inline
  T euclideanLength(Vector<T, N> vec) {
    return std::sqrt(squareLength(vec));
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  // specializations of euclideanLength for Vector2f and Vector2d using std::hypot

  template<>
  inline
  float euclideanLength<float, 2>(Vector<float, 2> vec) {
    return std::hypot(vec.x, vec.y);
  }

  template<>
  inline
  double euclideanLength<double, 2>(Vector<double, 2> vec) {
    return std::hypot(vec.x, vec.y);
  }
#endif

  /**
   * @relates Vector
   * @brief Chebyshev length of a vector
   *
   * The Chebyshev length @f$ \|\mathbf{u}\|_{\infty} @f$ of a vector
   * @f$ \mathbf{u} = (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ \|u\|_{\infty} = \max_{i = 1}^{N} |u_i| @f]
   *
   * The Chebyshev length is also called the infinity norm or maximum norm.
   *
   * @param vec A vector.
   * @returns The Chebyshev length of the vector
   *
   * @sa chebyshevDistance()
   */
  template<typename T, std::size_t N>
  inline
  T chebyshevLength(Vector<T, N> vec) {
    T out = std::abs(vec.data[0]);

    for (std::size_t i = 1; i < N; ++i) {
      out = std::max(out, std::abs(vec.data[i]));
    }

    return out;
  }

  /**
   * @relates Vector
   * @brief Natural length of a vector
   *
   * The natural length @f$ L @f$ of a vector:
   * @f$ \mathbf{u} = (u_1, \ldots, u_N) @f$ is:
   *
   * @f[ L = \|\mathbf{u}\|_1 + \|\mathbf{u}\|_2^2
   * = \sum_{i = 1}^{N} |u_i| + \sum_{i = 1}^{N} u_i^2 @f]
   *
   * It's the sum of the Manhattan length and the square length.
   *
   * @param vec A vector.
   * @returns The natural length of the vector
   *
   * @sa naturalDistance()
   */
  template<typename T, std::size_t N>
  inline
  T naturalLength(Vector<T, N> vec) {
    return manhattanLength(vec) + squareLength(vec);
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
   * @sa manhattanLength()
   */
  template<typename T, std::size_t N>
  inline
  T manhattanDistance(Vector<T, N> lhs, Vector<T, N> rhs) {
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
   * @sa squareLength(), euclideanDistance()
   */
  template<typename T, std::size_t N>
  inline
  T squareDistance(Vector<T, N> lhs, Vector<T, N> rhs) {
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
   * @sa euclideanLength()
   */
  template<typename T, std::size_t N>
  inline
  T euclideanDistance(Vector<T, N> lhs, Vector<T, N> rhs) {
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
   * @sa chebyshevLength()
   */
  template<typename T, std::size_t N>
  inline
  T chebyshevDistance(Vector<T, N> lhs, Vector<T, N> rhs) {
    return chebyshevLength(lhs - rhs);
  }

  /**
   * @relates Vector
   * @brief Natural distance between two vectors
   *
   * The natural distance between two vectors is the natural length of the
   * difference of the two vectors.
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @returns The natural distance between the two vectors
   *
   * @sa naturalLength()
   */
  template<typename T, std::size_t N>
  inline
  T naturalDistance(Vector<T, N> lhs, Vector<T, N> rhs) {
    return naturalLength(lhs - rhs);
  }

  /**
   * @relates Vector
   * @brief Normalize a vector
   *
   * The normalized vector of @f$ \mathbf{u} @f$ is a vector in the same
   * direction but with a (euclidean) length of 1:
   *
   * @f[ \frac{\mathbf{u}}{\|\mathbf{u}\|_2} @f]
   *
   * A normalized vector is also called a
   * [unit vector](https://en.wikipedia.org/wiki/Unit_vector).
   *
   * @param vec A vector
   * @returns A normalized vector
   */
  template<typename T, std::size_t N>
  inline
  Vector<T, N> normalize(Vector<T, N> vec) {
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
   * @brief Angle of a vector relative to the x-axis
   *
   * @param vec The vector
   * @returns The angle of the vector
   */
  template<typename T>
  inline
  float angle(Vector<T, 2> vec) {
    return std::atan2(vec.y, vec.x);
  }

  /**
   * @relates Vector
   * @brief Perpendicular vector
   *
   * The perpendicular vector @f$ \mathbf{u}^{\perp} @f$ of vector @f$ \mathbf{u} = (x, y) @f$ is:
   *
   * @f[ \mathbf{u}^{\perp} = (-y, x) @f]
   *
   * @param vec A vector
   * @returns A perpendicular vector
   */
  template<typename T>
  constexpr
  Vector<T, 2> perp(Vector<T, 2> vec) {
    return { -vec.y, vec.x };
  }

  /**
   * @relates Vector
   * @brief Regular vector triple product
   *
   * The regular [vector triple product](https://en.wikipedia.org/wiki/Triple_product#Vector_triple_product)
   * of vectors @f$ \mathbf{a} @f$, @f$ \mathbf{b} @f$ and @f$ \mathbf{c} @f$
   * is:
   *
   * @f[ \mathbf{a} \times (\mathbf{b} \times \mathbf{c})
   * = (\mathbf{a} \cdot \mathbf{c}) \mathbf{b} - (\mathbf{a} \cdot \mathbf{b}) \mathbf{c} @f]
   *
   * @sa inverseVectorTripleProduct()
   */
  template<typename T>
  inline
  Vector<T, 2> vectorTripleProduct(Vector<T, 2> a, Vector<T, 2> b, Vector<T, 2> c) {
    return dot(a, c) * b - dot(a, b) * c;
  }

  /**
   * @relates Vector
   * @brief Inverse vector triple product
   *
   * The inverse [vector triple product](https://en.wikipedia.org/wiki/Triple_product#Vector_triple_product)
   * of vectors @f$ \mathbf{a} @f$, @f$ \mathbf{b} @f$ and @f$ \mathbf{c} @f$
   * is:
   *
   * @f[ (\mathbf{a} \times \mathbf{b}) \times \mathbf{c}
   *  = -\mathbf{c} \times (\mathbf{a} \times \mathbf{b})
   *  = -(\mathbf{c} \cdot \mathbf{b}) \mathbf{a} + (\mathbf{c} \cdot \mathbf{a}) \mathbf{b} @f]
   *
   * @sa vectorTripleProduct()
   */
  template<typename T>
  inline
  Vector<T, 2> inverseVectorTripleProduct(Vector<T, 2> a, Vector<T, 2> b, Vector<T, 2> c) {
    return - dot(c, b) * a + dot(c, a) * b;
  }

  /**
   * @relates Vector
   * @brief Cross product for 2D vectors
   *
   * The cross product of 2D vectors is not really a cross product, it is the
   * magnitude of the vector resulting from a 3D cross product of 2D vectors
   * with @f$ z = 0 @f$. The cross product @f$ \mathbf{a} \times \mathbf{b} @f$
   * of the vectors @f$ \mathbf{a} @f$ and @f$ \mathbf{b} @f$ is:
   *
   * @f[ \mathbf{a} \times \mathbf{b} = \mathbf{a}^{\perp} \cdot \mathbf{b} @f]
   *
   * The 2D cross product is also known as the perp dot product or wedge product.
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @return The cross product of the two vectors
   */
  template<typename T>
  constexpr
  T cross(Vector<T, 2> lhs, Vector<T, 2> rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
  }


  /**
   * @relates Vector
   * @brief Cross product for 3D vectors
   *
   * @param lhs A first vector
   * @param rhs A second vector
   * @return The cross product of the two vectors
   */
  template<typename T>
  constexpr
  Vector<T, 3> cross(Vector<T, 3> lhs, Vector<T, 3> rhs) {
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

#endif // GF_VECTOR_OPS_H
