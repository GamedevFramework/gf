/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include <gf/MessageManager.h>

#include <cassert>
#include <algorithm>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  MessageManager::MessageManager()
  : m_currentId(0)
  {
  }

  MessageHandlerId MessageManager::registerHandler(Id type, MessageHandler handler) {
    assert(handler);
    auto it = m_handlers.find(type);

    if (it == m_handlers.end()) {
      bool inserted;
      std::tie(it, inserted) = m_handlers.insert(std::make_pair(type, std::vector<Handler>()));
      assert(inserted);
    }

    MessageHandlerId id = m_currentId++;
    it->second.push_back({ id, handler });
    return id;
  }

  void MessageManager::removeHandler(MessageHandlerId id) {
    for (auto& handlers : m_handlers) {
      auto& vec = handlers.second;
      vec.erase(std::remove_if(vec.begin(), vec.end(), [id](const Handler& h) {
        return h.id == id;
      }), vec.end());
    }
  }

  void MessageManager::sendMessage(Id type, Message *event) {
    auto it = m_handlers.find(type);

    if (it == m_handlers.end()) {
      return;
    }

    std::vector<Handler> kept;

    for (auto& handler : it->second) {
      if (handler.handler(type, event) == MessageStatus::Keep) {
        kept.push_back(handler);
      }
    }

    std::swap(it->second, kept);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
