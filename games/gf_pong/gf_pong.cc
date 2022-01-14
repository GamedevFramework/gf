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

#include "local/Ball.h"
#include "local/Ground.h"
#include "local/Paddle.h"
#include "local/Points.h"
#include "local/Singletons.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);

  gf::Log::setLevel(gf::Log::Info);

  // setup singletons

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gMessageManager);

  // initialize window

  gf::Window window("gf Pong!", ScreenSize);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  gf::RenderWindow renderer(window);

  // add cameras

  gf::ViewContainer views;

  gf::FitView mainView;
  views.addView(mainView);

  views.setInitialFramebufferSize(ScreenSize);

  mainView.setSize({ Ground::Width, Ground::Height });

  // add actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);


  gf::Action leftPaddleUpAction("Left paddle up");
  leftPaddleUpAction.addScancodeKeyControl(gf::Scancode::S);
  leftPaddleUpAction.setContinuous();
  actions.addAction(leftPaddleUpAction);

  gf::Action leftPaddleDownAction("Left paddle down");
  leftPaddleDownAction.addScancodeKeyControl(gf::Scancode::X);
  leftPaddleDownAction.setContinuous();
  actions.addAction(leftPaddleDownAction);

  gf::Action rightPaddleUpAction("Right paddle up");
  rightPaddleUpAction.addScancodeKeyControl(gf::Scancode::Up);
  rightPaddleUpAction.setContinuous();
  actions.addAction(rightPaddleUpAction);

  gf::Action rightPaddleDownAction("Left paddle down");
  rightPaddleDownAction.addScancodeKeyControl(gf::Scancode::Down);
  rightPaddleDownAction.setContinuous();
  actions.addAction(rightPaddleDownAction);

  // add entities

  gf::EntityContainer mainEntities;

  Ground ground;
  mainEntities.addEntity(ground);

  Points points;
  mainEntities.addEntity(points);

  Paddle leftPaddle(Paddle::Location::Left);
  mainEntities.addEntity(leftPaddle);

  Paddle rightPaddle(Paddle::Location::Right);
  mainEntities.addEntity(rightPaddle);

  Ball ball;
  mainEntities.addEntity(ball);

  // main loop

  renderer.clear(gf::Color::Gray(0.2f));

  gf::Clock clock;
  bool fullscreen = false;

  while (window.isOpen()) {
    // input
    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      fullscreen = !fullscreen;
      window.setFullscreen(fullscreen);
    }

    if (leftPaddleDownAction.isActive()) {
      leftPaddle.move(Paddle::Move::Down);
    } else if (leftPaddleUpAction.isActive()) {
      leftPaddle.move(Paddle::Move::Up);
    } else {
      leftPaddle.move(Paddle::Move::Stop);
    }

    if (rightPaddleDownAction.isActive()) {
      rightPaddle.move(Paddle::Move::Down);
    } else if (rightPaddleUpAction.isActive()) {
      rightPaddle.move(Paddle::Move::Up);
    } else {
      rightPaddle.move(Paddle::Move::Stop);
    }

    // update
    gf::Time time = clock.restart();
    mainEntities.update(time);

    // render
    renderer.clear();
    renderer.setView(mainView);
    mainEntities.render(renderer);
    renderer.display();

    actions.reset();
  }

  return 0;
}
