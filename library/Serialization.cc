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
#include <memory>

#include <gf/DataObject.h>
#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    // constants from: https://github.com/msgpack/msgpack/blob/master/spec.md
    struct MsgPack {
      // headers
      static constexpr uint8_t Nil      = 0xC0;
      // 0xC1 not used
      static constexpr uint8_t False    = 0xC2;
      static constexpr uint8_t True     = 0xC3;
      static constexpr uint8_t Bin8     = 0xC4;
      static constexpr uint8_t Bin16    = 0xC5;
      static constexpr uint8_t Bin32    = 0xC6;
      static constexpr uint8_t Ext8     = 0xC7;
      static constexpr uint8_t Ext16    = 0xC8;
      static constexpr uint8_t Ext32    = 0xC9;
      static constexpr uint8_t Float32  = 0xCA;
      static constexpr uint8_t Float64  = 0xCB;
      static constexpr uint8_t UInt8    = 0xCC;
      static constexpr uint8_t UInt16   = 0xCD;
      static constexpr uint8_t UInt32   = 0xCE;
      static constexpr uint8_t UInt64   = 0xCF;
      static constexpr uint8_t Int8     = 0xD0;
      static constexpr uint8_t Int16    = 0xD1;
      static constexpr uint8_t Int32    = 0xD2;
      static constexpr uint8_t Int64    = 0xD3;
      static constexpr uint8_t FixExt1  = 0xD4;
      static constexpr uint8_t FixExt2  = 0xD5;
      static constexpr uint8_t FixExt4  = 0xD6;
      static constexpr uint8_t FixExt8  = 0xD7;
      static constexpr uint8_t FixExt16 = 0xD8;
      static constexpr uint8_t Str8     = 0xD9;
      static constexpr uint8_t Str16    = 0xDA;
      static constexpr uint8_t Str32    = 0xDB;
      static constexpr uint8_t Array16  = 0xDC;
      static constexpr uint8_t Array32  = 0xDD;
      static constexpr uint8_t Map16    = 0xDE;
      static constexpr uint8_t Map32    = 0xDF;

      // base
      static constexpr uint8_t FixMap         = 0x80;
      static constexpr uint8_t FixArray       = 0x90;
      static constexpr uint8_t FixStr         = 0xA0;

      // special
      static constexpr uint8_t PositiveFixIntLast   = 0x7F;
      static constexpr uint8_t NegativeFixIntFirst  = 0xE0;

      static constexpr int64_t SignedPositiveFixIntLast     = 127;
      static constexpr int64_t SignedNegativeFixIntFirst    = -32;

      static constexpr uint64_t UnsignedPositiveFixIntLast  = 127;

      //
      static constexpr uint8_t FixMapSizeMax    = 16;
      static constexpr uint8_t FixArraySizeMax  = 16;
      static constexpr uint8_t FixStrSizeMax    = 32;
    };



  }

  /*
   * Deserializer
   */

  Serializer::Serializer(const Path& filename)
  : m_file(filename, BinaryFile::Mode::Write)
  {

  }

  void Serializer::writeNil() {
    m_file.write(MsgPack::Nil);
  }

  void Serializer::writeBoolean(bool data) {
    if (data) {
      m_file.write(MsgPack::True);
    } else {
      m_file.write(MsgPack::False);
    }
  }

  void Serializer::writeSigned(int64_t data) {
    if (data >= 0) {
      if (data <= MsgPack::SignedPositiveFixIntLast) {
        m_file.write(static_cast<int8_t>(data));
      } else if (data <= INT8_MAX) {
        m_file.write(MsgPack::Int8);
        m_file.write(static_cast<int8_t>(data));
      } else if (data <= INT16_MAX) {
        m_file.write(MsgPack::Int16);
        writeBigEndian16(static_cast<int16_t>(data));
      } else if (data <= INT32_MAX) {
        m_file.write(MsgPack::Int32);
        writeBigEndian32(static_cast<int32_t>(data));
      } else {
        m_file.write(MsgPack::Int64);
        writeBigEndian64(data);
      }
    } else { // data < 0
      if (data >= MsgPack::SignedNegativeFixIntFirst) {
        m_file.write(static_cast<uint8_t>(data));
      } else if (data >= INT8_MIN) {
        m_file.write(MsgPack::Int8);
        m_file.write(static_cast<int8_t>(data));
      } else if (data >= INT16_MIN) {
        m_file.write(MsgPack::Int16);
        writeBigEndian16(static_cast<int16_t>(data));
      } else if (data >= INT32_MIN) {
        m_file.write(MsgPack::Int32);
        writeBigEndian32(static_cast<int32_t>(data));
      } else {
        m_file.write(MsgPack::Int64);
        writeBigEndian64(data);
      }
    }
  }

  void Serializer::writeUnsigned(uint64_t data) {
    if (data <= MsgPack::UnsignedPositiveFixIntLast) {
      m_file.write(static_cast<uint8_t>(data));
    } else if (data <= UINT8_MAX) {
      m_file.write(MsgPack::UInt8);
      m_file.write(static_cast<uint8_t>(data));
    } else if (data <= UINT16_MAX) {
      m_file.write(MsgPack::UInt16);
      writeBigEndian16(static_cast<uint16_t>(data));
    } else if (data <= UINT32_MAX) {
      m_file.write(MsgPack::UInt32);
      writeBigEndian32(static_cast<uint32_t>(data));
    } else {
      m_file.write(MsgPack::UInt64);
      writeBigEndian64(data);
    }
  }

  void Serializer::writeFloat(float data) {
    m_file.write(MsgPack::Float32);

    union {
      float f32;
      uint32_t u32;
    };

    f32 = data;
    writeBigEndian32(u32);
  }

  void Serializer::writeDouble(double data) {
    m_file.write(MsgPack::Float64);

    union {
      double f64;
      uint64_t u64;
    };

    f64 = data;
    writeBigEndian64(u64);
  }

  void Serializer::writeString(const char *data, uint32_t size) {
    if (size < MsgPack::FixStrSizeMax) {
      uint8_t header = MsgPack::FixStr | static_cast<uint8_t>(size);
      m_file.write(header);
    } else if (size <= UINT8_MAX) {
      m_file.write(MsgPack::Str8);
      m_file.write(static_cast<uint8_t>(size));
    } else if (size <= UINT16_MAX) {
      m_file.write(MsgPack::Str16);
      writeBigEndian16(static_cast<uint16_t>(size));
    } else {
      m_file.write(MsgPack::Str32);
      writeBigEndian32(size);
    }

    if (size > 0) {
      m_file.write(ArrayRef<uint8_t>(reinterpret_cast<const uint8_t *>(data), size));
    }
  }

  void Serializer::writeBinary(const uint8_t *data, uint32_t size) {
    if (size <= UINT8_MAX) {
      m_file.write(MsgPack::Bin8);
      m_file.write(static_cast<uint8_t>(size));
    } else if (size <= UINT16_MAX) {
      m_file.write(MsgPack::Bin16);
      writeBigEndian16(static_cast<uint16_t>(size));
    } else {
      m_file.write(MsgPack::Bin32);
      writeBigEndian32(size);
    }

    if (size > 0) {
      m_file.write(ArrayRef<uint8_t>(data, size));
    }
  }

  void Serializer::writeArrayHeader(uint32_t size) {
    if (size < MsgPack::FixArraySizeMax) {
      uint8_t header = MsgPack::FixArray | static_cast<uint8_t>(size);
      m_file.write(header);
    } else if (size <= UINT16_MAX) {
      m_file.write(MsgPack::Array16);
      writeBigEndian16(static_cast<uint16_t>(size));
    } else {
      m_file.write(MsgPack::Array32);
      writeBigEndian32(size);
    }
  }

  void Serializer::writeMapHeader(uint32_t size) {
    if (size < MsgPack::FixMapSizeMax) {
      uint8_t header = MsgPack::FixMap | static_cast<uint8_t>(size);
      m_file.write(header);
    } else if (size <= UINT16_MAX) {
      m_file.write(MsgPack::Map16);
      writeBigEndian16(static_cast<uint16_t>(size));
    } else {
      m_file.write(MsgPack::Map32);
      writeBigEndian32(size);
    }
  }

  void Serializer::writeExtension(int8_t type, const uint8_t *data, uint32_t size) {
    if (size == 1) {
      m_file.write(MsgPack::FixExt1);
      m_file.write(static_cast<uint8_t>(type));
    } else if (size == 2) {
      m_file.write(MsgPack::FixExt2);
      m_file.write(static_cast<uint8_t>(type));
    } else if (size == 4) {
      m_file.write(MsgPack::FixExt4);
      m_file.write(static_cast<uint8_t>(type));
    } else if (size == 8) {
      m_file.write(MsgPack::FixExt8);
      m_file.write(static_cast<uint8_t>(type));
    } else if (size == 16) {
      m_file.write(MsgPack::FixExt16);
      m_file.write(static_cast<uint8_t>(type));
    } else if (size <= UINT8_MAX) {
      m_file.write(MsgPack::Ext8);
      m_file.write(static_cast<uint8_t>(size));
      m_file.write(static_cast<uint8_t>(type));
    } else if (size <= UINT16_MAX) {
      m_file.write(MsgPack::Ext16);
      writeBigEndian16(static_cast<uint16_t>(size));
      m_file.write(static_cast<uint8_t>(type));
    } else {
      m_file.write(MsgPack::Ext32);
      writeBigEndian32(size);
      m_file.write(static_cast<uint8_t>(type));
    }

    m_file.write(ArrayRef<uint8_t>(data, size));
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

  /*
   * Deserializer
   */

  Deserializer::Deserializer(const Path& filename)
  : m_file(filename, BinaryFile::Mode::Read)
  , m_eof(false)
  {
    nextByte();
  }

  bool Deserializer::readNil() {
    if (isEof()) {
      Log::error("Asking for nil but the file is at the end.\n");
      return false;
    }

    if (getByte() == MsgPack::Nil) {
      nextByte();
      return true;
    }

    return false;
  }

  bool Deserializer::readBoolean(bool& data) {
    if (isEof()) {
      Log::error("Asking for boolean but the file is at the end.\n");
      return false;
    }

    if (getByte() == MsgPack::False) {
      data = false;
      nextByte();
      return true;
    }

    if (getByte() == MsgPack::True) {
      data = true;
      nextByte();
      return true;
    }

    Log::error("Header does not indicate a boolean.\n");
    return false;
  }

  bool Deserializer::readSigned(int64_t& data) {
    if (isEof()) {
      Log::error("Asking for signed but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();
//     Log::info("Header: 0x%" PRIX8 "\n", header);

    if (header <= MsgPack::PositiveFixIntLast || header >= MsgPack::NegativeFixIntFirst) {
      data = static_cast<int8_t>(header);
//       Log::info("Data: %" PRIi64 "\n", data);
    } else {
      uint64_t raw;

      switch (header) {
        case MsgPack::Int8:
          if (!readRawInt8(raw)) {
            return false;
          }
//           Log::info("Raw(I8): 0x%" PRIx64 " (%" PRIu64 ")\n", raw, raw);

          data = static_cast<int8_t>(raw);
          break;
        case MsgPack::Int16:
          if (!readRawInt16(raw)) {
            return false;
          }
//           Log::info("Raw(I16): 0x%" PRIx64 " (%" PRIu64 ")\n", raw, raw);

          data = static_cast<int16_t>(raw);
          break;
        case MsgPack::Int32:
          if (!readRawInt32(raw)) {
            return false;
          }
//           Log::info("Raw(I32): 0x%" PRIx64 " (%" PRIu64 ")\n", raw, raw);

          data = static_cast<int32_t>(raw);
          break;
        case MsgPack::Int64:
          if (!readRawInt64(raw)) {
            return false;
          }

          data = static_cast<int64_t>(raw);
          break;
        default:
          return false;
      }
    }

    nextByte();
    return true;
  }


  bool Deserializer::readUnsigned(uint64_t& data) {
    if (isEof()) {
      Log::error("Asking for unsigned but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();

    if (header <= MsgPack::PositiveFixIntLast) {
      data = static_cast<int8_t>(header);
    } else {
      switch (header) {
        case MsgPack::UInt8:
          if (!readRawInt8(data)) {
            return false;
          }
          break;
        case MsgPack::UInt16:
          if (!readRawInt16(data)) {
            return false;
          }
          break;
        case MsgPack::UInt32:
          if (!readRawInt32(data)) {
            return false;
          }
          break;
        case MsgPack::UInt64:
          if (!readRawInt64(data)) {
            return false;
          }
          break;
        default:
          return false;
      }
    }

    nextByte();
    return true;
  }

  bool Deserializer::readFloat(float& data) {
    if (isEof()) {
      Log::error("Asking for float but the file is at the end.\n");
      return false;
    }

    if (getByte() == MsgPack::Float32) {

      union {
        float f32;
        uint32_t u32;
      };

      if (!readBigEndian32(u32)) {
        return false;
      }

      data = f32;

      nextByte();
      return true;
    }

    return false;
  }

  bool Deserializer::readDouble(double& data) {
    if (isEof()) {
      Log::error("Asking for double but the file is at the end.\n");
      return false;
    }

    if (getByte() == MsgPack::Float64) {

      union {
        double f64;
        uint64_t u64;
      };

      if (!readBigEndian64(u64)) {
        return false;
      }

      data = f64;

      nextByte();
      return true;
    }

    return false;
  }

  bool Deserializer::readStringHeader(uint32_t& size) {
    if (isEof()) {
      Log::error("Asking for string but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();

    if (MsgPack::FixStr <= header  && header < MsgPack::FixStr + MsgPack::FixStrSizeMax) {
      header &= (MsgPack::FixStrSizeMax - 1);
      size = header;
      return true;
    }

    if (header == MsgPack::Str8) {
      return readSize8(size);
    }

    if (header == MsgPack::Str16) {
      return readSize16(size);
    }

    if (header == MsgPack::Str32) {
      return readSize32(size);
    }

    return false;
  }

  bool Deserializer::readString(char *data, uint32_t size) {
    if (m_file.read(BufferRef<uint8_t>(reinterpret_cast<uint8_t*>(data), size)) != size) {
      m_eof = true;
      return false;
    }

    nextByte();
    return true;
  }

  bool Deserializer::readBinaryHeader(uint32_t& size) {
    if (isEof()) {
      Log::error("Asking for binary but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();

    if (header == MsgPack::Bin8) {
      return readSize8(size);
    }

    if (header == MsgPack::Bin16) {
      return readSize16(size);
    }

    if (header == MsgPack::Bin32) {
      return readSize32(size);
    }

    return false;
  }

  bool Deserializer::readBinary(uint8_t *data, uint32_t size) {
    if (m_file.read(BufferRef<uint8_t>(data, size)) != size) {
      m_eof = true;
      return false;
    }

    nextByte();
    return true;
  }

  bool Deserializer::readArrayHeader(uint32_t& size) {
    if (isEof()) {
      Log::error("Asking for array but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();

    if (MsgPack::FixArray <= header  && header < MsgPack::FixArray + MsgPack::FixArraySizeMax) {
      header &= (MsgPack::FixArraySizeMax - 1);
      size = header;
      nextByte();
      return true;
    }

    if (header == MsgPack::Array16) {
      if (!readSize16(size)) {
        return true;
      }

      nextByte();
      return true;
    }

    if (header == MsgPack::Array32) {
      if (!readSize32(size)) {
        return false;
      }

      nextByte();
      return true;
    }

    return false;
  }

  bool Deserializer::readMapHeader(uint32_t& size) {
    if (isEof()) {
      Log::error("Asking for map but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();

    if (MsgPack::FixMap <= header  && header < MsgPack::FixMap + MsgPack::FixMapSizeMax) {
      header &= (MsgPack::FixMapSizeMax - 1);
      size = header;
      nextByte();
      return true;
    }

    if (header == MsgPack::Map16) {
      if (!readSize16(size)) {
        return false;
      }

      nextByte();
      return true;
    }

    if (header == MsgPack::Map32) {
      if (!readSize32(size)) {
        return false;
      }

      nextByte();
      return true;
    }

    return false;
  }

  bool Deserializer::readExtensionHeader(int8_t& type, uint32_t& size) {
    if (isEof()) {
      Log::error("Asking for extension but the file is at the end.\n");
      return false;
    }

    switch (getByte()) {
      case MsgPack::FixExt1:
        size = 1;
        break;
      case MsgPack::FixExt2:
        size = 2;
        break;
      case MsgPack::FixExt4:
        size = 4;
        break;
      case MsgPack::FixExt8:
        size = 8;
        break;
      case MsgPack::FixExt16:
        size = 16;
        break;
      case MsgPack::Ext8:
        if (!readSize8(size)) {
          return false;
        }
        break;
      case MsgPack::Ext16:
        if (!readSize16(size)) {
          return false;
        }
        break;
      case MsgPack::Ext32:
        if (!readSize32(size)) {
          return false;
        }
        break;
      default:
        return false;
    }

    nextByte();

    if (isEof()) {
      return false;
    }

    type = static_cast<int8_t>(getByte());
    return true;
  }

  bool Deserializer::readExtension(uint8_t *data, uint32_t size) {
    if (m_file.read(BufferRef<uint8_t>(data, size)) != size) {
      m_eof = true;
      return false;
    }

    nextByte();
    return true;
  }

  bool Deserializer::readDataObject(DataObject& object) {
    if (isEof()) {
      Log::error("Asking for data object but the file is at the end.\n");
      return false;
    }

    uint8_t header = getByte();

    if (header <= MsgPack::PositiveFixIntLast) {
      object.type = DataType::Unsigned;
      return readUnsigned(object.u64);
    }

    if (header >= MsgPack::NegativeFixIntFirst) {
      object.type = DataType::Signed;
      return readSigned(object.i64);
    }

    if (header >= MsgPack::FixMap && header < MsgPack::FixMap + MsgPack::FixMapSizeMax) {
      return readDataObjectMap(object);
    }

    if (header >= MsgPack::FixArray && header < MsgPack::FixArray + MsgPack::FixArraySizeMax) {
      return readDataObjectArray(object);
    }

    if (header >= MsgPack::FixStr && header < MsgPack::FixStr + MsgPack::FixStrSizeMax) {
      return readDataObjectString(object);
    }

    switch (header) {
      case MsgPack::Nil:
        object.type = DataType::Nil;
        return readNil();
      case MsgPack::False:
      case MsgPack::True:
        object.type = DataType::Boolean;
        return readBoolean(object.boolean);
      case MsgPack::Bin8:
      case MsgPack::Bin16:
      case MsgPack::Bin32:
        return readDataObjectBinary(object);
      case MsgPack::Ext8:
      case MsgPack::Ext16:
      case MsgPack::Ext32:
      case MsgPack::FixExt1:
      case MsgPack::FixExt2:
      case MsgPack::FixExt4:
      case MsgPack::FixExt8:
      case MsgPack::FixExt16:
        return readDataObjectExtension(object);
      case MsgPack::Float32:
        object.type = DataType::Float;
        return readFloat(object.f32);
      case MsgPack::Float64:
        object.type = DataType::Double;
        return readDouble(object.f64);
      case MsgPack::UInt8:
      case MsgPack::UInt16:
      case MsgPack::UInt32:
      case MsgPack::UInt64:
        object.type = DataType::Unsigned;
        return readUnsigned(object.u64);
      case MsgPack::Int8:
      case MsgPack::Int16:
      case MsgPack::Int32:
      case MsgPack::Int64:
        object.type = DataType::Signed;
        return readSigned(object.i64);
      case MsgPack::Str8:
      case MsgPack::Str16:
      case MsgPack::Str32:
        return readDataObjectString(object);
      case MsgPack::Array16:
      case MsgPack::Array32:
        return readDataObjectArray(object);
      case MsgPack::Map16:
      case MsgPack::Map32:
        return readDataObjectMap(object);
      default:
        return false;
    }

	assert(false);
    return false;
  }




  bool Deserializer::readRawInt8(uint64_t& raw) {
    nextByte();

    if (isEof()) {
      return false;
    }

    raw = getByte();
    return true;
  }

  bool Deserializer::readRawInt16(uint64_t& raw) {
    uint16_t raw16;

    if (!readBigEndian16(raw16)) {
      return false;
    }

    raw = raw16;
    return true;
  }

  bool Deserializer::readRawInt32(uint64_t& raw) {
    uint32_t raw32;

    if (!readBigEndian32(raw32)) {
      return false;
    }

    raw = raw32;
    return true;
  }

  bool Deserializer::readRawInt64(uint64_t& raw) {
    return readBigEndian64(raw);
  }

  bool Deserializer::readSize8(uint32_t& size) {
    nextByte();

    if (isEof()) {
      return false;
    }

    size = getByte();
    return true;
  }

  bool Deserializer::readSize16(uint32_t& size) {
    uint16_t raw;

    if (!readBigEndian16(raw)) {
      return false;
    }

    size = raw;
    return true;
  }

  bool Deserializer::readSize32(uint32_t& size) {
    uint32_t raw;

    if (!readBigEndian32(raw)) {
      return false;
    }

    size = raw;
    return true;
  }

  bool Deserializer::readDataObjectMap(DataObject& object) {
    uint32_t size;

    if (!readMapHeader(size)) {
      return false;
    }

    auto data = std::make_unique<DataKeyValue[]>(size);

    for (uint32_t i = 0; i < size; ++i) {
      if (!readDataObject(data[i].key)) {
        return false;
      }

      if (!readDataObject(data[i].value)) {
        return false;
      }
    }

    object.type = DataType::Map;
    object.map.size = size;
    object.map.data = data.release();
    return true;
  }

  bool Deserializer::readDataObjectArray(DataObject& object) {
    uint32_t size;

    if (!readArrayHeader(size)) {
      return false;
    }

    auto data = std::make_unique<DataObject[]>(size);

    for (uint32_t i = 0; i < size; ++i) {
      if (!readDataObject(data[i])) {
        return false;
      }
    }

    object.type = DataType::Array;
    object.array.size = size;
    object.array.data = data.release();
    return true;
  }

  bool Deserializer::readDataObjectString(DataObject& object) {
    uint32_t size;

    if (!readStringHeader(size)) {
      return false;
    }

    auto data = std::make_unique<char[]>(size + 1);

    if (!readString(data.get(), size)) {
      return false;
    }

    data[size] = '\0';

    object.type = DataType::String;
    object.string.size = size;
    object.string.data = data.release();
    return true;
  }

  bool Deserializer::readDataObjectBinary(DataObject& object) {
    uint32_t size;

    if (!readBinaryHeader(size)) {
      return false;
    }

    auto data = std::make_unique<uint8_t[]>(size);

    if (!readBinary(data.get(), size)) {
      return false;
    }

    object.type = DataType::Binary;
    object.binary.size = size;
    object.binary.data = data.release();
    return true;
  }

  bool Deserializer::readDataObjectExtension(DataObject& object) {
    int8_t type;
    uint32_t size;

    if (!readExtensionHeader(type, size)) {
      return false;
    }

    auto data = std::make_unique<uint8_t[]>(size);

    if (!readExtension(data.get(), size)) {
      return false;
    }

    object.type = DataType::Extension;
    object.extension.size = size;
    object.extension.type = type;
    object.extension.data = data.release();
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

  bool Deserializer::isEof() const {
    return m_eof;
  }

  uint8_t Deserializer::getByte() const {
    return m_next;
  }

  void Deserializer::nextByte() {
    if (!m_file.isEof()) {
      auto size = m_file.read(m_next);
      m_eof = (size != 1);
    } else {
      m_eof = true;
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
