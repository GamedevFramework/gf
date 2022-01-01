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
#include <gf/Singleton.h>

#include "gtest/gtest.h"

class Foo {
public:
  Foo(int bar = 42)
  : m_bar(bar)
  {

  }

  int getBar() const {
    return m_bar;
  }

private:
  int m_bar;
};

static gf::Singleton<Foo> gFoo;

TEST(SingletonTest, IsValid) {
  gFoo.reset();
  EXPECT_FALSE(gFoo.isValid());
}

TEST(SingletonTest, DefaultCtor) {
  gFoo.reset();

  gf::SingletonStorage<Foo> storageForFoo(gFoo);
  EXPECT_TRUE(gFoo.isValid());
  EXPECT_EQ(42, gFoo().getBar());
}

TEST(SingletonTest, CtorWithArgument) {
  gFoo.reset();

  gf::SingletonStorage<Foo> storageForFoo(gFoo, 69);
  EXPECT_TRUE(gFoo.isValid());
  EXPECT_EQ(69, gFoo().getBar());
}
