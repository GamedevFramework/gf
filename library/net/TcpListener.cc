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
#include <gf/TcpListener.h>

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

    SocketHandle nativeBindListen(const std::string& service) {
      struct addrinfo hints;
      struct addrinfo *result;

      std::memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_UNSPEC;      /* Allow IPv4 or IPv6 */
      hints.ai_socktype = SOCK_STREAM;  /* Datagram socket */
      hints.ai_flags = AI_PASSIVE;      /* For wildcard IP address */
      hints.ai_protocol = 0;            /* Any protocol */

      int err = ::getaddrinfo(nullptr, service.c_str(), &hints, &result);

      if (err != 0) {
        gf::Log::error("Error while getting an address for service '%s' : '%s'\n", service.c_str(), ::gai_strerror(err));
        return InvalidSocketHandle;
      }

      struct addrinfo *rp = nullptr;
      SocketHandle sock = InvalidSocketHandle;

      for (rp = result; rp != nullptr; rp = rp->ai_next) {
        sock = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (sock == InvalidSocketHandle) {
          continue;
        }

        if (::bind(sock, rp->ai_addr, rp->ai_addrlen) != 0) {
          Socket::nativeCloseSocket(sock);
          continue;
        }

        if (::listen(sock, SOMAXCONN) != 0) {
          Socket::nativeCloseSocket(sock);
          continue;
        }

        break;
      }

      ::freeaddrinfo(result);
      return sock;
    }

  }

  TcpListener::TcpListener(const std::string& service)
  : Socket(nativeBindListen(service))
  {

  }

  TcpSocket TcpListener::accept() {
    SocketHandle handle = ::accept(getHandle(), nullptr, nullptr);

    if (handle == InvalidSocketHandle) {
      gf::Log::error("Error while accepting.\n");
    }

    return TcpSocket(handle);
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
