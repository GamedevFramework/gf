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
#ifndef GF_SOCKET_TYPES_H
#define GF_SOCKET_TYPES_H

#include <cstddef>

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "NetApi.h"

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
   * @brief The status of the selector
   */
  enum class SocketSelectorStatus {
    Event,    ///< An event is pending on the sockets
    Timeout,  ///< The call timed out
    Error,    ///< An error occurred
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_TYPES_H
