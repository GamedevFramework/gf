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
#include <gf/Window.h>

#include <cassert>
#include <type_traits>

#include <SDL.h>

#include <gf/Cursor.h>
#include <gf/Event.h>
#include <gf/Gamepad.h>
#include <gf/Keyboard.h>
#include <gf/Log.h>
#include <gf/Mouse.h>
#include <gf/Sleep.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>
#include <gfpriv/SdlDebug.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    Uint32 getFlagsFromHints(Flags<WindowHints> hints) {
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

    void *createContextFromWindow(SDL_Window *window) {
      if (window == nullptr) {
        return nullptr;
      }

      void *context = SDL_CHECK_EXPR(SDL_GL_CreateContext(window));

      if (context == nullptr) {
        Log::error("Failed to create a context: %s\n", SDL_GetError());
        return nullptr;
      }

      int err = SDL_CHECK_EXPR(SDL_GL_MakeCurrent(window, context));

      if (err != 0) {
        Log::error("Failed to make the context current: %s\n", SDL_GetError());
      }

#ifndef __APPLE__
#ifdef GF_OPENGL3
      if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
        Log::error("Failed to load GL3.\n");
      }
#else
      if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0) {
        Log::error("Failed to load GLES2.\n");
      }
#endif
#endif

      return context;
    }
  }

  Window::Window(const std::string& title, Vector2i size, Flags<WindowHints> hints)
  : m_window(nullptr)
  , m_windowId(0xFFFFFF)
  , m_mainContext(nullptr)
  , m_sharedContext(nullptr)
  , m_shouldClose(false)
  , m_isFullscreen(false)
  , m_vao(0)
  {
    auto flags = getFlagsFromHints(hints);
    m_window = SDL_CHECK_EXPR(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, flags));
    assert(m_window);
    m_windowId = SDL_CHECK_EXPR(SDL_GetWindowID(m_window));

    SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1));
    m_sharedContext = SDL_CHECK_EXPR(SDL_GL_CreateContext(m_window));
    m_mainContext = createContextFromWindow(m_window);

    if (m_sharedContext == nullptr) {
      Log::error("Failed to create a shared context: %s\n", SDL_GetError());
      m_sharedContext = nullptr;
    }

    if (m_mainContext != nullptr) {
      GL_CHECK(glEnable(GL_BLEND));
      GL_CHECK(glEnable(GL_SCISSOR_TEST));
      GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

#ifdef GF_OPENGL3
      GL_CHECK(glGenVertexArrays(1, &m_vao));
      GL_CHECK(glBindVertexArray(m_vao));
#endif
    }
  }

  Window::~Window() {
    makeMainContextCurrent();

    if (m_sharedContext != nullptr) {
      SDL_CHECK(SDL_GL_DeleteContext(m_sharedContext));
    }

    if (m_mainContext != nullptr) {
#ifdef GF_OPENGL3
      GL_CHECK(glBindVertexArray(0));
      GL_CHECK(glDeleteVertexArrays(1, &m_vao));
#endif
      SDL_CHECK(SDL_GL_DeleteContext(m_mainContext));
    }

    if (m_window != nullptr) {
      SDL_CHECK(SDL_DestroyWindow(m_window));
    }
  }

  bool Window::isOpen() {
    return !m_shouldClose;
  }

  void Window::close() {
    assert(m_window);
    m_shouldClose = true;
  }

  void Window::setTitle(const std::string& title) {
    assert(m_window);
    SDL_CHECK(SDL_SetWindowTitle(m_window, title.c_str()));
  }

  Vector2i Window::getPosition() const {
    assert(m_window);
    Vector2i position;
    SDL_CHECK(SDL_GetWindowPosition(m_window, &position.x, &position.y));
    return position;
  }

  void Window::setPosition(Vector2i position) {
    assert(m_window);
    SDL_CHECK(SDL_SetWindowPosition(m_window, position.x, position.y));
  }

  Vector2i Window::getSize() const {
    assert(m_window);
    Vector2i size;
    SDL_CHECK(SDL_GetWindowSize(m_window, &size.width, &size.height));
    return size;
  }

  void Window::setSize(Vector2i size) {
    assert(m_window);
    SDL_CHECK(SDL_SetWindowSize(m_window, size.width, size.height));
  }

  Vector2i Window::getFramebufferSize() const {
    assert(m_window);
    Vector2i size;
    SDL_CHECK(SDL_GL_GetDrawableSize(m_window, &size.width, &size.height));
    return size;
  }

  void Window::setFullscreen(bool full) {
    assert(m_window);

    if (full) {
      SDL_CHECK(SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP));
    } else {
      SDL_CHECK(SDL_SetWindowFullscreen(m_window, 0));
    }

    m_isFullscreen = full;
  }

  void Window::toggleFullscreen() {
    setFullscreen(!m_isFullscreen);
  }


  bool Window::isMinimized() const {
    assert(m_window);
    auto flags = SDL_CHECK_EXPR(SDL_GetWindowFlags(m_window));
    return (flags & SDL_WINDOW_MINIMIZED) != 0;
  }

  void Window::minimize() {
    assert(m_window);
    SDL_CHECK(SDL_MinimizeWindow(m_window));
  }

  void Window::restore() {
    assert(m_window);
    SDL_CHECK(SDL_RestoreWindow(m_window));
  }

  bool Window::isMaximized() const {
    assert(m_window);
    auto flags = SDL_CHECK_EXPR(SDL_GetWindowFlags(m_window));
    return (flags & SDL_WINDOW_MAXIMIZED) != 0;
  }

  void Window::maximize() {
    assert(m_window);
    SDL_CHECK(SDL_MaximizeWindow(m_window));
  }

  bool Window::isVisible() const {
    assert(m_window);
    auto flags = SDL_CHECK_EXPR(SDL_GetWindowFlags(m_window));
    return (flags & SDL_WINDOW_SHOWN) != 0;
  }

  void Window::show() {
    assert(m_window);
    SDL_CHECK(SDL_ShowWindow(m_window));
  }

  void Window::hide() {
    assert(m_window);
    SDL_CHECK(SDL_HideWindow(m_window));
  }

  void Window::setVisible(bool visible) {
    if (visible) {
      show();
    } else {
      hide();
    }
  }

  bool Window::isDecorated() const {
    assert(m_window);
    auto flags = SDL_CHECK_EXPR(SDL_GetWindowFlags(m_window));
    return (flags & SDL_WINDOW_BORDERLESS) == 0;
  }

  void Window::setDecorated(bool decorated) {
    assert(m_window);
    SDL_CHECK(SDL_SetWindowBordered(m_window, decorated ? SDL_TRUE : SDL_FALSE));
  }

  bool Window::isFocused() const {
    assert(m_window);
    auto flags = SDL_CHECK_EXPR(SDL_GetWindowFlags(m_window));
    return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
  }

  bool Window::isResizable() const {
    assert(m_window);
    auto flags = SDL_CHECK_EXPR(SDL_GetWindowFlags(m_window));
    return (flags & SDL_WINDOW_RESIZABLE) != 0;
  }

  void Window::setResizable(bool resizable) {
    assert(m_window);
#if SDL_VERSION_ATLEAST(2,0,5)
    SDL_CHECK(SDL_SetWindowResizable(m_window, resizable ? SDL_TRUE : SDL_FALSE));
#else
    gf::unused(resizable);
    Log::error("Window can not be set resizable. You must compile with SDL 2.0.5 at least.\n");
#endif
  }


  namespace {

    MouseButton getMouseButtonFromButton(Uint8 button) {
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

    GamepadButton getGamepadButtonFromButton(Uint8 button) {
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

    GamepadAxis getGamepadAxisFromAxis(Uint8 axis) {
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

    Flags<Mod> getModifiersFromMod(Uint16 mod) {
      Flags<Mod> modifiers(None);

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

    bool translateEvent(Vector2i size, const SDL_Event *in, Event& out, Flags<EventFilter> filters) {
      out.timestamp = in->common.timestamp;

      switch (in->type) {
        case SDL_WINDOWEVENT:
          switch (in->window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
              out.type = EventType::Resized;
              out.resize.windowId = in->window.windowID;
              out.resize.size.width = in->window.data1;
              out.resize.size.height = in->window.data2;
              break;

            case SDL_WINDOWEVENT_CLOSE:
              out.type = EventType::Closed;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
              out.type = EventType::FocusGained;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_FOCUS_LOST:
              out.type = EventType::FocusLost;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_ENTER:
              out.type = EventType::MouseEntered;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_LEAVE:
              out.type = EventType::MouseLeft;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_SHOWN:
              out.type = EventType::Shown;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_HIDDEN:
              out.type = EventType::Hidden;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_EXPOSED:
              out.type = EventType::Exposed;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_MINIMIZED:
              out.type = EventType::Minimized;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_MAXIMIZED:
              out.type = EventType::Maximized;
              out.window.windowId = in->window.windowID;
              break;

            case SDL_WINDOWEVENT_RESTORED:
              out.type = EventType::Restored;
              out.window.windowId = in->window.windowID;
              break;

            default:
              return false;
          }
          break;

        case SDL_QUIT:
          out.type = EventType::Quit;
          break;

        case SDL_KEYDOWN:
          assert(in->key.state == SDL_PRESSED);

          if (in->key.repeat == 0) {
            out.type = EventType::KeyPressed;
          } else {
            out.type = EventType::KeyRepeated;
          }

          out.key.windowId = in->key.windowID;
          out.key.keycode = static_cast<Keycode>(in->key.keysym.sym);
          out.key.scancode = static_cast<Scancode>(in->key.keysym.scancode);
          out.key.modifiers = getModifiersFromMod(in->key.keysym.mod);
          break;

        case SDL_KEYUP:
          assert(in->key.state == SDL_RELEASED);
          out.type = EventType::KeyReleased;
          out.key.windowId = in->key.windowID;
          out.key.keycode = static_cast<Keycode>(in->key.keysym.sym);
          out.key.scancode = static_cast<Scancode>(in->key.keysym.scancode);
          out.key.modifiers = getModifiersFromMod(in->key.keysym.mod);
          break;

        case SDL_MOUSEWHEEL:
          if (in->wheel.which == SDL_TOUCH_MOUSEID && !filters.test(EventFilter::TouchAsMouse)) {
            return false;
          }

          out.type = EventType::MouseWheelScrolled;
          out.mouseWheel.windowId = in->wheel.windowID;
          out.mouseWheel.offset.x = in->wheel.x;
          out.mouseWheel.offset.y = in->wheel.y;

          if (in->wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
            out.mouseWheel.offset = - out.mouseWheel.offset;
          }
          break;

        case SDL_MOUSEBUTTONDOWN:
          assert(in->button.state == SDL_PRESSED);

          if (in->button.which == SDL_TOUCH_MOUSEID && !filters.test(EventFilter::TouchAsMouse)) {
            return false;
          }

          out.type = EventType::MouseButtonPressed;
          out.mouseButton.windowId = in->button.windowID;
          out.mouseButton.button = getMouseButtonFromButton(in->button.button);
          out.mouseButton.coords.x = in->button.x;
          out.mouseButton.coords.y = in->button.y;
          out.mouseButton.clicks = in->button.clicks;
          break;

        case SDL_MOUSEBUTTONUP:
          assert(in->button.state == SDL_RELEASED);

          if (in->button.which == SDL_TOUCH_MOUSEID && !filters.test(EventFilter::TouchAsMouse)) {
            return false;
          }

          out.type = EventType::MouseButtonReleased;
          out.mouseButton.windowId = in->button.windowID;
          out.mouseButton.button = getMouseButtonFromButton(in->button.button);
          out.mouseButton.coords.x = in->button.x;
          out.mouseButton.coords.y = in->button.y;
          out.mouseButton.clicks = in->button.clicks;
          break;

        case SDL_MOUSEMOTION:
          if (in->motion.which == SDL_TOUCH_MOUSEID && !filters.test(EventFilter::TouchAsMouse)) {
            return false;
          }

          out.type = EventType::MouseMoved;
          out.mouseCursor.windowId = in->motion.windowID;
          out.mouseCursor.coords.x = in->motion.x;
          out.mouseCursor.coords.y = in->motion.y;
          out.mouseCursor.motion.x = in->motion.xrel;
          out.mouseCursor.motion.y = in->motion.yrel;
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
          static_assert(std::extent<decltype(out.text.rune.data)>::value == std::extent<decltype(in->text.text)>::value, "Buffer size mismatch.");
          static_assert(std::extent<decltype(out.text.rune.data)>::value == Rune::Size, "Buffer size mismatch.");
          std::copy_n(std::begin(in->text.text), Rune::Size, std::begin(out.text.rune.data));
          break;

        case SDL_FINGERDOWN:
          out.type = EventType::TouchBegan;
          out.touch.finger = in->tfinger.fingerId;
          out.touch.coords.x = static_cast<int>(in->tfinger.x * size.width);
          out.touch.coords.y = static_cast<int>(in->tfinger.y * size.height);
          out.touch.motion.x = static_cast<int>(in->tfinger.dx * size.width);
          out.touch.motion.y = static_cast<int>(in->tfinger.dy * size.height);
          out.touch.pressure = in->tfinger.pressure;
          break;

        case SDL_FINGERMOTION:
          out.type = EventType::TouchMoved;
          out.touch.finger = in->tfinger.fingerId;
          out.touch.coords.x = static_cast<int>(in->tfinger.x * size.width);
          out.touch.coords.y = static_cast<int>(in->tfinger.y * size.height);
          out.touch.motion.x = static_cast<int>(in->tfinger.dx * size.width);
          out.touch.motion.y = static_cast<int>(in->tfinger.dy * size.height);
          out.touch.pressure = in->tfinger.pressure;
          break;

        case SDL_FINGERUP:
          out.type = EventType::TouchEnded;
          out.touch.finger = in->tfinger.fingerId;
          out.touch.coords.x = static_cast<int>(in->tfinger.x * size.width);
          out.touch.coords.y = static_cast<int>(in->tfinger.y * size.height);
          out.touch.motion.x = static_cast<int>(in->tfinger.dx * size.width);
          out.touch.motion.y = static_cast<int>(in->tfinger.dy * size.height);
          out.touch.pressure = in->tfinger.pressure;
          break;

        default:
          return false;
      }

      return true;
    }

    bool isEventWindowDependent(const Event& event) {
      switch (event.type) {
        case EventType::Resized:
        case EventType::Closed:
        case EventType::FocusGained:
        case EventType::FocusLost:
        case EventType::Shown:
        case EventType::Hidden:
        case EventType::Exposed:
        case EventType::Minimized:
        case EventType::Maximized:
        case EventType::Restored:
        case EventType::KeyPressed:
        case EventType::KeyRepeated:
        case EventType::KeyReleased:
        case EventType::MouseWheelScrolled:
        case EventType::MouseButtonPressed:
        case EventType::MouseButtonReleased:
        case EventType::MouseMoved:
        case EventType::TextEntered:
          return true;
        default:
          break;
      }

      return false;
    }

    uint32_t getWindowIdFromEvent(const Event& event) {
      switch (event.type) {
        case EventType::Resized:
          return event.resize.windowId;
        case EventType::Closed:
        case EventType::FocusGained:
        case EventType::FocusLost:
        case EventType::Shown:
        case EventType::Hidden:
        case EventType::Exposed:
        case EventType::Minimized:
        case EventType::Maximized:
        case EventType::Restored:
          return event.window.windowId;
        case EventType::KeyPressed:
        case EventType::KeyRepeated:
        case EventType::KeyReleased:
          return event.key.windowId;
        case EventType::MouseWheelScrolled:
          return event.mouseWheel.windowId;
        case EventType::MouseButtonPressed:
        case EventType::MouseButtonReleased:
          return event.mouseButton.windowId;
        case EventType::MouseMoved:
          return event.mouseCursor.windowId;
        case EventType::TextEntered:
          return event.text.windowId;
        default:
          break;
      }

      return uint32_t(-1);
    }

  } // anonymous namespace

  bool Window::pollEvent(Event& event, Flags<EventFilter> filters) {
    assert(m_window);

    if (pickEventForWindow(m_windowId, event)) {
      return true;
    }

    SDL_Event ev;

    for (;;) {
      do {
        int status = SDL_CHECK_EXPR(SDL_PollEvent(&ev));

        if (status == 0) {
          return false;
        }
      } while (!translateEvent(getSize(), &ev, event, filters));

      if (isEventWindowDependent(event) && (filters.test(EventFilter::AnyWindow) || getWindowIdFromEvent(event) != m_windowId)) {
        g_pendingEvents.push_back(event);
      } else {
        break;
      }
    }

    return true;
  }

  bool Window::waitEvent(Event& event, Flags<EventFilter> filters) {
    assert(m_window);
    SDL_Event ev;

    if (pickEventForWindow(m_windowId, event)) {
      return true;
    }

    for (;;) {
      do {
        int status = SDL_CHECK_EXPR(SDL_WaitEvent(&ev));

        if (status == 0) {
          return false;
        }
      } while (!translateEvent(getSize(), &ev, event, filters));

      if (isEventWindowDependent(event) && (filters.test(EventFilter::AnyWindow) || getWindowIdFromEvent(event) != m_windowId)) {
        g_pendingEvents.push_back(event);
      } else {
        break;
      }
    }

    return true;
  }

  void Window::setVerticalSyncEnabled(bool enabled) {
    SDL_CHECK(SDL_GL_SetSwapInterval(enabled ? 1 : 0));
  }

  bool Window::isVerticalSyncEnabled() const {
    return SDL_CHECK_EXPR(SDL_GL_GetSwapInterval()) != 0;
  }

  void Window::setFramerateLimit(unsigned int limit) {
    if (limit == 0) {
      m_duration = Time::zero();
    } else {
      m_duration = seconds(1.0f / limit);
    }
  }

  void Window::display() {
    assert(m_window);
    SDL_CHECK(SDL_GL_SwapWindow(m_window));

    // handle framerate limit

    if (m_duration == Time::zero()) {
      return;
    }

    sleep(m_duration - m_clock.getElapsedTime());
    m_clock.restart();
  }

  void Window::setMouseCursorVisible(bool visible) {
    SDL_CHECK(SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE));
  }

  void Window::setMouseCursorGrabbed(bool grabbed) {
    SDL_CHECK(SDL_SetWindowGrab(m_window, grabbed ? SDL_TRUE : SDL_FALSE));
  }

  void Window::setMouseCursor(const Cursor& cursor) {
    if (cursor.m_cursor == nullptr) {
      return;
    }

    SDL_CHECK(SDL_SetCursor(cursor.m_cursor));
  }

  void Window::makeMainContextCurrent() {
    if (SDL_CHECK_EXPR(SDL_GL_GetCurrentContext()) != m_mainContext) {
      SDL_CHECK(SDL_GL_MakeCurrent(m_window, m_mainContext));
    }
  }

  void Window::makeSharedContextCurrent() {
    SDL_CHECK(SDL_GL_MakeCurrent(m_window, m_sharedContext));
  }

  void Window::makeNoContextCurrent() {
    SDL_CHECK(SDL_GL_MakeCurrent(m_window, nullptr));
  }

  std::vector<Event> Window::g_pendingEvents;

  bool Window::pickEventForWindow(uint32_t windowId, Event& event) {
    auto it = std::find_if(g_pendingEvents.begin(), g_pendingEvents.end(), [windowId](const Event& current) {
      return getWindowIdFromEvent(current) == windowId;
    });

    if (it != g_pendingEvents.end()) {
      event = *it;
      g_pendingEvents.erase(it);
      return true;
    }

    return false;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
