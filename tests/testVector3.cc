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

TEST(Vector3Test, Access) {
  gf::Vector3i vec;

  vec[0] = 42;
  vec[1] = 69;
  vec[2] = 23;

  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
}

TEST(Vector3Test, Brace1Ctor) {
  gf::Vector3i vec{42};
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector3Test, ValueCtor) {
  gf::Vector3i vec(42);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(42, vec[1]);
  EXPECT_EQ(42, vec[2]);
}

TEST(Vector3Test, MemberCtor) {
  gf::Vector3i vec(42, 69, 23);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
}

TEST(Vector3Test, Brace3Ctor) {
  gf::Vector3i vec{42, 69, 23};
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
}

TEST(Vector3Test, PointerCtor) {
  const int data[3] = { 42, 69, 23 };
  gf::Vector3i vec(data);
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
}

TEST(Vector3Test, InitCtor) {
  gf::Vector3i vec = {42, 69, 23};
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
}

TEST(Vector3Test, CopyCtor) {
  gf::Vector3i original(42, 69, 23);
  gf::Vector3i vec = original;
  EXPECT_EQ(42, vec[0]);
  EXPECT_EQ(69, vec[1]);
  EXPECT_EQ(23, vec[2]);
}


TEST(Vector3Test, ZeroCtor) {
  gf::Vector3i vec = gf::Zero;
  EXPECT_EQ(0, vec[0]);
  EXPECT_EQ(0, vec[1]);
  EXPECT_EQ(0, vec[2]);
}

TEST(Vector3Test, AltFields) {
  gf::Vector3i vec(42, 69, 23);

  EXPECT_EQ(42, vec.x);
  EXPECT_EQ(42, vec.r);

  EXPECT_EQ(69, vec.y);
  EXPECT_EQ(69, vec.g);

  EXPECT_EQ(23, vec.z);
  EXPECT_EQ(23, vec.b);
}

TEST(Vector3Test, RangeFor) {
  gf::Vector3i vec(1, 2, 3);

  int expected = 1;

  for (int elem : vec) {
    EXPECT_EQ(expected, elem);
    ++expected;
  }

  EXPECT_EQ(expected, 4);
}

TEST(Vector3Test, Iterator) {
  gf::Vector3i vec1(1, 2, 3);

  EXPECT_EQ(std::distance(vec1.begin(), vec1.end()), 3);

  const gf::Vector3i vec2(1, 2, 3);

  EXPECT_EQ(std::distance(vec2.begin(), vec2.end()), 3);
  EXPECT_EQ(std::distance(vec2.cbegin(), vec2.cend()), 3);
}
