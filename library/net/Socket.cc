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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Socket.h>

#include <cstring>
#include <utility>

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

#include <gf/Log.h>
#include <gf/Unused.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Socket::~Socket() {
    if (m_handle != InvalidSocketHandle) {
      nativeCloseSocket(m_handle);
    }
  }

  Socket::Socket(Socket&& other) noexcept
  : m_handle(std::exchange(other.m_handle, InvalidSocketHandle))
  {

  }

  Socket& Socket::operator=(Socket&& other) noexcept {
    SocketGuard::operator=(std::move(other));
    std::swap(m_handle, other.m_handle);
    return *this;
  }

  SocketAddress Socket::getLocalAddress() const {
    SocketAddress address;
    address.m_length = sizeof(address.m_storage);
    int err = ::getsockname(m_handle, reinterpret_cast<sockaddr*>(&address.m_storage), &address.m_length);

    if (err != 0) {
      gf::Log::error("Could not get the local address: %s\n", getErrorString().c_str());
    }

    return address;
  }

  void Socket::setBlocking() {
    if (!nativeSetBlockMode(m_handle, false)) {
      gf::Log::error("Could not set the socket blocking: %s\n", getErrorString().c_str());
    }
  }

  void Socket::setNonBlocking() {
    if (!nativeSetBlockMode(m_handle, true)) {
      gf::Log::error("Could not set the socket non-blocking: %s\n", getErrorString().c_str());
    }
  }

#ifdef _WIN32
  bool Socket::nativeCloseSocket(SocketHandle handle) {
    return ::closesocket(handle) == 0;
  }

  bool Socket::nativeSetBlockMode(SocketHandle handle, bool blocking) {
    u_long mode = blocking ? 1 : 0;
    return ::ioctlsocket(handle, FIONBIO, &mode) == 0;
  }

  bool Socket::nativeWouldBlock(int err) {
    return getErrorCode() == WSAEWOULDBLOCK;
  }

  auto Socket::sendLength(ArrayRef<uint8_t> buffer) -> SendLengthType {
    return static_cast<SendLengthType>(buffer.getSize());
  }

  auto Socket::sendPointer(ArrayRef<uint8_t> buffer) -> SendPointerType {
    return reinterpret_cast<SendPointerType>(buffer.getData());
  }

  auto Socket::recvLength(BufferRef<uint8_t> buffer) -> RecvLengthType {
    return static_cast<SendLengthType>(buffer.getSize());
  }

  auto Socket::recvPointer(BufferRef<uint8_t> buffer) -> RecvPointerType {
    return reinterpret_cast<RecvPointerType>(buffer.getData());
  }

  int Socket::getErrorCode() {
    return WSAGetLastError();
  }

  std::string Socket::getErrorString() {
    static constexpr std::size_t BufferSize = 1024;
    int err = getErrorCode();
    char buffer[BufferSize];
    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, BufferSize, nullptr);
    return buffer;
  }

#else
  bool Socket::nativeCloseSocket(SocketHandle handle) {
    return ::close(handle) == 0;
  }

  bool Socket::nativeSetBlockMode(SocketHandle handle, bool blocking) {
    int flags = ::fcntl(handle, F_GETFL, 0);

    if (flags == -1) {
      return false;
    }

    return ::fcntl(handle, F_SETFL, blocking ? (flags | O_NONBLOCK) : (flags & ~(O_NONBLOCK))) != -1;
  }

  bool Socket::nativeWouldBlock(int err) {
    return err == EAGAIN || err == EWOULDBLOCK;
  }

  auto Socket::sendLength(ArrayRef<uint8_t> buffer) -> SendLengthType {
    return buffer.getSize();
  }

  auto Socket::sendPointer(ArrayRef<uint8_t> buffer) -> SendPointerType {
    return static_cast<SendPointerType>(buffer.getData());
  }

  auto Socket::recvLength(BufferRef<uint8_t> buffer) -> RecvLengthType {
    return buffer.getSize();
  }

  auto Socket::recvPointer(BufferRef<uint8_t> buffer) -> RecvPointerType {
    return static_cast<RecvPointerType>(buffer.getData());
  }

  int Socket::getErrorCode() {
    return errno;
  }

  std::string Socket::getErrorString() {
    static constexpr std::size_t BufferSize = 1024;
    int err = getErrorCode();
    char buffer[BufferSize];
    ::strerror_r(err, buffer, BufferSize);
    return buffer;
  }

#endif

  auto Socket::getRemoteAddressInfo(const std::string& hostname, const std::string& service, SocketType type, SocketFamily family) -> std::vector<SocketAddressInfo> {
    return getAddressInfoEx(hostname.c_str(), service.c_str(), NoFlag, type, family);
  }

  auto Socket::getLocalAddressInfo(const std::string& service, SocketType type, SocketFamily family) -> std::vector<SocketAddressInfo> {
    return getAddressInfoEx(nullptr, service.c_str(), AI_PASSIVE, type, family);
  }

  auto Socket::getAddressInfoEx(const char *hostname, const char *service, int flags, SocketType type, SocketFamily family) -> std::vector<SocketAddressInfo> {
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
