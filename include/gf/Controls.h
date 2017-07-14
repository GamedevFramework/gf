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
#ifndef GF_CONTROLS_H
#define GF_CONTROLS_H

#include "Control.h"
#include "Gamepad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief A key control based on keycode.
   */
  class GF_API KeycodeKeyControl : public Control {
  public:
    /**
     * @brief Construct a key control.
     *
     * @param code The keycode of the key
     */
    explicit KeycodeKeyControl(Keycode code);

    virtual void processEvent(const Event& event) override;

  private:
    Keycode m_code;
  };

  /**
   * @ingroup window
   * @brief A key control based on scancode.
   */
  class GF_API ScancodeKeyControl : public Control {
  public:
    /**
     * @brief Construct a key control.
     *
     * @param code The scancode of the key
     */
    explicit ScancodeKeyControl(Scancode code);

    virtual void processEvent(const Event& event) override;

  private:
    Scancode m_code;
  };

  /**
   * @ingroup window
   * @brief A mouse button control.
   */
  class GF_API MouseButtonControl : public Control {
  public:
    /**
     * @brief Construct a mouse button control.
     *
     * @param button The button of the mouse.
     */
    explicit MouseButtonControl(MouseButton button);

    virtual void processEvent(const Event& event) override;

  private:
    MouseButton m_button;
  };

  /**
   * @ingroup window
   * @brief A gamepad button control
   */
  class GF_API GamepadButtonControl : public Control {
  public:
    /**
     * @brief Contruct a gamepad button control.
     *
     * @param id The id of the gamepad
     * @param button The button of the gamepad
     * @sa gf::Gamepad::open
     */
    GamepadButtonControl(GamepadId id, GamepadButton button);

    virtual void processEvent(const Event& event) override;

  private:
    GamepadId m_id;
    GamepadButton m_button;
  };

  /**
   * @ingroup window
   * @brief A gamepad axis control
   */
  class GF_API GamepadAxisControl : public Control {
  public:
    /**
     * @brief Construct a gamepad axis control
     *
     * @param id The id of the gamepad
     * @param axis The axis of the gamepad
     * @param dir The direction of the axis
     * @sa gf::Gamepad::open, gf::GamepadAxisDirection
     */
    GamepadAxisControl(GamepadId id, GamepadAxis axis, GamepadAxisDirection dir);

    virtual void processEvent(const Event& event) override;

  private:
    GamepadId m_id;
    GamepadAxis m_axis;
    GamepadAxisDirection m_dir;
  };

  /**
   * @ingroup window
   * @brief A close control.
   *
   */
  class GF_API CloseControl : public Control {
  public:
    /**
     * @brief Construct a close control.
     */
    CloseControl();

    virtual void processEvent(const Event& event) override;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CONTROLS_H
