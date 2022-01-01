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
#include <gf/Dice.h>

#include "gtest/gtest.h"

#include <gf/Random.h>

using namespace gf::literals;

TEST(DiceTest, Ctor) {
  gf::Dice d1(3, 2, 4);

  EXPECT_EQ(d1.getFaces(), 3);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 4);
}

TEST(DiceTest, Faces) {
  gf::Dice d1 = "d3"_dice;

  EXPECT_EQ(d1.getFaces(), 3);
  EXPECT_EQ(d1.getCount(), 1);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, FacesAndCount) {
  gf::Dice d1 = "2D3"_dice;

  EXPECT_EQ(d1.getFaces(), 3);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, FacesCountAndModifier) {
  gf::Dice d1 = "2d3+4"_dice;

  EXPECT_EQ(d1.getFaces(), 3);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 4);
}

TEST(DiceTest, FacesAndModifier) {
  gf::Dice d1 = "D3+4"_dice;

  EXPECT_EQ(d1.getFaces(), 3);
  EXPECT_EQ(d1.getCount(), 1);
  EXPECT_EQ(d1.getModifier(), 4);
}

TEST(DiceTest, Constexpr) {
  constexpr gf::Dice d1 = "2d3+4"_dice;

  static_assert(d1.getFaces() == 3, "Dice::getFaces()");
  static_assert(d1.getCount() == 2, "Dice::getCount()");
  static_assert(d1.getModifier() == 4, "Dice::getModifier()");

  EXPECT_EQ(d1.getFaces(), 3);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 4);
}

TEST(DiceTest, Roll) {
  constexpr gf::Dice d1 = "2D3+4"_dice;

  gf::Random random;

  for (int i = 0; i < 1000; ++i) {
    int x = d1.roll(random);
    ASSERT_GE(x, 6);
    ASSERT_LE(x, 10);
  }
}

TEST(DiceTest, D4) {
  constexpr gf::Dice d1 = 2_d4;

  EXPECT_EQ(d1.getFaces(), 4);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, D6) {
  constexpr gf::Dice d1 = 2_d6;

  EXPECT_EQ(d1.getFaces(), 6);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, D8) {
  constexpr gf::Dice d1 = 2_d8;

  EXPECT_EQ(d1.getFaces(), 8);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, D10) {
  constexpr gf::Dice d1 = 2_d10;

  EXPECT_EQ(d1.getFaces(), 10);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, D12) {
  constexpr gf::Dice d1 = 2_d12;

  EXPECT_EQ(d1.getFaces(), 12);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, D20) {
  constexpr gf::Dice d1 = 2_d20;

  EXPECT_EQ(d1.getFaces(), 20);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}

TEST(DiceTest, D100) {
  constexpr gf::Dice d1 = 2_d100;

  EXPECT_EQ(d1.getFaces(), 100);
  EXPECT_EQ(d1.getCount(), 2);
  EXPECT_EQ(d1.getModifier(), 0);
}
