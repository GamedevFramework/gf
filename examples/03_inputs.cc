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
#include <cassert>
#include <iostream>

#include <gf/Action.h>
#include <gf/Controls.h>
#include <gf/Event.h>
#include <gf/Window.h>

std::ostream& operator<<(std::ostream& o, const gf::Flags<gf::Mod>& mods) {
  return o << (mods.test(gf::Mod::Shift) ? " shift" : "")
           << (mods.test(gf::Mod::Control) ? " control" : "")
           << (mods.test(gf::Mod::Alt) ? " alt" : "")
           << (mods.test(gf::Mod::Super) ? " super" : "");
}

std::ostream& operator<<(std::ostream& o, gf::Keycode keycode) {
  return o << std::hex << "0x" << static_cast<int>(keycode);
}

std::ostream& operator<<(std::ostream& o, gf::Scancode scancode) {
  return o << std::hex << "0x" << static_cast<int>(scancode);
}

std::ostream& operator<<(std::ostream& o, const gf::KeyEvent& event) {
  o << "\twindowId: " << event.windowId << '\n';
  o << "\tkeycode: " << event.keycode << " (" << gf::Keyboard::getKeycodeName(event.keycode) << ")\n";
  o << "\tscancode: " << event.scancode << " (" << gf::Keyboard::getScancodeName(event.scancode) << ")\n";
  o << "\tmodifiers:" << event.modifiers;
  return o;
}

std::ostream& operator<<(std::ostream& o, const gf::MouseButtonEvent& event) {
  o << "\twindowId: " << event.windowId << '\n';
  o << "\tbutton: " << static_cast<int>(event.button) << '\n';
  o << "\tcoordinates: " << event.coords.x << 'x' << event.coords.y << '\n';
  o << "\tclicks: " << static_cast<int>(event.clicks);
  return o;
}

std::ostream& operator<<(std::ostream& o, const gf::MouseCursorEvent& event) {
  o << "\twindowId: " << event.windowId << '\n';
  o << "\tcoordinates: " << event.coords.x << 'x' << event.coords.y << '\n';
  o << "\tmotion: " << event.motion.x << 'x' << event.motion.y;
  return o;
}

std::ostream& operator<<(std::ostream& o, const gf::GamepadButtonEvent& event) {
  o << "\tname: " << gf::Gamepad::getName(event.id) << '\n';
  o << "\tid: " << static_cast<int32_t>(event.id) << '\n';
  o << "\tbutton: " << gf::Gamepad::getButtonName(event.button);
  return o;
}

std::ostream& operator<<(std::ostream& o, const gf::GamepadAxisEvent& event) {
  o << "\tname: " << gf::Gamepad::getName(event.id) << '\n';
  o << "\tid: " << static_cast<int32_t>(event.id) << '\n';
  o << "\taxis: " << gf::Gamepad::getAxisName(event.axis) << '\n';
  o << "\tvalue: " << event.value;
  return o;
}

std::ostream& operator<<(std::ostream& o, const gf::TouchEvent& event) {
  o << "\tfinger: " << event.finger << '\n';
  o << "\tcoordinates: " << event.coords.x << 'x' << event.coords.y << '\n';
  o << "\tmotion: " << event.motion.x << 'x' << event.motion.y;
  return o;
}

int main() {
  bool cursorVisible = true;
  bool cursorGrabbed = false;

  gf::Gamepad::initialize();

  gf::Window window("03_inputs", { 640, 480 }, ~gf::WindowHints::Resizable);

  gf::KonamiKeyboardControl konami;

  gf::Action easterEgg("Easter egg");
  easterEgg.addControl(konami);
  easterEgg.setInstantaneous();

  std::cout << "Gamedev Framework (gf) example #03: Inputs\n";
  std::cout << "This example prints all input events\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Toggle cursor visibility\n";
  std::cout << "\t2: Toggle cursor grab\n";
  std::cout << "Cursor visibility: " << (cursorVisible ? "yes" : "no") << '\n';
  std::cout << "Cursor grab: " << (cursorGrabbed ? "yes" : "no") << '\n';

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          std::cout << "KeyPressed:\n" << event.key << '\n';

          if (event.key.keycode == gf::Keycode::Escape) {
            window.close();
          } else if (event.key.scancode == gf::Scancode::Num1) {
            cursorVisible = !cursorVisible;
            window.setMouseCursorVisible(cursorVisible);
            std::cout << "Cursor visibility: " << (cursorVisible ? "yes" : "no") << '\n';
          } else if (event.key.scancode == gf::Scancode::Num2) {
            cursorGrabbed = !cursorGrabbed;
            window.setMouseCursorGrabbed(cursorGrabbed);
            std::cout << "Cursor grab: " << (cursorGrabbed ? "yes" : "no") << '\n';
          }
          break;

        case gf::EventType::KeyReleased:
          std::cout << "KeyReleased:\n" << event.key << '\n';
          break;

        case gf::EventType::KeyRepeated:
          std::cout << "KeyRepeated:\n" << event.key << '\n';
          break;

        case gf::EventType::MouseButtonPressed:
          std::cout << "MouseButtonPressed:\n" << event.mouseButton << '\n';
          break;

        case gf::EventType::MouseButtonReleased:
          std::cout << "MouseButtonReleased:\n" << event.mouseButton << '\n';
          break;

//         case gf::EventType::MouseMoved:
//           std::cout << "MouseMoved:\n" << event.mouseCursor << '\n';
//           break;

        case gf::EventType::MouseEntered:
          std::cout << "MouseEntered\n";
          break;

        case gf::EventType::MouseLeft:
          std::cout << "MouseLeft\n";
          break;

        case gf::EventType::MouseWheelScrolled:
          std::cout << "MouseWheelScrolled: " << event.mouseWheel.offset.x << ',' << event.mouseWheel.offset.y << '\n';
          break;

        case gf::EventType::GamepadConnected:
          std::cout << "GamepadConnected: " << static_cast<int>(event.gamepadConnection.id) << '\n';
          gf::Gamepad::open(event.gamepadConnection.id);
          break;

        case gf::EventType::GamepadDisconnected:
          std::cout << "GamepadDisconnected: " << static_cast<int32_t>(event.gamepadDisconnection.id) << '\n';
          gf::Gamepad::close(event.gamepadDisconnection.id);
          break;

        case gf::EventType::GamepadButtonPressed:
          std::cout << "GamepadButtonPressed:\n" << event.gamepadButton << '\n';
          break;

        case gf::EventType::GamepadButtonReleased:
          std::cout << "GamepadButtonReleased:\n" << event.gamepadButton << '\n';
          break;

        case gf::EventType::GamepadAxisMoved:
          std::cout << "GamepadAxisMoved:\n" << event.gamepadAxis << '\n';
          break;

        case gf::EventType::TextEntered:
          std::cout << "TextEntered: '" << event.text.rune.data << "'\n";
          break;

        case gf::EventType::TouchBegan:
          std::cout << "TouchBegan:\n" << event.touch << '\n';
          break;

        case gf::EventType::TouchMoved:
          std::cout << "TouchMoved:\n" << event.touch << '\n';
          break;

        case gf::EventType::TouchEnded:
          std::cout << "TouchEnded:\n" << event.touch << '\n';
          break;

        default:
          break;
      }

      easterEgg.processEvent(event);
    }

    if (easterEgg.isActive()) {
      std::cout << "###############\n";
      std::cout << "# Easter egg! #\n";
      std::cout << "###############\n";
    }

    window.display();

    easterEgg.reset();
  }

  return 0;
}
