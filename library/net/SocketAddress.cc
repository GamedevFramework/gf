/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
#include <gf/SocketAddress.h>

#include <cstring>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SocketAddress::SocketAddress(sockaddr *a_storage, StorageLengthType a_length)
  : length(a_length)
  {
    std::memcpy(&storage, a_storage, static_cast<std::size_t>(a_length));
  }

  SocketFamily SocketAddress::getFamily() const {
    return static_cast<SocketFamily>(storage.ss_family);
  }

  std::string SocketAddress::getHostname(SocketAddressFormat format) const {
    char host[NI_MAXHOST];
    auto err = ::getnameinfo(asSockAddr(), length, host, NI_MAXHOST, nullptr, 0, format == SocketAddressFormat::Numeric ? NI_NUMERICHOST : 0);

    if (err != 0) {
      gf::Log::error("Error while getting the host of an address. Reason: %s\n", ::gai_strerror(err));
      return "";
    }

    return host;
  }

  std::string SocketAddress::getService(SocketAddressFormat format) const {
    char serv[NI_MAXSERV];
    auto err = ::getnameinfo(asSockAddr(), length, nullptr, 0, serv, NI_MAXSERV, format == SocketAddressFormat::Numeric ? NI_NUMERICSERV : 0);

    if (err != 0) {
      gf::Log::error("Error while getting the service of an address. Reason: %s\n", ::gai_strerror(err));
      return "";
    }

    return serv;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
