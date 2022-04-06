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
#include <gf/SerializationOps.h>

#include <cstdint>
#include <cstring>

#include <limits>
#include <numeric>

#include <gf/Array2D.h>
#include <gf/Path.h>
#include <gf/Paths.h>
#include <gf/Streams.h>
#include <gf/VectorOps.h>

#include "gtest/gtest.h"

namespace {

  template<typename T>
  void saveAndLoad(T& in, T& out) {
    std::vector<uint8_t> bytes;

    {
      gf::BufferOutputStream ostream(&bytes);
      gf::Serializer serializer(ostream);
      serializer | in;
    }

    {
      gf::BufferInputStream istream(&bytes);
      gf::Deserializer deserializer(istream);
      deserializer | out;
    }
  }

  struct SizeWrapper {
    std::size_t size;
  };

  gf::Serializer& operator|(gf::Serializer& ar, const SizeWrapper& data) {
    ar.writeSizeHeader(data.size);
    return ar;
  }

  gf::Deserializer& operator|(gf::Deserializer& ar, SizeWrapper& data) {
    data.size = 0;
    ar.readSizeHeader(data.size);
    return ar;
  }

}


TEST(SerialTest, Version) {
  constexpr uint16_t Version = 42;

  std::vector<uint8_t> bytes;

  {
    gf::BufferOutputStream ostream(&bytes);
    gf::Serializer serializer(ostream, Version);
  }

  {
    gf::BufferInputStream istream(&bytes);
    gf::Deserializer deserializer(istream);
    EXPECT_EQ(deserializer.getVersion(), Version);
  }
}

TEST(SerialTest, Boolean) {
  bool in, out;

  in = true;
  out = false;
  saveAndLoad(in, out);
  EXPECT_EQ(in, out);

  in = false;
  out = true;
  saveAndLoad(in, out);
  EXPECT_EQ(in, out);
}

TEST(SerialTest, Signed8) {
  int8_t tests[] = {
    0,
    -1,
    1,
    INT8_MIN,
    INT8_MAX
  };

  int8_t out = -1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Signed16) {
  int16_t tests[] = {
    0,
    -1,
    1,
    INT16_MIN,
    INT16_MAX
  };

  int16_t out = -1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Signed32) {
  int32_t tests[] = {
    0,
    -1,
    1,
    INT32_MIN,
    INT32_MAX
  };

  int32_t out = -1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Signed64) {
  int64_t tests[] = {
    0,
    -1,
    1,
    INT64_MIN,
    INT64_MAX
  };

  int64_t out = -1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Unsigned8) {
  uint8_t tests[] = {
    0,
    1,
    INT8_MAX,
    UINT8_MAX
  };

  uint8_t out = 1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Unsigned16) {
  uint16_t tests[] = {
    0,
    1,
    INT16_MAX,
    UINT16_MAX
  };

  uint16_t out = 1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Unsigned32) {
  uint32_t tests[] = {
    0,
    1,
    INT32_MAX,
    UINT32_MAX
  };

  uint32_t out = 1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Unsigned64) {
  uint64_t tests[] = {
    0,
    1,
    INT64_MAX,
    UINT64_MAX
  };

  uint64_t out = 1;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Enum) {
  enum class Foo {
    Bar,
    Baz,
    Qux = 42,
  };

  Foo tests[] = {
    Foo::Bar,
    Foo::Baz,
    Foo::Qux,
  };

  Foo out;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Float) {
  float tests[] = {
    0.0f,
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::infinity()
  };

  float out = 1.0f;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_FLOAT_EQ(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Double) {
  double tests[] = {
    0.0,
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::infinity()
  };

  double out = 1.0;

  for (auto in : tests) {
    saveAndLoad(in, out);
    EXPECT_DOUBLE_EQ(in, out);
    EXPECT_EQ(in, out);
  }
}

TEST(SerialTest, Size) {
  static_assert(sizeof(std::size_t) == 8, "Assume size_t is uint64_t");

  std::size_t tests[] = {
    0,
    1,
    42,
    0xFE,
    0xFF,
    0xFFFE,
    0xFFFF,
    0xFFFFFE,
    0xFFFFFF,
    0xFFFFFFFE,
    0xFFFFFFFF,
    0xFFFFFFFFFE,
    0xFFFFFFFFFF,
    0xFFFFFFFFFFFE,
    0xFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFE,
    0xFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFE,
    0xFFFFFFFFFFFFFFFF,
  };

  SizeWrapper out;

  for (auto size : tests) {
    SizeWrapper in{size};
    saveAndLoad(in, out);
    EXPECT_EQ(in.size, out.size);
  }
}

TEST(SerialTest, String) {
  using namespace std::literals;

  std::string tests1[] = {
    ""s,
    "gf"s,
    std::string(32, 'a'),
    std::string(33, 'a'),
    std::string(UINT8_MAX + 1, 'a'),
    std::string(UINT16_MAX + 1, 'a')
  };

  std::string out1 = "-";

  for (auto& in1 : tests1) {
    saveAndLoad(in1, out1);
    EXPECT_EQ(in1, out1);
  }

  char in3[256] = "unique";
  char out3[256]; // same size as above

  saveAndLoad(in3, out3);
  EXPECT_TRUE(std::strcmp(in3, out3) == 0);
}

TEST(SerialTest, Binary) {
  std::vector<uint8_t> tests1[] = {
    { },
    { 0x00, 0xFF },
    std::vector<uint8_t>(UINT8_MAX + 1, 0x02),
    std::vector<uint8_t>(UINT16_MAX + 1, 0x03)
  };

  std::vector<uint8_t> out1 = { 0x01 };

  for (auto& in1 : tests1) {
    saveAndLoad(in1, out1);
    EXPECT_EQ(in1, out1);
  }

  std::array<uint8_t, 256> tests2[] = {
    { },
    {{ 0x00, 0xFF }},
    {{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }}
  };

  std::array<uint8_t, 256> out2;

  for (auto& in2 : tests2) {
    saveAndLoad(in2, out2);
    EXPECT_EQ(in2, out2);
  }

  uint8_t in3[] = { 0x01, 0x02, 0x03, 0x04 };
  uint8_t out3[4]; // same size as in3

  saveAndLoad(in3, out3);

  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(in3[i], out3[i]);
  }
}

TEST(SerialTest, Array) {
  std::vector<int32_t> tests1[] = {
    {  },
    { 0, 2, INT16_MIN, INT16_MAX, INT32_MIN, INT32_MAX },
    std::vector<int>(16, 42),
    std::vector<int>(17, 42),
    std::vector<int>(UINT8_MAX + 1, 42),
    std::vector<int>(UINT16_MAX + 1, 42),
  };

  std::vector<int32_t> out1;

  for (auto& in1 : tests1) {
    saveAndLoad(in1, out1);
    EXPECT_EQ(in1, out1);
  }

  std::vector<int32_t> out1bis[6];

  saveAndLoad(tests1, out1bis);

  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(tests1[i], out1bis[i]);
  }

  std::array<int32_t, 256> tests2[] = {
    { },
    {{ 0x00, 0xFF }},
    {{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }}
  };

  std::array<int32_t, 256> out2;

  for (auto& in2 : tests2) {
    saveAndLoad(in2, out2);
    EXPECT_EQ(in2, out2);
  }

  int32_t in3[] = { 1, 2, 3, 4 };
  int32_t out3[4]; // same size as in3

  saveAndLoad(in3, out3);

  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(in3[i], out3[i]);
  }
}

TEST(SerialTest, Set) {
  std::set<std::string> tests1[] = {
    { },
    { "First" "Second" },
    { },
    { },
    { },
    { },
  };

  for (int32_t i = 0; i < 16; ++i) {
    tests1[2].emplace(std::to_string(i));
  }

  for (int32_t i = 0; i < 17; ++i) {
    tests1[3].emplace(std::to_string(i));
  }

  for (int32_t i = 0; i < UINT8_MAX + 1; ++i) {
    tests1[4].emplace(std::to_string(i));
  }

  std::set<std::string> out1;

  for (auto& in1 : tests1) {
    saveAndLoad(in1, out1);
    EXPECT_EQ(in1, out1);
  }

  std::set<std::string> out1bis[6];

  saveAndLoad(tests1, out1bis);

  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(tests1[i], out1bis[i]);
  }

  std::unordered_set<std::string> tests2[] = {
    { },
    { "First", "Second" },
    { },
    { },
    { },
    { },
  };

  for (int32_t i = 0; i < 16; ++i) {
    tests2[2].emplace(std::to_string(i));
  }

  for (int32_t i = 0; i < 17; ++i) {
    tests2[3].emplace(std::to_string(i));
  }

  for (int32_t i = 0; i < UINT8_MAX + 1; ++i) {
    tests2[4].emplace(std::to_string(i));
  }

  std::unordered_set<std::string> out2;

  for (auto& in2 : tests2) {
    saveAndLoad(in2, out2);
    EXPECT_EQ(in2, out2);
  }

  std::unordered_set<std::string> out2bis[6];

  saveAndLoad(tests2, out2bis);

  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(tests2[i], out2bis[i]);
  }
}

TEST(SerialTest, Map) {
  std::map<std::string, int32_t> tests1[] = {
    { },
    { { "First", 1 }, { "Second", 2 } },
    { },
    { },
    { },
    { },
  };

  for (int32_t i = 0; i < 16; ++i) {
    tests1[2].emplace(std::to_string(i), i);
  }

  for (int32_t i = 0; i < 17; ++i) {
    tests1[3].emplace(std::to_string(i), i);
  }

  for (int32_t i = 0; i < UINT8_MAX + 1; ++i) {
    tests1[4].emplace(std::to_string(i), i);
  }

  std::map<std::string, int32_t> out1;

  for (auto& in1 : tests1) {
    saveAndLoad(in1, out1);
    EXPECT_EQ(in1, out1);
  }

  std::map<std::string, int32_t> out1bis[6];

  saveAndLoad(tests1, out1bis);

  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(tests1[i], out1bis[i]);
  }

  std::unordered_map<std::string, int32_t> tests2[] = {
    { },
    { { "First", 1 }, { "Second", 2 } },
    { },
    { },
    { },
    { },
  };

  for (int32_t i = 0; i < 16; ++i) {
    tests2[2].emplace(std::to_string(i), i);
  }

  for (int32_t i = 0; i < 17; ++i) {
    tests2[3].emplace(std::to_string(i), i);
  }

  for (int32_t i = 0; i < UINT8_MAX + 1; ++i) {
    tests2[4].emplace(std::to_string(i), i);
  }

  std::unordered_map<std::string, int32_t> out2;

  for (auto& in2 : tests2) {
    saveAndLoad(in2, out2);
    EXPECT_EQ(in2, out2);
  }

  std::unordered_map<std::string, int32_t> out2bis[6];

  saveAndLoad(tests2, out2bis);

  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(tests2[i], out2bis[i]);
  }
}

TEST(SerialTest, Vector) {
  gf::Vector4i in1(-1, 2, -3, 4);
  gf::Vector4i out1;

  saveAndLoad(in1, out1);

  EXPECT_EQ(in1, out1);
}

TEST(SerialTest, Array2D) {
  gf::Array2D<int> in1({ 3, 2 });
  in1({ 0, 0 }) = 1;
  in1({ 0, 1 }) = 2;
  in1({ 1, 0 }) = 3;
  in1({ 1, 1 }) = 4;
  in1({ 2, 0 }) = 5;
  in1({ 2, 1 }) = 6;

  gf::Array2D<int> out1;

  saveAndLoad(in1, out1);

  EXPECT_EQ(in1, out1);
}

TEST(SerialTest, Path) {
  gf::Path filename = "/etc/passwd";

  gf::Path in1 = filename;
  gf::Path out1;

  saveAndLoad(in1, out1);

  EXPECT_EQ(in1, out1);
}

TEST(SerialTest, Compressed) {
  std::vector<int32_t> in1(10 * 1024);
  std::iota(in1.begin(), in1.end(), 1);

  std::vector<int32_t> out1;

  std::vector<uint8_t> bytes;

  {
    gf::BufferOutputStream ostream(&bytes);
    gf::CompressedOutputStream compressed(ostream);
    gf::Serializer ar(compressed);
    ar | in1;
  }

  {
    gf::BufferInputStream istream(&bytes);
    gf::CompressedInputStream compressed(istream);
    gf::Deserializer ar(compressed);
    ar | out1;
  }

  EXPECT_EQ(in1, out1);
}
