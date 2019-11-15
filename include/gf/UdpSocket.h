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
#ifndef GF_UDP_SOCKET_H
#define GF_UDP_SOCKET_H

#include <cstdint>
#include <string>

#include "SocketAddress.h"
#include "ArrayRef.h"
#include "BufferRef.h"
#include "Portability.h"
#include "Socket.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class GF_API UdpSocket : public Socket {
  public:
    UdpSocket() = default;
    UdpSocket(const std::string& service, SocketFamily family = SocketFamily::Unspec);

    SocketAddress getRemoteAddress(const std::string& host, const std::string& service);

    SocketDataResult sendRawBytesTo(ArrayRef<uint8_t> buffer, const SocketAddress& address);
    SocketDataResult recvRawBytesFrom(BufferRef<uint8_t> buffer, SocketAddress& address);

  private:
    static SocketHandle nativeBind(const std::string& service, SocketFamily family);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_UDP_SOCKET_H
