/*
 * H.O.M.E.
 * Copyright (C) 2019 Hatunruna team
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
#include <gf/Cursor.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/ModelContainer.h>
#include <gf/RenderWindow.h>
#include <gf/Singleton.h>
#include <gf/Tmx.h>
#include <gf/Unused.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/BackgroundMusic.h"
#include "local/Backpack.h"
#include "local/DayNight.h"
#include "local/Hero.h"
#include "local/Map.h"
#include "local/Physics.h"
#include "local/Singletons.h"
#include "local/Spaceship.h"
#include "local/SupplyManager.h"
#include "local/Timekeeper.h"
#include "local/Timer.h"
#include "local/Victory.h"

#include "config.h"

namespace {

  enum GameState {
    Playing,
    Success,
    Failure,
  };

}

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(800.0f, 800.0f);
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);

  // initialization

  gf::Window window("H.O.M.E. - Harvest Oxygen in the Maldoran Ecosystem", ScreenSize);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  gf::RenderWindow renderer(window);

  // singletons

  gf::SingletonStorage<home::ResourceManager> storageForResourceManager(home::gResourceManager);
  home::gResourceManager().addSearchDir(HOME_DATA_DIR);

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(home::gMessageManager);
  gf::SingletonStorage<gf::Random> storageForRandom(home::gRandom);

  // cursor

  gf::Image image(home::gResourceManager().getAbsolutePath("cursor.png"));
  gf::Cursor cursor(image, { 32, 32 });
  window.setMouseCursor(cursor);

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialFramebufferSize(ScreenSize);

  // background music

  home::BackgroundMusic music;

  // actions

  gf::ActionContainer actions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  actions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  actions.addAction(fullscreenAction);

  gf::Action toogleMuteAction("Toggle Mute");
  toogleMuteAction.addKeycodeKeyControl(gf::Keycode::M);
  actions.addAction(toogleMuteAction);

  // models

  gf::ModelContainer models;

  home::Timekeeper timekeeper;
  models.addModel(timekeeper);

  // entities

  gf::TmxLayers layers;

  if (!layers.loadFromFile(home::gResourceManager().getAbsolutePath("map/Map.tmx"))) {
    gf::Log::error("Unable to load the map!\n");
    return EXIT_FAILURE;
  }

  gf::EntityContainer mainEntities;

  home::SupplyManager supplies;
  mainEntities.addEntity(supplies);

  home::MapGraphicsData data(layers, supplies);
  home::Map mapAbove(home::Map::Above, data);
  home::Map mapBelow(home::Map::Below, data);
  mainEntities.addEntity(mapAbove);
  mainEntities.addEntity(mapBelow);

  home::Hero hero;
  mainEntities.addEntity(hero);

  home::Spaceship spaceship;
  mainEntities.addEntity(spaceship);

  home::DayNight dayNight(timekeeper);
  mainEntities.addEntity(dayNight);

  gf::EntityContainer hudEntities;

  home::Backpack backpack;
  hudEntities.addEntity(backpack);

  home::Timer timer(timekeeper);
  hudEntities.addEntity(timer);

  home::VictoryScreen victory;

  // models (again)

  home::Physics physics(layers, hero);
  models.addModel(physics);

  // game loop

  home::gMessageManager().registerHandler<home::HeroPosition>([&mainView](gf::Id type, gf::Message *msg) {
    assert(type == home::HeroPosition::type);
    auto hero = static_cast<home::HeroPosition*>(msg);
    mainView.setCenter(hero->position);
    return gf::MessageStatus::Keep;
  });

  GameState state = GameState::Playing;

  home::gMessageManager().registerHandler<home::GameOver>([&state](gf::Id type, gf::Message *msg) {
    assert(type == home::GameOver::type);
    gf::unused(type, msg);
    state = GameState::Failure;
    return gf::MessageStatus::Die;
  });

  home::gMessageManager().registerHandler<home::Victory>([&state](gf::Id type, gf::Message *msg) {
    assert(type == home::Victory::type);
    gf::unused(type, msg);
    state = GameState::Success;
    return gf::MessageStatus::Die;
  });

  renderer.clear(gf::Color::White);

  gf::Clock clock;

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      actions.processEvent(event);
      views.processEvent(event);

      if (state == GameState::Playing) {
        if (event.type == gf::EventType::MouseButtonReleased) {
          home::CursorClickedPosition message;
          message.position = renderer.mapPixelToCoords(event.mouseButton.coords, mainView);;
          home::gMessageManager().sendMessage(&message);
        }
      }
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (toogleMuteAction.isActive()) {
      music.toggleMute();
    }


    // 2. update

    gf::Time time = clock.restart();
    mainEntities.update(time);
    hudEntities.update(time);
    models.update(time);


    // 3. draw

    if (state != GameState::Success) {
      renderer.clear();
      renderer.setView(mainView);
      mainEntities.render(renderer);

      renderer.setView(hudView);
      hudEntities.render(renderer);
      renderer.display();
    } else {
      renderer.clear(gf::Color::fromRgba32(0xAE, 0xF6, 0xB7, 0xFF));
      renderer.setView(hudView);
      victory.render(renderer, gf::RenderStates{});
      renderer.display();
    }

    actions.reset();
  }

  return 0;
}
