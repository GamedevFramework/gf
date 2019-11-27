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
#include <gf/Packet.h>

#include <algorithm>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  InputPacket::InputPacket(std::size_t size)
  : m_buffer(size)
  , m_offset(0)
  {
  }

  BufferRef<uint8_t> InputPacket::asBuffer() {
    return gf::buffer(m_buffer.data(), m_buffer.size());
  }

  std::size_t InputPacket::read(BufferRef<uint8_t> buffer) {
    if (buffer.getSize() == 0) {
      return 0;
    }

    std::size_t count = buffer.getSize();
    std::size_t available = m_buffer.size() - m_offset;

    if (count > available) {
      count = available;
    }

    if (count > 0) {
      std::copy_n(m_buffer.data() + m_offset, count, buffer.getData());
      m_offset += count;
    }

    return count;
  }

  void InputPacket::seek(std::ptrdiff_t position) {
    if (position < 0) {
      return;
    }

    std::size_t offset = static_cast<std::size_t>(position);
    m_offset = std::min(offset, m_buffer.size());
  }

  void InputPacket::skip(std::ptrdiff_t position) {
    if (position < 0) {
      std::size_t offset = static_cast<std::size_t>(-position);
      m_offset = offset < m_offset ? m_offset - offset : 0;
    } else {
      std::size_t offset = static_cast<std::size_t>(position);
      m_offset = std::min(m_offset + offset, m_buffer.size());
    }
  }

  bool InputPacket::isFinished() {
    return m_offset == m_buffer.size();
  }

  ArrayRef<uint8_t> OutputPacket::asBuffer() {
    return gf::array(m_buffer.data(), m_buffer.size());
  }

  std::size_t OutputPacket::write(ArrayRef<uint8_t> buffer) {
    std::copy_n(buffer.getData(), buffer.getSize(), std::back_inserter(m_buffer));
    return buffer.getSize();
  }

  std::size_t OutputPacket::getWrittenBytesCount() const {
    return m_buffer.size();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
