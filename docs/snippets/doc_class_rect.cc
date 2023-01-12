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
#include <gf/Rect.h>

#include "doc_utils.h"

void dummyRectUsage() {
  /// [rect]
  // Define a rectangle, located at (0, 0) with a size of 20x5
  gf::RectI r1 = gf::RectI::fromSize({ 20, 5 });

  // Define another rectangle, located at (4, 2) with a size of 18x10
  gf::Vector2i position(4, 2);
  gf::Vector2i size(18, 10);
  gf::RectI r2 = gf::RectI::fromPositionSize(position, size);

  // Test intersections with the point (3, 1)
  bool b1 = r1.contains({ 3, 1 }); // true
  bool b2 = r2.contains({ 3, 1 }); // false

  // Test the intersection between r1 and r2
  gf::RectI result;
  bool b3 = r1.intersects(r2, result); // true
  // result == (4, 2, 16, 3)
  /// [rect]

  unused(b1, b2, b3);
}
