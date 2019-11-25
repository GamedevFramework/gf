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
#include <gf/TcpSocket.h>

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
  TcpSocket::TcpSocket(const std::string& host, const std::string& service, SocketFamily family)
  {
    setHandle(nativeConnect(host, service, family));
  }

  TcpSocket::TcpSocket(SocketHandle handle)
  {
    setHandle(handle);
  }

  TcpSocket::~TcpSocket() {
#ifdef _WIN32
    static constexpr int ReadWrite = SD_BOTH;
#else
    static constexpr int ReadWrite = SHUT_RDWR;
#endif
    ::shutdown(getHandle(), ReadWrite);
  }


  SocketAddress TcpSocket::getRemoteAddress() const {
    SocketAddress address;
    address.m_length = sizeof(address.m_storage);
    int err = ::getpeername(getHandle(), reinterpret_cast<sockaddr*>(&address.m_storage), &address.m_length);

    if (err != 0) {
      gf::Log::error("Could not get the remote address. Reason: %s\n", getErrorString().c_str());
    }

    return address;
  }

  SocketDataResult TcpSocket::sendRawBytes(ArrayRef<uint8_t> buffer) {
    int res = ::send(getHandle(), sendBufferPointer(buffer), sendBufferLength(buffer), NoFlag);

    if (res == InvalidCommunication) {
      gf::Log::error("Error while sending data. Reason: %s\n", getErrorString().c_str());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketDataResult TcpSocket::recvRawBytes(BufferRef<uint8_t> buffer) {
    int res = ::recv(getHandle(), recvBufferPointer(buffer), recvBufferLength(buffer), NoFlag);

    if (res == InvalidCommunication) {
      gf::Log::error("Error while receiving data. Reason: %s\n", getErrorString().c_str());
      return { SocketStatus::Error, 0u };
    }

    if (res == 0) {
      return { SocketStatus::Close, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }


  SocketHandle TcpSocket::nativeConnect(const std::string& host, const std::string& service, SocketFamily family) {
    auto addresses = getRemoteAddressInfo(host, service, SocketType::Tcp, family);

    for (auto info : addresses) {
      SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (::connect(sock, info.address.getData(), info.address.getLength()) != 0) {
        nativeCloseSocket(sock);
        continue;
      }

      return sock;
    }

    gf::Log::error("Unable to connect to '%s:%s'\n", host.c_str(), service.c_str());
    return InvalidSocketHandle;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


