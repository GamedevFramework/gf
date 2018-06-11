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
#include <gf/Serialization.h>

#include <cassert>
#include <cinttypes>

#include <limits>
#include <memory>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    constexpr uint8_t Magic[2] = { 'g', 'f' };

  }

  /*
   * Serializer
   */

  Serializer::Serializer(const Path& filename, uint16_t version)
  : m_file(filename, BinaryFile::Mode::Write)
  , m_version(version)
  {
    m_file.write(Magic);
    writeBigEndian16(version);
  }

  void Serializer::writeBoolean(bool data) {
    writeUnsigned8(data);
  }

  void Serializer::writeChar(char data) {
    if (std::numeric_limits<char>::is_signed) {
      writeSigned8(data);
    } else {
      writeUnsigned8(data);
    }
  }

  void Serializer::writeSigned8(int8_t data) {
    writeBigEndian8(data);
  }

  void Serializer::writeSigned16(int16_t data) {
    writeBigEndian16(data);
  }

  void Serializer::writeSigned32(int32_t data) {
    writeBigEndian32(data);
  }

  void Serializer::writeSigned64(int64_t data) {
    writeBigEndian64(data);
  }

  void Serializer::writeUnsigned8(uint8_t data) {
    writeBigEndian8(data);
  }

  void Serializer::writeUnsigned16(uint16_t data) {
    writeBigEndian16(data);
  }

  void Serializer::writeUnsigned32(uint32_t data) {
    writeBigEndian32(data);
  }

  void Serializer::writeUnsigned64(uint64_t data) {
    writeBigEndian64(data);
  }

  void Serializer::writeFloat(float data) {
    union {
      float f32;
      uint32_t u32;
    };

    f32 = data;
    writeBigEndian32(u32);
  }

  void Serializer::writeDouble(double data) {
    union {
      double f64;
      uint64_t u64;
    };

    f64 = data;
    writeBigEndian64(u64);
  }

  void Serializer::writeString(const char *data, std::size_t size) {
    writeSizeHeader(size);

    if (size > 0) {
      m_file.write(ArrayRef<uint8_t>(reinterpret_cast<const uint8_t *>(data), size));
    }
  }

  void Serializer::writeSizeHeader(std::size_t size) {
    uint64_t data = static_cast<uint64_t>(size);
    writeUnsigned64(data);
  }


  void Serializer::writeBigEndian64(uint64_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];

    for (std::size_t i = 0; i < Size; ++i) {
      buf[Size - i - 1] = static_cast<uint8_t>(data >> (8 * i));
    }

    m_file.write(buf);
  }

  void Serializer::writeBigEndian32(uint32_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];

    for (std::size_t i = 0; i < Size; ++i) {
      buf[Size - i - 1] = static_cast<uint8_t>(data >> (8 * i));
    }

    m_file.write(buf);
  }

  void Serializer::writeBigEndian16(uint16_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];

    for (std::size_t i = 0; i < Size; ++i) {
      buf[Size - i - 1] = static_cast<uint8_t>(data >> (8 * i));
    }

    m_file.write(buf);
  }

  void Serializer::writeBigEndian8(uint8_t data) {
    m_file.write(data);
  }

  /*
   * Deserializer
   */

  Deserializer::Deserializer(const Path& filename)
  : m_file(filename, BinaryFile::Mode::Read)
  , m_version(0)
  , m_eof(false)
  {
    uint8_t magic[2] = { 0u, 0u };

    readBigEndian8(magic[0]);
    readBigEndian8(magic[1]);

    if (magic[0] != Magic[0] || magic[1] != Magic[1]) {
      Log::error("This file is not a gf archive: '%s'.\n", filename.string().c_str());
      m_eof = true;
      return;
    }

    readBigEndian16(m_version);
  }

  bool Deserializer::readBoolean(bool& data) {
    uint8_t x;

    if (!readBigEndian8(x)) {
      Log::error("Asking for boolean but the file is at the end.\n");
      return false;
    }

    data = (x != 0);
    return true;
  }

  bool Deserializer::readChar(char& data) {
    bool res;

    if (std::numeric_limits<char>::is_signed) {
      int8_t x;
      res = readSigned8(x);
      data = x;
    } else {
      uint8_t x;
      res = readUnsigned8(x);
      data = x;
    }

    return res;
  }

  bool Deserializer::readSigned8(int8_t& data) {
    union {
      uint8_t u8;
      int8_t i8;
    };

    if (!readBigEndian8(u8)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    data = i8;
    return true;
  }

  bool Deserializer::readSigned16(int16_t& data) {
    union {
      uint16_t u16;
      int16_t i16;
    };

    if (!readBigEndian16(u16)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    data = i16;
    return true;
  }

  bool Deserializer::readSigned32(int32_t& data) {
    union {
      uint32_t u32;
      int32_t i32;
    };

    if (!readBigEndian32(u32)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    data = i32;
    return true;
  }

  bool Deserializer::readSigned64(int64_t& data) {
    union {
      uint64_t u64;
      int64_t i64;
    };

    if (!readBigEndian64(u64)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    data = i64;
    return true;
  }

  bool Deserializer::readUnsigned8(uint8_t& data) {
    if (!readBigEndian8(data)) {
      Log::error("Asking for unsigned but the file is at the end.\n");
      return false;
    }

    return true;
  }

  bool Deserializer::readUnsigned16(uint16_t& data) {
    if (!readBigEndian16(data)) {
      Log::error("Asking for unsigned but the file is at the end.\n");
      return false;
    }

    return true;
  }

  bool Deserializer::readUnsigned32(uint32_t& data) {
    if (!readBigEndian32(data)) {
      Log::error("Asking for unsigned but the file is at the end.\n");
      return false;
    }

    return true;
  }

  bool Deserializer::readUnsigned64(uint64_t& data) {
    if (!readBigEndian64(data)) {
      Log::error("Asking for unsigned but the file is at the end.\n");
      return false;
    }

    return true;
  }

  bool Deserializer::readFloat(float& data) {
    union {
      float f32;
      uint32_t u32;
    };

    if (!readBigEndian32(u32)) {
      Log::error("Asking for float but the file is at the end.\n");
      return false;
    }

    data = f32;
    return true;
  }

  bool Deserializer::readDouble(double& data) {
    union {
      double f64;
      uint64_t u64;
    };

    if (!readBigEndian64(u64)) {
      return false;
    }

    data = f64;
    return true;
  }

  bool Deserializer::readString(char *data, std::size_t size) {
    if (m_file.read(BufferRef<uint8_t>(reinterpret_cast<uint8_t*>(data), size)) != size) {
      Log::error("Asking for a string but the file is at the end.\n");
      m_eof = true;
      return false;
    }

    return true;
  }

  bool Deserializer::readSizeHeader(std::size_t& size) {
    uint64_t data;

    if (!readUnsigned64(data)) {
      return false;
    }

    size = static_cast<std::size_t>(data);
    return true;
  }

  bool Deserializer::readBigEndian64(uint64_t& data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    if (m_file.read(buf) != Size) {
      m_eof = true;
      return false;
    }

    uint64_t result = 0;

    for (std::size_t i = 0; i < Size; ++i) {
      result = (result << 8) + buf[i];
    }

    data = result;

    return true;
  }

  bool Deserializer::readBigEndian32(uint32_t& data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    if (m_file.read(buf) != Size) {
      m_eof = true;
      return false;
    }

    uint32_t result = 0;

    for (std::size_t i = 0; i < Size; ++i) {
      result = (result << 8) + buf[i];
    }

    data = result;

    return true;
  }

  bool Deserializer::readBigEndian16(uint16_t& data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    if (m_file.read(buf) != Size) {
      m_eof = true;
      return false;
    }

    uint16_t result = 0;

    for (std::size_t i = 0; i < Size; ++i) {
      result = (result << 8) + buf[i];
    }

    data = result;

    return true;
  }

  bool Deserializer::readBigEndian8(uint8_t& data) {
    if (m_file.isEof()) {
      m_eof = true;
      return false;
    }

    auto size = m_file.read(data);
    m_eof = (size != 1);
    return !m_eof;
  }

  bool Deserializer::isEof() const {
    return m_eof;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
