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
 */

#include <utility>

#include <gf/Socket.h>

#include <gf/Log.h>

#include <gfpriv/SocketPrivate.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Socket::~Socket() {
    if (m_handle != InvalidSocketHandle) {
      priv::nativeCloseSocket(m_handle);
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

  SocketAddress Socket::getLocalAddress() const {
    SocketAddress address;
    address.length = sizeof(address.storage);
    int err = ::getsockname(m_handle, reinterpret_cast<sockaddr*>(&address.storage), &address.length);

    if (err != 0) {
      gf::Log::error("Could not get the local address: %s\n", priv::getErrorString().c_str());
    }

    return address;
  }

  void Socket::setBlocking() {
    if (!priv::nativeSetBlockMode(m_handle, false)) {
      gf::Log::error("Could not set the socket blocking: %s\n", priv::getErrorString().c_str());
    }
  }

  void Socket::setNonBlocking() {
    if (!priv::nativeSetBlockMode(m_handle, true)) {
      gf::Log::error("Could not set the socket non-blocking: %s\n", priv::getErrorString().c_str());
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
