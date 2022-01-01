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
#ifndef GF_TCP_LISTENER_H
#define GF_TCP_LISTENER_H

#include <string>

#include "NetApi.h"
#include "Socket.h"
#include "TcpSocket.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A TCP listener
   *
   * A TCP listener is a network socket that can handle incoming connections
   * from remote hosts. It can be used to create a TCP server.
   *
   * The listener is associated to a service. The service can be a port number.
   *
   * @snippet snippets/doc_class_tcp_listener.cc listener_port
   *
   * For well-known services, it can also be a name.
   *
   * @snippet snippets/doc_class_tcp_listener.cc listener_service
   *
   * A TCP listener is generally created oustide the main loop. Then, for a
   * synchronous server that handles one connection at the time, the listener
   * accept a connection and handle the client.
   *
   * @snippet snippets/doc_class_tcp_listener.cc listener_loop
   *
   * @sa gf::TcpSocket
   */
  class GF_NET_API TcpListener : public Socket {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates an invalid listener.
     */
    TcpListener() = default;

    /**
     * @brief Full constructor
     *
     * This constructor creates a valid listener with an associated service.
     * The service can be a port number (in a string) or a well-known name
     * (such as "http").
     *
     * @param service The service associated to the listener
     * @param family The socket family of the listener
     */
    TcpListener(const std::string& service, SocketFamily family = SocketFamily::Unspec);

    /**
     * @brief Accept a new connection from a remote client
     *
     * This member function blocks until a new connection arrives (unless the
     * socket was made non-blocking). Then a socket is created for the remote
     * client and returned. The returned socket can be used to communicate with
     * the client.
     *
     * @returns A new socket representing the remote client
     */
    TcpSocket accept();

    /**
     * @brief Accept a new connection from a remote client
     *
     * This member function blocks until a new connection arrives (unless the
     * socket was made non-blocking). Then a socket is created for the remote
     * client and returned. The returned socket can be used to communicate with
     * the client.
     *
     * In addition, the caller can obtain the socket address of the remote
     * client when it is accepted.
     *
     * @param address A reference to store the socket address of the remote client
     * @returns A new socket representing the remote client
     */
    TcpSocket accept(SocketAddress& address);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TCP_LISTENER_H
