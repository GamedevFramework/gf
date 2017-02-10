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
#ifndef GF_QUEUE_H
#define GF_QUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A simple concurrent queue
   *
   * This is a simple concurrent queue that can be used in a multi-threaded
   * application. For example, it can be used between a thread that receives
   * messages from the network and the main loop that handles the messages.
   */
  template<typename T>
  class GF_API Queue {
  public:
    /**
     * @brief Poll a value from the queue, if possible
     *
     * @param value A reference for the result
     * @return True if a value was poped from the queue
     */
    bool poll(T& value) {
      std::unique_lock<std::mutex> lock(m_mutex);

      if (m_queue.empty()) {
        return false;
      }

      value = m_queue.front();
      m_queue.pop_front();
      return true;
    }

    /**
     * @brief Push a value on the queue
     *
     * @param value The value to push on the queue
     */
    void push(const T& value) {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_queue.push_back(value);
    }

    /**
     * @brief Clear the queue
     */
    void clear() {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_queue.clear();
    }

  private:
    std::mutex m_mutex;
    std::deque<T> m_queue;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_QUEUE_H
