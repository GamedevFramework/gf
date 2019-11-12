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
#include <gf/UdpSocket.h>

#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  UdpSocket::UdpSocket(const std::string& service, SocketFamily family)
  : Socket(nativeBind(service, family))
  {
  }

  SocketDataResult UdpSocket::sendRawBytesTo(ArrayRef<uint8_t> buffer, const SocketAddress& address) {
    auto res = ::sendto(getHandle(), sendBufferPointer(buffer), sendBufferLength(buffer), NoFlag, reinterpret_cast<const sockaddr*>(&address.m_storage), address.m_length);

    if (res == InvalidCommunication) {
      gf::Log::error("Error while sending data: %d\n",  getErrorCode());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketDataResult UdpSocket::recvRawBytesFrom(BufferRef<uint8_t> buffer, SocketAddress& address) {
    address.m_length = sizeof(sockaddr_storage);
    auto res = ::recvfrom(getHandle(), recvBufferPointer(buffer), recvBufferLength(buffer), NoFlag, reinterpret_cast<sockaddr*>(&address.m_storage), &address.m_length);

    if (res == InvalidCommunication) {
      gf::Log::error("Error while receiving data: %d\n",  getErrorCode());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketHandle UdpSocket::nativeBind(const std::string& service, SocketFamily family) {
    auto addresses = getLocalAddressInfo(service, SocketType::Udp, family);

    for (auto info : addresses) {
      SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (::bind(sock, info.address.getData(), info.address.getLength()) != 0) {
        nativeCloseSocket(sock);
        continue;
      }

      return sock;
    }

    gf::Log::error("Unable to bind service '%s'\n", service.c_str());
    return InvalidSocketHandle;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


