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
#ifndef GFPRIV_SOCKET_PRIVATE_H
#define GFPRIV_SOCKET_PRIVATE_H

#include <cstdint>
#include <cstddef>
#include <cstring>

#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <gf/Log.h>
#include <gf/Span.h>
#include <gf/SocketTypes.h>
#include <gf/SocketAddress.h>
#include <gf/Time.h>

namespace gf {
namespace priv {

  bool nativeCloseSocket(SocketHandle handle);

  bool nativeSetReuseAddress(SocketHandle handle, bool reuse = true);

  bool nativeSetBlockMode(SocketHandle handle, bool blocking);

  bool nativeWouldBlock(int err);

  SocketSelectorStatus nativePoll(std::vector<pollfd>& fds, Time duration);

  SocketHandle nativeBindListen(const std::string& service, SocketFamily family);

  SocketHandle nativeConnect(const std::string& host, const std::string& service, SocketFamily family);

  SocketHandle nativeBind(const std::string& service, SocketFamily family);

#ifdef _WIN32
  inline
  int sendLength(Span<const uint8_t> buffer) {
    return static_cast<int>(buffer.getSize());
  }

  inline
  const char *sendPointer(Span<const uint8_t> buffer) {
    return reinterpret_cast<const char *>(buffer.getData());
  }

  inline
  int recvLength(Span<uint8_t> buffer) {
    return static_cast<int>(buffer.getSize());
  }

  inline
  char *recvPointer(Span<uint8_t> buffer) {
    return reinterpret_cast<char *>(buffer.getData());
  }
#else
  inline
  std::size_t sendLength(Span<const uint8_t> buffer) {
    return buffer.getSize();
  }

  inline
  const void *sendPointer(Span<const uint8_t> buffer) {
    return reinterpret_cast<const void *>(buffer.getData());
  }

  inline
  std::size_t recvLength(Span<uint8_t> buffer) {
    return buffer.getSize();
  }

  inline
  void *recvPointer(Span<uint8_t> buffer) {
    return reinterpret_cast<void *>(buffer.getData());
  }
#endif

  int getErrorCode();
  std::string getErrorString();

#ifdef _WIN32
  constexpr int InvalidCommunication = SOCKET_ERROR;
#else
  constexpr ssize_t InvalidCommunication = -1;
#endif

  enum SocketType : int {
    Tcp     = SOCK_STREAM,
    Udp     = SOCK_DGRAM,
  };

  struct SocketAddressInfo {
    SocketFamily family;
    SocketType type;
    SocketAddress address;
  };

  constexpr int NoFlag = 0;

  std::vector<SocketAddressInfo> getRemoteAddressInfo(const std::string& hostname, const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);
  std::vector<SocketAddressInfo> getLocalAddressInfo(const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);

  struct SizeHeader {
    uint8_t data[8];
  };

  constexpr SizeHeader encodeHeader(uint64_t size) {
    SizeHeader header = {{ 0, 0, 0, 0, 0, 0, 0, 0 }};

    for (int i = 0; i < 8; ++i) {
      header.data[8 - i - 1] = static_cast<uint8_t>(size % 0x100);
      size >>= 8;
    }

    return header;
  }

  constexpr uint64_t decodeHeader(const SizeHeader& header) {
    uint64_t size = 0;

    for (uint64_t x : header.data) {
      size = (size << 8) + x;
    }

    return size;
  }

}
}

#endif // GFPRIV_SOCKET_PRIVATE_H
