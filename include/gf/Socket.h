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
#ifndef GF_SOCKET_H
#define GF_SOCKET_H

#include <cstddef>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif

#include "ArrayRef.h"
#include "BufferRef.h"
#include "Portability.h"
#include "SocketAddress.h"
#include "SocketGuard.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

#ifdef _WIN32
  using SocketHandle = SOCKET;
  constexpr SocketHandle InvalidSocketHandle = INVALID_SOCKET;
#else
  using SocketHandle = int;
  constexpr SocketHandle InvalidSocketHandle = -1;
#endif


  enum class SocketStatus {
    Data,
    Close,
    Error,
  };

  struct SocketDataResult {
    SocketStatus status;
    std::size_t length;
  };

  class GF_API Socket : private SocketGuard {
  public:
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    operator bool () const noexcept {
      return m_handle != InvalidSocketHandle;
    }

    SocketAddress getLocalAddress() const;

  protected:
    Socket()
    : m_handle(InvalidSocketHandle)
    {

    }

    Socket(SocketHandle handle)
    : m_handle(handle)
    {

    }

    SocketHandle getHandle() const {
      return m_handle;
    }

    static bool nativeCloseSocket(SocketHandle handle);

#ifdef _WIN32
    using SendBufferLengthType = int;
    using SendBufferPointerType = const char *;
    using RecvBufferLengthType = int;
    using RecvBufferPointerType = char *;
#else
    using SendBufferLengthType = std::size_t;
    using SendBufferPointerType = const void *;
    using RecvBufferLengthType = std::size_t;
    using RecvBufferPointerType = void *;
#endif

    static SendBufferLengthType sendBufferLength(ArrayRef<uint8_t> buffer);
    static SendBufferPointerType sendBufferPointer(ArrayRef<uint8_t> buffer);

    static RecvBufferLengthType recvBufferLength(BufferRef<uint8_t> buffer);
    static RecvBufferPointerType recvBufferPointer(BufferRef<uint8_t> buffer);

    static int getErrorCode();

#ifdef _WIN32
    static constexpr int InvalidCommunication = SOCKET_ERROR;
#else
    static constexpr ssize_t InvalidCommunication = -1;
#endif

  protected:
    enum SocketType : int {
      Tcp     = SOCK_STREAM,
      Udp     = SOCK_DGRAM,
    };

    struct SocketAddressInfo {
      SocketFamily family;
      SocketType type;
      SocketAddress address;
    };

    static constexpr int NoFlag = 0;

    static std::vector<SocketAddressInfo> getRemoteAddressInfo(const std::string& host, const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);
    static std::vector<SocketAddressInfo> getLocalAddressInfo(const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);

  private:
    static std::vector<SocketAddressInfo> getAddressInfoEx(const char *host, const char *service, int flags, SocketType type, SocketFamily family);

  private:
    SocketHandle m_handle;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_H
