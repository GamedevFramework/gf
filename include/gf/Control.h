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
#ifndef GF_CONTROL_H
#define GF_CONTROL_H

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct Event;

  /**
   * @ingroup window
   * @brief A physical control.
   *
   */
  class GF_API Control {
  public:
    /**
     * @brief Construct a control
     *
     */
    Control()
    : m_active(false)
    {
    }

    /**
     * @brief Destroy the control.
     */
    virtual ~Control();

    /**
     * @brief Check if the control is active.
     *
     * @return true if the control is active.
     *
     * @sa Action::isActive()
     */
    bool isActive() {
      return m_active;
    }

    /**
     * @brief Change the active state of the control.
     *
     * @param active The new state of the control.
     */
    void setActive(bool active = true) {
      m_active = active;
    }

    /**
     * @brief Desactivate the control.
     *
     * This call is equivalent to:
     *
     * ~~~{.cc}
     * this.setActive(false);
     * ~~~
     *
     * @sa Action::reset()
     */
    void reset() {
      m_active = false;
    }

    /**
     * @brief Update the state of the control thanks to an event.
     *
     * @param event The event to update the control.
     *
     * @sa Action::processEvent()
     */
    virtual void processEvent(const Event& event) = 0;

  private:
    bool m_active;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CONTROL_H
