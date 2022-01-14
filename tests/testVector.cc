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
#include <gf/Vector.h>

#include "gtest/gtest.h"

TEST(VectorTest, ConversionIntToFloat) {
  gf::Vector2i vec(42, 69);
  gf::Vector2f copy(vec);

  EXPECT_FLOAT_EQ(42.0f, copy[0]);
  EXPECT_FLOAT_EQ(69.0f, copy[1]);
}

TEST(VectorTest, ConversionFloatToInt) {
  gf::Vector2f vec(42.0, 69.0);
  gf::Vector2i copy(vec);

  EXPECT_EQ(42, copy[0]);
  EXPECT_EQ(69, copy[1]);
}

TEST(VectorTest, UniversalFactory) {
  auto vec1 = gf::vec(1, 2);
  static_assert(std::is_same<decltype(vec1), gf::Vector<int, 2>>::value, "");

  EXPECT_EQ(vec1.x, 1);
  EXPECT_EQ(vec1.y, 2);

  auto vec2 = gf::vec(1, 2.0f, 3);
  static_assert(std::is_same<decltype(vec2), gf::Vector<float, 3>>::value, "");

  EXPECT_FLOAT_EQ(vec2.x, 1.0f);
  EXPECT_FLOAT_EQ(vec2.y, 2.0f);
  EXPECT_FLOAT_EQ(vec2.z, 3.0f);
}
