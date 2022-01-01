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
#include <gf/Flags.h>

#include "gtest/gtest.h"

enum Foo {
  Foo1 = 0x01,
  Foo2 = 0x02,
  Foo3 = 0x04,
  Foo4 = 0x10,
};

namespace gf {
  template<>
  struct EnableBitmaskOperators<Foo> {
    static constexpr bool value = true;
  };
}

enum class Bar : uint32_t {
  Bar1 = 0x10000000,
  Bar2 = 0x20000000,
  Bar3 = 0x40000000,
  Bar4 = 0x80000000,
};

namespace gf {
  template<>
  struct EnableBitmaskOperators<Bar> {
    static constexpr bool value = true;
  };
}

enum class Baz {
  Baz1 = 0x1000,
  Baz2 = 0x2000,
  Baz3 = 0x8000,
};


TEST(FlagsTest, NoneCtor) {
  gf::Flags<Foo> foo(gf::None);
  ASSERT_FALSE(foo);

  gf::Flags<Bar> bar(gf::None);
  ASSERT_FALSE(bar);

  gf::Flags<Baz> baz(gf::None);
  ASSERT_FALSE(baz);
}

TEST(FlagsTest, AllCtor) {
  gf::Flags<Foo> foo(gf::All);
  ASSERT_TRUE(foo);

  gf::Flags<Bar> bar(gf::All);
  ASSERT_TRUE(bar);

  gf::Flags<Baz> baz(gf::All);
  ASSERT_TRUE(baz);
}

TEST(FlagsTest, EnumCtor) {
  gf::Flags<Foo> foo(Foo1);
  ASSERT_TRUE(foo);
  ASSERT_TRUE(foo.test(Foo1));

  gf::Flags<Bar> bar(Bar::Bar1);
  ASSERT_TRUE(bar);
  ASSERT_TRUE(bar.test(Bar::Bar1));

  gf::Flags<Baz> baz(Baz::Baz1);
  ASSERT_TRUE(baz);
  ASSERT_TRUE(baz.test(Baz::Baz1));
}

TEST(FlagsTest, EnumConversion) {
  gf::Flags<Foo> foo;
  foo = Foo1;
  ASSERT_TRUE(foo);
  ASSERT_TRUE(foo.test(Foo1));

  gf::Flags<Bar> bar;
  bar = Bar::Bar1;
  ASSERT_TRUE(bar);
  ASSERT_TRUE(bar.test(Bar::Bar1));

  gf::Flags<Baz> baz;
  baz = Baz::Baz1;
  ASSERT_TRUE(baz);
  ASSERT_TRUE(baz.test(Baz::Baz1));
}

TEST(FlagsTest, FlagsOr) {
  gf::Flags<Foo> foo1 = Foo1;
  gf::Flags<Foo> foo2 = Foo2;
  gf::Flags<Foo> foo3 = foo1 | foo2;
  ASSERT_TRUE(foo3);
  ASSERT_TRUE(foo3.test(Foo1));
  ASSERT_TRUE(foo3.test(Foo2));
  ASSERT_FALSE(foo3.test(Foo3));

  foo3 |= Foo3;
  ASSERT_TRUE(foo3.test(Foo1));
  ASSERT_TRUE(foo3.test(Foo2));
  ASSERT_TRUE(foo3.test(Foo3));

  gf::Flags<Bar> bar1 = Bar::Bar1;
  gf::Flags<Bar> bar2 = Bar::Bar2;
  gf::Flags<Bar> bar3 = bar1 | bar2;
  ASSERT_TRUE(bar3);
  ASSERT_TRUE(bar3.test(Bar::Bar1));
  ASSERT_TRUE(bar3.test(Bar::Bar2));
  ASSERT_FALSE(bar3.test(Bar::Bar3));
  ASSERT_FALSE(bar3.test(Bar::Bar4));

  bar3 |= Bar::Bar3;
  ASSERT_TRUE(bar3.test(Bar::Bar1));
  ASSERT_TRUE(bar3.test(Bar::Bar2));
  ASSERT_TRUE(bar3.test(Bar::Bar3));
  ASSERT_FALSE(bar3.test(Bar::Bar4));

  gf::Flags<Baz> baz1 = Baz::Baz1;
  gf::Flags<Baz> baz2 = Baz::Baz2;
  gf::Flags<Baz> baz3 = baz1 | baz2;
  ASSERT_TRUE(baz3);
  ASSERT_TRUE(baz3.test(Baz::Baz1));
  ASSERT_TRUE(baz3.test(Baz::Baz2));
  ASSERT_FALSE(baz3.test(Baz::Baz3));

  baz3 |= Baz::Baz3;
  ASSERT_TRUE(baz3);
  ASSERT_TRUE(baz3.test(Baz::Baz1));
  ASSERT_TRUE(baz3.test(Baz::Baz2));
  ASSERT_TRUE(baz3.test(Baz::Baz3));
}

TEST(FlagsTest, FlagsNot) {
  gf::Flags<Foo> foo1 = Foo1;
  gf::Flags<Foo> foo2 = ~foo1;
  ASSERT_TRUE(foo2);
  ASSERT_FALSE(foo2.test(Foo1));
  ASSERT_TRUE(foo2.test(Foo2));
  ASSERT_TRUE(foo2.test(Foo3));

  gf::Flags<Bar> bar1 = Bar::Bar1;
  gf::Flags<Bar> bar2 = ~bar1;
  ASSERT_TRUE(bar2);
  ASSERT_FALSE(bar2.test(Bar::Bar1));
  ASSERT_TRUE(bar2.test(Bar::Bar2));
  ASSERT_TRUE(bar2.test(Bar::Bar3));
  ASSERT_TRUE(bar2.test(Bar::Bar4));

  gf::Flags<Baz> baz1 = Baz::Baz1;
  gf::Flags<Baz> baz2 = ~baz1;
  ASSERT_TRUE(baz2);
  ASSERT_FALSE(baz2.test(Baz::Baz1));
  ASSERT_TRUE(baz2.test(Baz::Baz2));
  ASSERT_TRUE(baz2.test(Baz::Baz3));
}

TEST(FlagsTest, EnumOr) {
  using gf::operator|;

  gf::Flags<Foo> foo = Foo1 | Foo2;
  ASSERT_TRUE(foo);
  ASSERT_TRUE(foo.test(Foo1));
  ASSERT_TRUE(foo.test(Foo2));
  ASSERT_FALSE(foo.test(Foo3));

  gf::Flags<Bar> bar = Bar::Bar1 | Bar::Bar2 | Bar::Bar3;
  ASSERT_TRUE(bar);
  ASSERT_TRUE(bar.test(Bar::Bar1));
  ASSERT_TRUE(bar.test(Bar::Bar2));
  ASSERT_TRUE(bar.test(Bar::Bar3));
  ASSERT_FALSE(bar.test(Bar::Bar4));

  // no "enum or" for Baz
}


enum class AnimalProperties {
  HasClaws     = 0x01,
  CanFly       = 0x02,
  EatsFish     = 0x04,
  IsEndangered = 0x08,
};

namespace gf {
  template<>
  struct EnableBitmaskOperators<AnimalProperties> {
    static constexpr bool value = true;
  };
}

TEST(FlagsTest, Example) {
  using gf::operator|;

  gf::Flags<AnimalProperties> seahawk = AnimalProperties::CanFly | AnimalProperties::EatsFish;
  seahawk |= AnimalProperties::IsEndangered;

  ASSERT_FALSE(seahawk.test(AnimalProperties::HasClaws));
}
