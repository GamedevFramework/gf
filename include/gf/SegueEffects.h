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
#ifndef GF_SEGUE_EFFECTS_H
#define GF_SEGUE_EFFECTS_H

#include "GraphicsApi.h"
#include "SegueEffect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_segues
   * @brief A fade to black segue effect
   */
  class GF_GRAPHICS_API BlackoutSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    BlackoutSegueEffect();
  };

  /**
   * @ingroup graphics_segues
   * @brief A fade segue effect
   */
  class GF_GRAPHICS_API FadeSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    FadeSegueEffect();
  };


  /**
   * @ingroup graphics_segues
   * @brief A slide segue effect
   *
   * By default, there is one vertical stripe.
   */
  class GF_GRAPHICS_API SlideSegueEffect : public SegueEffect {
  public:
    /**
     * @brief The orientation of the stripes
     */
    enum Orientation {
      Horizontal  = 0,  ///< The stripes are horizontal
      Vertical    = 1,  ///< The stripes are vertical
    };

    /**
     * @brief Default constructor
     */
    SlideSegueEffect();

    /**
     * @brief Set the number of stripes
     *
     * @param stripes The new number of stripes
     */
    void setStripes(int stripes);

    /**
     * @brief Set the stripe orientation
     *
     * @param orientation The new orientation of the stripes
     */
    void setStripeOrientation(Orientation orientation);
  };


  /**
   * @ingroup graphics_segues
   * @brief A glitch segue effect
   */
  class GF_GRAPHICS_API GlitchSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    GlitchSegueEffect();
  };


  /**
   * @ingroup graphics_segues
   * @brief A checkerboard segue effect
   *
   * By default, the board is @f$ 32 \times 18 @f$.
   */
  class GF_GRAPHICS_API CheckerboardSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    CheckerboardSegueEffect();

    /**
     * @brief Set the board size (number of cells)
     *
     * @param size The new size of the board
     */
    void setBoardSize(Vector2i size);

    /**
     * @brief Set the smoothness
     *
     * @param smoothness The new smoothness
     */
    void setSmoothness(float smoothness);
  };


  /**
   * @ingroup graphics_segues
   * @brief A circle segue effect
   *
   * By default, the circle opens.
   */
  class GF_GRAPHICS_API CircleSegueEffect : public SegueEffect {
  public:
    /**
     * @brief The type of circle
     */
    enum Type {
      Open  =  1, ///< The circle opens
      Close = -1, ///< The circle closes
    };

    /**
     * @brief Default constructor
     */
    CircleSegueEffect();

    /**
     * @brief Set the type of circle
     *
     * @param type The new type of circle
     */
    void setType(Type type);

    /**
     * @brief Set the framebuffer size
     *
     * @param size The new size of the framebuffer
     */
    void setFramebufferSize(Vector2i size);

  };


  /**
   * @ingroup graphics_segues
   * @brief A pixelate segue effect
   */
  class GF_GRAPHICS_API PixelateSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    PixelateSegueEffect();

    /**
     * @brief Set the framebuffer size
     *
     * @param size The new size of the framebuffer
     */
    void setFramebufferSize(Vector2i size);
  };


  /**
   * @ingroup graphics_segues
   * @brief A radial segue effect
   */
  class GF_GRAPHICS_API RadialSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    RadialSegueEffect();
  };

  /**
   * @ingroup graphics_segues
   * @brief A zoom blur segue effect
   *
   * By default, the strength is @f$ 1.0 @f$
   */
  class GF_GRAPHICS_API ZoomBlurSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    ZoomBlurSegueEffect();

    /**
     * @brief Set the strength of the zoom
     *
     * @param strength The new strength of the zoom
     */
    void setStrength(float strength);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SEGUE_EFFECTS_H
