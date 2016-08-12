/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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

#include "gtest/gtest.h"

TEST(RectTest, DefaultCtor) {
  gf::RectI ri;

  EXPECT_EQ(0, ri.left);
  EXPECT_EQ(0, ri.top);
  EXPECT_EQ(0, ri.width);
  EXPECT_EQ(0, ri.height);

  EXPECT_EQ(0, ri.position.x);
  EXPECT_EQ(0, ri.position.y);
  EXPECT_EQ(0, ri.size.width);
  EXPECT_EQ(0, ri.size.height);
}

TEST(RectTest, ValueCtor) {
  gf::RectI ri{0, 5, 10, 15};

  EXPECT_EQ(0, ri.left);
  EXPECT_EQ(5, ri.top);
  EXPECT_EQ(10, ri.width);
  EXPECT_EQ(15, ri.height);

  EXPECT_EQ(0, ri.position.x);
  EXPECT_EQ(5, ri.position.y);
  EXPECT_EQ(10, ri.size.width);
  EXPECT_EQ(15, ri.size.height);
}

TEST(RectTest, PositionSizeCtor) {
  gf::RectI ri{ { 0, 5 }, { 10, 15 } };

  EXPECT_EQ(0, ri.left);
  EXPECT_EQ(5, ri.top);
  EXPECT_EQ(10, ri.width);
  EXPECT_EQ(15, ri.height);

  EXPECT_EQ(0, ri.position.x);
  EXPECT_EQ(5, ri.position.y);
  EXPECT_EQ(10, ri.size.width);
  EXPECT_EQ(15, ri.size.height);
}

TEST(RectTest, IsEmpty) {
  gf::RectI ri1{0, 5, 10, 15};
  EXPECT_FALSE(ri1.isEmpty());

  gf::RectI ri2{0, 5, 0, 15};
  EXPECT_TRUE(ri2.isEmpty());

  gf::RectI ri3{0, 5, 10, 0};
  EXPECT_TRUE(ri3.isEmpty());

  gf::RectI ri4{0, 5, 0, 0};
  EXPECT_TRUE(ri4.isEmpty());
}

TEST(RectTest, ContainsVector) {
  gf::RectI ri{0, 5, 10, 15};

  // inside point
  EXPECT_TRUE(ri.contains({ 5, 10 }));

  // outside point
  EXPECT_FALSE(ri.contains({ 0, 0 }));

  // top-left
  EXPECT_TRUE(ri.contains({ 0, 5 }));

  // top-right
  EXPECT_FALSE(ri.contains({ 10, 5 }));

  // bottom-left
  EXPECT_FALSE(ri.contains({ 0, 20 }));

  // bottom-right
  EXPECT_FALSE(ri.contains({ 10, 20 }));
}

TEST(RectTest, ContainsRect) {
  gf::RectI ri0{0, 5, 10, 15};

  // reflexivity
  EXPECT_TRUE(ri0.contains(ri0));

  // inside
  gf::RectI ri1{1, 6, 1, 1};
  EXPECT_TRUE(ri0.contains(ri1));
  EXPECT_FALSE(ri1.contains(ri0));

  // outside
  gf::RectI ri2{0, 0, 1, 1};
  EXPECT_FALSE(ri0.contains(ri2));
  EXPECT_FALSE(ri2.contains(ri0));

  // crossing
  gf::RectI ri3{2, 3, 10, 15};
  EXPECT_FALSE(ri0.contains(ri3));
  EXPECT_FALSE(ri3.contains(ri0));

  // corner
  gf::RectI ri4{10, 20, 10, 15};
  EXPECT_FALSE(ri0.contains(ri4));
  EXPECT_FALSE(ri4.contains(ri0));
}

TEST(RectTest, Intersects) {
  gf::RectI ri0{0, 5, 10, 15};

  // reflexivity
  EXPECT_TRUE(ri0.intersects(ri0));

  // inside
  gf::RectI ri1{1, 6, 1, 1};
  EXPECT_TRUE(ri0.intersects(ri1));
  EXPECT_TRUE(ri1.intersects(ri0));

  // outside
  gf::RectI ri2{0, 0, 1, 1};
  EXPECT_FALSE(ri0.intersects(ri2));
  EXPECT_FALSE(ri2.intersects(ri0));

  // crossing
  gf::RectI ri3{2, 3, 10, 15};
  EXPECT_TRUE(ri0.intersects(ri3));
  EXPECT_TRUE(ri3.intersects(ri0));

  // corner
  gf::RectI ri4{10, 20, 10, 15};
  EXPECT_FALSE(ri0.intersects(ri4));
  EXPECT_FALSE(ri4.intersects(ri0));
}
