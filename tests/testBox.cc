/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/Box.h>

#include "gtest/gtest.h"

TEST(BoxTest, DefaultCtor) {
  gf::Box2i box;

  EXPECT_TRUE(box.isEmpty());
}

TEST(BoxTest, MinMaxCtor) {
  gf::Box2i box0({ 0, 5 }, { 10, 20 });

  EXPECT_EQ(0, box0.min.x);
  EXPECT_EQ(5, box0.min.y);
  EXPECT_EQ(10, box0.max.x);
  EXPECT_EQ(20, box0.max.y);

  gf::Box2i box1({ 10, 20 }, { 0, 5 });

  EXPECT_EQ(0, box1.min.x);
  EXPECT_EQ(5, box1.min.y);
  EXPECT_EQ(10, box1.max.x);
  EXPECT_EQ(20, box1.max.y);
}

TEST(BoxTest, PointCtor) {
  gf::Box2i box{{ 0, 5 }};

  EXPECT_EQ(0, box.min.x);
  EXPECT_EQ(5, box.min.y);
  EXPECT_EQ(0, box.max.x);
  EXPECT_EQ(5, box.max.y);
}

TEST(BoxTest, IsEmpty) {
  gf::Box2i box1({ 0, 5 }, { 10, 20 });
  EXPECT_FALSE(box1.isEmpty());

  gf::Box2i box2( { 0, 5 }, { 0, 20 });
  EXPECT_TRUE(box2.isEmpty());

  gf::Box2i box3({ 0, 5 }, { 10, 5 });
  EXPECT_TRUE(box3.isEmpty());

  gf::Box2i box4({ 0, 5 }, { 0, 5 });
  EXPECT_TRUE(box4.isEmpty());
}

TEST(BoxTest, ContainsVector) {
  gf::Box2i box({ 0, 5 }, { 10, 20 });

  // inside point
  EXPECT_TRUE(box.contains({ 5, 10 }));

  // outside point
  EXPECT_FALSE(box.contains({ 0, 0 }));

  // top-left
  EXPECT_TRUE(box.contains({ 0, 5 }));

  // top-right
  EXPECT_FALSE(box.contains({ 10, 5 }));

  // bottom-left
  EXPECT_FALSE(box.contains({ 0, 20 }));

  // bottom-right
  EXPECT_FALSE(box.contains({ 10, 20 }));
}

TEST(BoxTest, ContainsBox) {
  gf::Box2i box0({ 0, 5 }, { 10, 20 });

  // reflexivity
  EXPECT_TRUE(box0.contains(box0));

  // inside
  gf::Box2i box1({ 1, 6 }, { 2, 7 });
  EXPECT_TRUE(box0.contains(box1));
  EXPECT_FALSE(box1.contains(box0));

  // outside
  gf::Box2i box2({ 0, 0 }, { 1, 1 });
  EXPECT_FALSE(box0.contains(box2));
  EXPECT_FALSE(box2.contains(box0));

  // crossing
  gf::Box2i box3( { 2, 3 }, { 12, 18 });
  EXPECT_FALSE(box0.contains(box3));
  EXPECT_FALSE(box3.contains(box0));

  // corner
  gf::Box2i box4({ 10, 20 }, { 20, 35 });
  EXPECT_FALSE(box0.contains(box4));
  EXPECT_FALSE(box4.contains(box0));
}

TEST(BoxTest, Intersects) {
  gf::Box2i box0({ 0, 5 }, { 10, 20 });

  // reflexivity
  EXPECT_TRUE(box0.intersects(box0));

  // inside
  gf::Box2i box1({ 1, 6 }, { 2, 7 });
  EXPECT_TRUE(box0.intersects(box1));
  EXPECT_TRUE(box1.intersects(box0));

  // outside
  gf::Box2i box2({ 0, 0 }, { 1, 1 });
  EXPECT_FALSE(box0.intersects(box2));
  EXPECT_FALSE(box2.intersects(box0));

  // crossing
  gf::Box2i box3( { 2, 3 }, { 12, 18 });
  EXPECT_TRUE(box0.intersects(box3));
  EXPECT_TRUE(box3.intersects(box0));

  // corner
  gf::Box2i box4({ 10, 20 }, { 20, 35 });
  EXPECT_FALSE(box0.intersects(box4));
  EXPECT_FALSE(box4.intersects(box0));
}

TEST(BoxTest, Intersection) {
  gf::Box2i box0({ 0, 5 }, { 10, 20 });

  gf::Box2i box1;
  gf::Box2i box1Intersection = box0.getIntersection(box1);
  EXPECT_TRUE(box1Intersection.isEmpty());
}

TEST(BoxTest, Extended) {
  gf::Box2i box0({ 0, 5 }, { 10, 20 });

  gf::Box2i box1;
  gf::Box2i box1Extended = box0.getExtended(box1);
  EXPECT_EQ(box0, box1Extended);
}

TEST(BoxTest, Volume) {
  gf::Box2i box({ 0, 5 }, { 10, 20 });

  EXPECT_EQ(150, box.getVolume());
}

TEST(BoxTest, ExtentLength) {
  gf::Box2i box({ 0, 5 }, { 10, 20 });

  EXPECT_EQ(25, box.getExtentLength());
}
