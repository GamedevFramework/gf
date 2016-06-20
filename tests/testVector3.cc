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
#include <gf/Vector.h>

#include "gtest/gtest.h"

TEST(Vector3Test, Access) {
  gf::Vector3i vec;
  vec.data[0] = 42;
  EXPECT_EQ(42, vec.data[0]);
  EXPECT_EQ(42, vec[0]);
  vec.data[1] = 69;
  EXPECT_EQ(69, vec.data[1]);
  EXPECT_EQ(69, vec[1]);
  vec.data[2] = 23;
  EXPECT_EQ(23, vec.data[2]);
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
  int data[3] = {42, 69, 23};
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

TEST(Vector3Test, AltFields) {
  gf::Vector3i vec(42, 69, 23);

  EXPECT_EQ(42, vec.x);
  EXPECT_EQ(42, vec.r);
  EXPECT_EQ(42, vec.xy.x);

  EXPECT_EQ(69, vec.y);
  EXPECT_EQ(69, vec.g);
  EXPECT_EQ(69, vec.xy.y);

  EXPECT_EQ(23, vec.z);
  EXPECT_EQ(23, vec.b);
}
