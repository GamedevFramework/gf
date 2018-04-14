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
#ifndef GF_SERIALIZATION_H
#define GF_SERIALIZATION_H

#include <cstddef>
#include <cstdint>

#include "BinaryFile.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct DataObject;

  /**
   * @ingroup game
   * @brief A serializer to a binary file
   *
   * The format used to serialize is [MessagePack](https://msgpack.org/).
   *
   * You should not use the methods in this class. Prefer using and overloading operator `|`.
   *
   * @sa gf::Deserializer
   */
  class GF_API Serializer {
  public:
    /**
     * @brief Constructor
     *
     * @param filename The name of the binary file
     */
    Serializer(const Path& filename);

    /**
     * @brief Write a null object
     */
    void writeNil();

    /**
     * @brief Write a boolean object
     */
    void writeBoolean(bool data);

    /**
     * @brief Write a signed integer object
     */
    void writeSigned(int64_t data);

    /**
     * @brief Write an unsigned object
     */
    void writeUnsigned(uint64_t data);

    /**
     * @brief Write a single precison float object
     */
    void writeFloat(float data);

    /**
     * @brief Write a double precison float object
     */
    void writeDouble(double data);

    /**
     * @brief Write a string object
     */
    void writeString(const char *data, uint32_t size);

    /**
     * @brief Write a binary object
     */
    void writeBinary(const uint8_t *data, uint32_t size);

    /**
     * @brief Write an array header
     */
    void writeArrayHeader(uint32_t size);

    /**
     * @brief Write a map header
     */
    void writeMapHeader(uint32_t size);

    /**
     * @brief Write an extension object
     */
    void writeExtension(int8_t type, const uint8_t *data, uint32_t size);

  private:
    void writeBigEndian64(uint64_t data);
    void writeBigEndian32(uint32_t data);
    void writeBigEndian16(uint16_t data);

  private:
    BinaryFile m_file;
  };

  /**
   * @ingroup game
   * @brief A deserializer from a binary file
   *
   * The format used to deserialize is [MessagePack](https://msgpack.org/).
   *
   * You should not use the methods in this class. Prefer using and overloading operator `|`.
   *
   * @sa gf::Serializer
   */
  class GF_API Deserializer {
  public:
    /**
     * @brief Constructor
     *
     * @param filename The name of the binary file
     */
    Deserializer(const Path& filename);

    /**
     * @brief Read a null object
     *
     * @returns True if there was no error while reading
     */
    bool readNil();

    /**
     * @brief Read a boolean object
     *
     * @returns True if there was no error while reading
     */
    bool readBoolean(bool& data);

    /**
     * @brief Read a signed integer object
     *
     * @returns True if there was no error while reading
     */
    bool readSigned(int64_t& data);

    /**
     * @brief Read an unsigned integer object
     *
     * @returns True if there was no error while reading
     */
    bool readUnsigned(uint64_t& data);

    /**
     * @brief Read a single precison float object
     *
     * @returns True if there was no error while reading
     */
    bool readFloat(float& data);

    /**
     * @brief Read a double precison float object
     *
     * @returns True if there was no error while reading
     */
    bool readDouble(double& data);

    /**
     * @brief Read a string header
     *
     * @returns True if there was no error while reading
     */
    bool readStringHeader(uint32_t& size);

    /**
     * @brief Read a string body
     *
     * @returns True if there was no error while reading
     */
    bool readString(char *data, uint32_t size);

    /**
     * @brief Read a binary header
     *
     * @returns True if there was no error while reading
     */
    bool readBinaryHeader(uint32_t& size);

    /**
     * @brief Read a binary body
     *
     * @returns True if there was no error while reading
     */
    bool readBinary(uint8_t *data, uint32_t size);

    /**
     * @brief Read an array header
     *
     * @returns True if there was no error while reading
     */
    bool readArrayHeader(uint32_t& size);

    /**
     * @brief Read a map header
     *
     * @returns True if there was no error while reading
     */
    bool readMapHeader(uint32_t& size);

    /**
     * @brief Read an extension header
     *
     * @returns True if there was no error while reading
     */
    bool readExtensionHeader(int8_t& type, uint32_t& size);

    /**
     * @brief Read an extension body
     *
     * @returns True if there was no error while reading
     */
    bool readExtension(uint8_t *data, uint32_t size);


    /**
     * @brief Read a data object
     *
     * @returns True if there was no error while reading
     */
    bool readDataObject(DataObject& object);

  private:
    bool readRawInt8(uint64_t& raw);
    bool readRawInt16(uint64_t& raw);
    bool readRawInt32(uint64_t& raw);
    bool readRawInt64(uint64_t& raw);

    bool readSize8(uint32_t& size);
    bool readSize16(uint32_t& size);
    bool readSize32(uint32_t& size);

    bool readDataObjectMap(DataObject& object);
    bool readDataObjectArray(DataObject& object);
    bool readDataObjectString(DataObject& object);
    bool readDataObjectBinary(DataObject& object);
    bool readDataObjectExtension(DataObject& object);

  private:
    bool readBigEndian64(uint64_t& data);
    bool readBigEndian32(uint32_t& data);
    bool readBigEndian16(uint16_t& data);

    bool isEof() const;
    uint8_t getByte() const;
    void nextByte();

  private:
    BinaryFile m_file;
    uint8_t m_next;
    bool m_eof;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SERIALIZATION_H
