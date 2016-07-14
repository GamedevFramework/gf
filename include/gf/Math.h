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
#ifndef GF_MATH_H
#define GF_MATH_H

#include <cmath>

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
  GF_API constexpr
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
  GF_API constexpr
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
  GF_API constexpr
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
  GF_API inline
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
  GF_API inline
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
  GF_API constexpr
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
  GF_API constexpr
  T square(T val) {
    return val * val;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MATH_H
