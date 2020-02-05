/*
 * Huaca, find the ritual to escape the temple
 * Copyright (C) 2016-2017  Hatunruna team
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
#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/RenderWindow.h>
#include <gf/Unused.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Display.h"
#include "local/Hero.h"
#include "local/Level.h"
#include "local/Messages.h"
#include "local/Singletons.h"
#include "local/Timer.h"

#include "config.h"

static constexpr float VisibleSize = 320.0f;

int main() {
  gf::Log::setLevel(gf::Log::Info);

  // initialize window

  gf::Vector2i screenSize = { 1024, 576 };
  gf::Window window("Huaca", screenSize);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  gf::RenderWindow renderer(window);

  // setup singletons

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(huaca::gMessageManager);
  gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(huaca::gResourceManager);

  // setup resource directories

  huaca::gResourceManager().addSearchDir(HUACA_DATA_DIR);

  // add cameras

  gf::ViewContainer views;

  gf::ExtendView mainView;
  mainView.setSize({ VisibleSize, VisibleSize });
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialFramebufferSize(screenSize);

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
  leftAction.setContinuous();
  actions.addAction(leftAction);

  gf::Action rightAction("Right");
  rightAction.addScancodeKeyControl(gf::Scancode::D);
  rightAction.addScancodeKeyControl(gf::Scancode::Right);
  rightAction.setContinuous();
  actions.addAction(rightAction);

  gf::Action upAction("Up");
  upAction.addScancodeKeyControl(gf::Scancode::W);
  upAction.addScancodeKeyControl(gf::Scancode::Up);
  upAction.setContinuous();
  actions.addAction(upAction);

  gf::Action downAction("Down");
  downAction.addScancodeKeyControl(gf::Scancode::S);
  downAction.addScancodeKeyControl(gf::Scancode::Down);
  downAction.setContinuous();
  actions.addAction(downAction);

  gf::Action portalAction("Portal");
  portalAction.addKeycodeKeyControl(gf::Keycode::Space);
  actions.addAction(portalAction);

  // add entities

  gf::Random random;

  gf::EntityContainer mainEntities;

  huaca::Hero hero;
  mainEntities.addEntity(hero);

  huaca::Level level;
  level.generateFirst(random);
  mainEntities.addEntity(level);

  huaca::Roof roof = level.getRoof();
  mainEntities.addEntity(roof);


  gf::EntityContainer hudEntities;

  huaca::Display display;
  hudEntities.addEntity(display);

  huaca::Timer timer;
  hudEntities.addEntity(timer);


  // handle messages

  huaca::gMessageManager().registerHandler<huaca::HeroPositionMessage>([&mainView](gf::Id id, gf::Message *msg) {
    assert(id == huaca::HeroPositionMessage::type);
    gf::unused(id);
    auto heroPosition = static_cast<huaca::HeroPositionMessage*>(msg);
    mainView.setCenter(heroPosition->position);
    return gf::MessageStatus::Keep;
  });

  // main loop

  renderer.clear(gf::Color::Black);

  gf::Clock clock;

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
      window.toggleFullscreen();
    }

    if (rightAction.isActive()) {
      hero.goRight();
    } else if (leftAction.isActive()) {
      hero.goLeft();
    } else if (upAction.isActive()) {
      hero.goUp();
    } else if (downAction.isActive()) {
      hero.goDown();
    } else {
      hero.stop();
    }

    if (portalAction.isActive()) {
      hero.dropPortal();
    }

    // update

    if (level.isFinished()) {
      level.generateNew(random);
      roof = level.getRoof();
    }

    gf::Time time = clock.restart();
    mainEntities.update(time);
    hudEntities.update(time);

    // render

    renderer.clear();

    renderer.setView(mainView);
    mainEntities.render(renderer);

    renderer.setView(hudView);
    hudEntities.render(renderer);

    renderer.display();

    actions.reset();
  }

  return 0;
}
