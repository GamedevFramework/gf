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
#include <gf/Serialization.h>

#include <cassert>
#include <cinttypes>
#include <cstring>

#include <limits>
#include <memory>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    constexpr uint8_t Magic[2] = { 'g', 'f' };


    template<bool IsSigned>
    struct CharWriter;

    template<>
    struct CharWriter<true> {
      static void write(Serializer& serializer, char data) {
        serializer.writeSigned8(data);
      }
    };

    template<>
    struct CharWriter<false> {
      static void write(Serializer& serializer, char data) {
        serializer.writeUnsigned8(data);
      }
    };


    template<bool IsSigned>
    struct CharReader;

    template<>
    struct CharReader<true> {
      static bool read(Deserializer& deserializer, char& data) { // Flawfinder: ignore
        int8_t x;

        if (deserializer.readSigned8(x)) {
          data = x;
          return true;
        }

        return false;
      }
    };

    template<>
    struct CharReader<false> {
      static bool read(Deserializer& deserializer, char& data) {
        uint8_t x;

        if (deserializer.readUnsigned8(x)) {
          data = x;
          return true;
        }

        return false;
      }
    };

  }

  /*
   * Serializer
   */

  Serializer::Serializer(OutputStream& stream, uint16_t version)
  : m_stream(&stream)
  , m_version(version)
  {
    m_stream->write(Magic);
    writeBigEndian16(version);
  }

  void Serializer::writeBoolean(bool data) {
    writeUnsigned8(static_cast<uint8_t>(data));
  }

  void Serializer::writeChar(char data) {
    CharWriter<std::numeric_limits<char>::is_signed> writer;
    writer.write(*this, data);
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
      m_stream->write(gf::span(reinterpret_cast<const uint8_t *>(data), size));
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

    m_stream->write(buf);
  }

  void Serializer::writeBigEndian32(uint32_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];

    for (std::size_t i = 0; i < Size; ++i) {
      buf[Size - i - 1] = static_cast<uint8_t>(data >> (8 * i));
    }

    m_stream->write(buf);
  }

  void Serializer::writeBigEndian16(uint16_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];

    for (std::size_t i = 0; i < Size; ++i) {
      buf[Size - i - 1] = static_cast<uint8_t>(data >> (8 * i));
    }

    m_stream->write(buf);
  }

  void Serializer::writeBigEndian8(uint8_t data) {
    m_stream->write(data);
  }

  /*
   * Deserializer
   */

  Deserializer::Deserializer(InputStream& stream)
  : m_stream(&stream)
  , m_version(0)
  {
    uint8_t magic[2] = { 0u, 0u };
    m_stream->read(magic); // Flawfinder: ignore

    if (magic[0] != Magic[0] || magic[1] != Magic[1]) {
      Log::error("The stream is not a gf archive.\n"); // throw?
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
    CharReader<std::numeric_limits<char>::is_signed> reader;
    return reader.read(*this, data); // Flawfinder: ignore
  }

  bool Deserializer::readSigned8(int8_t& data) {
    uint8_t u8;

    if (!readBigEndian8(u8)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    std::memcpy(&data, &u8, sizeof(int8_t));
    return true;
  }

  bool Deserializer::readSigned16(int16_t& data) {
    uint16_t u16;

    if (!readBigEndian16(u16)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    std::memcpy(&data, &u16, sizeof(int16_t));
    return true;
  }

  bool Deserializer::readSigned32(int32_t& data) {
    uint32_t u32;

    if (!readBigEndian32(u32)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    std::memcpy(&data, &u32, sizeof(int32_t));
    return true;
  }

  bool Deserializer::readSigned64(int64_t& data) {
    uint64_t u64;

    if (!readBigEndian64(u64)) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    std::memcpy(&data, &u64, sizeof(int64_t));
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
    if (m_stream->read(Span<uint8_t>(reinterpret_cast<uint8_t*>(data), size)) != size) {
      Log::error("Asking for a string but the file is at the end.\n");
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
    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
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
    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
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
    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
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
    return m_stream->read(data) == 1; // Flawfinder: ignore
  }

  bool Deserializer::isEof() const {
    return m_stream->isFinished();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
