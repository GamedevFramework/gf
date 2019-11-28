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
#include "Socket.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class GF_API TcpSocket : public Socket {
  public:
    TcpSocket() = default;
    TcpSocket(const std::string& host, const std::string& service, SocketFamily family = SocketFamily::Unspec);

    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;

    TcpSocket(TcpSocket&&) = default;
    TcpSocket& operator=(TcpSocket&&) = default;

    ~TcpSocket();

    SocketAddress getRemoteAddress() const;

    SocketDataResult sendRawBytes(ArrayRef<uint8_t> buffer);
    SocketDataResult recvRawBytes(BufferRef<uint8_t> buffer);

    bool sendBytes(ArrayRef<uint8_t> buffer);
    bool recvBytes(BufferRef<uint8_t> buffer);

    bool sendPacket(const OutputPacket& packet);
    bool recvPacket(InputPacket& packet);

    template<typename T>
    bool sendData(const T& data) {
      OutputPacket packet;
      gf::Serializer serializer(packet);
      serializer | const_cast<T&>(data);
      return sendPacket(packet);
    }

    template<typename T>
    bool recvData(T& data) {
      InputPacket packet;
      bool res = recvPacket(packet);

      if (res) {
        gf::Deserializer deserializer(packet);
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
