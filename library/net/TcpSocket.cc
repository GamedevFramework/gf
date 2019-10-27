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
  namespace {
    constexpr int NoFlag = 0;

    SocketHandle nativeConnect(const std::string& host, const std::string& service) {
      struct addrinfo hints;
      struct addrinfo *result;

      std::memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = 0;
      hints.ai_flags = AI_NUMERICSERV;

      int err = ::getaddrinfo(host.c_str(), service.c_str(), &hints, &result);

      if (err != 0) {
        gf::Log::error("Error while getting an address for '%s:%s' : '%s'\n", host.c_str(), service.c_str(), ::gai_strerror(err));
        return InvalidSocketHandle;
      }

      struct addrinfo *rp = nullptr;
      SocketHandle sock;

      for (rp = result; rp != nullptr; rp = rp->ai_next) {
        sock = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (sock == InvalidSocketHandle) {
          continue;
        }

        if (::connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
          break;
        }

        Socket::nativeCloseSocket(sock);
      }

      ::freeaddrinfo(result);

      if (rp == nullptr) {
        gf::Log::error("Unable to connect to '%s:%s'\n", host.c_str(), service.c_str());
        return InvalidSocketHandle;
      }

      return sock;
    }

  }

  TcpSocket::TcpSocket(const std::string& host, const std::string& service)
  : Socket(nativeConnect(host, service))
  {

  }

  TcpSocket::TcpSocket(SocketHandle handle)
  : Socket(handle)
  {

  }

#ifdef _WIN32
  SocketDataResult TcpSocket::sendRawBytes(ArrayRef<uint8_t> buffer) {
    int res = ::send(getHandle(), static_cast<const char *>(buffer.getData()), static_cast<int>(buffer.getSize()), NoFlag);

    if (res == SOCKET_ERROR) {
      gf::Log::error("Error while sending data: %d\n",  WSAGetLastError());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketDataResult TcpSocket::recvRawBytes(BufferRef<uint8_t> buffer) {
    int res = ::recv(getHandle(), static_cast<char *>(buffer.getData()), static_cast<int>(buffer.getSize()), NoFlag);

    if (res == SOCKET_ERROR) {
      gf::Log::error("Error while receiving data: %d\n",  WSAGetLastError());
      return { SocketStatus::Error, 0u };
    }

    if (res == 0) {
      return { SocketStatus::Close, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

#else
  SocketDataResult TcpSocket::sendRawBytes(ArrayRef<uint8_t> buffer) {
    auto res = ::send(getHandle(), static_cast<const void *>(buffer.getData()), buffer.getSize(), NoFlag);

    if (res == -1) {
      gf::Log::error("Error while sending data: %d\n",  errno);
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketDataResult TcpSocket::recvRawBytes(BufferRef<uint8_t> buffer) {
    auto res = ::recv(getHandle(), static_cast<void *>(buffer.getData()), buffer.getSize(), NoFlag);

    if (res == -1) {
      gf::Log::error("Error while receiving data: %d\n",  errno);
      return { SocketStatus::Error, 0u };
    }

    if (res == 0) {
      return { SocketStatus::Close, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


