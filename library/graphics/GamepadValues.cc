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
#include <gf/GamepadValues.h>

#include <cassert>

#include <gf/Event.h>
#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  int16_t GamepadValues::getLatestValue(GamepadId id, GamepadAxis axis) const {
    if (id == AnyGamepad) {
      if (m_ids.empty()) {
        gf::Log::warning("There is no gamepad connected, you should not use AnyGamepad.\n");
        return 0;
      }

      if (m_ids.size() > 1) {
        gf::Log::warning("There is more than one gamepad connected, you should not use AnyGamepad.\n");
      }

      id = *m_ids.begin();
    }

    if (auto it = m_values.find(std::make_tuple(id, axis)); it != m_values.end()) {
      return it->second;
    }

    return 0;
  }

  void GamepadValues::processEvent(const Event& event) {
    switch (event.type) {
      case EventType::GamepadAxisMoved:
        m_ids.insert(event.gamepadAxis.id);
        m_values.insert_or_assign(std::make_tuple(event.gamepadAxis.id, event.gamepadAxis.axis), event.gamepadAxis.value);
        break;

      case EventType::GamepadDisconnected:
        m_ids.erase(event.gamepadDisconnection.id);
        break;

      default:
        break;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
