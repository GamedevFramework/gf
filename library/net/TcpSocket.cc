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
#include <gf/TcpSocket.h>

#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <gf/Log.h>
#include <gf/Packet.h>

#include <gfpriv/SocketPrivate.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  TcpSocket::TcpSocket(const std::string& hostname, const std::string& service, SocketFamily family)
  {
    setHandle(priv::nativeConnect(hostname, service, family));
  }

  TcpSocket::TcpSocket(SocketHandle handle)
  {
    setHandle(handle);
  }

  TcpSocket::~TcpSocket() {
#ifdef _WIN32
    static constexpr int ReadWrite = SD_BOTH;
#else
    static constexpr int ReadWrite = SHUT_RDWR;
#endif
    ::shutdown(getHandle(), ReadWrite);
  }


  SocketAddress TcpSocket::getRemoteAddress() const {
    SocketAddress address;
    address.length = sizeof(address.storage);
    int err = ::getpeername(getHandle(), reinterpret_cast<sockaddr*>(&address.storage), &address.length);

    if (err != 0) {
      gf::Log::error("Could not get the remote address. Reason: %s\n", priv::getErrorString().c_str());
    }

    return address;
  }

  SocketDataResult TcpSocket::sendRawBytes(Span<const uint8_t> buffer) {
#ifdef MSG_NOSIGNAL
    static constexpr int SendFlags = MSG_NOSIGNAL;
#else
    static constexpr int SendFlags = priv::NoFlag;
#endif
    int res = ::send(getHandle(), priv::sendPointer(buffer), priv::sendLength(buffer), SendFlags);

    if (res == priv::InvalidCommunication) {
      if (priv::nativeWouldBlock(priv::getErrorCode())) {
        return { SocketStatus::Block, 0u };
      }

      gf::Log::error("Error while sending data. Reason: %s\n", priv::getErrorString().c_str());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketDataResult TcpSocket::recvRawBytes(Span<uint8_t> buffer) {
    int res = ::recv(getHandle(), priv::recvPointer(buffer), priv::recvLength(buffer), priv::NoFlag);

    if (res == priv::InvalidCommunication) {
      if (priv::nativeWouldBlock(priv::getErrorCode())) {
        return { SocketStatus::Block, 0u };
      }

      gf::Log::error("Error while receiving data. Reason: %s\n", priv::getErrorString().c_str());
      return { SocketStatus::Error, 0u };
    }

    if (res == 0) {
      return { SocketStatus::Close, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketStatus TcpSocket::sendBytes(Span<const uint8_t> buffer) {
    do {
      auto res = sendRawBytes(buffer);

      switch (res.status) {
        case SocketStatus::Data:
          buffer = buffer.lastExcept(res.length);
          break;
        case SocketStatus::Block:
          continue;
        case SocketStatus::Close:
        case SocketStatus::Error:
          return res.status;
      }

    } while (!buffer.isEmpty());

    return SocketStatus::Data;
  }

  SocketStatus TcpSocket::recvBytes(Span<uint8_t> buffer) {
    do {
      auto res = recvRawBytes(buffer);

      switch (res.status) {
        case SocketStatus::Data:
          buffer = buffer.lastExcept(res.length);
          break;
        case SocketStatus::Block:
          continue;
        case SocketStatus::Close:
        case SocketStatus::Error:
          return res.status;
      }

    } while (!buffer.isEmpty());

    return SocketStatus::Data;
  }

  SocketStatus TcpSocket::sendPacket(const Packet& packet) {
    auto size = static_cast<uint64_t>(packet.bytes.size());
    auto header = priv::encodeHeader(size);

    auto status = sendBytes(header.data);

    if (status != SocketStatus::Data) {
      return status;
    }

    return sendBytes(packet.bytes);
  }

  SocketStatus TcpSocket::recvPacket(Packet& packet) {
    priv::SizeHeader header;
    auto status = recvBytes(header.data);

    if (status != SocketStatus::Data) {
      return status;
    }

    auto size = priv::decodeHeader(header);
    packet.bytes.resize(static_cast<std::size_t>(size));
    return recvBytes(packet.bytes);
  }



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


