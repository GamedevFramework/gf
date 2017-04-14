/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include <gf/InputStreams.h>

#include <cstring>
#include <string>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  // FileInputStream

  FileInputStream::FileInputStream(const Path& path)
  : m_file(nullptr)
  , m_size(0)
  {
    if (!boost::filesystem::is_regular_file(path)) {
      Log::error("Could not find the following file for streaming: %s\n", path.string().c_str());
      return;
    }

    m_file = std::fopen(path.string().c_str(), "rb");

    if (m_file == nullptr) {
      Log::error("Could not find the following file for streaming: %s\n", path.string().c_str());
    }

    m_size = boost::filesystem::file_size(path);
  }

  FileInputStream::~FileInputStream() {
    if (m_file) {
      std::fclose(m_file);
    }
  }

  std::size_t FileInputStream::read(void* data, std::size_t size) {
    if (m_file == nullptr) {
      return 0;
    }

    return std::fread(data, 1, size, m_file);
  }

  long FileInputStream::seek(long position) {
    if (m_file == nullptr) {
      return 0;
    }

    std::fseek(m_file, position, SEEK_SET);
    return tell();
  }

  long FileInputStream::tell() {
    if (m_file == nullptr) {
      return 0;
    }

    return std::ftell(m_file);
  }

  std::size_t FileInputStream::getSize() {
    return m_size;
  }


  // MemoryInputStream

  MemoryInputStream::MemoryInputStream(const void *data, std::size_t size)
  : m_data(static_cast<const char *>(data))
  , m_size(size)
  , m_offset(0)
  {

  }

  std::size_t MemoryInputStream::read(void* data, std::size_t size) {
    if (m_data == nullptr) {
      return 0;
    }

    std::size_t end = m_offset + size;
    std::size_t count = end <= m_size ? size : m_size - m_offset;

    if (count > 0) {
      std::memcpy(data, m_data + m_offset, count);
      m_offset += count;
    }

    return count;
  }

  long MemoryInputStream::seek(long position) {
    if (m_data == nullptr) {
      return 0;
    }

    long size = static_cast<long>(m_size);
    m_offset = position < size ? position : size;
    return m_offset;
  }

  long MemoryInputStream::tell() {
    return m_offset;
  }

  std::size_t MemoryInputStream::getSize() {
    return m_size;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
