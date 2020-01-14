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
 */
#ifndef GF_TCP_SOCKET_H
#define GF_TCP_SOCKET_H

#include <cstdint>
#include <string>

#include "ArrayRef.h"
#include "BufferRef.h"
#include "Packet.h"
#include "Portability.h"
#include "Serialization.h"
#include "Streams.h"
#include "Socket.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A TCP socket
   *
   * A TCP socket is a socket for TCP (%Transmission %Control %Protocol). TCP
   * provides a reliable communication between two hosts. TCP is
   * connection-oriented i.e. once the connection is established, it can be used
   * to send and/or receive data until it is shutdown.
   *
   * @snippet snippets/doc_class_tcp_socket.cc tcp_socket
   *
   * @sa gf::TcpListener, gf::UdpSocket
   */
  class GF_API TcpSocket : public Socket {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates an invalid socket
     */
    TcpSocket() = default;

    /**
     * @brief Full constructor
     *
     * @param hostname The name of the host to connect
     * @param service The service on the host to connect
     * @param family The prefered socket family
     */
    TcpSocket(const std::string& hostname, const std::string& service, SocketFamily family = SocketFamily::Unspec);

    /**
     * @brief Deleted copy constructor
     */
    TcpSocket(const TcpSocket&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    TcpSocket& operator=(const TcpSocket&) = delete;

    /**
     * @brief Default move constructor
     */
    TcpSocket(TcpSocket&&) = default;

    /**
     * @brief Default move assignment
     */
    TcpSocket& operator=(TcpSocket&&) = default;

    /**
     * @brief Destructor
     *
     * The destructor shutdowns the socket before closing it.
     */
    ~TcpSocket();

    /**
     * @brief Get the remote address of the host
     */
    SocketAddress getRemoteAddress() const;

    /**
     * @brief Send some bytes over to the socket
     *
     * @param buffer The buffer that contains the bytes to send
     * @returns A result for the operation
     */
    SocketDataResult sendRawBytes(ArrayRef<uint8_t> buffer);

    /**
     * @brief Receive some bytes from the socket
     *
     * @param buffer The buffer to store the received bytes
     * @returns A result for the operation
     */
    SocketDataResult recvRawBytes(BufferRef<uint8_t> buffer);

    /**
     * @brief Send a whole buffer to the socket
     *
     * This function ensures the whole buffer is sent unless an error occurs.
     *
     * @param buffer The buffer that contains the bytes to send
     * @returns True if no error occurred and the buffer was sent
     */
    bool sendBytes(ArrayRef<uint8_t> buffer);

    /**
     * @brief Receive a whole buffer from the socket
     *
     * This function ensures the whole buffer is received unless an error
     * occurs.
     *
     * @param buffer The buffer to store the received bytes
     * @returns True if no error occurred and the buffer was received
     */
    bool recvBytes(BufferRef<uint8_t> buffer);

    /**
     * @brief Send a packet to the socket
     *
     * @param packet The packet that contains the bytes to send
     * @returns True if no error occurred and the packet was sent
     */
    bool sendPacket(const Packet& packet);

    /**
     * @brief Receive a packet from the socket
     *
     * @param packet The packet to store the received bytes
     * @returns True if no error occurred and the packet was received
     */
    bool recvPacket(Packet& packet);

    /**
     * @brief Send arbitrary data to the socket
     *
     * The data is seralized in a packet.
     *
     * @tparam T The type of data to send
     * @param data The data to send
     * @returns True if no error occurred and data was sent
     */
    template<typename T>
    bool sendData(const T& data) {
      Packet packet;
      BufferOutputStream stream(&packet.bytes);
      gf::Serializer serializer(stream);
      serializer | const_cast<T&>(data);
      return sendPacket(packet);
    }

    /**
     * @brief Receive arbitrary data from the socket
     *
     * The data is deserialized from a packet.
     *
     * @tparam T The type of data to receive
     * @param data The data to receive
     * @returns True if no error occurred and data was received
     */
    template<typename T>
    bool recvData(T& data) {
      Packet packet;
      bool res = recvPacket(packet);

      if (res) {
        BufferInputStream stream(&packet.bytes);
        gf::Deserializer deserializer(stream);
        deserializer | data;
      }

      return res;
    }

  private:
    TcpSocket(SocketHandle handle);

    friend class TcpListener;

  private:
    static SocketHandle nativeConnect(const std::string& host, const std::string& service, SocketFamily family);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TCP_SOCKET_H
