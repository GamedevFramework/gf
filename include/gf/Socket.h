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
#ifndef GF_SOCKET_H
#define GF_SOCKET_H

#include "NetApi.h"
#include "SocketAddress.h"
#include "SocketGuard.h"
#include "SocketTypes.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A network socket
   *
   *
   * @sa gf::TcpListener, gf::TcpSocket, gf::UdpSocket
   */
  class GF_NET_API Socket : private SocketGuard {
  public:
    /**
     * @brief Destructor
     *
     * The socket is closed.
     */
    ~Socket();

    /**
     * @brief Deleted copy constructor
     */
    Socket(const Socket&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Socket& operator=(const Socket&) = delete;

    /**
     * @brief Move constructor
     *
     * @param other The socket from which to move the socket handle
     */
    Socket(Socket&& other) noexcept;

    /**
     * @brief Move assignment
     *
     * @param other The socket from which to move the socket handle
     * @returns A reference to the socket
     */
    Socket& operator=(Socket&& other) noexcept;

    /**
     * @brief Boolean conversion
     *
     * @returns True if the socket is valid
     */
    operator bool () const noexcept {
      return m_handle != InvalidSocketHandle;
    }

    /**
     * @brief Get the local address of the socket
     *
     * @returns The local address of the socket
     */
    SocketAddress getLocalAddress() const;

    /**
     * @brief Set the socket in blocking mode
     *
     * @sa setNonBlocking()
     */
    void setBlocking();

    /**
     * @brief Set the socket in non-blocking mode
     *
     * @sa setBlocking()
     */
    void setNonBlocking();

  protected:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    Socket()
    : m_handle(InvalidSocketHandle)
    {

    }

    void setHandle(SocketHandle handle) {
      m_handle = handle;
    }

    SocketHandle getHandle() const {
      return m_handle;
    }
#endif

  private:
    friend class SocketSelector;
    SocketHandle m_handle;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_H
