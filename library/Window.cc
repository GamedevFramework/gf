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
#include <gf/Window.h>

#include <cassert>
#include <cstring>

#include <SDL.h>

#include <glad/glad.h>

#include <gf/Cursor.h>
#include <gf/Event.h>
#include <gf/Gamepad.h>
#include <gf/Keyboard.h>
#include <gf/Log.h>
#include <gf/Mouse.h>
#include <gf/Sleep.h>
#include <gf/Vector.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static Uint32 getFlagsFromHints(WindowFlags hints) {
    Uint32 flags = SDL_WINDOW_OPENGL;

    if (hints.test(WindowHints::Resizable)) {
      flags |= SDL_WINDOW_RESIZABLE;
    }

    if (hints.test(WindowHints::Visible)) {
      flags |= SDL_WINDOW_SHOWN;
    } else {
      flags |= SDL_WINDOW_HIDDEN;
    }

    if (!hints.test(WindowHints::Decorated)) {
      flags |= SDL_WINDOW_BORDERLESS;
    }

    return flags;
  }

  static void *createContextFromWindow(SDL_Window *window) {
    if (window == nullptr) {
      return nullptr;
    }

    void *context = SDL_GL_CreateContext(window);

    if (context == nullptr) {
      Log::error("Failed to create a context: %s\n", SDL_GetError());
      return nullptr;
    }

    int err = SDL_GL_MakeCurrent(window, context);

    if (err != 0) {
      Log::error("Failed to make the context current: %s\n", SDL_GetError());
    }

    if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress)) {
      Log::error("Failed to load GLES2.\n");
    }

    return context;
  }

  Window::Window(StringRef title, Vector2u size, WindowFlags hints)
  : m_window(nullptr)
  , m_context(nullptr)
  , m_shouldClose(false)
  , m_isFullscreen(false)
  {
    auto flags = getFlagsFromHints(hints);
    m_window = SDL_CreateWindow(title.getData(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, flags);
    m_context = createContextFromWindow(m_window);

    if (m_context) {
      glCheck(glEnable(GL_BLEND));
      glCheck(glEnable(GL_SCISSOR_TEST));
      glCheck(glClear(GL_COLOR_BUFFER_BIT));
    }
  }

  Window::~Window() {
    if (m_context) {
      SDL_GL_DeleteContext(m_context);
    }

    if (m_window) {
      SDL_DestroyWindow(m_window);
    }
  }

  bool Window::isOpen() {
    return !m_shouldClose;
  }

  void Window::close() {
    assert(m_window);
    m_shouldClose = true;
  }

  void Window::setTitle(StringRef title) {
    assert(m_window);
    SDL_SetWindowTitle(m_window, title.getData());
  }


  Vector2i Window::getPosition() const {
    assert(m_window);
    Vector2i position;
    SDL_GetWindowPosition(m_window, &position.x, &position.y);
    return position;
  }

  void Window::setPosition(Vector2i position) {
    assert(m_window);
    SDL_SetWindowPosition(m_window, position.x, position.y);
  }


  Vector2u Window::getSize() const {
    assert(m_window);
    Vector2i size;
    SDL_GetWindowSize(m_window, &size.width, &size.height);
    return size;
  }

  void Window::setSize(Vector2u size) {
    assert(m_window);
    SDL_SetWindowSize(m_window, size.width, size.height);
  }

  Vector2u Window::getFramebufferSize() const {
    assert(m_window);
    Vector2i size;
    SDL_GL_GetDrawableSize(m_window, &size.width, &size.height);
    return size;
  }

  void Window::setFullscreen(bool full) {
    assert(m_window);

    if (full) {
      SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
      SDL_SetWindowFullscreen(m_window, 0);
    }

    m_isFullscreen = full;
  }

  void Window::toggleFullscreen() {
    setFullscreen(!m_isFullscreen);
  }

  void Window::minimize() {
    assert(m_window);
    SDL_MinimizeWindow(m_window);
  }

  void Window::restore() {
    assert(m_window);
    SDL_RestoreWindow(m_window);
  }

  void Window::maximize() {
    assert(m_window);
    SDL_MaximizeWindow(m_window);
  }

  void Window::show() {
    assert(m_window);
    SDL_ShowWindow(m_window);
  }

  void Window::hide() {
    assert(m_window);
    SDL_HideWindow(m_window);
  }

  void Window::setVisible(bool visible) {
    if (visible) {
      show();
    } else {
      hide();
    }
  }

  void Window::setDecorated(bool decorated) {
    assert(m_window);
    SDL_SetWindowBordered(m_window, decorated ? SDL_TRUE : SDL_FALSE);
  }

  bool Window::isFocused() const {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
  }

  bool Window::isMinimized() const {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_MINIMIZED) != 0;
  }

  bool Window::isVisible() const {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_SHOWN) != 0;
  }

  bool Window::isResizable() const {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_RESIZABLE) != 0;
  }

  bool Window::isDecorated() const {
    assert(m_window);
    auto flags = SDL_GetWindowFlags(m_window);
    return (flags & SDL_WINDOW_BORDERLESS) == 0;
  }

  static MouseButton getMouseButtonFromButton(Uint8 button) {
    switch (button) {
      case SDL_BUTTON_LEFT:
        return MouseButton::Left;
      case SDL_BUTTON_MIDDLE:
        return MouseButton::Middle;
      case SDL_BUTTON_RIGHT:
        return MouseButton::Right;
      case SDL_BUTTON_X1:
        return MouseButton::XButton1;
      case SDL_BUTTON_X2:
        return MouseButton::XButton2;
    }

    /*
     * Another button may happen in the case of a touchpad (value 6 or 7),
     * which happens when pressing with two fingers.
     */
    return MouseButton::Other;
  }

  static GamepadButton getGamepadButtonFromButton(Uint8 button) {
    switch (button) {
      case SDL_CONTROLLER_BUTTON_A:
        return GamepadButton::A;
      case SDL_CONTROLLER_BUTTON_B:
        return GamepadButton::B;
      case SDL_CONTROLLER_BUTTON_X:
        return GamepadButton::X;
      case SDL_CONTROLLER_BUTTON_Y:
        return GamepadButton::Y;
      case SDL_CONTROLLER_BUTTON_BACK:
        return GamepadButton::Back;
      case SDL_CONTROLLER_BUTTON_GUIDE:
        return GamepadButton::Guide;
      case SDL_CONTROLLER_BUTTON_START:
        return GamepadButton::Start;
      case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        return GamepadButton::LeftStick;
      case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        return GamepadButton::RightStick;
      case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        return GamepadButton::LeftBumper;
      case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        return GamepadButton::RightBumper;
      case SDL_CONTROLLER_BUTTON_DPAD_UP:
        return GamepadButton::DPadUp;
      case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        return GamepadButton::DPadDown;
      case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        return GamepadButton::DPadLeft;
      case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        return GamepadButton::DPadRight;
    }

    assert(false);
    return GamepadButton::Invalid;
  }

  static GamepadAxis getGamepadAxisFromAxis(Uint8 axis) {
    switch (axis) {
      case SDL_CONTROLLER_AXIS_LEFTX:
        return GamepadAxis::LeftX;
      case SDL_CONTROLLER_AXIS_LEFTY:
        return GamepadAxis::LeftY;
      case SDL_CONTROLLER_AXIS_RIGHTX:
        return GamepadAxis::RightX;
      case SDL_CONTROLLER_AXIS_RIGHTY:
        return GamepadAxis::RightY;
      case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
        return GamepadAxis::TriggerLeft;
      case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
        return GamepadAxis::TriggerRight;
    }

    assert(false);
    return GamepadAxis::Invalid;
  }

  static Modifiers getModifiersFromMod(Uint16 mod) {
    Modifiers modifiers(None);

    if ((mod & KMOD_SHIFT) != 0) {
      modifiers |= Mod::Shift;
    }

    if ((mod & KMOD_CTRL) != 0) {
      modifiers |= Mod::Control;
    }

    if ((mod & KMOD_ALT) != 0) {
      modifiers |= Mod::Alt;
    }

    if ((mod & KMOD_GUI) != 0) {
      modifiers |= Mod::Super;
    }

    return modifiers;
  }


  static bool translateEvent(Uint32 windowId, const SDL_Event *in, Event& out) {
    switch (in->type) {
      case SDL_WINDOWEVENT:
        if (windowId != in->window.windowID) {
          return false;
        }

        switch (in->window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            out.type = EventType::Resized;
            out.size.width = in->window.data1;
            out.size.height = in->window.data2;
            break;

          case SDL_WINDOWEVENT_CLOSE:
            out.type = EventType::Closed;
            break;

          case SDL_WINDOWEVENT_FOCUS_GAINED:
            out.type = EventType::FocusGained;
            break;

          case SDL_WINDOWEVENT_FOCUS_LOST:
            out.type = EventType::FocusLost;
            break;

          case SDL_WINDOWEVENT_ENTER:
            out.type = EventType::MouseEntered;
            break;

          case SDL_WINDOWEVENT_LEAVE:
            out.type = EventType::MouseLeft;
            break;

          default:
            return false;
        }
        break;

      case SDL_QUIT:
        out.type = EventType::Closed;
        break;

      case SDL_KEYDOWN:
        assert(in->key.state == SDL_PRESSED);

        if (in->key.repeat == 0) {
          out.type = EventType::KeyPressed;
        } else {
          out.type = EventType::KeyRepeated;
        }

        out.key.keycode = static_cast<Keycode>(in->key.keysym.sym);
        out.key.scancode = static_cast<Scancode>(in->key.keysym.scancode);
        out.key.modifiers = getModifiersFromMod(in->key.keysym.mod);
        break;

      case SDL_KEYUP:
        assert(in->key.state == SDL_RELEASED);
        out.type = EventType::KeyReleased;
        out.key.keycode = static_cast<Keycode>(in->key.keysym.sym);
        out.key.scancode = static_cast<Scancode>(in->key.keysym.scancode);
        out.key.modifiers = getModifiersFromMod(in->key.keysym.mod);
        break;

      case SDL_MOUSEWHEEL:
        if (in->wheel.which == SDL_TOUCH_MOUSEID) {
          return false;
        }

        out.type = EventType::MouseWheelScrolled;
        out.mouseWheel.offset.x = in->wheel.x;
        out.mouseWheel.offset.y = in->wheel.y;
        // TODO: handle SDL_MOUSEWHEEL_FLIPPED?
        break;

      case SDL_MOUSEBUTTONDOWN:
        assert(in->button.state == SDL_PRESSED);

        if (in->button.which == SDL_TOUCH_MOUSEID) {
          return false;
        }

        out.type = EventType::MouseButtonPressed;
        out.mouseButton.button = getMouseButtonFromButton(in->button.button);
        out.mouseButton.coords.x = in->button.x;
        out.mouseButton.coords.y = in->button.y;
        break;

      case SDL_MOUSEBUTTONUP:
        assert(in->button.state == SDL_RELEASED);

        if (in->button.which == SDL_TOUCH_MOUSEID) {
          return false;
        }

        out.type = EventType::MouseButtonReleased;
        out.mouseButton.button = getMouseButtonFromButton(in->button.button);
        out.mouseButton.coords.x = in->button.x;
        out.mouseButton.coords.y = in->button.y;
        break;

      case SDL_MOUSEMOTION:
        if (in->motion.which == SDL_TOUCH_MOUSEID) {
          return false;
        }

        out.type = EventType::MouseMoved;
        out.mouseCursor.coords.x = in->motion.x;
        out.mouseCursor.coords.y = in->motion.y;
        break;

      case SDL_CONTROLLERDEVICEADDED:
        assert(SDL_IsGameController(in->cdevice.which));
        out.type = EventType::GamepadConnected;
        out.gamepadConnection.id = static_cast<GamepadHwId>(in->cdevice.which);
        break;

      case SDL_CONTROLLERDEVICEREMOVED:
        out.type = EventType::GamepadDisconnected;
        out.gamepadDisconnection.id = static_cast<GamepadId>(in->cdevice.which);
        break;

      case SDL_CONTROLLERBUTTONDOWN:
        assert(in->cbutton.state == SDL_PRESSED);
        out.type = EventType::GamepadButtonPressed;
        out.gamepadButton.id = static_cast<GamepadId>(in->cbutton.which);
        out.gamepadButton.button = getGamepadButtonFromButton(in->cbutton.button);
        break;

      case SDL_CONTROLLERBUTTONUP:
        assert(in->cbutton.state == SDL_RELEASED);
        out.type = EventType::GamepadButtonReleased;
        out.gamepadButton.id = static_cast<GamepadId>(in->cbutton.which);
        out.gamepadButton.button = getGamepadButtonFromButton(in->cbutton.button);
        break;

      case SDL_CONTROLLERAXISMOTION:
        out.type = EventType::GamepadAxisMoved;
        out.gamepadAxis.id = static_cast<GamepadId>(in->caxis.which);
        out.gamepadAxis.axis = getGamepadAxisFromAxis(in->caxis.axis);
        out.gamepadAxis.value = in->caxis.value;
        break;

      case SDL_TEXTINPUT:
        out.type = EventType::TextEntered;
        std::strncpy(out.text.rune.data, in->text.text, Rune::Size);
        break;

      default:
        return false;
    }

    return true;
  }

  bool Window::pollEvent(Event& event) {
    assert(m_window);

    Uint32 windowId = SDL_GetWindowID(m_window);
    SDL_Event ev;

    do {
      int status = SDL_PollEvent(&ev);

      if (status == 0) {
        return false;
      }
    } while (!translateEvent(windowId, &ev, event));

    return true;
  }

  bool Window::waitEvent(Event& event) {
    assert(m_window);

    Uint32 windowId = SDL_GetWindowID(m_window);
    SDL_Event ev;

    do {
      int status = SDL_WaitEvent(&ev);

      if (status == 0) {
        return false;
      }
    } while (!translateEvent(windowId, &ev, event));

    return true;
  }

  void Window::setVerticalSyncEnabled(bool enabled) {
    SDL_GL_SetSwapInterval(enabled ? 1 : 0);
  }

  bool Window::isVerticalSyncEnabled() const {
    return SDL_GL_GetSwapInterval() != 0;
  }

  void Window::setFramerateLimit(unsigned int limit) {
    if (limit == 0) {
      m_duration = Time::Zero;
    } else {
      m_duration = seconds(1.0f / limit);
    }
  }

  void Window::display() {
    assert(m_window);
    SDL_GL_SwapWindow(m_window);

    // handle framerate limit

    if (m_duration == Time::Zero) {
      return;
    }

    sleep(m_duration - m_clock.getElapsedTime());
    m_clock.restart();
  }

  void Window::setMouseCursorVisible(bool visible) {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
  }

  void Window::setMouseCursorGrabbed(bool grabbed) {
    SDL_SetWindowGrab(m_window, grabbed ? SDL_TRUE : SDL_FALSE);
  }

  void Window::setMouseCursor(const Cursor& cursor) {
    if (cursor.m_cursor == nullptr) {
      return;
    }

    SDL_SetCursor(cursor.m_cursor);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
