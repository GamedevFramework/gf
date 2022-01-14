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
#include <gf/Id.h>

#include "gtest/gtest.h"

struct FNVTestCase {
  const char *input;
  gf::Id expected;
};

TEST(IdTest, FNV) {
  // test vectors from:
  // http://www.isthe.com/chongo/src/fnv/test_fnv.c

  static constexpr FNVTestCase TestVectors[] {
    { "",       0xcbf29ce484222325 },
    { "a",      0xaf63dc4c8601ec8c },
    { "b",      0xaf63df4c8601f1a5 },
    { "c",      0xaf63de4c8601eff2 },
    { "d",      0xaf63d94c8601e773 },
    { "e",      0xaf63d84c8601e5c0 },
    { "f",      0xaf63db4c8601ead9 },
    { "fo",     0x08985907b541d342 },
    { "foo",    0xdcb27518fed9d577 },
    { "foob",   0xdd120e790c2512af },
    { "fooba",  0xcac165afa2fef40a },
    { "foobar", 0x85944171f73967e8 },
  };

  for (auto& test : TestVectors) {
    EXPECT_EQ(gf::hash(test.input), test.expected);
  }
}
