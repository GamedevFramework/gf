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
#include <gf/Vector.h>

#include "gtest/gtest.h"

TEST(Vector1Test, Access) {
  gf::Vector<int, 1> vec;

  vec.data[0] = 42;
  EXPECT_EQ(42, vec.data[0]);
  EXPECT_EQ(42, vec[0]);

  vec[0] = 69;
  EXPECT_EQ(69, vec.data[0]);
  EXPECT_EQ(69, vec[0]);
}

TEST(Vector1Test, Brace1Ctor) {
  gf::Vector<int, 1> vec{42};
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector1Test, ValueCtor) {
  gf::Vector<int, 1> vec(42);
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector1Test, PointerCtor) {
  const int data[1] = { 42 };
  gf::Vector<int, 1> vec(data);
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector1Test, InitCtor) {
  gf::Vector<int, 1> vec = { 42 };
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector1Test, CopyCtor) {
  gf::Vector<int, 1> original(42);
  gf::Vector<int, 1> vec = original;
  EXPECT_EQ(42, vec[0]);
}

TEST(Vector1Test, ZeroCtor) {
  gf::Vector<int, 1> vec = gf::Zero;
  EXPECT_EQ(0, vec[0]);
}

TEST(Vector1Test, RangeFor) {
  gf::Vector<int, 1> vec(1);

  int expected = 1;

  for (int elem : vec) {
    EXPECT_EQ(expected, elem);
    ++expected;
  }

  EXPECT_EQ(expected, 2);
}

TEST(Vector1Test, Iterator) {
  gf::Vector<int, 1> vec1(1);

  EXPECT_EQ(std::distance(vec1.begin(), vec1.end()), 1);

  const gf::Vector<int, 1> vec2(1);

  EXPECT_EQ(std::distance(vec2.begin(), vec2.end()), 1);
  EXPECT_EQ(std::distance(vec2.cbegin(), vec2.cend()), 1);
}
