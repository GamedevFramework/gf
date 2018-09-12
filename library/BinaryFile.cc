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
#include <gf/BinaryFile.h>

#include <cassert>

#include <gf/Log.h>
#include <gf/Unused.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {
    const char *getModeString(BinaryFile::Mode mode) {
      switch (mode) {
        case BinaryFile::Mode::Read:
          return "rb";
        case BinaryFile::Mode::Write:
          return "wb";
        case BinaryFile::Mode::Append:
          return "ab";
      }

      assert(false);
      return "";
    }

  }

  BinaryFile::BinaryFile(const Path& filename, Mode mode, BinaryFormat format)
  : m_file(std::fopen(filename.string().c_str(), getModeString(mode)))
  , m_mode(mode)
  , m_format(format)
  , m_start(0)
  , m_stop(0)
  , m_eof(false)
  {
    if (m_file == nullptr) {
      Log::error("Could not open file '%s'\n", filename.string().c_str()); // throw?
      return;
    }

    if (m_format == BinaryFormat::Compressed) {
      m_stream.zalloc = nullptr;
      m_stream.zfree = nullptr;

      switch (m_mode) {
        case Mode::Read: {
          int err = inflateInit(&m_stream);
          assert(err == Z_OK); // throw?
          gf::unused(err);
          break;
        }
        case Mode::Write:
        case Mode::Append: {
          int err = deflateInit(&m_stream, Z_DEFAULT_COMPRESSION);
          assert(err == Z_OK); // throw?
          gf::unused(err);
          break;
        }
      }

    }
  }

  BinaryFile::BinaryFile(BinaryFile&& other)
  : m_file(std::exchange(other.m_file, nullptr))
  , m_mode(other.m_mode)
  , m_format(other.m_format)
  , m_stream(other.m_stream)
  , m_start(other.m_start)
  , m_stop(other.m_stop)
  , m_eof(other.m_eof)
  {
    std::memcpy(m_buffer, other.m_buffer, BufferSize);
  }

  BinaryFile& BinaryFile::operator=(BinaryFile&& other) {
    std::swap(m_file, other.m_file);
    std::swap(m_mode, other.m_mode);
    std::swap(m_format, other.m_format);
    std::swap(m_stream, other.m_stream);
    std::swap(m_buffer, other.m_buffer);
    std::swap(m_start, other.m_start);
    std::swap(m_stop, other.m_stop);
    std::swap(m_eof, other.m_eof);
    return *this;
  }

  BinaryFile::~BinaryFile() {
    if (m_file != nullptr) {
      if (m_format == BinaryFormat::Compressed) {
        switch (m_mode) {
          case Mode::Read: {
            int err = inflateEnd(&m_stream);
            assert(err == Z_OK);
            break;
          }
          case Mode::Write:
          case Mode::Append: {
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
                std::size_t flushed = std::fwrite(m_buffer, sizeof(Bytef), written, m_file);
                assert(flushed == written);

                m_stream.next_out = m_buffer;
                m_stream.avail_out = BufferSize;
              }
            } while (err != Z_STREAM_END);

            err = deflateEnd(&m_stream);
            assert(err == Z_OK);
            break;
          }

        }
      }

      std::fclose(m_file);
    }
  }

  std::size_t BinaryFile::write(ArrayRef<uint8_t> buffer) {
    assert(m_file && (m_mode == Mode::Write || m_mode == Mode::Append));

    if (m_format == BinaryFormat::Plain) {
      return std::fwrite(buffer.getData(), sizeof(uint8_t), buffer.getSize(), m_file);
    }

    m_stream.next_out = m_buffer;
    m_stream.avail_out = BufferSize;
    m_stream.next_in = buffer.getData();
    m_stream.avail_in = buffer.getSize();

    do {
      int err = deflate(&m_stream, Z_NO_FLUSH);
      assert(err == Z_OK);
      uInt written = BufferSize - m_stream.avail_out;

      if (written > 0) {
        std::size_t flushed = std::fwrite(m_buffer, sizeof(Bytef), written, m_file);
        assert(flushed == written);

        m_stream.next_out = m_buffer;
        m_stream.avail_out = BufferSize;
      } else {
        assert(m_stream.avail_in == 0);
      }
    } while (m_stream.avail_in > 0);

    return buffer.getSize();
  }

  std::size_t BinaryFile::write(uint8_t byte) {
    return write(ArrayRef<uint8_t>(&byte, 1));
  }

  std::size_t BinaryFile::read(BufferRef<uint8_t> buffer) {
    assert(m_file && m_mode == Mode::Read);

    if (m_format == BinaryFormat::Plain) {
      return std::fread(buffer.getData(), sizeof(uint8_t), buffer.getSize(), m_file);
    }

    m_stream.next_out = buffer.getData();
    m_stream.avail_out = buffer.getSize();

    do {
      if (m_start == m_stop) {
        m_start = 0;
        m_stop = std::fread(m_buffer, sizeof(uint8_t), BufferSize, m_file);
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

  std::size_t BinaryFile::read(uint8_t& byte) {
    return read(BufferRef<uint8_t>(&byte, 1));
  }

  bool BinaryFile::isEof() {
    assert(m_file);

    if (m_format == BinaryFormat::Plain) {
      return std::feof(m_file);
    }

    return m_eof;
  }

  void BinaryFile::close() {
    assert(m_file);
    std::fclose(m_file);
    m_file = nullptr;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
