/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/Blend.h>
#include <gf/Unused.h>

void dummyBlendModeUsage() {

  /// [predefined]
  gf::BlendMode alphaBlending          = gf::BlendAlpha;
  gf::BlendMode additiveBlending       = gf::BlendAdd;
  gf::BlendMode multiplicativeBlending = gf::BlendMultiply;
  gf::BlendMode noBlending             = gf::BlendNone;
  /// [predefined]

  gf::unused(alphaBlending, additiveBlending, multiplicativeBlending, noBlending);

}
