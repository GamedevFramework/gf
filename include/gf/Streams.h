/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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

#include "Stream.h"
#include "Path.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief File based input stream
   */
  class GF_API FileInputStream : public InputStream {
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

    virtual std::size_t read(BufferRef<uint8_t> buffer) override;
    virtual void seek(std::ptrdiff_t position) override;
    virtual void skip(std::ptrdiff_t position) override;
    virtual bool isFinished() override;

  private:
    std::FILE *m_file;
  };


  /**
   * @ingroup core
   * @brief Memory based input stream
   */
  class GF_API MemoryInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * This class does not own the memory. The user must keep the memory
     * available when the stream is active.
     *
     * @param memory The source memory
     */
    explicit MemoryInputStream(ArrayRef<uint8_t> memory);

    virtual std::size_t read(BufferRef<uint8_t> buffer) override;
    virtual void seek(std::ptrdiff_t position) override;
    virtual void skip(std::ptrdiff_t position) override;
    virtual bool isFinished() override;

  private:
    ArrayRef<uint8_t> m_memory;
    std::size_t m_offset;
  };


  /**
   * @ingroup core
   * @brief Compressed input stream
   */
  class GF_API CompressedInputStream : public InputStream {
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

    virtual std::size_t read(BufferRef<uint8_t> buffer) override;
    virtual void seek(std::ptrdiff_t position) override;
    virtual void skip(std::ptrdiff_t position) override;
    virtual bool isFinished() override;

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
   * @ingroup core
   * @brief File based output stream
   */
  class GF_API FileOutputStream : public OutputStream {
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

    virtual std::size_t write(ArrayRef<uint8_t> buffer) override;

    virtual std::size_t getWrittenBytesCount() const override;

  private:
    std::FILE *m_file;
    std::size_t m_written;
  };

  /**
   * @ingroup core
   * @brief Memory based output stream
   */
  class GF_API MemoryOutputStream : public OutputStream {
  public:
    /**
     * @brief Constructor
     *
     * This class does not own the memory. The user must keep the memory
     * available when the stream is active.
     *
     * @param memory The destination buffer
     */
    explicit MemoryOutputStream(BufferRef<uint8_t> memory);

    virtual std::size_t write(ArrayRef<uint8_t> buffer) override;

    virtual std::size_t getWrittenBytesCount() const override;
  private:
    BufferRef<uint8_t> m_memory;
    std::size_t m_offset;
  };


  /**
   * @ingroup core
   * @brief Compressed output stream
   */
  class GF_API CompressedOutputStream : public OutputStream {
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

    virtual std::size_t write(ArrayRef<uint8_t> buffer) override;

    virtual std::size_t getWrittenBytesCount() const override;
  private:
    static constexpr uInt BufferSize = 256;

    OutputStream *m_compressed;
    z_stream m_stream;
    Bytef m_buffer[BufferSize];
  };



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STREAMS_H
