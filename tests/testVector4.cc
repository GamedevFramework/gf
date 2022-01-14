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

TEST(Vector4Test, Access) {
  gf::Vector4i vec;

  vec[0] = 42;
  vec[1] = 69;
  vec[2] = 23;
  vec[3] = 17;

  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
  EXPECT_EQ(17, vec[3]);
}

TEST(Vector4Test, Brace1Ctor) {
  gf::Vector4i vec{42};
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector4Test, ValueCtor) {
  gf::Vector4i vec(42);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(42, vec[1]);
  EXPECT_EQ(42, vec[2]);
  EXPECT_EQ(42, vec[3]);
}

TEST(Vector4Test, MemberCtor) {
  gf::Vector4i vec(42, 69, 23, 17);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
  EXPECT_EQ(17, vec[3]);
}

TEST(Vector4Test, Brace4Ctor) {
  gf::Vector4i vec{42, 69, 23, 17};
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
  EXPECT_EQ(17, vec[3]);
}

TEST(Vector4Test, PointerCtor) {
  const int data[4] = { 42, 69, 23, 17 };
  gf::Vector4i vec(data);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
  EXPECT_EQ(17, vec[3]);
}

TEST(Vector4Test, InitCtor) {
  gf::Vector4i vec = {42, 69, 23, 17};
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
  EXPECT_EQ(17, vec[3]);
}

TEST(Vector4Test, CopyCtor) {
  gf::Vector4i original(42, 69, 23, 17);
  gf::Vector4i vec = original;
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
  EXPECT_EQ(17, vec[3]);
}

TEST(Vector4Test, ZeroCtor) {
  gf::Vector4i vec = gf::Zero;
  EXPECT_EQ(0, vec[0]);
  EXPECT_EQ(0, vec[1]);
  EXPECT_EQ(0, vec[2]);
  EXPECT_EQ(0, vec[3]);
}

TEST(Vector4Test, AltFields) {
  gf::Vector4i vec(42, 69, 23, 17);

  EXPECT_EQ(42, vec.x);
  EXPECT_EQ(42, vec.r);

  EXPECT_EQ(69, vec.y);
  EXPECT_EQ(69, vec.g);

  EXPECT_EQ(23, vec.z);
  EXPECT_EQ(23, vec.b);

  EXPECT_EQ(17, vec.w);
  EXPECT_EQ(17, vec.a);
}

TEST(Vector4Test, RangeFor) {
  gf::Vector4i vec(1, 2, 3, 4);

  int expected = 1;

  for (int elem : vec) {
    EXPECT_EQ(expected, elem);
    ++expected;
  }

  EXPECT_EQ(expected, 5);
}

TEST(Vector4Test, Iterator) {
  gf::Vector4i vec1(1, 2, 3, 4);

  EXPECT_EQ(std::distance(vec1.begin(), vec1.end()), 4);

  const gf::Vector4i vec2(1, 2, 3, 4);

  EXPECT_EQ(std::distance(vec2.begin(), vec2.end()), 4);
  EXPECT_EQ(std::distance(vec2.cbegin(), vec2.cend()), 4);
}
