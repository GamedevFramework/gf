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

#include <cinttypes>
#include <memory>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Serializer& operator|(Serializer& ar, bool data) {
    ar.writeBoolean(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, char data) {
    ar.writeChar(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint8_t data) {
    ar.writeUnsigned8(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint16_t data) {
    ar.writeUnsigned16(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint32_t data) {
    ar.writeUnsigned32(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, uint64_t data) {
    ar.writeUnsigned64(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int8_t data) {
    ar.writeSigned8(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int16_t data) {
    ar.writeSigned16(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int32_t data) {
    ar.writeSigned32(data);
    return ar;
  }

  Serializer& operator|(Serializer& ar, int64_t data) {
    ar.writeSigned64(data);
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

  Serializer& operator|(Serializer& ar, const std::string& str) {
    ar.writeString(str.data(), str.length());
    return ar;
  }


  /*
   * Deserializer
   */

  Deserializer& operator|(Deserializer& ar, bool& data) {
    if (!ar.readBoolean(data)) {
      Log::error("Could not read a boolean.\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, char& data) {
    if (!ar.readChar(data)) {
      Log::error("Could not read a char.\n");
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint8_t& data) {
    ar.readUnsigned8(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint16_t& data) {
    ar.readUnsigned16(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint32_t& data) {
    ar.readUnsigned32(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, uint64_t& data) {
    ar.readUnsigned64(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int8_t& data) {
    ar.readSigned8(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int16_t& data) {
    ar.readSigned16(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int32_t& data) {
    ar.readSigned32(data);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, int64_t& data) {
    ar.readSigned64(data);
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

  Deserializer& operator|(Deserializer& ar, std::string& str) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    str.resize(size);

    if (!ar.readString(str.data(), str.size())) {
      return ar;
    }

    return ar;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
