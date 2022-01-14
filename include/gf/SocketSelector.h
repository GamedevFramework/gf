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
#ifndef GF_SOCKET_SELECTOR_H
#define GF_SOCKET_SELECTOR_H

#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <poll.h>
#endif

#include "NetApi.h"
#include "Socket.h"
#include "SocketGuard.h"
#include "SocketTypes.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A socket selector
   *
   * @snippet snippets/doc_class_socket_selector.cc socket_selector
   *
   */
  class GF_NET_API SocketSelector : private SocketGuard {
  public:
    /**
     * @brief Constructor
     */
    SocketSelector();

    /**
     * @brief Add a socket to the selector
     */
    void addSocket(Socket& socket);

    /**
     * @brief Remove a socket from the selector
     */
    void removeSocket(Socket& socket);

    /**
     * @brief Clear all the sockets in the selector
     */
    void clear();

    /**
     * @brief Wait for an event of the set of sockets
     *
     * A duration for a timeout can be provided. In this case, if the duration
     * is negative, it indicates an infinite timeout. If the duration is zero,
     * the call will return immediately. Otherwise, the timeout can occur after
     * the provided duration if no event occurred.
     *
     * @param duration The duration for the timeout
     */
    SocketSelectorStatus wait(Time duration = seconds(-1));

    /**
     * @brief Tell if the provided socket is ready
     */
    bool isReady(Socket& socket);

  private:
    std::vector<pollfd>::iterator find(Socket& socket);

  private:
    std::vector<pollfd> m_fds;
    bool m_sorted;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SOCKET_SELECTOR_H
