#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Gamepad.h>
#include <gf/Log.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "config.h"
#include "local/Compass.h"
#include "local/Gold.h"
#include "local/DecorationManager.h"
#include "local/Hero.h"
#include "local/Messages.h"
#include "local/Sea.h"
#include "local/Singletons.h"
#include "local/Steam.h"
#include "local/TreasureManager.h"
#include "local/TurretManager.h"
#include "local/WaveManager.h"

int main() {
  static constexpr gf::Vector2u ScreenSize(600, 600);
  static constexpr gf::Vector2f ViewSize(576.0f, 576.0f);
  static constexpr gf::Vector2f ViewCenter(288.0f, 288.0f);

  // initialization

  gf::Window window("Bygone Islands", ScreenSize);
  // window.setFullscreen(true);
  gf::RenderWindow renderer(window);

  gf::SingletonStorage<gf::MessageManager> storageForMessageManager(bi::gMessageManager);
  gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(bi::gResourceManager);
  gf::SingletonStorage<gf::TextureAtlas> storageForTextureAtlas(bi::gTextureAtlas);
  gf::SingletonStorage<gf::Random> storageForRandom(bi::gRandom);

  bi::gResourceManager().addSearchDir(ISLANDS_DATA_DIR);
  bi::gTextureAtlas().loadFromFile("sheet.xml", bi::gResourceManager());

  // splash screen

  gf::Sprite logo(bi::gResourceManager().getTexture("logo.png"));
  renderer.clear();
  renderer.draw(logo);
  renderer.display();

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);

  bi::gMessageManager().registerHandler<bi::HeroPosition>([&mainView](gf::Id type, gf::Message *msg) {
    assert(type == bi::HeroPosition::type);
    auto positionHeroMessage = static_cast<bi::HeroPosition*>(msg);
    mainView.setCenter(positionHeroMessage->position);
    return gf::MessageStatus::Keep;
  });

  // actions

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

  gf::Action scanAction("Scan");
  scanAction.addScancodeKeyControl(gf::Scancode::Space);
  actions.addAction(scanAction);

  // entities

  gf::EntityContainer mainEntities;

  bi::Steam steam;
  mainEntities.addEntity(steam);

  bi::Hero hero(steam, gf::Vector2f(bi::Sea::Size * bi::Sea::TileSize / 2));
  mainEntities.addEntity(hero);

  bi::TreasureManager treasures;
  mainEntities.addEntity(treasures);

  bi::DecorationManager decorationsAbove(bi::DecorationManager::Above);
  mainEntities.addEntity(decorationsAbove);

  bi::DecorationManager decorationsBelow(bi::DecorationManager::Below);
  mainEntities.addEntity(decorationsBelow);

  bi::TurretManager turrets;
  mainEntities.addEntity(turrets);

  bi::Sea sea;
  sea.generate(treasures, decorationsAbove, decorationsBelow, turrets);
  mainEntities.addEntity(sea);

  bi::WaveManager waves;
  mainEntities.addEntity(waves);

  bi::Compass compass;
  mainEntities.addEntity(compass);

  // hud entities

  gf::EntityContainer hudEntities;

  bi::Gold gold;
  hudEntities.addEntity(gold);

  // game loop

  renderer.clear(gf::Color::Black);
  gf::Clock clock;
  window.setFullscreen(true);

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

    if (scanAction.isActive()) {
      if (hero.scanAvailable()) {
        // Send the start scan
        bi::StartScan message;
        bi::gMessageManager().sendMessage(&message);
      }
    }

    if (rightAction.isActive()) {
      hero.turnRight();
    } else if (leftAction.isActive()) {
      hero.turnLeft();
    } else {
      hero.turnNone();
    }

    if (upAction.isActive()) {
      hero.moveForward();
    } else if (downAction.isActive()) {
      hero.moveBackward();
    } else {
      hero.moveStop();
    }

    // 2. update

    float dt = clock.restart().asSeconds();

    if (!gold.isGameOver()) {
      mainEntities.update(dt);
      hudEntities.update(dt);
    }

    // 3. draw

    renderer.clear();

    if (gold.isGameOver() || gold.isWin()) {
      renderer.setView(hudView);
      gold.render(renderer);
    } else {
      renderer.setView(mainView);
      mainEntities.render(renderer);

      renderer.setView(hudView);
      hudEntities.render(renderer);
    }

    renderer.display();

    actions.reset();
  }

  return 0;
}
