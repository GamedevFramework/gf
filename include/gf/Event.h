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
#ifndef GF_EVENT_H
#define GF_EVENT_H

#include <cstdint>

#include "Gamepad.h"
#include "GraphicsApi.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Rune.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_events
   * @brief Enumeration of the different types of events
   */
  enum class EventType {
    // global event
    Quit,         ///< The application is asked to quit

    // window events
    Resized,      ///< The window was resized (data in event.resize)
    Closed,       ///< The window requested to be closed (data in event.window)
    FocusGained,  ///< The window gained focus (data in event.window)
    FocusLost,    ///< The window lost focus (data in event.window)
    Shown,        ///< The window is shown (data in event.window)
    Hidden,       ///< The window is hidden (data in event.window)
    Exposed,      ///< The window is exposed (data in event.window)
    Minimized,    ///< The window is minimized (data in event.window)
    Maximized,    ///< The window is maximized (data in event.window)
    Restored,     ///< The window is restored (data in event.window)

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

    TouchBegan, ///< A touch began
    TouchMoved, ///< A touch moved
    TouchEnded, ///< A touch ended
  };

  /**
   * @ingroup graphics_events
   * @brief Window event parameters (EventType::Closed, EventType::FocusGained, EventType::FocusLost, EventType::Shown, EventType::Hidden, EventType::Exposed, EventType::Minimized, EventType::Maximized, EventType::Restored)
   */
  struct WindowEvent {
    uint32_t windowId; ///< The window id of the event
  };

  /**
   * @ingroup graphics_events
   * @brief Resize event parameters (EventType::Resized)
   */
  struct ResizeEvent {
    uint32_t windowId;  ///< The window id of the event
    Vector2i size;      ///< The new size of the window
  };

  /**
   * @ingroup graphics_events
   * @brief Keyboard event parameters (EventType::KeyPressed, EventType::KeyReleased, EventType::KeyRepeated)
   */
  struct KeyEvent {
    uint32_t windowId;    ///< The window id of the event
    Keycode keycode;      ///< Keycode of the key
    Scancode scancode;    ///< Scancode of the key
    Flags<Mod> modifiers; ///< Modifiers that are pressed
  };

  /**
   * @ingroup graphics_events
   * @brief Text event parameters (EventType::TextEntered)
   */
  struct TextEvent {
    uint32_t windowId;  ///< The window id of the event
    Rune rune;          ///< The rune
  };

  /**
   * @ingroup graphics_events
   * @brief Mouse button event parameters (EventType::MouseButtonPressed, EventType::MouseButtonReleased)
   */
  struct MouseButtonEvent {
    uint32_t windowId;    ///< The window id of the event
    MouseButton button;   ///< Code of the button that has been pressed
    Vector2i coords;      ///< Position of the mouse cursor
    uint8_t clicks;       ///< Number of clicks
  };

  /**
   * @ingroup graphics_events
   * @brief Mouse cursor move event parameters (EventType::MouseMoved)
   */
  struct MouseCursorEvent {
    uint32_t windowId;  ///< The window id of the event
    Vector2i coords;    ///< Position of the mouse cursor
    Vector2i motion;    ///< Relative position of the mouse cursor
  };

  /**
   * @ingroup graphics_events
   * @brief Mouse wheel event parameters (EventType::MouseWheelScrolled)
   */
  struct MouseWheelEvent {
    uint32_t windowId;  ///< The window id of the event
    Vector2i offset;    ///< Offset of the mouse wheel
  };

  /**
   * @ingroup graphics_events
   * @brief Gamepad button event parameters (EventType::GamepadButtonPressed, EventType::GamepadButtonReleased)
   */
  struct GamepadButtonEvent {
    GamepadId id;         ///< Id of the gamepad
    GamepadButton button; ///< Button of the gamepad
  };

  /**
   * @ingroup graphics_events
   * @brief Gamepad axis event parameters (EventType::GamepadAxisMoved)
   */
  struct GamepadAxisEvent {
    GamepadId id;     ///< Id of the gamepad
    GamepadAxis axis; ///< Axis of the gamepad
    int16_t value;    ///< Value of the axis
  };

  /**
   * @ingroup graphics_events
   * @brief Gamepad connection event parameters (EventType::GamepadConnected)
   */
  struct GamepadConnectionEvent {
    GamepadHwId id; ///< Hardware id of the gamepad
  };

  /**
   * @ingroup graphics_events
   * @brief Gamepad disconnection event parameters (EventType::GamepadDisconnected)
   */
  struct GamepadDisconnectionEvent {
    GamepadId id; ///< Id of the gamepad
  };

  /**
   * @ingroup graphics_events
   * @brief Touch event parameters (EventType::TouchBegan, EventType::TouchMoved, EventType::TouchEnded)
   */
  struct TouchEvent {
    int64_t finger;   ///< Finger that touched
    Vector2i coords;  ///< Position of the touch
    Vector2i motion;  ///< Relative position of the touch
    float pressure;   ///< The pressure applied (between 0 and 1)
  };

  /**
   * @ingroup graphics_events
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
   * @snippet snippets/doc_struct_event.cc event
   *
   * @sa gf::EventType
   */
  struct GF_GRAPHICS_API Event {
    EventType type;     ///< Type of the event
    uint32_t timestamp; ///< A timestamp

    union {
      WindowEvent window; ///< Window event parameters (EventType::Closed, EventType::FocusGained, EventType::FocusLost)
      ResizeEvent resize; ///< Size event parameters (EventType::Resized)
      KeyEvent key; ///< Key event parameters (EventType::KeyPressed, EventType::KeyReleased, EventType::KeyRepeated)
      TextEvent text; ///< Text event parameters (EventType::TextEntered)
      MouseButtonEvent mouseButton; ///< Mouse button event parameters (EventType::MouseButtonPressed, EventType::MouseButtonReleased)
      MouseCursorEvent mouseCursor; ///< Mouse cursor move event parameters (EventType::MouseMoved)
      MouseWheelEvent mouseWheel; ///< Mouse wheel event parameters (EventType::MouseWheelScrolled)
      GamepadButtonEvent gamepadButton; ///< Gamepad button event parameters (EventType::GamepadButtonPressed, EventType::GamepadButtonReleased)
      GamepadAxisEvent gamepadAxis; ///< Gamepad axis event parameters (EventType::GamepadAxisMoved)
      GamepadConnectionEvent gamepadConnection; ///< Gamepad connection event parameters (EventType::GamepadConnected)
      GamepadDisconnectionEvent gamepadDisconnection; ///< Gamepad disconnection event parameters (EventType::GamepadDisconnected)
      TouchEvent touch; ///< Touch event parameters (EventType::TouchBegan, EventType::Moved, EventType::Ended)
    };

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GL_EVENT_H
