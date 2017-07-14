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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_EASINGS_H
#define GF_EASINGS_H

#include <cmath>

#include "Math.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief An easing function
   *
   * An easing function is a function that specifies how a value changes over
   * time. A normalized easing function has the following properties:
   *
   * - @f$ f(0) = 0 @f$
   * - @f$ f(1) = 1 @f$
   *
   * Usual easing functions are provided in gf::Ease.
   *
   * @sa gf::Ease, gf::Step
   */
  using Easing = float (*)(float);


  /**
   * @ingroup core
   * @brief Predefined easing functions
   *
   * This class defines usual easing functions.
   *
   * # First set of easing functions
   *
   * The first set of functions are common interpolation functions.
   *
   * - linear
   *   @f[ f(t) = t @f]
   * - smooth
   *   @f[ f(t) = -2 * t^3 + 3 * t^2 @f]
   * - smoother
   *   @f[ f(t) = 6 * t^5 - 15 * t^4 + 10 * t^3 @f]
   *
   * Here are the plots of these functions:
   *
   * <table>
   * <tr>
   *   <th>Functions</th>
   *   <th>Plot</th>
   * </tr>
   * <tr>
   *   <td>linear</td>
   *   <td> @image html linear.png </td>
   * </tr>
   * <tr>
   *   <td>smooth</td>
   *   <td> @image html smooth.png </td>
   * </tr>
   * <tr>
   *   <td>smoother</td>
   *   <td> @image html smoother.png </td>
   * </tr>
   * </table>
   *
   * # Second set of easing functions
   *
   * ## Definitions
   *
   * The second set are the easing functions defined by Robert Penner. From a
   * base function @f$ f(t) @f$, four flavors are defined:
   *
   * - Ease-In: start slow and speed up at the end
   * @f[ \mathtt{easeIn}(t) = f(t) @f]
   * - Ease-Out: start fast and slow down at the end
   * @f[ \mathtt{easeOut}(t) = 1 - \mathtt{easeIn}(1 - t) @f]
   * - Ease-In-Out: start slow, then speed up in the middle and finally slow down at the end
   * @f[ \mathtt{easeInOut}(t) =
   *   \begin{cases}
   *     \mathtt{easeIn}(2 \times t) \div 2 & \text{if } t < 0.5 \\
   *     \mathtt{easeOut}(2 \times t + 1) \div 2 + 0.5 & \text{if } t \geq 0.5
   *   \end{cases}
   * @f]
   * - Ease-Out-In: start fast, then slow down in the middle and finally speed up at the end
   * @f[ \mathtt{easeOutIn}(t) =
   *   \begin{cases}
   *     \mathtt{easeOut}(2 \times t) \div 2 & \text{if } t < 0.5 \\
   *     \mathtt{easeIn}(2 \times t + 1) \div 2 + 0.5 & \text{if } t \geq 0.5
   *   \end{cases}
   * @f]
   *
   * ## Base functions
   *
   * Here are the base functions:
   *
   * - quad
   *   @f[ f(t) = t^2 @f]
   * - cubic
   *   @f[ f(t) = t^3 @f]
   * - quart
   *   @f[ f(t) = t^4 @f]
   * - quint
   *   @f[ f(t) = t^5 @f]
   * - sine
   *   @f[ f(t) = 1 - \cos\left(t \times \frac{\pi}{2}\right)@f]
   * - expo
   *   @f[ f(t) = \begin{cases} 0 & \text{if } t = 0 \\ 2^{10 \times (t - 1)} & \text{if } t > 0 \end{cases} @f]
   * - circ
   *   @f[ f(t) = 1 - \sqrt{1 - t^2} @f]
   * - back
   *   @f[ f(t) = t^2 \times (2.70158 \times t - 1.70158) @f]
   * - bounce
   *   @f[ f(t) =
   *     \begin{cases}
   *       1 - 7.5625 \times (1 - t)^2 & \text{if } 1 - t < 1 / 2.75 \\
   *       1 - (7.5625 \times (1 - t - 1.5 / 2.75)^2 + 0.75) & \text{if } 1 - t < 2 / 2.75 \\
   *       1 - (7.5625 \times (1 - t - 2.25 / 2.75)^2 + 0.9375) & \text{if } 1 - t < 2.5 / 2.75 \\
   *       1 - (7.5625 \times (1 - t - 2.625 / 2.75)^2 + 0.984375) & \text{otherwise}
   *     \end{cases}
   *   @f]
   * - elastic
   *   @f[ f(t) = -2^{10 \times (t - 1)} \times \sin\left(\left((t - 1) - \frac{0.3}{4}\right) \times \frac{2 \times \pi}{0.3}\right) @f]
   *
   * ## Plots
   *
   * Here are the plots for all the defined easing functions:
   *
   * <table>
   * <tr>
   *   <th>Function</th>
   *   <th>Ease-In</th>
   *   <th>Ease-Out</th>
   *   <th>Ease-In-Out</th>
   *   <th>Ease-Out-In</th>
   * </tr>
   * <tr>
   *   <td>quad</th>
   *   <td> @image html quadin.png </td>
   *   <td> @image html quadout.png </td>
   *   <td> @image html quadinout.png </td>
   *   <td> @image html quadoutin.png </td>
   * </tr>
   * <tr>
   *   <td>cubic</th>
   *   <td> @image html cubicin.png </td>
   *   <td> @image html cubicout.png </td>
   *   <td> @image html cubicinout.png </td>
   *   <td> @image html cubicoutin.png </td>
   * </tr>
   * <tr>
   *   <td>quart</th>
   *   <td> @image html quartin.png </td>
   *   <td> @image html quartout.png </td>
   *   <td> @image html quartinout.png </td>
   *   <td> @image html quartoutin.png </td>
   * </tr>
   * <tr>
   *   <td>quint</th>
   *   <td> @image html quintin.png </td>
   *   <td> @image html quintout.png </td>
   *   <td> @image html quintinout.png </td>
   *   <td> @image html quintoutin.png </td>
   * </tr>
   * <tr>
   *   <td>sine</th>
   *   <td> @image html sinein.png </td>
   *   <td> @image html sineout.png </td>
   *   <td> @image html sineinout.png </td>
   *   <td> @image html sineoutin.png </td>
   * </tr>
   * <tr>
   *   <td>expo</th>
   *   <td> @image html expoin.png </td>
   *   <td> @image html expoout.png </td>
   *   <td> @image html expoinout.png </td>
   *   <td> @image html expooutin.png </td>
   * </tr>
   * <tr>
   *   <td>circ</th>
   *   <td> @image html circin.png </td>
   *   <td> @image html circout.png </td>
   *   <td> @image html circinout.png </td>
   *   <td> @image html circoutin.png </td>
   * </tr>
   * <tr>
   *   <td>back</th>
   *   <td> @image html backin.png </td>
   *   <td> @image html backout.png </td>
   *   <td> @image html backinout.png </td>
   *   <td> @image html backoutin.png </td>
   * </tr>
   * <tr>
   *   <td>bounce</th>
   *   <td> @image html bouncein.png </td>
   *   <td> @image html bounceout.png </td>
   *   <td> @image html bounceinout.png </td>
   *   <td> @image html bounceoutin.png </td>
   * </tr>
   * <tr>
   *   <td>elastic</th>
   *   <td> @image html elasticin.png </td>
   *   <td> @image html elasticout.png </td>
   *   <td> @image html elasticinout.png </td>
   *   <td> @image html elasticoutin.png </td>
   * </tr>
   * </table>
   *
   *
   * @sa gf::Easing
   * @sa [Robert Penner's Easing Functions](http://robertpenner.com/easing/)
   * @sa [Easing Functions Cheat Sheet](http://easings.net/)
   */
  class GF_API Ease {
  public:

    /**
     * @brief Linear easing
     *
     * This is the default easing function
     */
    static float linear(float t);

    /**
     * @brief Smooth easing
     *
     * @sa [Smoothstep - Wikipedia](https://en.wikipedia.org/wiki/Smoothstep)
     */
    static float smooth(float t);

    /**
     * @brief Smoother easing
     *
     * @sa [Smootherstep - Wikipedia](https://en.wikipedia.org/wiki/Smoothstep#Variations)
     */
    static float smoother(float t);

    /**
     * @brief Ease-In flavor of quad easing
     */
    static float quadIn(float t);

    /**
     * @brief Ease-Out flavor of quad easing
     */
    static float quadOut(float t);

    /**
     * @brief Ease-In-Out flavor of quad easing
     */
    static float quadInOut(float t);

    /**
     * @brief Ease-Out-In flavor of quad easing
     */
    static float quadOutIn(float t);

    /**
     * @brief Ease-In flavor of cubic easing
     */
    static float cubicIn(float t);

    /**
     * @brief Ease-Out flavor of cubic easing
     */
    static float cubicOut(float t);

    /**
     * @brief Ease-In-Out flavor of cubic easing
     */
    static float cubicInOut(float t);

    /**
     * @brief Ease-Out-In flavor of cubic easing
     */
    static float cubicOutIn(float t);

    /**
     * @brief Ease-In flavor of quart easing
     */
    static float quartIn(float t);

    /**
     * @brief Ease-Out flavor of quart easing
     */
    static float quartOut(float t);

    /**
     * @brief Ease-In-Out flavor of quart easing
     */
    static float quartInOut(float t);

    /**
     * @brief Ease-Out-In flavor of quart easing
     */
    static float quartOutIn(float t);

    /**
     * @brief Ease-In flavor of quint easing
     */
    static float quintIn(float t);

    /**
     * @brief Ease-Out flavor of quint easing
     */
    static float quintOut(float t);

    /**
     * @brief Ease-In-Out flavor of quint easing
     */
    static float quintInOut(float t);

    /**
     * @brief Ease-Out-In flavor of quint easing
     */
    static float quintOutIn(float t);

    /**
     * @brief Ease-In flavor of circ easing
     */
    static float circIn(float t);

    /**
     * @brief Ease-Out flavor of circ easing
     */
    static float circOut(float t);

    /**
     * @brief Ease-In-Out flavor of circ easing
     */
    static float circInOut(float t);

    /**
     * @brief Ease-Out-In flavor of circ easing
     */
    static float circOutIn(float t);

    /**
     * @brief Ease-In flavor of sine easing
     */
    static float sineIn(float t);

    /**
     * @brief Ease-Out flavor of sine easing
     */
    static float sineOut(float t);

    /**
     * @brief Ease-In-Out flavor of sine easing
     */
    static float sineInOut(float t);

    /**
     * @brief Ease-Out-In flavor of sine easing
     */
    static float sineOutIn(float t);

    /**
     * @brief Ease-In flavor of back easing
     */
    static float backIn(float t);

    /**
     * @brief Ease-Out flavor of back easing
     */
    static float backOut(float t);

    /**
     * @brief Ease-In-Out flavor of back easing
     */
    static float backInOut(float t);

    /**
     * @brief Ease-Out-In flavor of back easing
     */
    static float backOutIn(float t);

    /**
     * @brief Ease-In flavor of bounce easing
     */
    static float bounceIn(float t);

    /**
     * @brief Ease-Out flavor of bounce easing
     */
    static float bounceOut(float t);

    /**
     * @brief Ease-Out-In flavor of bounce easing
     */
    static float bounceInOut(float t);

    /**
     * @brief Ease-In-Out flavor of bounce easing
     */
    static float bounceOutIn(float t);

    /**
     * @brief Ease-In flavor of elastic easing
     */
    static float elasticIn(float t);

    /**
     * @brief Ease-Out flavor of elastic easing
     */
    static float elasticOut(float t);

    /**
     * @brief Ease-In-Out flavor of elastic easing
     */
    static float elasticInOut(float t);

    /**
     * @brief Ease-Out-In flavor of elastic easing
     */
    static float elasticOutIn(float t);

    /**
     * @brief Ease-In flavor of expo easing
     */
    static float expoIn(float t);

    /**
     * @brief Ease-Out flavor of expo easing
     */
    static float expoOut(float t);

    /**
     * @brief Ease-In-Out flavor of expo easing
     */
    static float expoInOut(float t);

    /**
     * @brief Ease-Out-In flavor of expo easing
     */
    static float expoOutIn(float t);

    /**
     * @brief Deleted constructor
     */
    Ease() = delete;

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_EASINGS_H
