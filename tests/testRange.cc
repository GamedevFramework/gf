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
#include <gf/Range.h>

#include "gtest/gtest.h"

TEST(RangeTest, Ctor) {
  gf::RangeI ri{0, 10};

  EXPECT_EQ(0, ri.lo);
  EXPECT_EQ(10, ri.hi);
}

TEST(RangeTest, Contains) {
  gf::RangeI ri1{1, 10};

  EXPECT_TRUE(ri1.contains(1));
  EXPECT_TRUE(!ri1.contains(10));

  EXPECT_TRUE(ri1.contains(5));
  EXPECT_TRUE(!ri1.contains(0));
  EXPECT_TRUE(!ri1.contains(11));

  gf::RangeF ri2{1.0f, 10.0f};

  EXPECT_TRUE(ri2.contains(1.0f));
  EXPECT_TRUE(!ri2.contains(10.0f));

  EXPECT_TRUE(ri2.contains(5.0f));
  EXPECT_TRUE(!ri2.contains(0.0f));
  EXPECT_TRUE(!ri2.contains(11.0f));

  static_assert(gf::RangeI{1, 10}.contains(1), "Range::contains");
  static_assert(!gf::RangeI{1, 10}.contains(10), "Range::contains");

  static_assert(gf::RangeI{1, 10}.contains(5), "Range::contains");
  static_assert(!gf::RangeI{1, 10}.contains(0), "Range::contains");
  static_assert(!gf::RangeI{1, 10}.contains(11), "Range::contains");
}

TEST(RangeTest, Length) {
  gf::RangeI ri1{1, 10};
  EXPECT_EQ(9, ri1.length());

  gf::RangeF ri2{1.0, 10.0};
  ASSERT_FLOAT_EQ(9.0, ri2.length());

  static_assert(gf::RangeI{1, 10}.length() == 9, "Range::length");
}

TEST(RangeTest, IsEmpty) {
  gf::RangeI ri1{1, 10};
  EXPECT_TRUE(!ri1.isEmpty());

  gf::RangeI ri2{1, 1};
  EXPECT_TRUE(ri2.isEmpty());

  gf::RangeI ri3{10, 1};
  EXPECT_TRUE(ri3.isEmpty());

  static_assert(!gf::RangeI{1, 10}.isEmpty(), "Range::isEmpty");
  static_assert(gf::RangeI{1, 1}.isEmpty(), "Range::isEmpty");
  static_assert(gf::RangeI{10, 1}.isEmpty(), "Range::isEmpty");
}

TEST(RangeTest, IsValid) {
  gf::RangeI ri1{1, 10};
  EXPECT_TRUE(ri1.isValid());

  gf::RangeI ri2{1, 1};
  EXPECT_TRUE(ri2.isValid());

  gf::RangeI ri3{10, 1};
  EXPECT_TRUE(!ri3.isValid());

  static_assert(gf::RangeI{1, 10}.isValid(), "Range::isValid");
  static_assert(gf::RangeI{1, 1}.isValid(), "Range::isValid");
  static_assert(!gf::RangeI{10, 1}.isValid(), "Range::isValid");
}

TEST(RangeTest, BeginEnd) {
  gf::RangeI ri{1, 10};

  int count = 0;

  for (auto x : ri) {
    ASSERT_TRUE(ri.contains(x));
    count++;
  }

  ASSERT_EQ(count, ri.length());
}
