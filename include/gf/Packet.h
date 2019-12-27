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
 */
#ifndef GF_PACKET_H
#define GF_PACKET_H

#include "Portability.h"
#include "Stream.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   */
  class GF_API InputPacket : public InputStream {
  public:
    InputPacket(std::size_t size = 0);

    void reset(std::size_t size);
    BufferRef<uint8_t> getRef();

    virtual std::size_t read(BufferRef<uint8_t> buffer) override;
    virtual void seek(std::ptrdiff_t position) override;
    virtual void skip(std::ptrdiff_t position) override;
    virtual bool isFinished() override;
  private:
    std::vector<uint8_t> m_buffer;
    std::size_t m_offset;
  };

  /**
   * @ingroup net_sockets
   */
  class GF_API OutputPacket : public OutputStream {
  public:
    ArrayRef<uint8_t> getRef() const;

    virtual std::size_t write(ArrayRef<uint8_t> buffer) override;
    virtual std::size_t getWrittenBytesCount() const override;

  private:
    std::vector<uint8_t> m_buffer;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PACKET_H
