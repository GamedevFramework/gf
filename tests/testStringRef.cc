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
#include <gf/StringRef.h>

#include "gtest/gtest.h"

TEST(StringRefTest, DefaultCtor) {
  gf::StringRef ref;

  EXPECT_EQ(0u, ref.getSize());
  EXPECT_EQ(nullptr, ref.getData());
}

TEST(StringRefTest, PointerCtor) {
  const char *str = "a string";

  gf::StringRef ref(str);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(str, ref.getData());
}

TEST(StringRefTest, PointerSizeCtor) {
  const char *str = "a string";

  gf::StringRef ref(str, 8);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(str, ref.getData());
}

TEST(StringRefTest, ArrayCtor) {
  const char str[] = "a string";

  gf::StringRef ref(str);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(&str[0], ref.getData());
}

TEST(StringRefTest, StringCtor) {
  std::string str("a string");

  gf::StringRef ref(str);

  EXPECT_EQ(8u, ref.getSize());
  EXPECT_EQ(&str[0], ref.getData());
}
