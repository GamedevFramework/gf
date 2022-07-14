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
#include <gf/Streams.h>

#include <algorithm>
#include <stdexcept>
#include <string>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /*
   * FileInputStream
   */

  FileInputStream::FileInputStream(const Path& path)
  : m_file(nullptr)
  {
    if (!std::filesystem::is_regular_file(path)) {
      Log::error("Could not find the following file for streaming: %s\n", path.string().c_str()); // throw?
      return;
    }

    m_file = std::fopen(path.string().c_str(), "rb");

    if (m_file == nullptr) {
      Log::error("Could not open the following file for streaming: '%s'\n", path.string().c_str()); // throw?
    }
  }

  FileInputStream::~FileInputStream() {
    if (m_file != nullptr) {
      std::fclose(m_file);
    }
  }

  std::size_t FileInputStream::read(Span<uint8_t> buffer) {
    if (m_file == nullptr) {
      return 0;
    }

    return std::fread(buffer.getData(), sizeof(uint8_t), buffer.getSize(), m_file);
  }

  void FileInputStream::seek(std::ptrdiff_t position) {
    if (m_file == nullptr) {
      return;
    }

    std::fseek(m_file, static_cast<long>(position), SEEK_SET);
  }

  void FileInputStream::skip(std::ptrdiff_t position) {
    if (m_file == nullptr) {
      return;
    }

    std::fseek(m_file, static_cast<long>(position), SEEK_CUR);
  }

  bool FileInputStream::isFinished() {
    if (m_file == nullptr) {
      return true;
    }

    return std::feof(m_file) != 0;
  }

  /*
   * MemoryInputStream
   */

  MemoryInputStream::MemoryInputStream(Span<const uint8_t> memory)
  : m_memory(memory)
  , m_offset(0)
  {

  }

  std::size_t MemoryInputStream::read(Span<uint8_t> buffer) {
    if (buffer.getSize() == 0) {
      return 0;
    }

    std::size_t count = buffer.getSize();
    std::size_t available = m_memory.getSize() - m_offset;

    if (count > available) {
      count = available;
    }

    if (count > 0) {
      std::copy_n(m_memory.getData() + m_offset, count, buffer.getData());
      m_offset += count;
    }

    return count;
  }

  void MemoryInputStream::seek(std::ptrdiff_t position) {
    if (position < 0) {
      return;
    }

    std::size_t offset = static_cast<std::size_t>(position);
    m_offset = std::min(offset, m_memory.getSize());
  }

  void MemoryInputStream::skip(std::ptrdiff_t position) {
    if (position < 0) {
      std::size_t offset = static_cast<std::size_t>(-position);
      m_offset = offset < m_offset ? m_offset - offset : 0;
    } else {
      std::size_t offset = static_cast<std::size_t>(position);
      m_offset = std::min(m_offset + offset, m_memory.getSize());
    }
  }

  bool MemoryInputStream::isFinished() {
    return m_offset == m_memory.getSize();
  }

  /*
   * CompressedInputStream
   */

  CompressedInputStream::CompressedInputStream(InputStream& compressed)
  : m_compressed(&compressed)
  , m_start(0)
  , m_stop(0)
  , m_eof(false)
  {
    m_stream.zalloc = nullptr;
    m_stream.zfree = nullptr;
    [[maybe_unused]] int err = inflateInit(&m_stream);
    assert(err == Z_OK); // throw?
  }

  CompressedInputStream::~CompressedInputStream() {
    [[maybe_unused]] int err = inflateEnd(&m_stream);
    assert(err == Z_OK);
  }

  std::size_t CompressedInputStream::read(Span<uint8_t> buffer) {
    m_stream.next_out = buffer.getData();
    m_stream.avail_out = static_cast<uInt>(buffer.getSize());

    do {
      if (m_start == m_stop) {
        m_start = 0;
        m_stop = static_cast<uInt>(m_compressed->read(m_buffer)); // Flawfinder: ignore
      }

      uInt remaining = m_stop - m_start;

      m_stream.next_in = m_buffer + m_start;
      m_stream.avail_in = remaining;
      int err = inflate(&m_stream, Z_NO_FLUSH);

      if (err != Z_OK && err != Z_STREAM_END && err != Z_BUF_ERROR) {
        Log::debug("Error while calling inflate: %d '%s'\n", err, m_stream.msg);
      }

      if (err == Z_STREAM_END) {
        m_eof = true;
        return buffer.getSize() - m_stream.avail_out;
      }

      assert(err == Z_OK || err == Z_STREAM_END || err == Z_BUF_ERROR);
      m_start += remaining - m_stream.avail_in;
    } while (m_stream.avail_out > 0);

    return buffer.getSize();
  }

  void CompressedInputStream::seek([[maybe_unused]] std::ptrdiff_t position) {
    throw std::runtime_error("Not supported");
  }

  void CompressedInputStream::skip([[maybe_unused]] std::ptrdiff_t position) {
    throw std::runtime_error("Not supported");
  }

  bool CompressedInputStream::isFinished() {
    return m_eof;
  }


  /*
   * BufferInputStream
   */

  BufferInputStream::BufferInputStream(std::vector<uint8_t> *bytes)
  : m_bytes(bytes)
  , m_offset(0)
  {
    assert(bytes != nullptr);
  }

  std::size_t BufferInputStream::read(Span<uint8_t> buffer) {
    if (buffer.getSize() == 0) {
      return 0;
    }

    std::size_t count = buffer.getSize();
    std::size_t available = m_bytes->size() - m_offset;

    if (count > available) {
      count = available;
    }

    if (count > 0) {
      std::copy_n(m_bytes->data() + m_offset, count, buffer.getData());
      m_offset += count;
    }

    return count;
  }

  void BufferInputStream::seek(std::ptrdiff_t position) {
    if (position < 0) {
      return;
    }

    std::size_t offset = static_cast<std::size_t>(position);
    m_offset = std::min(offset, m_bytes->size());
  }

  void BufferInputStream::skip(std::ptrdiff_t position) {
    if (position < 0) {
      std::size_t offset = static_cast<std::size_t>(-position);
      m_offset = offset < m_offset ? m_offset - offset : 0;
    } else {
      std::size_t offset = static_cast<std::size_t>(position);
      m_offset = std::min(m_offset + offset, m_bytes->size());
    }
  }

  bool BufferInputStream::isFinished() {
    return m_offset == m_bytes->size();
  }

  /*
   * FileOutputStream
   */

  namespace {
    const char *getModeString(FileOutputStream::Mode mode) {
      switch (mode) {
        case FileOutputStream::Mode::Write:
          return "wb";
        case FileOutputStream::Mode::Append:
          return "ab";
      }

      assert(false);
      return "";
    }
  }

  FileOutputStream::FileOutputStream(const Path& path, Mode mode)
  : m_file(nullptr)
  , m_written(0)
  {
    m_file = std::fopen(path.string().c_str(), getModeString(mode));

    if (m_file == nullptr) {
      Log::error("Could not open the following file for streaming: '%s'\n", path.string().c_str()); // throw?
    }
  }

  FileOutputStream::~FileOutputStream() {
    if (m_file != nullptr) {
      std::fclose(m_file);
    }
  }

  std::size_t FileOutputStream::write(Span<const uint8_t> buffer) {
    if (m_file == nullptr) {
      return 0;
    }

    if (buffer.getSize() == 0) {
      return 0;
    }

    m_written += buffer.getSize();

    return std::fwrite(buffer.getData(), sizeof(uint8_t), buffer.getSize(), m_file);
  }

  std::size_t FileOutputStream::getWrittenBytesCount() const {
    return m_written;
  }

  /*
   * MemoryOutputStream
   */

  MemoryOutputStream::MemoryOutputStream(Span<uint8_t> memory)
  : m_memory(memory)
  , m_offset(0)
  {

  }

  std::size_t MemoryOutputStream::write(Span<const uint8_t> buffer) {
    std::size_t remaining = m_memory.getSize() - m_offset;
    std::size_t size = std::min(remaining, buffer.getSize());
    std::copy_n(buffer.getData(), size, m_memory.getData() + m_offset);
    m_offset += size;
    return size;
  }

  std::size_t MemoryOutputStream::getWrittenBytesCount() const {
    return m_offset;
  }

  /*
   * CompressedOutputStream
   */

  CompressedOutputStream::CompressedOutputStream(OutputStream& compressed)
  : m_compressed(&compressed)
  {
    m_stream.zalloc = nullptr;
    m_stream.zfree = nullptr;
    [[maybe_unused]] int err = deflateInit(&m_stream, Z_DEFAULT_COMPRESSION);
    assert(err == Z_OK); // throw?
  }

  CompressedOutputStream::~CompressedOutputStream() {
    m_stream.next_in = nullptr;
    m_stream.avail_in = 0;
    m_stream.next_out = m_buffer;
    m_stream.avail_out = BufferSize;

    int err;

    do {
      err = deflate(&m_stream, Z_FINISH);
      assert(err == Z_OK || err == Z_STREAM_END);
      uInt written = BufferSize - m_stream.avail_out;

      if (written > 0) {
        [[maybe_unused]] std::size_t flushed = m_compressed->write(gf::span(m_buffer, written));
        assert(flushed == written);

        m_stream.next_out = m_buffer;
        m_stream.avail_out = BufferSize;
      }
    } while (err != Z_STREAM_END);

    err = deflateEnd(&m_stream);
    assert(err == Z_OK);
  }

  std::size_t CompressedOutputStream::write(Span<const uint8_t> buffer) {
    m_stream.next_in = buffer.getData();
    m_stream.avail_in = static_cast<uInt>(buffer.getSize());
    m_stream.next_out = m_buffer;
    m_stream.avail_out = BufferSize;

    do {
      [[maybe_unused]] int err = deflate(&m_stream, Z_NO_FLUSH);
      assert(err == Z_OK);
      uInt written = BufferSize - m_stream.avail_out;

      if (written > 0) {
        [[maybe_unused]] std::size_t flushed = m_compressed->write(gf::span(m_buffer, written));
        assert(flushed == written);

        m_stream.next_out = m_buffer;
        m_stream.avail_out = BufferSize;
      } else {
        assert(m_stream.avail_in == 0);
      }
    } while (m_stream.avail_in > 0);

    return buffer.getSize();
  }

  std::size_t CompressedOutputStream::getWrittenBytesCount() const {
    return m_compressed->getWrittenBytesCount();
  }

  /*
   * BufferOutputStream
   */

  BufferOutputStream::BufferOutputStream(std::vector<uint8_t> *bytes)
  : m_bytes(bytes)
  {
    assert(bytes != nullptr);
  }

  std::size_t BufferOutputStream::write(Span<const uint8_t> buffer) {
    std::copy_n(buffer.getData(), buffer.getSize(), std::back_inserter(*m_bytes));
    return buffer.getSize();
  }

  std::size_t BufferOutputStream::getWrittenBytesCount() const {
    return m_bytes->size();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
