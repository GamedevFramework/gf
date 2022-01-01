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
#include <gf/MouseValues.h>

#include <cassert>

#include <gf/Event.h>
#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Vector2i MouseValues::getLatestValue() const {
    return m_moveValue;
  }

  Vector2i MouseValues::getLatestValue(MouseButton button) const {
    if (auto it = m_values.find(button); it != m_values.end()) {
      return it->second;
    }

    return gf::vec(0, 0);
  }

  void MouseValues::processEvent(const Event& event) {
    switch (event.type) {
      case EventType::MouseButtonPressed:
      case EventType::MouseButtonReleased:
        m_values.insert_or_assign(event.mouseButton.button, event.mouseButton.coords);
        break;

      case EventType::MouseMoved:
        m_moveValue = event.mouseCursor.coords;
        break;

      default:
        break;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
