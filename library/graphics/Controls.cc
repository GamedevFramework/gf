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
#include <gf/Controls.h>

#include <cassert>

#include <gf/Event.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  // keycode key control

  KeycodeKeyControl::KeycodeKeyControl(Keycode code)
  : m_code(code)
  {
  }

  void KeycodeKeyControl::processEvent(const Event& event) {
    if (event.type == EventType::KeyPressed) {
      if (event.key.keycode == m_code) {
        setActive();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.keycode == m_code) {
        setActive(false);
      }
    }
  }

  // scancode key control

  ScancodeKeyControl::ScancodeKeyControl(Scancode code)
  : m_code(code)
  {
  }

  void ScancodeKeyControl::processEvent(const Event& event) {
    if (event.type == EventType::KeyPressed) {
      if (event.key.scancode == m_code) {
        setActive();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.scancode == m_code) {
        setActive(false);
      }
    }
  }

  // mouse button control

  MouseButtonControl::MouseButtonControl(MouseButton button)
  : m_button(button)
  {
  }

  void MouseButtonControl::processEvent(const Event& event) {
    if (event.type == EventType::MouseButtonPressed) {
      if (event.mouseButton.button == m_button) {
        setActive();
      }
    }

    if (event.type == EventType::MouseButtonReleased) {
      if (event.mouseButton.button == m_button) {
        setActive(false);
      }
    }
  }

  // gamepad button control

  GamepadButtonControl::GamepadButtonControl(GamepadId id, GamepadButton button)
  : m_id(id)
  , m_button(button)
  {
  }

  void GamepadButtonControl::processEvent(const Event& event) {
    if (event.type == EventType::GamepadButtonPressed) {
      if ((m_id == AnyGamepad || event.gamepadButton.id == m_id) && event.gamepadButton.button == m_button) {
        setActive();
      }
    }

    if (event.type == EventType::GamepadButtonReleased) {
      if ((m_id == AnyGamepad || event.gamepadButton.id == m_id) && event.gamepadButton.button == m_button) {
        setActive(false);
      }
    }
  }

  // gamepad axis control

  GamepadAxisControl::GamepadAxisControl(GamepadId id, GamepadAxis axis, GamepadAxisDirection dir)
  : m_id(id)
  , m_axis(axis)
  , m_dir(dir)
  , m_repeated(false)
  {
  }

  static constexpr int16_t GamepadAxisThreshold = 8000;

  void GamepadAxisControl::processEvent(const Event& event) {
    if (event.type == EventType::GamepadAxisMoved) {
      if ((m_id == AnyGamepad || event.gamepadAxis.id == m_id) && event.gamepadAxis.axis == m_axis) {
        switch (m_dir) {
          case GamepadAxisDirection::Positive:
            if (event.gamepadAxis.value > GamepadAxisThreshold) {
              if (!m_repeated) {
                setActive();
              };

              m_repeated = true;
            } else {
              setActive(false);
              m_repeated = false;
            }
            break;
          case GamepadAxisDirection::Negative:
            if (event.gamepadAxis.value < -GamepadAxisThreshold) {
              if (!m_repeated) {
                setActive();
              }

              m_repeated = true;
            } else {
              setActive(false);
              m_repeated = false;
            }
            break;
        }
      }
    }
  }


  // close control

  CloseControl::CloseControl()
  {
  }

  void CloseControl::processEvent(const Event& event) {
    if (event.type == EventType::Closed) {
      setActive();
    }
  }


  // konami control

  namespace {

    constexpr int KonamiCount = 10;

    constexpr Keycode KeyForKonami[KonamiCount] = {
      Keycode::Up,
      Keycode::Up,
      Keycode::Down,
      Keycode::Down,
      Keycode::Left,
      Keycode::Right,
      Keycode::Left,
      Keycode::Right,
      Keycode::B,
      Keycode::A,
    };

  }

  KonamiKeyboardControl::KonamiKeyboardControl()
  : m_index(0)
  , m_state(Released)
  {
  }

  void KonamiKeyboardControl::processEvent(const Event& event) {
    if (m_state == Released) {
      if (event.type == EventType::KeyPressed) {
        assert(0 <= m_index && m_index < KonamiCount);

        if (event.key.keycode == KeyForKonami[m_index]) {
          m_state = Pressed;
        } else {
          m_index = 0;
        }
      }
    } else {
      assert(m_state == Pressed);

      if (event.type == EventType::KeyReleased) {
        m_state = Released;
        assert(0 <= m_index && m_index < KonamiCount);

        if (event.key.keycode == KeyForKonami[m_index]) {
          ++m_index;
        } else {
          m_index = 0;
        }
      }
    }

    if (m_index == KonamiCount) {
      setActive(true);
      m_index = 0;
    } else {
      setActive(false);
    }
  }

  namespace {

    constexpr GamepadButton ButtonForKonami[KonamiCount] = {
      GamepadButton::DPadUp,
      GamepadButton::DPadUp,
      GamepadButton::DPadDown,
      GamepadButton::DPadDown,
      GamepadButton::DPadLeft,
      GamepadButton::DPadRight,
      GamepadButton::DPadLeft,
      GamepadButton::DPadRight,
      GamepadButton::B,
      GamepadButton::A,
    };

  }

  KonamiGamepadControl::KonamiGamepadControl(GamepadId id)
  : m_id(id)
  , m_index(0)
  , m_state(Released)
  {
  }

  void KonamiGamepadControl::processEvent(const Event& event) {
    if (m_state == Released) {
      if (event.type == EventType::GamepadButtonPressed) {
        assert(0 <= m_index && m_index < KonamiCount);

        if ((m_id == AnyGamepad || event.gamepadButton.id == m_id) && event.gamepadButton.button == ButtonForKonami[m_index]) {
          m_state = Pressed;
        } else {
          m_index = 0;
        }
      }
    } else {
      assert(m_state == Pressed);

      if (event.type == EventType::GamepadButtonReleased) {
        m_state = Released;
        assert(0 <= m_index && m_index < KonamiCount);

        if ((m_id == AnyGamepad || event.gamepadButton.id == m_id) && event.gamepadButton.button == ButtonForKonami[m_index]) {
          ++m_index;
        } else {
          m_index = 0;
        }
      }
    }

    if (m_index == KonamiCount) {
      setActive(true);
      m_index = 0;
    } else {
      setActive(false);
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
