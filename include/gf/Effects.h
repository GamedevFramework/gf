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
#ifndef GF_EFFECTS_H
#define GF_EFFECTS_H

#include "Effect.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Default effect
   *
   * This post-processing effect does nothing
   */
  class GF_API DefaultEffect : public Effect {
  public:
    /**
     * @brief Default constructor
     */
    DefaultEffect();
  };


  /**
   * @ingroup graphics
   * @brief Anti-aliasing effect
   *
   * This effect uses [Fast Approximate Anti-Aliasing (FXAA)](https://en.wikipedia.org/wiki/Fast_approximate_anti-aliasing).
   */
  class GF_API AntiAliasingEffect : public Effect {
  public:
    /**
     * @brief Default constructor
     */
    AntiAliasingEffect();

    /**
     * @brief Set the framebuffer size
     *
     * Call this function when the size of the framebuffer changes.
     */
    void setFramebufferSize(Vector2f size);
  };


  /**
   * @ingroup graphics
   * @brief Generic color matrix effect
   *
   * This effect uses a color matrix. You can use it directly or use a
   * subclass that defines their own color matrix for various needs.
   *
   * @sa gf::ColorEffect, gf::ColorBlindEffect
   */
  class GF_API ColorMatrixEffect : public Effect {
  public:
    /**
     * @brief Default constructor
     */
    ColorMatrixEffect();

    /**
     * @brief Set the color matrix
     *
     * You can use this function to set your own color matrix.
     * The color of the texture is left-multiplied by the color
     * matrix to get the final color.
     *
     * @param mat The color matrix
     */
    void setColorMatrix(const Matrix4f& mat);
  };

  /**
   * @ingroup graphics
   * @brief Simple color effects
   */
  class GF_API ColorEffect : public ColorMatrixEffect {
  public:
    /**
     * @brief Type of color effect
     */
    enum Type {
      Normal, ///< No effect
      Grayscale, ///< Grayscale
      Sepia, ///< Sepia colors
      NightVision, ///< Night vision
      Warm, ///< Warm colors
      Cool, ///< Cool colors
    };

    /**
     * @brief Constructor
     *
     * @param type The type of color blindness
     */
    ColorEffect(Type type);

    /**
     * @brief Change the type of color effect
     *
     * @param type The type of color effect
     */
    void setType(Type type);
  };

  /**
   * @ingroup graphics
   * @brief Simulation of color blindness
   *
   * [Color blindness](https://en.wikipedia.org/wiki/Color_blindness) is a
   * deficiency of the vision that affects a significant percentage of the
   * population.
   *
   * This effect simulate different types of color blindness.
   */
  class GF_API ColorBlindEffect : public ColorMatrixEffect {
  public:
    /**
     * @brief Type of color blindness
     */
    enum Type {
      Normal, ///< Normal vision
      Protanopia, ///< Protanopia (red dichromacy, 1% of males affected)
      Protanomaly, ///< Protanomaly (red trichromacy, 1% of males affected)
      Deuteranopia, ///< Deuteranopia (green dichromacy, 1% of males affected)
      Deuteranomaly, ///< Deuteranomaly (green trichromacy, 6% of males affected)
      Tritanopia, ///< Tritanopia (blue dichromacy, rare)
      Tritanomaly, ///< Tritanomaly (blue trichromacy, very rare)
      Achromatopsia, ///< Achromatopsia (rod monochromacy, very rare)
      Achromatomaly, ///< Achromatomaly (blue cone monochromacy, very rare)
    };

    /**
     * @brief Constructor
     *
     * @param type The type of color blindness
     */
    ColorBlindEffect(Type type = Normal);

    /**
     * @brief Change the type of color blindness
     *
     * @param type The type of color blindness
     */
    void setType(Type type);
  };


  /**
   * @ingroup graphics
   * @brief Edge detector
   *
   * This effect uses a [Sobel filter](https://en.wikipedia.org/wiki/Sobel_operator).
   *
   */
  class GF_API EdgeEffect : public Effect {
  public:
    /**
     * @brief Default constructor
     */
    EdgeEffect();

    /**
     * @brief Set the framebuffer size
     *
     * Call this function when the size of the framebuffer changes.
     */
    void setFramebufferSize(Vector2f size);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_EFFECTS_H
