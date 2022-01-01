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
#include <gf/Matrix.h>
#include <gf/Vector.h>

#include <iostream>

#include "gtest/gtest.h"

using Mat42i = gf::Matrix<int, 4, 2>;
using Mat24i = gf::Matrix<int, 2, 4>;

TEST(MatrixTest, Access) {
  Mat42i m;

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(1, 0) = 3;
  m(1, 1) = 4;
  m(2, 0) = 5;
  m(2, 1) = 6;
  m(3, 0) = 7;
  m(3, 1) = 8;

  EXPECT_EQ(m(0, 0), 1);
  EXPECT_EQ(m(0, 1), 2);
  EXPECT_EQ(m(1, 0), 3);
  EXPECT_EQ(m(1, 1), 4);
  EXPECT_EQ(m(2, 0), 5);
  EXPECT_EQ(m(2, 1), 6);
  EXPECT_EQ(m(3, 0), 7);
  EXPECT_EQ(m(3, 1), 8);
}

TEST(MatrixTest, ZeroCtor) {
  Mat42i m = gf::Zero;

  EXPECT_EQ(m(0, 0), 0);
  EXPECT_EQ(m(0, 1), 0);
  EXPECT_EQ(m(1, 0), 0);
  EXPECT_EQ(m(1, 1), 0);
  EXPECT_EQ(m(2, 0), 0);
  EXPECT_EQ(m(2, 1), 0);
  EXPECT_EQ(m(3, 0), 0);
  EXPECT_EQ(m(3, 1), 0);
}

TEST(MatrixTest, ValueCtor) {
  Mat42i m(42);

  EXPECT_EQ(m(0, 0), 42);
  EXPECT_EQ(m(0, 1), 42);
  EXPECT_EQ(m(1, 0), 42);
  EXPECT_EQ(m(1, 1), 42);
  EXPECT_EQ(m(2, 0), 42);
  EXPECT_EQ(m(2, 1), 42);
  EXPECT_EQ(m(3, 0), 42);
  EXPECT_EQ(m(3, 1), 42);
}

TEST(MatrixTest, PointerCtor) {
  const int data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  Mat42i m(data);

  EXPECT_EQ(m(0, 0), 1);
  EXPECT_EQ(m(0, 1), 2);
  EXPECT_EQ(m(1, 0), 3);
  EXPECT_EQ(m(1, 1), 4);
  EXPECT_EQ(m(2, 0), 5);
  EXPECT_EQ(m(2, 1), 6);
  EXPECT_EQ(m(3, 0), 7);
  EXPECT_EQ(m(3, 1), 8);
}

TEST(MatrixTest, Equality) {
  const int data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  Mat42i m1(data);
  Mat42i m2(data);

  const int other[] = { 1, 2, 3, 4, 5, 6, 7, 0 };

  Mat42i m3(other);

  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 != m2);

  EXPECT_FALSE(m1 == m3);
  EXPECT_TRUE(m1 != m3);
  EXPECT_FALSE(m2 == m3);
  EXPECT_TRUE(m2 != m3);
}

TEST(MatrixTest, UnaryMinus) {
  const int data[] = { -1, -2, -3, -4, -5, -6, -7, -8 };

  Mat42i m1(data);
  Mat42i m2 = -m1;

  const int result[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  Mat42i m3(result);

  EXPECT_TRUE(m2 == m3);
}

