/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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

#include "generated/default.vert.h"
#include "generated/fade.frag.h"
#include "generated/glitch.frag.h"
#include "generated/slide.frag.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

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
  }

  /*
   * GlitchSegueEffect
   */

  GlitchSegueEffect::GlitchSegueEffect()
  : SegueEffect(default_vert, glitch_frag)
  {
    setUniform("u_intensity", 1.0f);
  }



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}





