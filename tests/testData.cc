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
#include <gf/Data.h>

#include <cstring>

#include "gtest/gtest.h"

TEST(DataTest, Nil) {
  gf::DataObject o1;
  ASSERT_EQ(o1.type, gf::DataType::Nil);

  gf::DataObject o2 = gf::Data::convert(nullptr);
  ASSERT_EQ(o2.type, gf::DataType::Nil);
}

TEST(DataTest, Bool) {
  gf::DataObject o1 = gf::Data::convert(true);
  ASSERT_EQ(o1.type, gf::DataType::Boolean);
  EXPECT_EQ(o1.boolean, true);

  gf::DataObject o2 = gf::Data::convert(false);
  ASSERT_EQ(o2.type, gf::DataType::Boolean);
  EXPECT_EQ(o2.boolean, false);
}

TEST(DataTest, Unsigned) {
  uint8_t d1 = 42;
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Unsigned);
  EXPECT_EQ(o1.u64, UINT64_C(42));

  uint16_t d2 = 42;
  gf::DataObject o2 = gf::Data::convert(d2);
  ASSERT_EQ(o2.type, gf::DataType::Unsigned);
  EXPECT_EQ(o2.u64, UINT64_C(42));

  uint32_t d3 = 42;
  gf::DataObject o3 = gf::Data::convert(d3);
  ASSERT_EQ(o3.type, gf::DataType::Unsigned);
  EXPECT_EQ(o3.u64, UINT64_C(42));

  uint64_t d4 = 42;
  gf::DataObject o4 = gf::Data::convert(d4);
  ASSERT_EQ(o4.type, gf::DataType::Unsigned);
  EXPECT_EQ(o4.u64, UINT64_C(42));
}

TEST(DataTest, Signed) {
  int8_t d1 = -42;
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Signed);
  EXPECT_EQ(o1.i64, INT64_C(-42));

  int16_t d2 = -42;
  gf::DataObject o2 = gf::Data::convert(d2);
  ASSERT_EQ(o2.type, gf::DataType::Signed);
  EXPECT_EQ(o2.i64, INT64_C(-42));

  int32_t d3 = -42;
  gf::DataObject o3 = gf::Data::convert(d3);
  ASSERT_EQ(o3.type, gf::DataType::Signed);
  EXPECT_EQ(o3.i64, INT64_C(-42));

  int64_t d4 = -42;
  gf::DataObject o4 = gf::Data::convert(d4);
  ASSERT_EQ(o4.type, gf::DataType::Signed);
  EXPECT_EQ(o4.i64, INT64_C(-42));
}

TEST(DataTest, Float) {
  float d1 = 42.0f;
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Float);
  EXPECT_FLOAT_EQ(o1.f32, 42.0f);
}

TEST(DataTest, Double) {
  double d1 = 42.0;
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Double);
  EXPECT_DOUBLE_EQ(o1.f64, 42.0);
}

TEST(DataTest, String) {
  const char *d1 = "Foo";
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::String);
  EXPECT_EQ(o1.string.size, std::strlen(d1));
  EXPECT_TRUE(std::strcmp(o1.string.data, d1) == 0);

  std::string d2 = "Bar";
  gf::DataObject o2 = gf::Data::convert(d2);
  ASSERT_EQ(o2.type, gf::DataType::String);
  EXPECT_EQ(o2.string.size, d2.size());
  EXPECT_EQ(o2.string.data, d2);
}

TEST(DataTest, Binary) {
  const uint8_t d1[] = { 0xDE, 0xAD, 0xBE, 0xEF };
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Binary);
  EXPECT_EQ(o1.binary.size, UINT32_C(4));

  std::vector<uint8_t> d2 =  { 0xDE, 0xAD, 0xBE, 0xEF };
  gf::DataObject o2 = gf::Data::convert(d2);
  ASSERT_EQ(o2.type, gf::DataType::Binary);
  EXPECT_EQ(o2.binary.size, d2.size());

  std::array<uint8_t, 4> d3 =  {{ 0xDE, 0xAD, 0xBE, 0xEF }};
  gf::DataObject o3 = gf::Data::convert(d3);
  ASSERT_EQ(o3.type, gf::DataType::Binary);
  EXPECT_EQ(o3.binary.size, d3.size());

}

TEST(DataTest, Array) {
  const int d1[] = { 42, 69 };
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Array);
  EXPECT_EQ(o1.array.size, UINT32_C(2));
  EXPECT_EQ(o1.array.data[0].type, gf::DataType::Signed);

  std::vector<int> d2 = { 42, 69 };
  gf::DataObject o2 = gf::Data::convert(d2);
  ASSERT_EQ(o2.type, gf::DataType::Array);
  EXPECT_EQ(o2.array.size, d2.size());
  EXPECT_EQ(o2.array.data[0].type, gf::DataType::Signed);

  std::array<int, 2> d3 = {{ 42, 69 }};
  gf::DataObject o3 = gf::Data::convert(d3);
  ASSERT_EQ(o3.type, gf::DataType::Array);
  EXPECT_EQ(o3.array.size, d3.size());
  EXPECT_EQ(o3.array.data[0].type, gf::DataType::Signed);
}

TEST(DataTest, Map) {
  std::map<std::string, int> d1 = { { "Foo", 1 }, { "Bar", 2 } };
  gf::DataObject o1 = gf::Data::convert(d1);
  ASSERT_EQ(o1.type, gf::DataType::Map);
  EXPECT_EQ(o1.map.size, d1.size());
  EXPECT_EQ(o1.map.data[0].key.type, gf::DataType::String);
  EXPECT_EQ(o1.map.data[0].value.type, gf::DataType::Signed);

  std::unordered_map<std::string, int> d2 = { { "Foo", 1 }, { "Bar", 2 } };
  gf::DataObject o2 = gf::Data::convert(d2);
  ASSERT_EQ(o2.type, gf::DataType::Map);
  EXPECT_EQ(o2.map.size, d2.size());
  EXPECT_EQ(o2.map.data[0].key.type, gf::DataType::String);
  EXPECT_EQ(o2.map.data[0].value.type, gf::DataType::Signed);
}
