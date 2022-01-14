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
#include <gf/SegueEffects.h>

#include "generated/blackout.frag.h"
#include "generated/checkerboard.frag.h"
#include "generated/circle.frag.h"
#include "generated/default.vert.h"
#include "generated/fade.frag.h"
#include "generated/glitch.frag.h"
#include "generated/slide.frag.h"
#include "generated/pixelate.frag.h"
#include "generated/radial.frag.h"
#include "generated/zoomblur.frag.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /*
   * FadeSegueEffect
   */

  BlackoutSegueEffect::BlackoutSegueEffect()
  : SegueEffect(default_vert, blackout_frag)
  {
  }

  /*
   * FadeSegueEffect
   */

  FadeSegueEffect::FadeSegueEffect()
  : SegueEffect(default_vert, fade_frag)
  {
  }

  /*
   * SlideSegueEffect
   */

  SlideSegueEffect::SlideSegueEffect()
  : SegueEffect(default_vert, slide_frag)
  {
    setStripes(1);
    setStripeOrientation(Vertical);
  }

  void SlideSegueEffect::setStripes(int stripes) {
    setUniform("u_stripes", stripes);
  }

  void SlideSegueEffect::setStripeOrientation(Orientation orientation) {
    setUniform("u_orientation", static_cast<int>(orientation));
  }

  /*
   * GlitchSegueEffect
   */

  GlitchSegueEffect::GlitchSegueEffect()
  : SegueEffect(default_vert, glitch_frag)
  {
    setUniform("u_intensity", 1.0f);
  }

  /*
   * CheckerboardSegueEffect
   */

  CheckerboardSegueEffect::CheckerboardSegueEffect()
  : SegueEffect(default_vert, checkerboard_frag)
  {
    setBoardSize({ 32, 18 });
    setSmoothness(0.0f);
  }

  void CheckerboardSegueEffect::setBoardSize(Vector2i size) {
    setUniform("u_size", size);
  }

  void CheckerboardSegueEffect::setSmoothness(float smoothness) {
    setUniform("u_smoothness", smoothness);
  }

  /*
   * CircleSegueEffect
   */

  CircleSegueEffect::CircleSegueEffect()
  : SegueEffect(default_vert, circle_frag)
  {
    setType(Open);
    setFramebufferSize({ 16, 9 });
  }

  void CircleSegueEffect::setType(Type type) {
    setUniform("u_direction", static_cast<int>(type));
  }

  void CircleSegueEffect::setFramebufferSize(Vector2i size) {
    setUniform("u_ratio", static_cast<float>(size.width) / static_cast<float>(size.height));
  }

  /*
   * PixelateSegueEffect
   */

  PixelateSegueEffect::PixelateSegueEffect()
  : SegueEffect(default_vert, pixelate_frag)
  {
    setFramebufferSize({ 1, 1 });
  }

  void PixelateSegueEffect::setFramebufferSize(Vector2i size) {
    setUniform("u_size", size);
  }

  /*
   * RadialSegueEffect
   */

  RadialSegueEffect::RadialSegueEffect()
  : SegueEffect(default_vert, radial_frag)
  {
  }

  /*
   * ZoomBlurSegueEffect
   */

  ZoomBlurSegueEffect::ZoomBlurSegueEffect()
  : SegueEffect(default_vert, zoomblur_frag)
  {
    setStrength(1.0f);
  }

  void ZoomBlurSegueEffect::setStrength(float strength) {
    setUniform("u_strength", strength);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
