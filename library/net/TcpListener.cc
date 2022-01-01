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
#include <gf/TcpListener.h>

#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <gf/Log.h>

#include <gfpriv/SocketPrivate.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  TcpListener::TcpListener(const std::string& service, SocketFamily family)
  {
    setHandle(priv::nativeBindListen(service, family));
  }

  TcpSocket TcpListener::accept() {
    SocketHandle handle = ::accept(getHandle(), nullptr, nullptr);

    if (handle == InvalidSocketHandle) {
      gf::Log::error("Error while accepting. Reason: %s\n", priv::getErrorString().c_str());
    }

    return TcpSocket(handle);
  }

  TcpSocket TcpListener::accept(SocketAddress& address) {
    address.length = sizeof(address.storage);
    SocketHandle handle = ::accept(getHandle(), reinterpret_cast<sockaddr*>(&address.storage), &address.length);

    if (handle == InvalidSocketHandle) {
      gf::Log::error("Error while accepting. Reason: %s\n", priv::getErrorString().c_str());
    }

    return TcpSocket(handle);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
