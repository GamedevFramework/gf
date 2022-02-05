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
#include <gfpriv/SocketPrivate.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#endif

namespace gf {
namespace priv {

#ifdef _WIN32
  bool nativeCloseSocket(SocketHandle handle) {
    return ::closesocket(handle) == 0;
  }

  bool nativeSetReuseAddress(SocketHandle handle, bool reuse) {
    BOOL val = reuse ? TRUE : FALSE;
    return ::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&val), sizeof(BOOL)) == 0;
  }

  bool nativeSetBlockMode(SocketHandle handle, bool blocking) {
    u_long mode = blocking ? 1 : 0;
    return ::ioctlsocket(handle, FIONBIO, &mode) == 0;
  }

  bool nativeWouldBlock(int err) {
    return err == WSAEWOULDBLOCK;
  }

  int getErrorCode() {
    return WSAGetLastError();
  }

  std::string getErrorString() {
    static constexpr std::size_t BufferSize = 1024;
    int err = getErrorCode();
    char buffer[BufferSize] = { '\0' };
    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, BufferSize, nullptr);
    return buffer;
  }

#else
  bool nativeCloseSocket(SocketHandle handle) {
    return ::close(handle) == 0;
  }

  bool nativeSetReuseAddress(SocketHandle handle, bool reuse) {
    int val = reuse ? 1 : 0;
    return ::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, static_cast<const void*>(&val), sizeof(int)) == 0;
  }

  bool nativeSetBlockMode(SocketHandle handle, bool blocking) {
    int flags = ::fcntl(handle, F_GETFL, 0);

    if (flags == -1) {
      return false;
    }

    return ::fcntl(handle, F_SETFL, blocking ? (flags | O_NONBLOCK) : (flags & ~(O_NONBLOCK))) != -1;
  }

  bool nativeWouldBlock(int err) {
    return err == EAGAIN || err == EWOULDBLOCK;
  }

  int getErrorCode() {
    return errno;
  }

  std::string getErrorString() {
    static constexpr std::size_t BufferSize = 1024;
    int err = getErrorCode();
    char buffer[BufferSize] = { '\0' };
    ::strerror_r(err, buffer, BufferSize);
    return buffer;
  }

#endif

  SocketSelectorStatus nativePoll(std::vector<pollfd>& fds, Time duration) {
    auto ms = duration.asMilliseconds();

#ifdef _WIN32
    int err = ::WSAPoll(fds.data(), static_cast<ULONG>(fds.size()), ms);
#else
    int err = ::poll(fds.data(), static_cast<nfds_t>(fds.size()), ms);
#endif

    if (err < 0) {
      Log::error("Error while polling: %s\n", priv::getErrorString().c_str());
      return SocketSelectorStatus::Error;
    }

    if (err == 0) {
      return SocketSelectorStatus::Timeout;
    }

    return SocketSelectorStatus::Event;
  }

  SocketHandle nativeBindListen(const std::string& service, SocketFamily family) {
    auto addresses = getLocalAddressInfo(service, SocketType::Tcp, family);

    for (auto info : addresses) {
      SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (!nativeSetReuseAddress(sock, true)) {
        gf::Log::error("Error when setting address reuse. Reason: %s\n", priv::getErrorString().c_str());
      }

      if (::bind(sock, info.address.asSockAddr(), info.address.length) != 0) {
        nativeCloseSocket(sock);
        continue;
      }

      if (::listen(sock, SOMAXCONN) != 0) {
        nativeCloseSocket(sock);
        continue;
      }

      return sock;
    }

    gf::Log::error("Unable to bind service '%s'.\n", service.c_str());
    return InvalidSocketHandle;
  }

  SocketHandle nativeConnect(const std::string& hostname, const std::string& service, SocketFamily family) {
    auto addresses = getRemoteAddressInfo(hostname, service, SocketType::Tcp, family);

    for (auto info : addresses) {
      SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (::connect(sock, info.address.asSockAddr(), info.address.length) != 0) {
        nativeCloseSocket(sock);
        continue;
      }

      return sock;
    }

    gf::Log::error("Unable to connect to '%s:%s'\n", hostname.c_str(), service.c_str());
    return InvalidSocketHandle;
  }

  SocketHandle nativeBind(const std::string& service, SocketFamily family) {
    auto addresses = getLocalAddressInfo(service, SocketType::Udp, family);

    for (auto info : addresses) {
      SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (::bind(sock, info.address.asSockAddr(), info.address.length) != 0) {
        nativeCloseSocket(sock);
        continue;
      }

      return sock;
    }

    gf::Log::error("Unable to bind service '%s'\n", service.c_str());
    return InvalidSocketHandle;
  }

  namespace {

    std::vector<SocketAddressInfo> getAddressInfoEx(const char *hostname, const char *service, int flags, SocketType type, SocketFamily family) {
      std::vector<SocketAddressInfo> result;

      struct addrinfo hints;
      struct addrinfo *first;

      std::memset(&hints, 0, sizeof hints);
      hints.ai_family = static_cast<int>(family);
      hints.ai_socktype = static_cast<int>(type);
      hints.ai_protocol = 0;
      hints.ai_flags = flags;

      int err = ::getaddrinfo(hostname, service, &hints, &first);

      if (err != 0) {
        if (hostname != nullptr) {
          gf::Log::error("Error while getting an address for hostname '%s:%s': '%s'\n", hostname, service, ::gai_strerror(err));
        } else {
          gf::Log::error("Error while getting an address for service '%s': '%s'\n", service, ::gai_strerror(err));
        }

        return result;
      }

      struct addrinfo *rp = nullptr;

      for (rp = first; rp != nullptr; rp = rp->ai_next) {
        SocketAddressInfo info;
        info.family = static_cast<SocketFamily>(rp->ai_family);
        info.type = static_cast<SocketType>(rp->ai_socktype);
        info.address = SocketAddress(rp->ai_addr, static_cast<SocketAddress::StorageLengthType>(rp->ai_addrlen));
        result.push_back(info);
      }

      ::freeaddrinfo(first);

      return result;
    }

  }


  std::vector<SocketAddressInfo> getRemoteAddressInfo(const std::string& hostname, const std::string& service, SocketType type, SocketFamily family) {
    return getAddressInfoEx(hostname.c_str(), service.c_str(), NoFlag, type, family);
  }

  std::vector<SocketAddressInfo> getLocalAddressInfo(const std::string& service, SocketType type, SocketFamily family) {
    return getAddressInfoEx(nullptr, service.c_str(), AI_PASSIVE, type, family);
  }

}
}
