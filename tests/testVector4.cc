/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
  vec.data[0] = 42;
  EXPECT_EQ(42, vec.data[0]);
  EXPECT_EQ(42, vec[0]);
  vec.data[1] = 69;
  EXPECT_EQ(69, vec.data[1]);
  EXPECT_EQ(69, vec[1]);
  vec.data[2] = 23;
  EXPECT_EQ(23, vec.data[2]);
  EXPECT_EQ(23, vec[2]);
  vec.data[3] = 17;
  EXPECT_EQ(17, vec.data[3]);
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
  int data[4] = {42, 69, 23, 17};
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

TEST(Vector4Test, AltFields) {
  gf::Vector4i vec(42, 69, 23, 17);

  EXPECT_EQ(42, vec.x);
  EXPECT_EQ(42, vec.r);
  EXPECT_EQ(42, vec.xy.x);
  EXPECT_EQ(42, vec.xyz.x);
  EXPECT_EQ(42, vec.rgb.r);

  EXPECT_EQ(69, vec.y);
  EXPECT_EQ(69, vec.g);
  EXPECT_EQ(69, vec.xy.y);
  EXPECT_EQ(69, vec.xyz.y);
  EXPECT_EQ(69, vec.rgb.g);

  EXPECT_EQ(23, vec.z);
  EXPECT_EQ(23, vec.b);
  EXPECT_EQ(23, vec.xyz.z);
  EXPECT_EQ(23, vec.rgb.b);

  EXPECT_EQ(17, vec.w);
  EXPECT_EQ(17, vec.a);
}
