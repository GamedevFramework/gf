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
#ifndef GF_SOCKET_GUARD_H
#define GF_SOCKET_GUARD_H

#include "NetApi.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A guard to handle library initialization
   *
   * This guard is used on Windows systems where the Winsock2 library must be
   * initalized. Otherwise, this class has absolutely no use (and no code).
   *
   * You should not create an instance of this class.
   *
   * @sa gf::TcpListener, gf::TcpSocket, gf::UdpSocket
   */
  class GF_NET_API SocketGuard {
  public:

#ifdef _WIN32
    /**
     * @brief Default constructor
     *
     * This function initialize the library
     */
    SocketGuard();

    /**
     * @brief Deleted copy constructor
     */
    SocketGuard(const SocketGuard&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    SocketGuard& operator=(const SocketGuard&) = delete;

    /**
     * @brief Move constructor
     */
    SocketGuard(SocketGuard&&) noexcept;

    /**
     * @brief Move assignment
     */
    SocketGuard& operator=(SocketGuard&&) noexcept;

    /**
     * @brief Destructor
     *
     * This function quits the library
     */
    ~SocketGuard();
#endif

  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_GUARD_H
