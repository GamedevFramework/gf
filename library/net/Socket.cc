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
#include <gf/Socket.h>

#include <utility>

#ifdef _WIN32

#else
#include <unistd.h>
#endif


namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Socket::~Socket() {
    if (m_handle != InvalidSocketHandle) {
      nativeCloseSocket(m_handle);
    }
  }

  Socket::Socket(Socket&& other) noexcept
  : m_handle(std::exchange(other.m_handle, InvalidSocketHandle))
  {

  }

  Socket& Socket::operator=(Socket&& other) noexcept {
    SocketGuard::operator=(std::move(other));
    std::swap(m_handle, other.m_handle);
    return *this;
  }

#ifdef _WIN32
    bool Socket::nativeCloseSocket(SocketHandle handle) {
      return ::closesocket(handle) == 0;
    }
#else
    bool Socket::nativeCloseSocket(SocketHandle handle) {
      return ::close(handle) == 0;
    }
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
