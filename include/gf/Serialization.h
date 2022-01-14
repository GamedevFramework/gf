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
#ifndef GF_SERIALIZATION_H
#define GF_SERIALIZATION_H

#include <cstddef>
#include <cstdint>

#include "CoreApi.h"
#include "Stream.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_serialization
   * @brief A serializer to a binary file
   *
   * You should not use the methods in this class. Prefer using and overloading operator `|`.
   *
   * @sa gf::Deserializer
   */
  class GF_CORE_API Serializer {
  public:

    /**
     * @brief Constructor
     *
     * @param stream The output stream
     * @param version The version of the archive format
     */
    Serializer(OutputStream& stream, uint16_t version = 0);

    /**
     * @brief Get the version of the current archive format
     */
    uint16_t getVersion() const {
      return m_version;
    }

    /**
     * @brief Write a boolean object
     */
    void writeBoolean(bool data);

    /**
     * @brief Write a char
     */
    void writeChar(char data);

    /**
     * @brief Write a signed integer object
     */
    void writeSigned8(int8_t data);

    /**
     * @brief Write a signed integer object
     */
    void writeSigned16(int16_t data);

    /**
     * @brief Write a signed integer object
     */
    void writeSigned32(int32_t data);

    /**
     * @brief Write a signed integer object
     */
    void writeSigned64(int64_t data);

    /**
     * @brief Write an unsigned object
     */
    void writeUnsigned8(uint8_t data);

    /**
     * @brief Write an unsigned object
     */
    void writeUnsigned16(uint16_t data);

    /**
     * @brief Write an unsigned object
     */
    void writeUnsigned32(uint32_t data);

    /**
     * @brief Write an unsigned object
     */
    void writeUnsigned64(uint64_t data);

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
    void writeString(const char *data, std::size_t size);

    /**
     * @brief Write a size header
     */
    void writeSizeHeader(std::size_t size);

  private:
    void writeBigEndian64(uint64_t data);
    void writeBigEndian32(uint32_t data);
    void writeBigEndian16(uint16_t data);
    void writeBigEndian8(uint8_t data);

  private:
    OutputStream *m_stream;
    uint16_t m_version;
  };


  /**
   * @ingroup core_serialization
   * @brief A deserializer from a binary file
   *
   * You should not use the methods in this class. Prefer using and overloading operator `|`.
   *
   * @sa gf::Serializer
   */
  class GF_CORE_API Deserializer {
  public:
    /**
     * @brief Constructor
     *
     * @param stream The input stream
     */
    Deserializer(InputStream& stream);

    /**
     * @brief Get the version of the current archive format
     */
    uint16_t getVersion() const {
      return m_version;
    }

    /**
     * @brief Read a boolean object
     *
     * @returns True if there was no error while reading
     */
    bool readBoolean(bool& data);

    /**
     * @brief Read a char
     *
     * @returns True if there was no error while reading
     */
    bool readChar(char& data);

    /**
     * @brief Read a signed integer object
     *
     * @returns True if there was no error while reading
     */
    bool readSigned8(int8_t& data);

    /**
     * @brief Read a signed integer object
     *
     * @returns True if there was no error while reading
     */
    bool readSigned16(int16_t& data);

    /**
     * @brief Read a signed integer object
     *
     * @returns True if there was no error while reading
     */
    bool readSigned32(int32_t& data);

    /**
     * @brief Read a signed integer object
     *
     * @returns True if there was no error while reading
     */
    bool readSigned64(int64_t& data);

    /**
     * @brief Read an unsigned integer object
     *
     * @returns True if there was no error while reading
     */
    bool readUnsigned8(uint8_t& data);

    /**
     * @brief Read an unsigned integer object
     *
     * @returns True if there was no error while reading
     */
    bool readUnsigned16(uint16_t& data);

    /**
     * @brief Read an unsigned integer object
     *
     * @returns True if there was no error while reading
     */
    bool readUnsigned32(uint32_t& data);

    /**
     * @brief Read an unsigned integer object
     *
     * @returns True if there was no error while reading
     */
    bool readUnsigned64(uint64_t& data);

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
     * @brief Read a string body
     *
     * @returns True if there was no error while reading
     */
    bool readString(char *data, std::size_t size);

    /**
     * @brief Read a size header
     *
     * @returns True if there was no error while reading
     */
    bool readSizeHeader(std::size_t& size);

  private:
    bool readBigEndian64(uint64_t& data);
    bool readBigEndian32(uint32_t& data);
    bool readBigEndian16(uint16_t& data);
    bool readBigEndian8(uint8_t& data);

    bool isEof() const;

  private:
    InputStream *m_stream;
    uint16_t m_version;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SERIALIZATION_H
