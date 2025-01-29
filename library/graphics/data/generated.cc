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

#include "shaders/blackout.frag.h"
#include "shaders/color_matrix.frag.h"
#include "shaders/default_alpha.frag.h"
#include "shaders/default.frag.h"
#include "shaders/default.vert.h"
#include "shaders/edge.frag.h"
#include "shaders/fxaa.frag.h"
#include "shaders/fade.frag.h"
#include "shaders/slide.frag.h"
#include "shaders/glitch.frag.h"
#include "shaders/checkerboard.frag.h"
#include "shaders/circle.frag.h"
#include "shaders/pixelate.frag.h"
#include "shaders/radial.frag.h"
#include "shaders/zoomblur.frag.h"

#include "shaders/light_shape.frag.h"
#include "shaders/light_unshadow.frag.h"
