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

#include "gtest/gtest.h"

TEST(CircTest, DefaultCtor) {
  gf::CircI ci;

  EXPECT_EQ(0, ci.center.x);
  EXPECT_EQ(0, ci.center.y);
  EXPECT_EQ(0, ci.radius);
}

TEST(CircTest, CenterRadiusCtor) {
  gf::CircI ci({ 0, 5 }, 10);

  EXPECT_EQ(0, ci.center.x);
  EXPECT_EQ(5, ci.center.y);
  EXPECT_EQ(10, ci.radius);
}

TEST(CircTest, IsEmpty) {
  gf::CircI ci1({ 0, 5 }, 10);
  EXPECT_FALSE(ci1.isEmpty());

  gf::CircI ci2({ 0, 5 }, 0);
  EXPECT_TRUE(ci2.isEmpty());
}

TEST(CircTest, Contains) {
  gf::CircI ci({ 3, 4 }, 5);

  // inside point
  EXPECT_TRUE(ci.contains({ 3, 4 }));

  // outside point
  EXPECT_FALSE(ci.contains({ 10, 10 }));

  // border
  EXPECT_TRUE(ci.contains({ 0, 0 }));
}

TEST(CircTest, Intersects) {
  gf::CircI ci0({ 3, 4 }, 5);

  // reflexivity
  EXPECT_TRUE(ci0.intersects(ci0));

  // inside
  gf::CircI ci1({ 4, 5 }, 1);
  EXPECT_TRUE(ci0.intersects(ci1));
  EXPECT_TRUE(ci1.intersects(ci0));

  // outside
  gf::CircI ci2({ 10, 10 }, 2);
  EXPECT_FALSE(ci0.intersects(ci2));
  EXPECT_FALSE(ci2.intersects(ci0));

  // crossing
  gf::CircI ci3({ 2, 3 }, 5);
  EXPECT_TRUE(ci0.intersects(ci3));
  EXPECT_TRUE(ci3.intersects(ci0));

  // single point
  gf::CircI ci4({ -3, -4 }, 5);
  EXPECT_TRUE(ci0.intersects(ci4));
  EXPECT_TRUE(ci4.intersects(ci0));
}
