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
#include <gf/Circ.h>

#include "doc_utils.h"

void dummyCircUsage() {

  /// [circ]
  // Define a circle, with a center at  (10, 10) and a radius of 20
  gf::CircI c1({ 10, 10 }, 20);

  // Define a circle with a center at (0, 0) and a radius of 2
  gf::CircI c2({ 0, 0 }, 2);

  // Test intersections with the point (3, 1)
  bool b1 = c1.contains({ 3, 1 }); // true
  bool b2 = c2.contains({ 3, 1 }); // false

  // Test the intersection between c1 and c2
  bool b3 = c1.intersects(c2); // true
  /// [circ]

  unused(c1, c2, b1, b2, b3);

}
