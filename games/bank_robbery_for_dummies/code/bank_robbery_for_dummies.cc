/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 * Copyright (C) 2017 Julien Bernard
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
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/MessageManager.h>
#include <gf/ModelContainer.h>
#include <gf/PhysicsModel.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Car.h"
#include "local/Level.h"
#include "local/Messages.h"
#include "local/Story.h"

#include "config.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(1000.0f, 1000.0f);
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);

  gf::Vector2f viewCenter(ViewCenter);

  // initialization

  gf::Window window("Bank Robbery for Dummies", ScreenSize);
  gf::RenderWindow renderer(window);

  // managers and utilities

  gf::ResourceManager resources;
  resources.addSearchDir(BRFD_DATA_DIR);

  gf::MessageManager messages;

  gf::Random random;

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(viewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);

  // actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);

  gf::Action leftAction("Turn left");
  leftAction.addScancodeKeyControl(gf::Scancode::A);
  leftAction.addScancodeKeyControl(gf::Scancode::Left);
  leftAction.setContinuous();
  actions.addAction(leftAction);

  gf::Action rightAction("Turn right");
  rightAction.addScancodeKeyControl(gf::Scancode::D);
  rightAction.addScancodeKeyControl(gf::Scancode::Right);
  rightAction.setContinuous();
  actions.addAction(rightAction);

  gf::Action upAction("Accelerate");
  upAction.addScancodeKeyControl(gf::Scancode::W);
  upAction.addScancodeKeyControl(gf::Scancode::Up);
  upAction.setContinuous();
  actions.addAction(upAction);

  gf::Action downAction("Brake");
  downAction.addScancodeKeyControl(gf::Scancode::S);
  downAction.addScancodeKeyControl(gf::Scancode::Down);
  downAction.setContinuous();
  actions.addAction(downAction);

  // entities and models

  // add models
  gf::ModelContainer models;

  gf::PhysicsModel physics;
  models.addModel(physics);

  brfd::StoryModel story(messages);
  models.addModel(story);

  // add entities to mainEntities
  gf::EntityContainer mainEntities;

  brfd::Level level(resources);
  level.generateLevel(random, physics);
  mainEntities.addEntity(level);

  brfd::HeroCar hero(messages, resources, physics);
  hero.startAt(level.getStartingPosition(), level.getStartingAngle());
  mainEntities.addEntity(hero);

  brfd::StoryView storyView(story, messages);
  mainEntities.addEntity(storyView);

  // add entities to hudEntities
  gf::EntityContainer hudEntities;

  brfd::StoryHUD storyHud(story, resources);
  hudEntities.addEntity(storyHud);

  // event

  messages.registerHandler<brfd::HeroPosition>([&mainView](gf::Id type, gf::Message *msg) {
    (void) type;
    auto heroPosition = static_cast<brfd::HeroPosition *>(msg);

    mainView.setCenter(heroPosition->position);

    return gf::MessageStatus::Keep;
  });



  static constexpr float TimeForDisplay = 5.0f;
  static constexpr float TimeForMission = 45.0f;

  story.addCaptionOnly({ brfd::Character::Flo, TimeForDisplay, "Great, that's a nice day. Perfect day to go robbing a bank!" });
  story.addCaptionAndTarget({ brfd::Character::Flo, TimeForDisplay, "I'm gonna meet my friend Roxy that will help me." },
      { TimeForMission, level.getPartnerPosition() }
  );

  story.addCaptionOnly({ brfd::Character::Roxy, TimeForDisplay, "Now that I am here, where do we go?" });
  story.addCaptionAndTarget({ brfd::Character::Flo, TimeForDisplay, "Let's go buy gloves and a balaclava so nobody will recognize us." },
      { TimeForMission, level.getClothingStorePosition() }
  );

  story.addCaptionOnly({ brfd::Character::Flo, TimeForDisplay, "What do we do now?" });
  story.addCaptionOnly({ brfd::Character::Roxy, TimeForDisplay, "Now, we need a gun and ammunitions, just in case." });
  story.addCaptionAndTarget({ brfd::Character::Flo, TimeForDisplay, "OK, I know a cheap shop." },
      { TimeForMission, level.getGunStorePosition() }
  );

  story.addCaptionOnly({ brfd::Character::Roxy, TimeForDisplay, "What do we do now?" });
  story.addCaptionAndTarget({ brfd::Character::Flo, TimeForDisplay, "Mmmhhh. Now that I think about it, we should find a rocket launcher, it would be wiser." },
      { TimeForMission, level.getRocketStorePosition() }
  );

  story.addCaptionAndTarget({ brfd::Character::Flo, TimeForDisplay, "Hurray! We are ready to go rob that bank! \\o/" },
      { TimeForMission, level.getBankPosition() }
  );

  story.addCaptionOnly({ brfd::Character::Flo, TimeForDisplay, "Oh-oh. What do we do now?" });
  story.addCaptionAndTarget({ brfd::Character::Roxy, TimeForDisplay, "Time to go home!" },
      { TimeForMission, level.getHomePosition() }
  );

  story.start();

  // game loop

  renderer.clear(gf::Color::White);

  gf::Clock clock;

  while (window.isOpen()) {
    // 1. input

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
      hero.turnRight();
    } else if (leftAction.isActive()) {
      hero.turnLeft();
    } else {
      hero.turnNone();
    }

    if (upAction.isActive()) {
      hero.accelerate();
    } else if (downAction.isActive()) {
      hero.brake();
    } else {
      hero.cruise();
    }

    // 2. update

    float dt = clock.restart().asSeconds();
    mainEntities.update(dt);
    hudEntities.update(dt);
    models.update(dt);

    // 3. draw

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
