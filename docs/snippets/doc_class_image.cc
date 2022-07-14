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
#include <gf/Image.h>

void dummyImageUsage() {

  /// [image]
  // Load an image file from a file
  gf::Image background("background.jpg");

  // Create a 20x20 image filled with black color
  gf::Image image({ 20, 20 }, gf::Color4u{0xFF, 0xFF, 0xFF, 0xFF});

  // Make the top-left pixel transparent
  gf::Color4u color = image.getPixel({ 0, 0 });
  color.a = 0;
  image.setPixel({ 0, 0 }, color);

  // Save the image to a file
  if (!image.saveToFile("result.png")) {
    // error
    return;
  }
  /// [image]

}
