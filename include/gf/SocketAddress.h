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
#ifndef GF_SOCKET_ADDRESS_H
#define GF_SOCKET_ADDRESS_H

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum SocketFamily : int {
    Unspec  = AF_UNSPEC,
    IPv4    = AF_INET,
    IPv6    = AF_INET6,
  };

  class GF_API SocketAddress {
  public:
#ifdef _WIN32
    using StorageLengthType = int;
#else
    using StorageLengthType = socklen_t;
#endif

    SocketAddress() = default;
    SocketAddress(sockaddr *storage, StorageLengthType length);

    const sockaddr *getData() const {
      return reinterpret_cast<const sockaddr*>(&m_storage);
    }

    StorageLengthType getLength() const {
      return m_length;
    }

    SocketFamily getFamily() const;

    std::string getHost() const;

    std::string getService() const;

  private:
    friend class Socket;
    friend class TcpSocket;
    friend class UdpSocket;

    sockaddr_storage m_storage;
    StorageLengthType m_length;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_ADDRESS_H
