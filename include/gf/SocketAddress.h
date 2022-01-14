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
#ifndef GF_SOCKET_ADDRESS_H
#define GF_SOCKET_ADDRESS_H

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include "NetApi.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A socket family
   *
   * A socket family indicates the type of the socket. The family can be
   * specified explicitly or unspecified, in which case the family is
   * set automatically by the system.
   *
   * @sa gf::SocketAddress
   */
  enum class SocketFamily : int {
    Unspec  = AF_UNSPEC,  ///< Unspecified (either IPv4 or IPv6)
    IPv4    = AF_INET,    ///< IPv4 (Internet Protocol version 4)
    IPv6    = AF_INET6,   ///< IPv6 (Internet Protocol version 6)
  };

  /**
   * @ingroup net_sockets
   * @brief A socket address format
   *
   * @sa gf::SocketAddress
   */
  enum class SocketAddressFormat {
    Unrestricted, ///< Allow name resolution for the address
    Numeric,      ///< Use a numeric form for the address
  };

  /**
   * @ingroup net_sockets
   * @brief A socket address
   *
   * A socket address is composed of an IP address (representing an hostname)
   * and a port number (representing a service).
   *
   * You can not create a socket address directly.
   *
   * In other libraries, it can also be called "endpoint".
   */
  class GF_NET_API SocketAddress {
  public:
    /**
     * @brief Default constructor
     */
    SocketAddress() = default;

    /**
     * @brief Get the family of the socket address
     */
    SocketFamily getFamily() const;

    /**
     * @brief Get the hostname associated to the address
     *
     * @param format The format of the socket address
     * @returns The hostname part of the address as a string
     * @sa getService()
     */
    std::string getHostname(SocketAddressFormat format = SocketAddressFormat::Unrestricted) const;

    /**
     * @brief Get the service associated to the address
     *
     * @param format The format of the socket address
     * @returns The service part of the address as a string
     * @sa getHostname()
     */
    std::string getService(SocketAddressFormat format = SocketAddressFormat::Unrestricted) const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef _WIN32
    using StorageLengthType = int;
#else
    using StorageLengthType = socklen_t;
#endif
#else
    /**
     * @brief The type to handle the storage length
     */
    using StorageLengthType = implementation-defined;
#endif

    /**
     * @brief Constructor with a raw sockaddr and a length
     */
    SocketAddress(sockaddr *a_storage, StorageLengthType a_length);

    /**
     * @brief Get the storage as a sockaddr address
     */
    const sockaddr *asSockAddr() const {
      return reinterpret_cast<const sockaddr*>(&storage);
    }

    sockaddr_storage storage; ///< The storage for the address
    StorageLengthType length; ///< The length of the address
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_ADDRESS_H
