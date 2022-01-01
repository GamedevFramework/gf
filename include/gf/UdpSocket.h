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
#ifndef GF_UDP_SOCKET_H
#define GF_UDP_SOCKET_H

#include <cstdint>
#include <string>

#include "NetApi.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "Span.h"
#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A UDP socket
   *
   * A UDP socket is a socket for UDP (%User %Datagram %Protocol). UDP
   * provides an unreliable communication between two hosts. UDP is
   * connectionless i.e. no connection is established, the receiver must be
   * set for each communication.
   *
   * @snippet snippets/doc_class_udp_socket.cc udp_socket
   *
   * @sa gf::TcpSocket
   */
  class GF_NET_API UdpSocket : public Socket {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates an invalid socket
     */
    UdpSocket() = default;

    /**
     * @brief Full constructor
     *
     * It creates a UDP socket that is not bound to a specific port. The port
     * is chosen by the system. It may be used by a client that do not need a
     * specific port.
     *
     * @param family The prefered socket family
     */
    UdpSocket(AnyType, SocketFamily family = SocketFamily::Unspec);

    /**
     * @brief Full constructor
     *
     * It creates a UDP socket that is bound on a specific port. It must be
     * used in case a server wants to be joined with UDP on a specific port.
     *
     * @param service The bound service
     * @param family The prefered socket family
     */
    UdpSocket(const std::string& service, SocketFamily family = SocketFamily::Unspec);

    /**
     * @brief Get a remote address for this socket
     *
     * This function provides a suitable socket address that can be used with
     * the sending functions.
     *
     * @param hostname The name of the host to communicate with
     * @param service The service on the host to communicate with
     */
    SocketAddress getRemoteAddress(const std::string& hostname, const std::string& service);

    /**
     * @brief Send some bytes over to the socket
     *
     * @param buffer The buffer that contains the bytes to send
     * @param address The address of the host to communicate with
     * @returns A result for the operation
     */
    SocketDataResult sendRawBytesTo(Span<const uint8_t> buffer, const SocketAddress& address);

    /**
     * @brief Receive some bytes from the socket
     *
     * @param buffer The buffer to store the received bytes
     * @param address The address of the sending host
     * @returns A result for the operation
     */
    SocketDataResult recvRawBytesFrom(Span<uint8_t> buffer, SocketAddress& address);

    /**
     * @brief Send a whole buffer to the socket
     *
     * This function ensures the buffer is sent in a single packet.
     *
     * @param buffer The buffer that contains the bytes to send
     * @param address The address of the host to communicate with
     * @returns True if no error occurred and the buffer was sent
     */
    bool sendBytesTo(Span<const uint8_t> buffer, const SocketAddress& address);

    /**
     * @brief Receive a whole buffer from the socket
     *
     * This function ensures the buffer is received in a single packet.
     *
     * @param buffer The buffer to store the received bytes
     * @param address The address of the sending host
     * @returns True if no error occurred and the buffer was received
     */
    bool recvBytesFrom(Span<uint8_t> buffer, SocketAddress& address);

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_UDP_SOCKET_H
