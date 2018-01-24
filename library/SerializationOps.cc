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
#include <gf/SerializationOps.h>

#include <cinttypes>

#include <gf/DataObject.h>
#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Serializer& operator|(Serializer& ar, std::nullptr_t) {
    ar.writeNil();
    return ar;
  }

  Serializer& operator|(Serializer& ar, bool data) {
    ar.writeBoolean(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint8_t data) {
    ar.writeUnsigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint16_t data) {
    ar.writeUnsigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint32_t data) {
    ar.writeUnsigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint64_t data) {
    ar.writeUnsigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int8_t data) {
    ar.writeSigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int16_t data) {
    ar.writeSigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int32_t data) {
    ar.writeSigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int64_t data) {
    ar.writeSigned(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, float data) {
    ar.writeFloat(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, double data) {
    ar.writeDouble(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, const char *str) {
    ar.writeString(str, std::strlen(str));
    return ar;
  }

  Serializer& operator|(Serializer& ar, const std::string& str) {
    ar.writeString(str.data(), str.size());
    return ar;
  }

  Serializer& operator|(Serializer& ar, const std::vector<uint8_t>& bin) {
    ar.writeBinary(bin.data(), static_cast<uint32_t>(bin.size()));
    return ar;
  }

  Serializer& operator|(Serializer& ar, const DataObject& object) {
    switch (object.type) {
      case DataType::Nil:
        ar.writeNil();
        break;

      case DataType::Boolean:
        ar.writeBoolean(object.boolean);
        break;

      case DataType::Signed:
        ar.writeSigned(object.i64);
        break;

      case DataType::Unsigned:
        ar.writeUnsigned(object.u64);
        break;

      case DataType::Float:
        ar.writeFloat(object.f32);
        break;

      case DataType::Double:
        ar.writeDouble(object.f64);
        break;

      case DataType::String:
        ar.writeString(object.string.data, object.string.size);
        break;

      case DataType::Binary:
        ar.writeBinary(object.binary.data, object.binary.size);
        break;

      case DataType::Array:
        ar.writeArrayHeader(object.array.size);

        for (auto& item : object.array) {
          ar | item;
        }

        break;

      case DataType::Map:
        ar.writeMapHeader(object.map.size);

        for (auto& item : object.map) {
          ar | item.key | item.value;
        }

        break;

      case DataType::Extension:
        ar.writeExtension(object.extension.type, object.extension.data, object.extension.size);
        break;
    }

    return ar;
  }

  /*
   * Deserializer
   */

  Deserializer& operator|(Deserializer& ar, std::nullptr_t) {
    ar.readNil();
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, bool& data) {
    if (!ar.readBoolean(data)) {
      Log::error("Could not read a boolean.\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint8_t& data) {
    uint64_t raw;

    if (ar.readUnsigned(raw)) {
      if (raw <= UINT8_MAX) {
        data = static_cast<uint8_t>(raw);
      } else {
        Log::error("Unsigned integer (uint8_t) out of range: %" PRIu64 " (max: %u)\n", raw, UINT8_MAX);
      }
    } else {
      Log::error("Could not read an unsigned integer (uint8_t)...\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint16_t& data) {
    uint64_t raw;

    if (ar.readUnsigned(raw)) {
      if (raw <= UINT16_MAX) {
        data = static_cast<uint16_t>(raw);
      } else {
        Log::error("Unsigned integer (uint16_t) out of range: %" PRIu64 " (max: %u)\n", raw, UINT16_MAX);
      }
    } else {
      Log::error("Could not read an unsigned integer (uint16_t)...\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint32_t& data) {
    uint64_t raw;

    if (ar.readUnsigned(raw)) {
      if (raw <= UINT32_MAX) {
        data = static_cast<uint32_t>(raw);
      } else {
        Log::error("Unsigned integer (uint32_t) out of range: %" PRIu64 " (max: %u)\n", raw, UINT32_MAX);
      }
    } else {
      Log::error("Could not read an unsigned integer (uint32_t)...\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint64_t& data) {
    ar.readUnsigned(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int8_t& data) {
    int64_t raw;

    if (ar.readSigned(raw)) {
      if (INT8_MIN <= raw && raw <= INT8_MAX) {
        data = static_cast<int8_t>(raw);
      } else {
        Log::error("Signed integer (int8_t) out of range: %" PRIi64 " (min: %i, max: %i)\n", raw, INT8_MIN, INT8_MAX);
      }
    } else {
      Log::error("Could not read a signed integer (int8_t)...\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int16_t& data) {
    int64_t raw;

    if (ar.readSigned(raw)) {
      if (INT16_MIN <= raw && raw <= INT16_MAX) {
        data = static_cast<int16_t>(raw);
      } else {
        Log::error("Signed integer (int16_t) out of range: %" PRIi64 " (min: %i, max: %i)\n", raw, INT16_MIN, INT16_MAX);
      }
    } else {
      Log::error("Could not read a signed integer (int16_t)...\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int32_t& data) {
    int64_t raw;

    if (ar.readSigned(raw)) {
      if (INT32_MIN <= raw && raw <= INT32_MAX) {
        data = static_cast<int32_t>(raw);
      } else {
        Log::error("Signed integer (int32_t) out of range: %" PRIi64 " (min: %i, max: %i)\n", raw, INT32_MIN, INT32_MAX);
      }
    } else {
      Log::error("Could not read a signed integer (int32_t)...\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int64_t& data) {
    ar.readSigned(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, float& data) {
    ar.readFloat(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, double& data) {
    ar.readDouble(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, BufferRef<char> str) {
    uint32_t size;

    if (!ar.readStringHeader(size)) {
      return ar;
    }

    if (size + 1 > str.getSize()) {
      return ar;
    }

    if (!ar.readString(str.getData(), size)) {
      return ar;
    }

    str[size] = '\0';
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, std::string& str) {
    uint32_t size;

    if (!ar.readStringHeader(size)) {
      return ar;
    }

    auto data = std::make_unique<char[]>(size);

    if (!ar.readString(data.get(), size)) {
      return ar;
    }

    str.clear();
    str.assign(data.get(), size);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, BufferRef<uint8_t> bin) {
    uint32_t size;

    if (!ar.readBinaryHeader(size)) {
      return ar;
    }

    if (size != bin.getSize()) {
      return ar;
    }

    if (!ar.readBinary(bin.getData(), size)) {
      return ar;
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, std::vector<uint8_t>& bin) {
    uint32_t size;

    if (!ar.readBinaryHeader(size)) {
      return ar;
    }

    auto data = std::make_unique<uint8_t[]>(size);

    if (!ar.readBinary(data.get(), size)) {
      return ar;
    }

    bin.clear();
    bin.assign(data.get(), data.get() + size);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, DataObject& object) {
    ar.readDataObject(object);
    return ar;
  }



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
