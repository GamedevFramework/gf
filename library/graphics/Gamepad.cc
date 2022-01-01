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
#include <gf/Gamepad.h>

#include <cassert>
#include <cinttypes>
#include <cstring>

#include <SDL.h>

#include <gf/Event.h>
#include <gf/Log.h>

#include <gfpriv/SdlDebug.h>

#define GF_IMPLEMENTATION
#include "generated/gamecontrollerdb.txt.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    SDL_GameControllerButton getButtonFromGamepadButton(GamepadButton button) {
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

      assert(false);
      return SDL_CONTROLLER_BUTTON_INVALID;
    }

    SDL_GameControllerAxis getAxisFromGamepadAxis(GamepadAxis axis) {
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

      assert(false);
      return SDL_CONTROLLER_AXIS_INVALID;
    }

  } // anonymous namespace

  const char *Gamepad::getAxisName(GamepadAxis axis) {
    return SDL_CHECK_EXPR(SDL_GameControllerGetStringForAxis(getAxisFromGamepadAxis(axis)));
  }

  const char *Gamepad::getButtonName(GamepadButton button) {
    return SDL_CHECK_EXPR(SDL_GameControllerGetStringForButton(getButtonFromGamepadButton(button)));
  }

  namespace {

    SDL_GameController *getController(GamepadId id) {
      return SDL_CHECK_EXPR(SDL_GameControllerFromInstanceID(static_cast<SDL_JoystickID>(id)));
    }

    GamepadId openController(int index) {
      SDL_GameController *controller = SDL_CHECK_EXPR(SDL_GameControllerOpen(index));

      if (controller == nullptr) {
        Log::error("Could not open gamepad %i: %s\n", index, SDL_GetError());
        return static_cast<GamepadId>(-1);
      }

      SDL_Joystick *joystick = SDL_CHECK_EXPR(SDL_GameControllerGetJoystick(controller));
      SDL_JoystickID instanceId = SDL_CHECK_EXPR(SDL_JoystickInstanceID(joystick));

      Log::debug("New gamepad (device: %i / instance: %i)\n", index, instanceId);

      return static_cast<GamepadId>(instanceId);
    }

  } // anonymous namespace

  GamepadId Gamepad::open(GamepadHwId hwid) {
    return openController(static_cast<int>(hwid));
  }

  bool Gamepad::isAttached(GamepadId id) {
    auto controller = getController(id);

    if (controller == nullptr) {
      return false;
    }

    return SDL_CHECK_EXPR(SDL_GameControllerGetAttached(controller)) == SDL_TRUE;
  }

  void Gamepad::close(GamepadId id) {
    auto controller = getController(id);

    if (controller == nullptr) {
      return;
    }

    SDL_CHECK(SDL_GameControllerClose(controller));
  }

  const char *Gamepad::getName(GamepadId id) {
    auto controller = getController(id);

    if (controller == nullptr) {
      return "?";
    }

    return SDL_CHECK_EXPR(SDL_GameControllerName(controller));
  }

  void Gamepad::initialize() {
    static bool alreadyInitialized = false;

    if (alreadyInitialized) {
      return;
    }

    int added = SDL_CHECK_EXPR(SDL_GameControllerAddMappingsFromRW(SDL_RWFromConstMem(gamecontrollerdb, sizeof gamecontrollerdb), 1));

    if (added == -1) {
      Log::error("Unable to load game controller mappings: '%s'\n", SDL_GetError());
    } else {
      Log::debug("Game controller mappings loaded: %i added\n", added);
    }

    for (int index = 0; index < SDL_NumJoysticks(); ++index) {
      if (SDL_CHECK_EXPR(SDL_IsGameController(index)) == SDL_TRUE) {
        openController(index);
      }
    }

    alreadyInitialized = true;
  }


  GamepadTracker::GamepadTracker()
  {
    Gamepad::initialize();
  }

  std::size_t GamepadTracker::getConnectedGamepadCount() const {
    return m_ids.size();
  }

  void GamepadTracker::processEvent(const Event& event) {
    switch (event.type) {
      case EventType::GamepadConnected:
      {
        GamepadId id = Gamepad::open(event.gamepadConnection.id);
        m_ids.push_back(id);
        Log::info("Gamepad connected: %s [%" PRIi32 "]\n", Gamepad::getName(id), static_cast<int32_t>(id));
        break;
      }

      case EventType::GamepadDisconnected:
      {
        GamepadId id = event.gamepadDisconnection.id;
        Log::info("Gamepad disconnected: %s [%" PRIi32 "]\n", Gamepad::getName(id), static_cast<int32_t>(id));
        m_ids.erase(std::remove(m_ids.begin(), m_ids.end(), id), m_ids.end());
        Gamepad::close(id);
        break;
      }

      default:
        break;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
