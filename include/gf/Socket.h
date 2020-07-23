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
#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif

#include "ArrayRef.h"
#include "BufferRef.h"
#include "NetApi.h"
#include "SocketAddress.h"
#include "SocketGuard.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef _WIN32
  using SocketHandle = SOCKET;
  constexpr SocketHandle InvalidSocketHandle = INVALID_SOCKET;
#else
  using SocketHandle = int;
  constexpr SocketHandle InvalidSocketHandle = -1;
#endif
#else
  /**
   * @ingroup net_sockets
   * @brief A native socket handle
   *
   * This type represents the native socket type. It is implementation-defined.
   * Normally, you should not have to manipulate this type.
   *
   * @sa gf::TcpListener, gf::TcpSocket, gf::UdpSocket
   */
  using SocketHandle = implementation-defined;

  /**
   * @ingroup net_sockets
   * @brief An invalid socket handle
   *
   * This constant is used internally to represent an invalid socket. It is
   * implementation-defined. Normally, you should not have to manipulate this
   * constant.
   *
   * @sa gf::SocketHandle, gf::Socket
   */
  constexpr SocketHandle InvalidSocketHandle = implemetation-defined;
#endif

  /**
   * @ingroup net_sockets
   * @brief The status of a socket operation
   *
   * @sa gf::SocketDataResult
   */
  enum class SocketStatus {
    Data,   ///< Some data has been sent or received
    Block,  ///< The socket would have blocked
    Close,  ///< The connection is closed
    Error,  ///< An error occurred
  };

  /**
   * @ingroup net_sockets
   * @brief The result of a socket operation
   *
   * @sa gf::SocketStatus
   */
  struct GF_NET_API SocketDataResult {
    SocketStatus status;  ///< The status of the operation
    std::size_t length;   ///< The length of sent or received data
  };

  /**
   * @ingroup net_sockets
   * @brief A network socket
   *
   *
   * @sa gf::TcpListener, gf::TcpSocket, gf::UdpSocket
   */
  class GF_NET_API Socket : private SocketGuard {
  public:
    /**
     * @brief Destructor
     *
     * The socket is closed.
     */
    ~Socket();

    /**
     * @brief Deleted copy constructor
     */
    Socket(const Socket&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Socket& operator=(const Socket&) = delete;

    /**
     * @brief Move constructor
     *
     * @param other The socket from which to move the socket handle
     */
    Socket(Socket&& other) noexcept;

    /**
     * @brief Move assignment
     *
     * @param other The socket from which to move the socket handle
     * @returns A reference to the socket
     */
    Socket& operator=(Socket&& other) noexcept;

    /**
     * @brief Boolean conversion
     *
     * @returns True if the socket is valid
     */
    operator bool () const noexcept {
      return m_handle != InvalidSocketHandle;
    }

    /**
     * @brief Get the local address of the socket
     *
     * @returns The local address of the socket
     */
    SocketAddress getLocalAddress() const;

    /**
     * @brief Set the socket in blocking mode
     *
     * @sa setNonBlocking()
     */
    void setBlocking();

    /**
     * @brief Set the socket in non-blocking mode
     *
     * @sa setBlocking()
     */
    void setNonBlocking();

  protected:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    Socket()
    : m_handle(InvalidSocketHandle)
    {

    }

    void setHandle(SocketHandle handle) {
      m_handle = handle;
    }

    SocketHandle getHandle() const {
      return m_handle;
    }

    static bool nativeCloseSocket(SocketHandle handle);
    static bool nativeSetBlockMode(SocketHandle handle, bool blocking);
    static bool nativeWouldBlock(int err);

#ifdef _WIN32
    using SendLengthType = int;
    using SendPointerType = const char *;
    using RecvLengthType = int;
    using RecvPointerType = char *;
#else
    using SendLengthType = std::size_t;
    using SendPointerType = const void *;
    using RecvLengthType = std::size_t;
    using RecvPointerType = void *;
#endif

    static SendLengthType sendLength(ArrayRef<uint8_t> buffer);
    static SendPointerType sendPointer(ArrayRef<uint8_t> buffer);

    static RecvLengthType recvLength(BufferRef<uint8_t> buffer);
    static RecvPointerType recvPointer(BufferRef<uint8_t> buffer);

    static int getErrorCode();
    static std::string getErrorString();

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

    static std::vector<SocketAddressInfo> getRemoteAddressInfo(const std::string& hostname, const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);
    static std::vector<SocketAddressInfo> getLocalAddressInfo(const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);
#endif

  private:
    static std::vector<SocketAddressInfo> getAddressInfoEx(const char *hostname, const char *service, int flags, SocketType type, SocketFamily family);

  protected:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    struct Header {
      uint8_t data[8];
    };

    static constexpr Header encodeHeader(uint64_t size) {
      Header header = {{ 0, 0, 0, 0, 0, 0, 0, 0 }};

      for (int i = 0; i < 8; ++i) {
        header.data[8 - i - 1] = static_cast<uint8_t>(size % 0x100);
        size >>= 8;
      }

      return header;
    }

    static constexpr uint64_t decodeHeader(const Header& header) {
      uint64_t size = 0;

      for (uint64_t x : header.data) {
        size = (size << 8) + x;
      }

      return size;
    }
#endif
  private:
    friend class SocketSelector;
    SocketHandle m_handle;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_H
