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
#include <gf/Action.h>

#include <algorithm>
#include <stdexcept>

#include <gf/Controls.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

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
    auto ptr = std::make_unique<KeycodeKeyControl>(code);
    addControl(*ptr);
    m_ownedControls.push_back(std::move(ptr));
  }

  void Action::addScancodeKeyControl(Scancode code) {
    auto ptr = std::make_unique<ScancodeKeyControl>(code);
    addControl(*ptr);
    m_ownedControls.push_back(std::move(ptr));
  }

  void Action::addMouseButtonControl(MouseButton button) {
    auto ptr = std::make_unique<MouseButtonControl>(button);
    addControl(*ptr);
    m_ownedControls.push_back(std::move(ptr));
  }

  void Action::addGamepadAxisControl(GamepadId id, GamepadAxis axis, GamepadAxisDirection dir) {
    auto ptr = std::make_unique<GamepadAxisControl>(id, axis, dir);
    addControl(*ptr);
    m_ownedControls.push_back(std::move(ptr));
  }

  void Action::addGamepadButtonControl(GamepadId id, GamepadButton button) {
    auto ptr = std::make_unique<GamepadButtonControl>(id, button);
    addControl(*ptr);
    m_ownedControls.push_back(std::move(ptr));
  }

  void Action::addCloseControl() {
    auto ptr = std::make_unique<CloseControl>();
    addControl(*ptr);
    m_ownedControls.push_back(std::move(ptr));
  }

  void Action::addControl(Control& control) {
    m_controls.push_back(control);
  }

  void Action::processEvent(const Event& event) {
    for (Control& control : m_controls) {
      control.processEvent(event);
    }
  }

  bool Action::isActive() const {
    for (Control& control : m_controls) {
      if (control.isActive()) {
        return true;
      }
    }

    return false;
  }

  void Action::reset() {
    if (isContinuous()) {
      return;
    }

    for (Control& control : m_controls) {
      control.reset();
    }
  }

  // ActionContainer

  void ActionContainer::addAction(Action& action) {
    m_actions.push_back(action);
  }

  bool ActionContainer::hasAction(const std::string& name) const {
    auto it = std::find_if(m_actions.begin(), m_actions.end(), [&name](const Action& action) {
      return action.getName() == name;
    });

    return it != m_actions.end();
  }

  Action& ActionContainer::getAction(const std::string& name) {
    auto it = std::find_if(m_actions.begin(), m_actions.end(), [&name](const Action& action) {
      return action.getName() == name;
    });

    if (it == m_actions.end()) {
      throw std::runtime_error("Action not found");
    }

    return *it;
  }

  const Action& ActionContainer::getAction(const std::string& name) const {
    auto it = std::find_if(m_actions.begin(), m_actions.end(), [&name](const Action& action) {
      return action.getName() == name;
    });

    if (it == m_actions.end()) {
      throw std::runtime_error("Action not found");
    }

    return *it;
  }

  void ActionContainer::processEvent(const Event& event) {
    for (Action& action : m_actions) {
      action.processEvent(event);
    }
  }

  void ActionContainer::reset() {
    for (Action& action : m_actions) {
      action.reset();
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
