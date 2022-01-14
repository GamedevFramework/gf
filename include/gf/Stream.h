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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_STREAM_H
#define GF_STREAM_H

#include <cstddef>
#include <cstdint>

#include "CoreApi.h"
#include "Span.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_streams
   * @brief Abstract class for custom file input streams
   *
   * This class allows users to define their own stream input sources from
   * which Gamedev Framework can load resources.
   *
   * Gamedev Framework resource classes like gf::Texture and gf::Shader
   * provide constructors that read data from conventional sources. However,
   * if you have data coming from a different source (over a network,
   * embedded, encrypted, compressed, etc) you can derive your own class
   * from gf::InputStream and use the constructor that takes a generic
   * input stream.
   *
   * @sa gf::OutputStream
   */
  class GF_CORE_API InputStream {
  public:
    /**
     * @brief Default constructor
     */
    InputStream() = default;

    /**
     * @brief Deleted copy constructor
     */
    InputStream(const InputStream&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    InputStream& operator=(const InputStream&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~InputStream();

    /**
     * @brief Read data from the stream
     *
     * After reading, the stream's reading position must be
     * advanced by the amount of bytes read.
     *
     * @param buffer The buffer where to copy the read data
     *
     * @return The number of bytes actually read
     */
    virtual std::size_t read(Span<uint8_t> buffer) = 0; // Flawfinder: ignore

    /**
     * @brief Read a single byte from the stream
     *
     * This is a convinience function to read a single byte instead of a
     * whole buffer.
     *
     * @param byte The byte where to copy the read data
     *
     * @return The number of bytes actually read
     */
    std::size_t read(uint8_t& byte) {        // Flawfinder: ignore
      return read(Span<uint8_t>(&byte, 1));  // Flawfinder: ignore
    }

    /**
     * @brief Change the current reading position
     *
     * @param position The position to seek to, from the beginning
     */
    virtual void seek(std::ptrdiff_t position) = 0;

    /**
     * @brief Change the current reading position
     *
     * @param position The position to seek to, from the current position
     */
    virtual void skip(std::ptrdiff_t position) = 0;

    /**
     * @brief Tell if the stream is at the end
     *
     * @return A boolean indicating the end of the stream
     */
    virtual bool isFinished() = 0;

  };


  /**
   * @ingroup core_streams
   * @brief Abstract class for custom file output streams
   *
   * This class allows users to define their own stream output sinks to
   * which Gamedev Framework can save resources.
   *
   * @sa gf::InputStream
   */
  class GF_CORE_API OutputStream {
  public:
    /**
     * @brief Default constructor
     */
    OutputStream() = default;

    /**
     * @brief Deleted copy constructor
     */
    OutputStream(const OutputStream&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    OutputStream& operator=(const OutputStream&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~OutputStream();

    /**
     * @brief Write data to the stream
     *
     * @param buffer The buffer to copy to the stream
     *
     * @return The number of bytes actually written
     */
    virtual std::size_t write(Span<const uint8_t> buffer) = 0;

    /**
     * @brief Write a single byte to the stream
     *
     * @param byte The byte to copy to the stream
     *
     * @return The number of bytes actually written
     */
    std::size_t write(uint8_t byte) {
      return write(Span<const uint8_t>(&byte, 1));
    }

    /**
     * @brief Get the number of bytes already written
     *
     * This function returns the number of bytes actually written i.e. it does
     * not take into account possible bytes that have not been flushed yet.
     *
     * @return The number of written bytes
     */
    virtual std::size_t getWrittenBytesCount() const = 0;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STREAM_H
