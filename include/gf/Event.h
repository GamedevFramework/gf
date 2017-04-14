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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_EVENT_H
#define GF_EVENT_H

#include "Gamepad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Portability.h"
#include "Rune.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief Enumeration of the different types of events
   */
  enum class EventType {
    // window events
    Resized,      ///< The window was resized (data in event.size)
    Closed,       ///< The window requested to be closed (no data)
    FocusGained,  ///< The window gained focus (no data)
    FocusLost,    ///< The window lost focus (no data)

    // inputs events
    KeyPressed,   ///< A key was pressed (data in event.key)
    KeyRepeated,  ///< A key was repeated (data in event.key)
    KeyReleased,  ///< A key was released (data in event.key)

    MouseWheelScrolled,   ///< The mouse wheel was scrolled (data in event.mouseWheel)
    MouseButtonPressed,   ///< A mouse button was pressed (data in event.mouseButton)
    MouseButtonReleased,  ///< A mouse button was released (data in event.mouseButton)
    MouseMoved,   ///< The mouse cursor moved (data in event.mouseCursor)
    MouseEntered, ///< The mouse cursor entered the window (no data)
    MouseLeft,    ///< The mouse cursor left the window (no data)

    GamepadButtonPressed,   ///< A gamepad button was pressed (data in event.gamepadButton)
    GamepadButtonReleased,  ///< A gamepad button was released (data in event.gamepadButton)
    GamepadAxisMoved,       ///< A gamepad axis was moved (data in event.gamepadAxis)
    GamepadConnected,       ///< A gamepad was connected (data in event.gamepadConnection)
    GamepadDisconnected,    ///< A gamepad was disconnected (data in event.gamepadDisconnection)

    TextEntered,  ///< A text was entered (data in event.text)
  };


  /**
   * @ingroup window
   * @brief Defines a system event and its parameters
   *
   * gf::Event holds all the informations about a system event
   * that just happened. Events are retrieved using the
   * `gf::Window::pollEvent()` and `gf::Window::waitEvent()` functions.
   *
   * A gf::Event instance contains the type of the event
   * (mouse moved, key pressed, window closed, ...) as well
   * as the details about this particular event. Please note that
   * the event parameters are defined in a union, which means that
   * only the member matching the type of the event will be properly
   * filled; all other members will have undefined values and must not
   * be read if the type of the event doesn't match. For example,
   * if you received a KeyType::KeyPressed event, then you must read the
   * `event.key` member, all other members such as `event.mouse`
   * or `event.text` will have undefined values.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::Event event;
   *
   * while (window.pollEvent(event)) {
   *   // Request for closing the window
   *   if (event.type == gf::EventType::Closed) {
   *     window.close();
   *   }
   *
   *   // The escape key was pressed
   *   if (event.type == gf::EventType::KeyPressed && event.key.keycode == gf::Keycode::Escape) {
   *     window.close();
   *   }
   *
   *   // The window was resized
   *   if (event.type == gf::EventType::Resized) {
   *     doSomethingWithTheNewSize(event.size.width, event.size.height);
   *   }
   *
   *   // etc ...
   * }
   * ~~~
   *
   * @sa gf::EventType
   */
  struct GF_API Event {
    /**
     * @brief Keyboard event parameters (EventType::KeyPressed, EventType::KeyReleased, EventType::KeyRepeated)
     */
    struct KeyEvent {
      Keycode keycode;      ///< Keycode of the key
      Scancode scancode;    ///< Scancode of the key
      Modifiers modifiers;  ///< Modifiers that are pressed
    };

    /**
     * @brief Text event parameters (EventType::TextEntered)
     */
    struct TextEvent {
      Rune rune; ///< The rune
    };

    /**
     * @brief Mouse button event parameters (EventType::MouseButtonPressed, EventType::MouseButtonReleased)
     */
    struct MouseButtonEvent {
      MouseButton button;   ///< Code of the button that has been pressed
      Vector2i coords;      ///< Position of the mouse cursor
    };

    /**
     * @brief Mouse cursor move event parameters (EventType::MouseMoved)
     */
    struct MouseCursorEvent {
      Vector2i coords;  ///< Position of the mouse cursor
    };

    /**
     * @brief Mouse wheel event parameters (EventType::MouseWheelScrolled)
     */
    struct MouseWheelEvent {
      Vector2i offset;  ///< Offset of the mouse wheel
    };

    /**
     * @brief Gamepad button event parameters (EventType::GamepadButtonPressed, EventType::GamepadButtonReleased)
     */
    struct GamepadButtonEvent {
      GamepadId id; ///< Id of the gamepad
      GamepadButton button; ///< Button of the gamepad
    };

    /**
     * @brief Gamepad axis event parameters (EventType::GamepadAxisMoved)
     */
    struct GamepadAxisEvent {
      GamepadId id; ///< Id of the gamepad
      GamepadAxis axis; ///< Axis of the gamepad
      int16_t value; ///< Value of the axis
    };

    /**
     * @brief Gamepad connection event parameters (EventType::GamepadConnected)
     */
    struct GamepadConnection {
      GamepadHwId id; ///< Hardware id of the gamepad
    };

    /**
     * @brief Gamepad disconnection event parameters (EventType::GamepadDisconnected)
     */
    struct GamepadDisconnection {
      GamepadId id; ///< Id of the gamepad
    };

    EventType type; ///< Type of the event

    union {
      Vector2u size; ///< Size event parameters (EventType::Resized)
      KeyEvent key; ///< Key event parameters (EventType::KeyPressed, EventType::KeyReleased, EventType::KeyRepeated)
      TextEvent text; ///< Text event parameters (EventType::TextEntered)
      MouseButtonEvent mouseButton; ///< Mouse button event parameters (EventType::MouseButtonPressed, EventType::MouseButtonReleased)
      MouseCursorEvent mouseCursor; ///< Mouse cursor move event parameters (EventType::MouseMoved)
      MouseWheelEvent mouseWheel; ///< Mouse wheel event parameters (EventType::MouseWheelScrolled)
      GamepadButtonEvent gamepadButton; ///< Gamepad button event parameters (EventType::GamepadButtonPressed, EventType::GamepadButtonReleased)
      GamepadAxisEvent gamepadAxis; ///< Gamepad axis event parameters (EventType::GamepadAxisMoved)
      GamepadConnection gamepadConnection; ///< Gamepad connection event parameters (EventType::GamepadConnected)
      GamepadDisconnection gamepadDisconnection; ///< Gamepad disconnection event parameters (EventType::GamepadDisconnected)
    };

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GL_EVENT_H
