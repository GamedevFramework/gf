/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016-2017  Lux team
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
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Bonus.h"
#include "local/Bullets.h"
#include "local/Hero.h"
#include "local/Particles.h"
#include "local/Scenario.h"
#include "local/Stars.h"
#include "local/World.h"

#include "config.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);

  // initialization

  gf::Window window("Lux - Year of Light", ScreenSize);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  gf::RenderWindow renderer(window);

  // resources

  gf::ResourceManager resources;
  resources.addSearchDir(LUX_DATA_DIR);

  // messages

  gf::MessageManager messages;

  // views

  gf::ViewContainer views;

  gf::FitView mainView(lux::WorldCenter, lux::WorldSize);
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

  // entities

  gf::Random random;

  gf::EntityContainer hudEntities;
  gf::EntityContainer mainEntities;

  lux::Stars stars1(random, 125, 1.5f);
  mainEntities.addEntity(stars1);

  lux::Stars stars2(random, 75, 1.25f);
  mainEntities.addEntity(stars2);

  lux::Stars stars3(random,  20, 1.0f, lux::Stars::AllWhiteStars);
  mainEntities.addEntity(stars3);

  lux::HeroProperties heroProperties(resources);
  hudEntities.addEntity(heroProperties);

  lux::Hero hero(heroProperties, messages, resources);
  mainEntities.addEntity(hero);

  lux::EnemyManager enemies(random, messages, resources);
  mainEntities.addEntity(enemies);

  lux::Scenario scenario(enemies, messages, resources);
  hudEntities.addEntity(scenario);

  lux::Bullets bullets(messages, resources);
  mainEntities.addEntity(bullets);

  lux::BonusManager bonus(random, messages, resources);
  mainEntities.addEntity(bonus);

  lux::Particles particles(random, messages);
  mainEntities.addEntity(particles);

  // game loop

  renderer.clear(gf::Color::Black);

  gf::Clock clock;
  bool fullscreen = false;

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);

      if (event.type == gf::EventType::MouseMoved) {
        hero.setPosition(renderer.mapPixelToCoords(event.mouseCursor.coords, mainView));
      }
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      fullscreen = !fullscreen;
      window.setFullscreen(fullscreen);
    }


    // 2. update

    gf::Time time = clock.restart();
    mainEntities.update(time);
    hudEntities.update(time);


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
