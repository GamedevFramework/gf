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
#include <gf/Span.h>

#include "gtest/gtest.h"

/*
 * Span
 */

TEST(SpanTest, DefaultCtor) {
  {
    gf::Span<int> span;

    EXPECT_TRUE(span.isEmpty());
    EXPECT_EQ(0u, span.getSize());
    EXPECT_EQ(nullptr, span.getData());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
  {
    gf::Span<const int> span;

    EXPECT_TRUE(span.isEmpty());
    EXPECT_EQ(0u, span.getSize());
    EXPECT_EQ(nullptr, span.getData());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
  {
    constexpr gf::Span<int> span;

    static_assert(span.isEmpty(), "");
    static_assert(span.getSize() == 0, "");
    static_assert(span.getData() == nullptr, "");
  }
}

TEST(SpanTest, PointerSizeCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::Span<int> span(input, 8);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
  {
    gf::Span<const int> span(input, 8);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
}

TEST(SpanTest, RawArrayCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::Span<int> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
  {
    gf::Span<const int> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
}

TEST(SpanTest, StdArrayCtor) {
  std::array<int, 8> input = {{ 1, 2, 3, 4, 5, 6, 7, 8 }};

  {
    gf::Span<int> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input.data(), span.getData());
  }
  {
    gf::Span<const int> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input.data(), span.getData());
  }
}

TEST(SpanTest, CompatibleCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::Span<int> span0(input);
  gf::Span<const int> span1(span0);

  EXPECT_FALSE(span1.isEmpty());
  EXPECT_EQ(8u, span1.getSize());
  EXPECT_EQ(input, span1.getData());
}

TEST(SpanTest, StaticSpanCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::StaticSpan<int, 8> span0(input);
  gf::Span<int> span1(span0);

  EXPECT_FALSE(span1.isEmpty());
  EXPECT_EQ(8u, span1.getSize());
  EXPECT_EQ(input, span1.getData());
}


TEST(SpanTest, Subscript) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::Span<int> span(input);

    EXPECT_EQ(8u, span.getSize());

    for (std::size_t i = 0; i < span.getSize(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
  {
    gf::Span<const int> span(input);

    EXPECT_EQ(8u, span.getSize());

    for (std::size_t i = 0; i < span.getSize(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
}

TEST(SpanTest, SpanFunction) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  auto span = gf::span(input, 5);
  EXPECT_FALSE(span.isEmpty());
  EXPECT_EQ(5u, span.getSize());
  EXPECT_EQ(input, span.getData());
  EXPECT_TRUE((std::is_same<decltype(span), gf::Span<int>>::value));
}


/*
 * StaticSpan
 */

TEST(StaticSpanTest, DefaultCtor) {
  {
    gf::StaticSpan<int, 10> span;

    EXPECT_TRUE(span.isEmpty());
    EXPECT_EQ(10u, span.getSize());
    EXPECT_EQ(nullptr, span.getData());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
  {
    gf::StaticSpan<const int, 10> span;

    EXPECT_TRUE(span.isEmpty());
    EXPECT_EQ(10u, span.getSize());
    EXPECT_EQ(nullptr, span.getData());

    for ([[maybe_unused]] auto item : span) {
      FAIL();
    }
  }
}

TEST(StaticSpanTest, PointerSizeCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input, 8);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
  {
    gf::StaticSpan<const int, 8> span(input, 8);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
}

TEST(StaticSpanTest, RawArrayCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input, span.getData());
  }
}

TEST(StaticSpanTest, StdArrayCtor) {
  std::array<int, 8> input = {{ 1, 2, 3, 4, 5, 6, 7, 8 }};

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input.data(), span.getData());
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_FALSE(span.isEmpty());
    EXPECT_EQ(8u, span.getSize());
    EXPECT_EQ(input.data(), span.getData());
  }
}

TEST(StaticSpanTest, CompatibleCtor) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  gf::StaticSpan<int, 8> span0(input);
  gf::StaticSpan<const int, 8> span1(span0);

  EXPECT_FALSE(span1.isEmpty());
  EXPECT_EQ(8u, span1.getSize());
  EXPECT_EQ(input, span1.getData());
}

TEST(StaticSpanTest, Subscript) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_EQ(8u, span.getSize());

    for (std::size_t i = 0; i < span.getSize(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_EQ(8u, span.getSize());

    for (std::size_t i = 0; i < span.getSize(); ++i) {
      EXPECT_EQ(span[i], input[i]);
    }
  }
}

TEST(StaticSpanTest, Get) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  {
    gf::StaticSpan<int, 8> span(input);

    EXPECT_EQ(8u, span.getSize());

    EXPECT_EQ(span.get<0>(), input[0]);
    EXPECT_EQ(span.get<1>(), input[1]);
    EXPECT_EQ(span.get<2>(), input[2]);
    EXPECT_EQ(span.get<3>(), input[3]);
    EXPECT_EQ(span.get<4>(), input[4]);
    EXPECT_EQ(span.get<5>(), input[5]);
    EXPECT_EQ(span.get<6>(), input[6]);
    EXPECT_EQ(span.get<7>(), input[7]);
  }
  {
    gf::StaticSpan<const int, 8> span(input);

    EXPECT_EQ(8u, span.getSize());

    EXPECT_EQ(span.get<0>(), input[0]);
    EXPECT_EQ(span.get<1>(), input[1]);
    EXPECT_EQ(span.get<2>(), input[2]);
    EXPECT_EQ(span.get<3>(), input[3]);
    EXPECT_EQ(span.get<4>(), input[4]);
    EXPECT_EQ(span.get<5>(), input[5]);
    EXPECT_EQ(span.get<6>(), input[6]);
    EXPECT_EQ(span.get<7>(), input[7]);
  }
}

TEST(StaticSpanTest, SpanFunction) {
  int input[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  auto span = gf::span(input);
  EXPECT_FALSE(span.isEmpty());
  EXPECT_EQ(8u, span.getSize());
  EXPECT_EQ(input, span.getData());
  EXPECT_TRUE((std::is_same<decltype(span), gf::StaticSpan<int, 8>>::value));
}
