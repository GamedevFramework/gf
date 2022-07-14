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
#include <gf/Font.h>
#include <gf/Text.h>

void dummyFontUsage() {
  /// [font]
  // Load a new font from a file
  gf::Font font("arial.ttf");

  // Create a text which uses our font
  gf::Text text1;
  text1.setString("Hello World!");
  text1.setFont(font);
  text1.setCharacterSize(30);

  // Create another text using the same font, but with different parameters
  gf::Text text2;
  text2.setString("Goodbye world!");
  text2.setFont(font);
  text2.setCharacterSize(50);
  /// [font]

}
