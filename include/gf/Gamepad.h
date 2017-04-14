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
#ifndef GF_GAMEPAD_H
#define GF_GAMEPAD_H

#include <cstdint>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief The gamepad buttons
   *
   * The buttons are named after the
   * [Xbox 360 controller](https://en.wikipedia.org/wiki/Xbox_360_controller).
   *
   * @sa gf::Gamepad::getButtonName()
   */
  enum class GamepadButton {
    Invalid,      ///< Invalid button
    A,            ///< The A button
    B,            ///< The B button
    X,            ///< The X button
    Y,            ///< The Y button
    Back,         ///< The Back button
    Guide,        ///< The Guide button
    Start,        ///< The Start button
    LeftStick,    ///< The left stick button
    RightStick,   ///< The right stick button
    LeftBumper,   ///< The left bumper button
    RightBumper,  ///< The right bumper button
    DPadUp,       ///< The directional pad up button
    DPadDown,     ///< The directional pad down button
    DPadLeft,     ///< The directional pad left button
    DPadRight,    ///< The directional pad right button
  };

  /**
   * @ingroup window
   * @brief The gamepad axis
   *
   * The axis are named after the
   * [Xbox 360 controller](https://en.wikipedia.org/wiki/Xbox_360_controller).
   *
   * @sa gf::Gamepad::getAxisName()
   */
  enum class GamepadAxis {
    Invalid,      ///< Invalid axis
    LeftX,        ///< The left stick X axis
    LeftY,        ///< The left stick Y axis
    RightX,       ///< The right stick X axis
    RightY,       ///< The right stick Y axis
    TriggerLeft,  ///< The left trigger axis
    TriggerRight, ///< The right trigger axis
  };

  /**
   * @ingroup window
   * @brief A gamepad axis direction
   *
   * The positive direction is right with a X axis and down with a Y axis.
   *
   * For triggers, there is only a positive direction.
   */
  enum class GamepadAxisDirection {
    Positive, ///< Positive direction of the axis.
    Negative, ///< Negative direction of the axis.
  };


  /**
   * @ingroup window
   * @brief A gamepad hardware identifier
   *
   * This identifier is given when a gamepad has just been connected. It must
   * be transformed in a gf::GamepadId with gf::Gamepad::open().
   *
   * @sa GamepadId
   */
  enum class GamepadHwId : int
  #ifdef DOXYGEN_SHOULD_SKIP_THIS
  {
  }
  #endif
  ;

  /**
   * @ingroup window
   * @brief A gamepad identifier
   *
   * This identifier is the representation of a connected gamepad that has
   * been opened with gf::Gamepad::open(). It is used in the subsequent events
   * related to this gamepad.
   *
   * @sa GamepadHwId
   */
  enum class GamepadId : int32_t
  #ifdef DOXYGEN_SHOULD_SKIP_THIS
  {
  }
  #endif
  ;

  /**
   * @ingroup window
   * @brief Some gamepad related functions
   */
  class GF_API Gamepad {
  public:
    /**
     * @brief Get the axis name
     *
     * @param axis A gamepad axis
     * @return A string representation of the axis
     */
    static const char *getAxisName(GamepadAxis axis);

    /**
     * @brief Get the button name
     *
     * @param button A gamepad button
     * @return A string representation of the button
     */
    static const char *getButtonName(GamepadButton button);

    /**
     * @name Gamepad management
     * @{
     */

    /**
     * @brief Open a gamepad
     *
     * Generally, this function is called after a GamepadConnected event.
     *
     * Example:
     *
     * ~~~{.cc}
     * if (event.type == gf::EventType::GamepadConnected) {
     *   gf::GamepadId id = gf::Gamepad::open(event.gamepadConnection.id);
     *   // do something with id
     * }
     * ~~~
     *
     * @param hwid The hardware identifier
     * @return The identifier of the gamepad
     * @sa close()
     */
    static GamepadId open(GamepadHwId hwid);

    /**
     * @brief Check if a gamepad is attached
     *
     * @param id The gamepad id
     * @return True if the gamepad is attached
     */
    static bool isAttached(GamepadId id);

    /**
     * @brief Close a gamepad
     *
     * Generally, this function is called after a GamepadDisconnected event.
     *
     * Example:
     *
     * ~~~{.cc}
     * if (event.type == gf::EventType::GamepadDisconnected) {
     *   gf::Gamepad::close(event.gamepadDisconnection.id);
     * }
     * ~~~
     *
     * @param id The gamepad id
     * @sa open()
     */
    static void close(GamepadId id);

    /**
     * @brief Get the name of the gamepad
     *
     * @param id The gamepad id
     * @return A string representation of the gamepad
     */
    static const char *getName(GamepadId id);

    /** @} */

    /**
     * @brief Initialize the already connected gamepads
     */
    static void initialize();

    /**
     * @brief Deleted constructor
     */
    Gamepad() = delete;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GAMEPAD_H
