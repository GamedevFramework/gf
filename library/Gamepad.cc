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
#include <gf/Gamepad.h>

#include <cstring>

#include <SDL.h>

#include <gf/Log.h>

#include "generated/gamecontrollerdb.txt.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static SDL_GameControllerButton getButtonFromGamepadButton(GamepadButton button) {
    switch (button) {
      case GamepadButton::A:
        return SDL_CONTROLLER_BUTTON_A;
      case GamepadButton::B:
        return SDL_CONTROLLER_BUTTON_B;
      case GamepadButton::X:
        return SDL_CONTROLLER_BUTTON_X;
      case GamepadButton::Y:
        return SDL_CONTROLLER_BUTTON_Y;
      case GamepadButton::Back:
        return SDL_CONTROLLER_BUTTON_BACK;
      case GamepadButton::Guide:
        return SDL_CONTROLLER_BUTTON_GUIDE;
      case GamepadButton::Start:
        return SDL_CONTROLLER_BUTTON_START;
      case GamepadButton::LeftStick:
        return SDL_CONTROLLER_BUTTON_LEFTSTICK;
      case GamepadButton::RightStick:
        return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
      case GamepadButton::LeftBumper:
        return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
      case GamepadButton::RightBumper:
        return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
      case GamepadButton::DPadUp:
        return SDL_CONTROLLER_BUTTON_DPAD_UP;
      case GamepadButton::DPadDown:
        return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
      case GamepadButton::DPadLeft:
        return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
      case GamepadButton::DPadRight:
        return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
      case GamepadButton::Invalid:
        return SDL_CONTROLLER_BUTTON_INVALID;
    }

    return SDL_CONTROLLER_BUTTON_INVALID;
  }

  static SDL_GameControllerAxis getAxisFromGamepadAxis(GamepadAxis axis) {
    switch (axis) {
      case GamepadAxis::LeftX:
        return SDL_CONTROLLER_AXIS_LEFTX;
      case GamepadAxis::LeftY:
        return SDL_CONTROLLER_AXIS_LEFTY;
      case GamepadAxis::RightX:
        return SDL_CONTROLLER_AXIS_RIGHTX;
      case GamepadAxis::RightY:
        return SDL_CONTROLLER_AXIS_RIGHTY;
      case GamepadAxis::TriggerLeft:
        return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
      case GamepadAxis::TriggerRight:
        return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
      case GamepadAxis::Invalid:
        return SDL_CONTROLLER_AXIS_INVALID;
    }

    return SDL_CONTROLLER_AXIS_INVALID;
  }

  const char *Gamepad::getAxisName(GamepadAxis axis) {
    return SDL_GameControllerGetStringForAxis(getAxisFromGamepadAxis(axis));
  }

  const char *Gamepad::getButtonName(GamepadButton button) {
    return SDL_GameControllerGetStringForButton(getButtonFromGamepadButton(button));
  }


#if SDL_VERSION_ATLEAST(2,0,4)
  static SDL_GameController *getController(GamepadId id) {
    return SDL_GameControllerFromInstanceID(static_cast<SDL_JoystickID>(id));
  }
#else
  static std::map<GamepadId, SDL_GameController*> g_controllers;

  static SDL_GameController *getController(GamepadId id) {
    auto it = g_controllers.find(id);

    if (it == g_controllers.end()) {
      return nullptr;
    }

    return it->second;
  }
#endif

  static GamepadId openController(int index) {
    SDL_GameController *controller = SDL_GameControllerOpen(index);

    if (controller == nullptr) {
      Log::error("Could not open gamepad %i: %s\n", index, SDL_GetError());
      return static_cast<GamepadId>(-1);
    }

    SDL_Joystick *joystick = SDL_GameControllerGetJoystick(controller);
    SDL_JoystickID instanceId = SDL_JoystickInstanceID(joystick);

    Log::debug("New gamepad (device: %i / instance: %i)\n", index, instanceId);

#if !SDL_VERSION_ATLEAST(2,0,4)
    g_controllers.insert(std::make_pair(static_cast<GamepadId>(instanceId), controller));
#endif

    return static_cast<GamepadId>(instanceId);
  }

  GamepadId Gamepad::open(GamepadHwId hwid) {
    return openController(static_cast<int>(hwid));
  }

  bool Gamepad::isAttached(GamepadId id) {
    auto controller = getController(id);

    if (controller == nullptr) {
      return false;
    }

    return SDL_GameControllerGetAttached(controller) == SDL_TRUE;
  }

  void Gamepad::close(GamepadId id) {
    auto controller = getController(id);

    if (controller == nullptr) {
      return;
    }

#if !SDL_VERSION_ATLEAST(2,0,4)
    g_controllers.erase(id);
#endif

    SDL_GameControllerClose(controller);
  }

  const char *Gamepad::getName(GamepadId id) {
    auto controller = getController(id);

    if (controller == nullptr) {
      return "?";
    }

    return SDL_GameControllerName(controller);
  }

  void Gamepad::initialize() {
    int added = SDL_GameControllerAddMappingsFromRW(SDL_RWFromConstMem(gamecontrollerdb, std::strlen(gamecontrollerdb)), 1);

    if (added == -1) {
      Log::error("Unable to load game controller mappings: '%s'\n", SDL_GetError());
    } else {
      Log::debug("Game controller mappings loaded: %i added\n", added);
    }

    for (int index = 0; index < SDL_NumJoysticks(); ++index) {
      if (SDL_IsGameController(index)) {
        openController(index);
      }
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
