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

#define GF_IMPLEMENTATION

#include "generated/blackout.frag.h"
#include "generated/color_matrix.frag.h"
#include "generated/default_alpha.frag.h"
#include "generated/default.frag.h"
#include "generated/default.vert.h"
#include "generated/edge.frag.h"
#include "generated/fxaa.frag.h"
#include "generated/fade.frag.h"
#include "generated/slide.frag.h"
#include "generated/glitch.frag.h"
#include "generated/checkerboard.frag.h"
#include "generated/circle.frag.h"
#include "generated/pixelate.frag.h"
#include "generated/radial.frag.h"
#include "generated/zoomblur.frag.h"

#include "generated/light_shape.frag.h"
#include "generated/light_unshadow.frag.h"
