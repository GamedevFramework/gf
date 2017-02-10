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
#include <gf/Action.h>

#include <algorithm>

#include <gf/Controls.h>

namespace gf {
inline namespace v1 {
  struct Event;

  Action::Action(std::string name)
  : m_name(std::move(name))
  , m_type(Type::Instantaneous)
  {
    
  }

  void Action::setContinuous() {
    m_type = Type::Continuous;
  }

  bool Action::isContinuous() const {
    return m_type == Type::Continuous;
  }

  void Action::setInstantaneous() {
    m_type = Type::Instantaneous;
  }

  bool Action::isInstantaneous() const {
    return m_type == Type::Instantaneous;
  }

  void Action::addKeycodeKeyControl(Keycode code) {
    std::unique_ptr<Control> ptr(new KeycodeKeyControl(code));
    m_controls.push_back(std::move(ptr));
  }

  void Action::addScancodeKeyControl(Scancode code) {
    std::unique_ptr<Control> ptr(new ScancodeKeyControl(code));
    m_controls.push_back(std::move(ptr));
  }

  void Action::addMouseButtonControl(MouseButton button) {
    std::unique_ptr<Control> ptr(new MouseButtonControl(button));
    m_controls.push_back(std::move(ptr));
  }

  void Action::addGamepadAxisControl(GamepadId id, GamepadAxis axis, GamepadAxisDirection dir) {
    std::unique_ptr<Control> ptr(new GamepadAxisControl(id, axis, dir));
    m_controls.push_back(std::move(ptr));
  }

  void Action::addGamepadButtonControl(GamepadId id, GamepadButton button) {
    std::unique_ptr<Control> ptr(new GamepadButtonControl(id, button));
    m_controls.push_back(std::move(ptr));
  }

  void Action::addCloseControl() {
    std::unique_ptr<Control> ptr(new CloseControl);
    m_controls.push_back(std::move(ptr));
  }

  void Action::processEvent(const Event& event) {
    for (auto& control : m_controls) {
      control->processEvent(event);
    }
  }

  bool Action::isActive() {
    for (auto& control : m_controls) {
      if (control->isActive()) {
        return true;
      }
    }

    return false;
  }

  void Action::reset() {
    if (isContinuous()) {
      return;
    }

    for (auto& control : m_controls) {
      control->reset();
    }
  }

  // ActionContainer

  void ActionContainer::addAction(Action& action) {
    m_actions.push_back(&action);
  }

  void ActionContainer::processEvent(const Event& event) {
    for (auto action : m_actions) {
      action->processEvent(event);
    }
  }

  void ActionContainer::reset() {
    for (auto action : m_actions) {
      action->reset();
    }
  }

}
}
