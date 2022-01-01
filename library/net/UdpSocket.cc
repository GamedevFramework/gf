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
#include <gf/UdpSocket.h>

#include <cassert>
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

#include <gfpriv/SocketPrivate.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  UdpSocket::UdpSocket(const std::string& service, SocketFamily family)
  {
    setHandle(priv::nativeBind(service, family));
  }

  UdpSocket::UdpSocket(AnyType, SocketFamily family)
  {
    setHandle(priv::nativeBind("0", family));
  }

  SocketAddress UdpSocket::getRemoteAddress(const std::string& hostname, const std::string& service) {
    auto addresses = priv::getRemoteAddressInfo(hostname, service, priv::SocketType::Udp, getLocalAddress().getFamily());

    if (!addresses.empty()) {
      return addresses.front().address;
    }

    return SocketAddress();
  }


  SocketDataResult UdpSocket::sendRawBytesTo(Span<const uint8_t> buffer, const SocketAddress& address) {
    auto res = ::sendto(getHandle(), priv::sendPointer(buffer), priv::sendLength(buffer), priv::NoFlag, reinterpret_cast<const sockaddr*>(&address.storage), address.length);

    if (res == priv::InvalidCommunication) {
      if (priv::nativeWouldBlock(priv::getErrorCode())) {
        return { SocketStatus::Block, 0u };
      }

      gf::Log::error("Error while sending data. Reason: %s\n", priv::getErrorString().c_str());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }

  SocketDataResult UdpSocket::recvRawBytesFrom(Span<uint8_t> buffer, SocketAddress& address) {
    address.length = sizeof(address.storage);
    auto res = ::recvfrom(getHandle(), priv::recvPointer(buffer), priv::recvLength(buffer), priv::NoFlag, reinterpret_cast<sockaddr*>(&address.storage), &address.length);

    if (res == priv::InvalidCommunication) {
      if (priv::nativeWouldBlock(priv::getErrorCode())) {
        return { SocketStatus::Block, 0u };
      }

      gf::Log::error("Error while receiving data. Reason: %s\n", priv::getErrorString().c_str());
      return { SocketStatus::Error, 0u };
    }

    return { SocketStatus::Data, static_cast<std::size_t>(res) };
  }


  static std::size_t MaxDatagramSize = 65507;

  bool UdpSocket::sendBytesTo(Span<const uint8_t> buffer, const SocketAddress& address) {
    if (buffer.getSize() > MaxDatagramSize) {
      return false;
    }

    auto res = sendRawBytesTo(buffer, address);

    if (res.status == SocketStatus::Data) {
      return res.length == buffer.getSize();
    }

    assert(res.status != SocketStatus::Close);
    return false;
  }

  bool UdpSocket::recvBytesFrom(Span<uint8_t> buffer, SocketAddress& address) {
    if (buffer.getSize() > MaxDatagramSize) {
      return false;
    }

    auto res = recvRawBytesFrom(buffer, address);

    if (res.status == SocketStatus::Data) {
      return res.length == buffer.getSize();
    }

    assert(res.status != SocketStatus::Close);
    return false;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
