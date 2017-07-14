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
#ifndef GF_MATH_H
#define GF_MATH_H

#include <cmath>
#include <limits>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief The @f$ \pi @f$ constant
   */
  constexpr float Pi = 3.14159265358979323846f;

  /**
   * @ingroup core
   * @brief The @f$ \frac{\pi}{2} @f$ constant
   */
  constexpr float Pi2 = Pi / 2;

  /**
   * @ingroup core
   * @brief The @f$ \frac{\pi}{4} @f$ constant
   */
  constexpr float Pi4 = Pi / 4;

  /**
   * @ingroup core
   * @brief The @f$ \sqrt{2} @f$ constant
   */
  constexpr float Sqrt2 = 1.41421356237309504880f;

  /**
   * @ingroup core
   * @brief The @f$ \frac{1}{\sqrt{2}} @f$ constant
   */
  constexpr float InvSqrt2 = 1 / Sqrt2;

  /**
   * @ingroup core
   * @brief The @f$ \sqrt{3} @f$ constant
   */
  constexpr float Sqrt3 = 1.7320508075688772935f;

  /**
   * @ingroup core
   * @brief Machine epsilon
   */
  constexpr float Epsilon = std::numeric_limits<float>::epsilon();

  /**
   * @ingroup core
   * @brief Compare two floats
   *
   * @param a The first float
   * @param b The second float
   * @param epsilon A small value that controls the equality comparison
   *
   * @sa [Comparison - The Floating-Point Guide](http://floating-point-gui.de/errors/comparison/)
   */
  template<typename T>
  inline
  bool almostEquals(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
    if (a == b) {
      return true;
    }

    T diff = std::abs(a - b);

    if (a == 0 || b == 0 || diff < std::numeric_limits<T>::denorm_min()) {
      return diff < (epsilon * std::numeric_limits<T>::denorm_min());
    }

    T sum = std::abs(a) + std::abs(b);
    sum = (sum < std::numeric_limits<T>::max()) ? sum : std::numeric_limits<T>::max();

    return (diff / sum) < epsilon;
  }

  /**
   * @ingroup core
   * @brief Convert degrees to radians
   *
   * @param degrees An angle in degrees
   * @returns The angle in radians
   * @sa radiansToDegrees()
   */
  constexpr float degreesToRadians(float degrees) {
    return degrees * Pi / 180.0f;
  }

  /**
   * @ingroup core
   * @brief Convert radians to degrees
   *
   * @param radians An angle in radians
   * @returns The angle in degrees
   * @sa degreesToRadians()
   */
  constexpr float radiansToDegrees(float radians) {
    return radians * 180.0f / Pi;
  }

  /**
   * @ingroup core
   * @brief A step is a function with special features.
   *
   * A step @f$ f @f$ is a (mathematical) function which has the following properties:
   *
   * - @f$ f(0) = 0 @f$
   * - @f$ f(1) = 1 @f$
   *
   * It can be used to smooth a linear interpolation.
   *
   * ~~~{.cc}
   * gf::Vector2f point1 = ...;
   * gf::Vector2f point2 = ...;
   * float t = ...;
   * gf::Step<float> step = gf::cubicStep<float>;
   *
   * gf::Vector2f interpolatedPoint = gf::lerp(point1, point2, t);
   * gf::Vector2f smoothedInterpolatedPoint = gf::lerp(point1, point2, step(t));
   * ~~~
   *
   * @sa linearStep(), cubicStep(), quinticStep(), cosineStep()
   */
  template<typename T>
  using Step = T (*)(T);

  /**
   * @ingroup core
   * @brief Linear step
   *
   * @f[ f(t) = t @f]
   *
   * @sa gf::Step
   */
  template<typename T>
  constexpr
  T linearStep(T t) {
    return t;
  }

  /**
   * @ingroup core
   * @brief Cubic step (smoothstep)
   *
   * @f[ f(t) = -2 * t^3 + 3 * t^2 @f]
   *
   * Compared to a step, it has the following properties:
   *
   * - @f$ f'(0) = 0 @f$
   * - @f$ f'(1) = 0 @f$
   *
   * This function is also known as [smoothstep](https://en.wikipedia.org/wiki/Smoothstep)
   *
   * @sa gf::Step
   */
  template<typename T>
  constexpr
  T cubicStep(T t) {
    return (-2 * t + 3) * t * t;
  }

  /**
   * @ingroup core
   * @brief Quintic step (smootherstep)
   *
   * @f[ f(t) = 6 * t^5 - 15 * t^4 + 10 * t^3 @f]
   *
   * Compared to a step, it has the following properties:
   *
   * - @f$ f'(0) = 0 @f$
   * - @f$ f'(1) = 0 @f$
   * - @f$ f''(0) = 0 @f$
   * - @f$ f''(1) = 0 @f$
   *
   * This function is also known as [smootherstep](https://en.wikipedia.org/wiki/Smoothstep#Variations)
   *
   * @sa gf::Step
   */
  template<typename T>
  constexpr
  T quinticStep(T t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
  }

  /**
   * @ingroup core
   * @brief Cosine step
   *
   * @f[ f(t) = (1 - \cos(\pi * t)) * 0.5 @f]
   *
   * @sa gf::Step
   */
  template<typename T>
  inline
  T cosineStep(T t) {
    return (1 - std::cos(Pi * t)) * 0.5;
  }

  /**
   * @ingroup core
   * @brief Linear interpolation function
   *
   * This functions returns an interpolated value between two values `lhs` and
   * `rhs` according to a parameter @f$ t @f$. When @f$ t = 0 @f$ then `lhs`
   * is returned, and when @f$ t = 1 @f$ then `rhs` is returned.
   *
   * @param lhs The first value
   * @param rhs The second value
   * @param t The interpolation parameter, generally in the interval @f$ [0,1] @f$
   * @returns A value between the first and second value
   */
  template<typename T, typename U>
  inline
  T lerp(T lhs, T rhs, U t) {
    return (1 - t) * lhs + t * rhs;
  }

  /**
   * @ingroup core
   * @brief Clamping function
   *
   * This function takes a value and returns it if it is in a specified range.
   * If not, the returned value is the nearest value in the range.
   *
   * @param val The value to clamp
   * @param lo The minimum accepted value
   * @param hi The maximum accepted value
   * @returns The clamped value
   */
  template<typename T>
  constexpr
  T clamp(T val, T lo, T hi) {
    return val < lo ? lo : (val > hi ? hi : val);
  }

  /**
   * @ingroup core
   * @brief Square function
   *
   * For a value @f$ x @f$, the square value is @f$ x^2 @f$.
   *
   * @param val A value
   * @returns The square of the value
   */
  template<typename T>
  constexpr
  T square(T val) {
    return val * val;
  }

  /**
   * @ingroup core
   * @brief Sign function
   *
   * The sign function of @f$ x @f$ is:
   *
   * - @f$ -1 @f$ if @f$ x < 0 @f$
   * - @f$ 0 @f$ if @f$ x = 0 @f$
   * - @f$ 1 @f$ if @f$ x > 0 @f$
   *
   * @param val A value
   * @returns The sign of the value
   * @sa [Sign function](https://en.wikipedia.org/wiki/Sign_function)
   */
  template<typename T>
  constexpr
  int sign(T val) {
    return (val > T(0)) - (val < T(0));
  }

  /**
   * @ingroup core
   * @brief Absolute difference of two values
   *
   * This function is defined as:
   *
   * @f[ \text{absdiff}(a, b) = | a - b | @f]
   *
   * @param lhs The first value
   * @param rhs The second value
   * @returns The absolute difference between the two values
   */
  template<typename T>
  constexpr
  T absdiff(T lhs, T rhs) {
    return lhs > rhs ? lhs - rhs : rhs - lhs;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MATH_H
