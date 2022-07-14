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
    static_assert(sizeof(uint32_t) == sizeof(float), "float should be 32 bits");
    uint32_t u32;
    std::memcpy(&u32, &data, sizeof(float));
    writeBigEndian32(u32);
  }

  void Serializer::writeDouble(double data) {
    static_assert(sizeof(uint64_t) == sizeof(double), "double should be 64 bits");
    uint64_t u64;
    std::memcpy(&u64, &data, sizeof(double));
    writeBigEndian64(u64);
  }

  void Serializer::writeString(const char *data, std::size_t size) {
    writeSizeHeader(size);

    if (size > 0) {
      m_stream->write(gf::span(reinterpret_cast<const uint8_t *>(data), size));
    }
  }

  namespace {

    template<typename I, std::size_t N>
    void writeInteger(I data, uint8_t (&buf)[N]) {
      for (std::size_t i = 0; i < N; ++i) {
        buf[N - i - 1] = static_cast<uint8_t>(data >> (8 * i));
      }
    }

  }

  void Serializer::writeSizeHeader(std::size_t size) {
    if (size < 0xFF) {
      uint8_t buf[1];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    if (size < 0xFFFF) {
      size -= 0xFF;
      assert(size < 0xFF00);
      uint8_t buf[2];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    if (size < 0xFFFFFF) {
      size -= 0xFFFF;
      assert(size < 0xFFFF00);
      uint8_t buf[3];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    if (size < 0xFFFFFFFF) {
      size -= 0xFFFFFF;
      assert(size < 0xFFFFFF00);
      uint8_t buf[4];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    if (size < 0xFFFFFFFFFF) {
      size -= 0xFFFFFFFF;
      assert(size < 0xFFFFFFFF00);
      uint8_t buf[5];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    if (size < 0xFFFFFFFFFFFF) {
      size -= 0xFFFFFFFFFF;
      assert(size < 0xFFFFFFFFFF00);
      uint8_t buf[6];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    if (size < 0xFFFFFFFFFFFFFF) {
      size -= 0xFFFFFFFFFFFF;
      assert(size < 0xFFFFFFFFFFFF00);
      uint8_t buf[7];
      writeInteger(size, buf);
      assert(buf[0] != 0xFF);
      m_stream->write(buf);
      return;
    }

    writeUnsigned8(0xFF);

    size -= 0xFFFFFFFFFFFFFF;
    uint8_t buf[8];
    writeInteger(size, buf);
    m_stream->write(buf);
  }


  void Serializer::writeBigEndian64(uint64_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    writeInteger(data, buf);
    m_stream->write(buf);
  }

  void Serializer::writeBigEndian32(uint32_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    writeInteger(data, buf);
    m_stream->write(buf);
  }

  void Serializer::writeBigEndian16(uint16_t data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    writeInteger(data, buf);
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
    uint32_t u32;

    if (!readBigEndian32(u32)) {
      Log::error("Asking for float but the file is at the end.\n");
      return false;
    }

    std::memcpy(&data, &u32, sizeof(float));
    return true;
  }

  bool Deserializer::readDouble(double& data) {
    uint64_t u64;

    if (!readBigEndian64(u64)) {
      Log::error("Asking for double but the file is at the end.\n");
      return false;
    }

    std::memcpy(&data, &u64, sizeof(double));
    return true;
  }

  bool Deserializer::readString(char *data, std::size_t size) {
    if (m_stream->read(Span<uint8_t>(reinterpret_cast<uint8_t*>(data), size)) != size) {
      Log::error("Asking for a string but the file is at the end.\n");
      return false;
    }

    return true;
  }

  namespace {

    template<typename I, std::size_t N>
    I readInteger(uint8_t (&buf)[N]) {
      I result = 0;

      for (std::size_t i = 0; i < N; ++i) {
        result = (result << 8) + buf[i];
      }

      return result;
    }

  }

  bool Deserializer::readSizeHeader(std::size_t& size) {
    static constexpr std::size_t Size = sizeof(std::size_t);

    uint8_t data;
    std::size_t n = 0;

    for (;;) {
      if (!readUnsigned8(data)) {
        return false;
      }

      if (data != 0xFF || n == Size - 1) {
        break;
      }

      ++n;
    }

    if (n == 0) {
      // early exit
      size = data;
      return true;
    }

    assert(n < Size);
    uint8_t buf[Size];
    std::size_t m = Size - n - 1;

    for (std::size_t i = 0; i < m; ++i) {
      buf[i] = 0x00;
    }

    buf[m] = data;

    for (std::size_t i = m + 1; i < Size; ++i) {
      if (!readUnsigned8(buf[i])) {
        return false;
      }
    }

    size = readInteger<std::size_t>(buf);

    switch (n) {
      case 1: size += 0xFF; break;
      case 2: size += 0xFFFF; break;
      case 3: size += 0xFFFFFF; break;
      case 4: size += 0xFFFFFFFF; break;
      case 5: size += 0xFFFFFFFFFF; break;
      case 6: size += 0xFFFFFFFFFFFF; break;
      case 7: size += 0xFFFFFFFFFFFFFF; break;
      case 8: size += 0xFFFFFFFFFFFFFFFF; break;
      default:
        assert(false);
        break;
    }

    return true;
  }

  bool Deserializer::readBigEndian64(uint64_t& data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
      return false;
    }

    data = readInteger<uint64_t>(buf);
    return true;
  }

  bool Deserializer::readBigEndian32(uint32_t& data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
      return false;
    }

    data = readInteger<uint32_t>(buf);
    return true;
  }

  bool Deserializer::readBigEndian16(uint16_t& data) {
    static constexpr std::size_t Size = sizeof(data);

    uint8_t buf[Size];
    if (m_stream->read(buf) != Size) { // Flawfinder: ignore
      return false;
    }

    data = readInteger<uint16_t>(buf);
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
