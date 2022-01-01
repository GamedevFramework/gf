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

using Mat22i = gf::Matrix<int, 2, 2>;

TEST(Matrix2Test, Access) {
  Mat22i m;

  m(0, 0) = 1;
  m(0, 1) = 2;
  m(1, 0) = 3;
  m(1, 1) = 4;

  EXPECT_EQ(m(0, 0), 1);
  EXPECT_EQ(m(0, 1), 2);
  EXPECT_EQ(m(1, 0), 3);
  EXPECT_EQ(m(1, 1), 4);
}

TEST(Matrix2Test, ValueCtor) {
  Mat22i m(42);

  EXPECT_EQ(m(0, 0), 42);
  EXPECT_EQ(m(0, 1), 42);
  EXPECT_EQ(m(1, 0), 42);
  EXPECT_EQ(m(1, 1), 42);
}

TEST(Matrix2Test, PointerCtor) {
  const int data[] = { 1, 2, 3, 4 };

  Mat22i m(data);

  EXPECT_EQ(m(0, 0), 1);
  EXPECT_EQ(m(0, 1), 2);
  EXPECT_EQ(m(1, 0), 3);
  EXPECT_EQ(m(1, 1), 4);
}

TEST(Matrix2Test, Equality) {
  const int data[] = { 1, 2, 3, 4 };

  Mat22i m1(data);
  Mat22i m2(data);

  const int other[] = { 1, 2, 3, 0 };

  Mat22i m3(other);

  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 != m2);

  EXPECT_FALSE(m1 == m3);
  EXPECT_TRUE(m1 != m3);
  EXPECT_FALSE(m2 == m3);
  EXPECT_TRUE(m2 != m3);
}

