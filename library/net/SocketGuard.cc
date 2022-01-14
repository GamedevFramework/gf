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
#include <gf/SocketGuard.h>

#ifdef _WIN32
#include <atomic>
#include <stdexcept>

#include <winsock2.h>

#include <gf/Log.h>
#endif

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

#ifdef _WIN32
  static std::atomic_int g_winsock{0};

  SocketGuard::SocketGuard()
  {
    if (g_winsock.fetch_add(1) == 0) { // we are the first
      WSADATA wsaData;
      auto res = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

      if (res != 0) {
        Log::error("Unable to initialize the Winsock2 library.\n");
        throw std::runtime_error("Unable to initialize the Winsock2 library.\n");
      }
    }
  }

  SocketGuard::SocketGuard(SocketGuard&&) noexcept
  {
    g_winsock.fetch_add(1);
  }

  SocketGuard& SocketGuard::operator=(SocketGuard&&) noexcept {
    g_winsock.fetch_add(1);
    return *this;
  }

  SocketGuard::~SocketGuard() {
    if (g_winsock.fetch_sub(1) == 1) { // we are the last
      ::WSACleanup();
    }
  }
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
