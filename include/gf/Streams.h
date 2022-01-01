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
#ifndef GF_STREAMS_H
#define GF_STREAMS_H

#include <cstddef>
#include <cstdio>

#include <zlib.h>

#include "CoreApi.h"
#include "Stream.h"
#include "Path.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_streams
   * @brief File based input stream
   */
  class GF_CORE_API FileInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * The constructor opens the file. If the file is not opened, an exception
     * is thrown.
     *
     * @param path The path to the file
     */
    explicit FileInputStream(const Path& path);

    /**
     * @brief Destructor
     *
     * The destructor closes the file.
     */
    ~FileInputStream();

    std::size_t read(Span<uint8_t> buffer) override; // Flawfinder: ignore
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool isFinished() override;

  private:
    std::FILE *m_file;
  };


  /**
   * @ingroup core_streams
   * @brief Memory based input stream
   */
  class GF_CORE_API MemoryInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * This class does not own the memory. The user must keep the memory
     * available when the stream is active.
     *
     * @param memory The source memory
     */
    explicit MemoryInputStream(Span<const uint8_t> memory);

    std::size_t read(Span<uint8_t> buffer) override; // Flawfinder: ignore
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool isFinished() override;

  private:
    Span<const uint8_t> m_memory;
    std::size_t m_offset;
  };


  /**
   * @ingroup core_streams
   * @brief Compressed input stream
   */
  class GF_CORE_API CompressedInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * @param compressed The compressed original stream
     */
    explicit CompressedInputStream(InputStream& compressed);

    /**
     * @brief Destructor
     */
    virtual ~CompressedInputStream();

    std::size_t read(Span<uint8_t> buffer) override; // Flawfinder: ignore
    void seek(std::ptrdiff_t position) override;
    void skip(std::ptrdiff_t position) override;
    bool isFinished() override;

  private:
    static constexpr uInt BufferSize = 256;

    InputStream *m_compressed;
    z_stream m_stream;
    uInt m_start;
    uInt m_stop;
    bool m_eof;
    Bytef m_buffer[BufferSize];
  };


  /**
   * @ingroup core_streams
   * @brief Buffer input stream
   */
  class GF_CORE_API BufferInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * @param bytes The supplied external bytes to be read
     */
    BufferInputStream(std::vector<uint8_t> *bytes);

    /**
     * @brief Get the underlying bytes
     */
    std::vector<uint8_t> *getBytes() {
      return m_bytes;
    }

    virtual std::size_t read(Span<uint8_t> buffer) override; // Flawfinder: ignore
    virtual void seek(std::ptrdiff_t position) override;
    virtual void skip(std::ptrdiff_t position) override;
    virtual bool isFinished() override;
  private:
    std::vector<uint8_t> *m_bytes;
    std::size_t m_offset;
  };


  /**
   * @ingroup core_streams
   * @brief File based output stream
   */
  class GF_CORE_API FileOutputStream : public OutputStream {
  public:
    /**
     * @brief Open mode for the file
     */
    enum class Mode {
      Write,  ///< Open the file in write only mode
      Append, ///< Open the file in append only mode
    };

    /**
     * @brief Constructor
     *
     * The constructor opens the file. If the file is not opened, an exception
     * is thrown.
     *
     * @param path The path to the file
     * @param mode The open mode of the file
     */
    FileOutputStream(const Path& path, Mode mode = Mode::Write);

    /**
     * @brief Destructor
     *
     * The destructor closes the file.
     */
    ~FileOutputStream();

    std::size_t write(Span<const uint8_t> buffer) override;

    std::size_t getWrittenBytesCount() const override;

  private:
    std::FILE *m_file;
    std::size_t m_written;
  };

  /**
   * @ingroup core_streams
   * @brief Memory based output stream
   */
  class GF_CORE_API MemoryOutputStream : public OutputStream {
  public:
    /**
     * @brief Constructor
     *
     * This class does not own the memory. The user must keep the memory
     * available when the stream is active.
     *
     * @param memory The destination buffer
     */
    explicit MemoryOutputStream(Span<uint8_t> memory);

    std::size_t write(Span<const uint8_t> buffer) override;

    std::size_t getWrittenBytesCount() const override;
  private:
    Span<uint8_t> m_memory;
    std::size_t m_offset;
  };


  /**
   * @ingroup core_streams
   * @brief Compressed output stream
   */
  class GF_CORE_API CompressedOutputStream : public OutputStream {
  public:
    /**
     * @brief Constructor
     *
     * @param compressed A stream to store the compressed data
     */
    explicit CompressedOutputStream(OutputStream& compressed);

    /**
     * @brief Destructor
     */
    ~CompressedOutputStream();

    std::size_t write(Span<const uint8_t> buffer) override;

    std::size_t getWrittenBytesCount() const override;
  private:
    static constexpr uInt BufferSize = 256;

    OutputStream *m_compressed;
    z_stream m_stream;
    Bytef m_buffer[BufferSize];
  };


  /**
   * @ingroup core_streams
   * @brief Buffer output stream
   */
  class GF_CORE_API BufferOutputStream : public OutputStream {
  public:
    /**
     * @brief Constructor
     *
     * @param bytes The supplied external bytes to be written
     */
    BufferOutputStream(std::vector<uint8_t> *bytes);

    /**
     * @brief Get the underlying bytes
     */
    std::vector<uint8_t> *getBytes() {
      return m_bytes;
    }

    virtual std::size_t write(Span<const uint8_t> buffer) override;
    virtual std::size_t getWrittenBytesCount() const override;

  private:
    std::vector<uint8_t> *m_bytes;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STREAMS_H
