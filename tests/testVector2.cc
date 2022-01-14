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

TEST(Vector2Test, Access) {
  gf::Vector2i vec;
  vec[0] = 42;
  vec[1] = 69;

  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(Vector2Test, Brace1Ctor) {
  gf::Vector2i vec{42};
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector2Test, ValueCtor) {
  gf::Vector2i vec(42);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(42, vec[1]);
}

TEST(Vector2Test, MemberCtor) {
  gf::Vector2i vec(42, 69);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(Vector2Test, Brace2Ctor) {
  gf::Vector2i vec{42, 69};
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(Vector2Test, PointerCtor) {
  const int data[2] = { 42, 69 };
  gf::Vector<int, 2> vec(data);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(Vector2Test, InitCtor) {
  gf::Vector2i vec = { 42, 69 };
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(Vector2Test, CopyCtor) {
  gf::Vector2i original(42, 69);
  gf::Vector2i vec = original;
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
}

TEST(Vector2Test, ZeroCtor) {
  gf::Vector2i vec = gf::Zero;
  EXPECT_EQ(0, vec[0]);
  EXPECT_EQ(0, vec[1]);
}

TEST(Vector2Test, AltFields) {
  gf::Vector2i vec(42, 69);

  EXPECT_EQ(42, vec.x);
  EXPECT_EQ(42, vec.u);
  EXPECT_EQ(42, vec.s);
  EXPECT_EQ(42, vec.width);
  EXPECT_EQ(42, vec.col);

  EXPECT_EQ(69, vec.y);
  EXPECT_EQ(69, vec.v);
  EXPECT_EQ(69, vec.t);
  EXPECT_EQ(69, vec.height);
  EXPECT_EQ(69, vec.row);
}

TEST(Vector2Test, RangeFor) {
  gf::Vector2i vec(1, 2);

  int expected = 1;

  for (int elem : vec) {
    EXPECT_EQ(expected, elem);
    ++expected;
  }

  EXPECT_EQ(expected, 3);
}

TEST(Vector2Test, Iterator) {
  gf::Vector2i vec1(1, 2);

  EXPECT_EQ(std::distance(vec1.begin(), vec1.end()), 2);

  const gf::Vector2i vec2(1, 2);

  EXPECT_EQ(std::distance(vec2.begin(), vec2.end()), 2);
  EXPECT_EQ(std::distance(vec2.cbegin(), vec2.cend()), 2);
}
