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

  BinaryFile::BinaryFile(const Path& filename, Mode mode)
  : m_file(std::fopen(filename.string().c_str(), getModeString(mode)))
  {
    if (m_file == nullptr) {
      Log::error("Could not open file '%s'\n", filename.string().c_str());
    }
  }

  BinaryFile::BinaryFile(BinaryFile&& other)
  : m_file(std::exchange(other.m_file, nullptr))
  {

  }

  BinaryFile& BinaryFile::operator=(BinaryFile&& other) {
    std::swap(m_file, other.m_file);
    return *this;
  }

  BinaryFile::~BinaryFile() {
    if (m_file != nullptr) {
      std::fclose(m_file);
    }
  }

  std::size_t BinaryFile::write(ArrayRef<uint8_t> buffer) const {
    assert(m_file);
    return std::fwrite(buffer.getData(), sizeof(uint8_t), buffer.getSize(), m_file);
  }

  std::size_t BinaryFile::write(uint8_t byte) const {
    assert(m_file);
    return std::fwrite(&byte, sizeof(uint8_t), 1, m_file);
  }

  std::size_t BinaryFile::read(BufferRef<uint8_t> buffer) const {
    assert(m_file);
    return std::fread(buffer.getData(), sizeof(uint8_t), buffer.getSize(), m_file);
  }

  std::size_t BinaryFile::read(uint8_t& byte) const {
    assert(m_file);
    return std::fread(&byte, sizeof(uint8_t), 1, m_file);
  }

  bool BinaryFile::isEof() const {
    assert(m_file);
    return std::feof(m_file);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
