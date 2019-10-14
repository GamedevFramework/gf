/*
 * Krokodile
 * Copyright (C) 2018 Hatunruna team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cassert>

#include <gf/Anchor.h>
#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Controls.h>
#include <gf/Coordinates.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Gamepad.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Text.h>
#include <gf/Unused.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include <iostream>

#include "config.h"
#include "local/FinalScreen.h"
#include "local/Hud.h"
#include "local/KreatureContainer.h"
#include "local/Map.h"
#include "local/Messages.h"
#include "local/Singletons.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(1000.0f, 1000.0f);
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);

  // initialization

  gf::Window window("Krokodile", ScreenSize);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  gf::RenderWindow renderer(window);

  gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(kkd::gResourceManager);
  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(kkd::gMessageManager);

  kkd::gResourceManager().addSearchDir(KROKODILE_DATA_DIR);

  gf::SingletonStorage<gf::Random> storageForRandom(kkd::gRandom);
  gf::SingletonStorage<gf::TextureAtlas> storageForTextureAtlas(kkd::gTextureAtlas, "atlas.xml", kkd::gResourceManager());

  kkd::gResourceManager().getTexture(kkd::gTextureAtlas().getTexturePath()).setSmooth();

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);

  kkd::gMessageManager().registerHandler<kkd::KrokodileState>([&mainView](gf::Id type, gf::Message *msg) {
    assert(type == kkd::KrokodileState::type);
    gf::unused(type);
    auto state = static_cast<kkd::KrokodileState*>(msg);
    mainView.setCenter(state->position);
    return gf::MessageStatus::Keep;
  });

  // actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  closeWindowAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Start);
  actions.addAction(fullscreenAction);

  gf::Action leftAction("Left");
  leftAction.addScancodeKeyControl(gf::Scancode::A);
  leftAction.addScancodeKeyControl(gf::Scancode::Left);
  leftAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::RightX, gf::GamepadAxisDirection::Negative);
  leftAction.setContinuous();
  actions.addAction(leftAction);

  gf::Action rightAction("Right");
  rightAction.addScancodeKeyControl(gf::Scancode::D);
  rightAction.addScancodeKeyControl(gf::Scancode::Right);
  rightAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::RightX, gf::GamepadAxisDirection::Positive);
  rightAction.setContinuous();
  actions.addAction(rightAction);

  gf::Action upAction("Up");
  upAction.addScancodeKeyControl(gf::Scancode::W);
  upAction.addScancodeKeyControl(gf::Scancode::Up);
  upAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
  upAction.setContinuous();
  actions.addAction(upAction);

  gf::Action downAction("Down");
  downAction.addScancodeKeyControl(gf::Scancode::S);
  downAction.addScancodeKeyControl(gf::Scancode::Down);
  downAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
  downAction.setContinuous();
  actions.addAction(downAction);

  gf::Action swapAction("Swap");
  swapAction.addScancodeKeyControl(gf::Scancode::Tab);
  swapAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
  actions.addAction(swapAction);

  gf::Action mergeAction("Merge");
  mergeAction.addScancodeKeyControl(gf::Scancode::Space);
  mergeAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::LeftBumper);
  actions.addAction(mergeAction);

  gf::Action sprintAction("Sprint");
  sprintAction.addScancodeKeyControl(gf::Scancode::LeftShift);
  sprintAction.addScancodeKeyControl(gf::Scancode::RightShift);
  sprintAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::RightBumper);
  sprintAction.setContinuous();
  actions.addAction(sprintAction);

  gf::KonamiKeyboardControl konamiKeyboard;
  gf::KonamiGamepadControl konamiGamepad(gf::AnyGamepad);

  gf::Action easterEggAction("Easter egg");
  easterEggAction.addControl(konamiKeyboard);
  easterEggAction.addControl(konamiGamepad);
  easterEggAction.setInstantaneous();
  actions.addAction(easterEggAction);

  gf::GamepadTracker gamepadTracker;

  // entities

  gf::EntityContainer mainEntities;

  kkd::Map map;
  mainEntities.addEntity(map);

  kkd::KreatureContainer kreatures;
  mainEntities.addEntity(kreatures);

  gf::EntityContainer hudEntities;

  kkd::Hud hud;
  hudEntities.addEntity(hud);

  gf::EntityContainer finalEntities;

  kkd::FinalScreen finalScreen;
  finalEntities.addEntity(finalScreen);

  // game variables

  bool isEndOfGame = false;

  kkd::gMessageManager().registerHandler<kkd::EndOfGame>([&isEndOfGame](gf::Id type, gf::Message *msg) {
    assert(type == kkd::EndOfGame::type);
    gf::unused(type, msg);
    isEndOfGame = true;
    return gf::MessageStatus::Keep;
  });

  // game loop

  renderer.clear(gf::Color::lighter(gf::Color::Chartreuse));
  gf::Clock clock;

  while (window.isOpen()) {
    // 1. input
    gf::Event event;
    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);
      gamepadTracker.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (sprintAction.isActive()) {
      kreatures.setPlayerSprint(true);
    } else {
      kreatures.setPlayerSprint(false);
    }

    if (rightAction.isActive()) {
      kreatures.setPlayerAngularMove(gf::AngularMove::Right);
    } else if (leftAction.isActive()) {
      kreatures.setPlayerAngularMove(gf::AngularMove::Left);
    } else {
      kreatures.setPlayerAngularMove(gf::AngularMove::None);
    }

    if (upAction.isActive()) {
      kreatures.setPlayerLinearMove(gf::LinearMove::Forward);
    } else if (downAction.isActive()) {
      kreatures.setPlayerLinearMove(gf::LinearMove::Backward);
    } else {
      kreatures.setPlayerLinearMove(gf::LinearMove::None);
    }

    if (swapAction.isActive()) {
      kreatures.swapKreatures();
    }

    if (mergeAction.isActive()) {
      if (!isEndOfGame) {
        kreatures.mergeKreatures();
      } else {
        finalScreen.reset();
        hud.reset();
        kreatures.resetKreatures();
        isEndOfGame = false;
      }
    }

    if (easterEggAction.isActive()) {
      kreatures.createKrokodile();
    }

    // 2. update
    gf::Time time = clock.restart();

    if (!isEndOfGame) {
      mainEntities.update(time);
      hudEntities.update(time);
    } else {
      finalEntities.update(time);
    }

    // 3. draw
    renderer.clear();

    if (!isEndOfGame) {
      renderer.setView(mainView);
      mainEntities.render(renderer);

      renderer.setView(hudView);
      hudEntities.render(renderer);
    } else {
      renderer.setView(hudView);
      finalEntities.render(renderer);
    }

    renderer.display();

    actions.reset();
  }

  return 0;
}
