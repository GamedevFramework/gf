/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include <gf/Vector.h>

#include "gtest/gtest.h"

TEST(VectorTest, ConversionIntToFloat) {
  gf::Vector2i vec(42, 69);
  gf::Vector2f copy(vec);

  EXPECT_FLOAT_EQ(42.0f, vec[0]);
  EXPECT_FLOAT_EQ(69.0f, vec[1]);
}

TEST(VectorTest, ConversionFloatToInt) {
  gf::Vector2f vec(42.0, 69.0);
  gf::Vector2i copy(vec);

  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(VectorTest, RangeFor) {
  gf::Vector4i vec(1, 2, 3, 4);

  int expected = 1;

  for (int elem : vec.data) {
    EXPECT_EQ(expected, elem);
    ++expected;
  }

  expected = 1;

  for (int elem : vec) {
    EXPECT_EQ(expected, elem);
    ++expected;
  }
}
