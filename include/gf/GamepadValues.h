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
#ifndef GF_GAMEPAD_VALUES_H
#define GF_GAMEPAD_VALUES_H

#include <cstdint>
#include <map>
#include <set>
#include <tuple>

#include "Gamepad.h"
#include "GraphicsApi.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class GF_GRAPHICS_API GamepadValues {
  public:

    int16_t getLatestValue(GamepadId id, GamepadAxis axis) const;

    /**
     * @brief Process an event
     */
    void processEvent(const Event& event);

  private:
    std::map<std::tuple<GamepadId, GamepadAxis>, int16_t> m_values;
    std::set<GamepadId> m_ids;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GAMEPAD_VALUES_H
