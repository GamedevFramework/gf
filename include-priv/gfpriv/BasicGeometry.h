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
#ifndef GFPRIV_BASIC_GEOMETRY_H
#define GFPRIV_BASIC_GEOMETRY_H

#include <gf/Span.h>
#include <gf/Vector.h>

namespace gf {
namespace priv {

  int computeWindingNumber(Vector2f point, Span<const Vector2f> points);

  float computeSignedArea(Span<const Vector2f> points);

}
}

#endif // GFPRIV_BASIC_GEOMETRY_H
