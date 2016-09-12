/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <cstdio>

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>


#include "local/Board.h"

#include "config.h"

int main() {
  gf::Log::setLevel(gf::Log::Info);

  // setup resource directory

  gf::ResourceManager resourceManager;
  resourceManager.addSearchDir(GF_2048_DATA_DIR);

  gf::Font& font = resourceManager.getFont("ClearSans-Bold.ttf");

  gf::Random random;

  // initialize window

  gf::WindowHints hints;
  hints.resizable = false;
  gf::Window window("gf 2048!", { 500, 500 }, hints);
  gf::RenderWindow renderer(window);

  // add cameras

  gf::ViewContainer views;

  gf::FitView mainView;
  auto viewRect = Board::getView();
  mainView.setSize(viewRect.getSize());
  mainView.setCenter(viewRect.getCenter());
  views.addView(mainView);

  // add actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);

  gf::Action leftAction("Left");
  leftAction.addScancodeKeyControl(gf::Scancode::A);
  leftAction.addScancodeKeyControl(gf::Scancode::Left);
  actions.addAction(leftAction);

  gf::Action rightAction("Right");
  rightAction.addScancodeKeyControl(gf::Scancode::D);
  rightAction.addScancodeKeyControl(gf::Scancode::Right);
  actions.addAction(rightAction);

  gf::Action upAction("Up");
  upAction.addScancodeKeyControl(gf::Scancode::W);
  upAction.addScancodeKeyControl(gf::Scancode::Up);
  actions.addAction(upAction);

  gf::Action downAction("Down");
  downAction.addScancodeKeyControl(gf::Scancode::S);
  downAction.addScancodeKeyControl(gf::Scancode::Down);
  actions.addAction(downAction);

  // add entities

  gf::EntityContainer mainEntities;

  Board board(font, random);
  mainEntities.addEntity(board);

  // main loop

  renderer.clear(gf::Color::rgba(0xFAF8EFFF));

  gf::Clock clock;
  bool fullscreen = false;

  while (window.isOpen()) {
    // input
    gf::Event event;

    while (window.pollEvent(event)) {
      actions.update(event);
      views.update(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      fullscreen = !fullscreen;
      window.setFullscreen(fullscreen);
    }

    if (leftAction.isActive()) {
      board.move(gf::Direction::Left);
    }

    if (rightAction.isActive()) {
      board.move(gf::Direction::Right);
    }

    if (upAction.isActive()) {
      board.move(gf::Direction::Up);
    }

    if (downAction.isActive()) {
      board.move(gf::Direction::Down);
    }

    // update
    auto dt = clock.restart().asSeconds();
    mainEntities.update(dt);

    // render
    renderer.clear();
    renderer.setView(mainView);
    mainEntities.render(renderer);
    renderer.display();

    actions.reset();
  }

  return 0;
}
