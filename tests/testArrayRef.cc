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
#include <gf/ArrayRef.h>

#include "gtest/gtest.h"

#include <gf/Unused.h>

TEST(ArrayRefTest, DefaultCtor) {
  gf::ArrayRef<int> ref;

  EXPECT_EQ(0u, ref.getSize());
  EXPECT_EQ(nullptr, ref.getData());

  for (auto item : ref) {
    gf::unused(item);
    FAIL();
  }
}


TEST(ArrayRefTest, PointerSizeCtor) {
  const int data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::ArrayRef<int> ref(data, 8);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(data, ref.getData());
}

TEST(ArrayRefTest, ArrayCtor) {
  const int data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::ArrayRef<int> ref(data);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(&data[0], ref.getData());
}

TEST(ArrayRefTest, StdVectorCtor) {
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::ArrayRef<int> ref(data);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(&data[0], ref.getData());
}

TEST(ArrayRefTest, EmptyStdVectorCtor) {
  std::vector<int> data;

  gf::ArrayRef<int> ref(data);

  EXPECT_EQ(0u, ref.getSize());

  for (auto item : ref) {
    gf::unused(item);
    FAIL();
  }
}

TEST(ArrayRefTest, StdArrayCtor) {
  std::array<int, 8> data = {{ 1, 2, 3, 4, 5, 6, 7, 8 }};

  gf::ArrayRef<int> ref(data);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(&data[0], ref.getData());
}

TEST(ArrayRefTest, InitializerListCtor) {
  auto data = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::ArrayRef<int> ref(data);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(data.begin(), ref.getData());
}
