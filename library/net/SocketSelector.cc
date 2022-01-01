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
#include <gf/SocketSelector.h>

#include <algorithm>

#include <gf/Log.h>

#include <gfpriv/SocketPrivate.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    struct PollFdEqualTo {
      explicit PollFdEqualTo(pollfd reference)
      : m_reference(reference)
      {
      }

      bool operator()(const pollfd& other) {
        return m_reference.fd == other.fd;
      }

    private:
      pollfd m_reference;
    };

    bool pollfdLess(const pollfd& lhs, const pollfd& rhs) {
      return lhs.fd < rhs.fd;
    }

  }

  SocketSelector::SocketSelector()
  : m_sorted(true)
  {
  }

  void SocketSelector::addSocket(Socket& socket) {
    m_fds.push_back(pollfd{ socket.m_handle, POLLIN, 0 });
    m_sorted = false;
  }

  void SocketSelector::removeSocket(Socket& socket) {
    m_fds.erase(std::remove_if(m_fds.begin(), m_fds.end(), PollFdEqualTo(pollfd{ socket.m_handle, 0, 0 })), m_fds.end());
    // the vector is still sorted (or unsorted)
  }

  void SocketSelector::clear() {
    m_fds.clear();
    m_sorted = true;
  }

  SocketSelectorStatus SocketSelector::wait(Time duration) {
    if (m_fds.empty()) {
      Log::error("Error! The selector does not contain any socket.\n");
      return SocketSelectorStatus::Error;
    }

    if (!m_sorted) {
      std::sort(m_fds.begin(), m_fds.end(), pollfdLess);
      m_sorted = true;
    }

    return priv::nativePoll(m_fds, duration);
  }

  bool SocketSelector::isReady(Socket& socket) {
    auto it = find(socket);

    if (it == m_fds.end()) {
      Log::error("Error! Searching for a socket that is not in the selector.\n");
      return false;
    }

    return it->revents != 0;
  }

  std::vector<pollfd>::iterator SocketSelector::find(Socket& socket) {
    if (m_sorted) {
      auto it = std::lower_bound(m_fds.begin(), m_fds.end(), pollfd{ socket.m_handle, 0, 0 }, pollfdLess);

      if (it != m_fds.end() && it->fd == socket.m_handle) {
        return it;
      }

      return m_fds.end();
    }

    return std::find_if(m_fds.begin(), m_fds.end(), PollFdEqualTo(pollfd{ socket.m_handle, 0, 0 }));
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

