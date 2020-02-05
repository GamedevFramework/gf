/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <iostream>

#include <gf/Entity.h>
#include <gf/Id.h>
#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <gf/SegueEffects.h>
#include <gf/Sprite.h>
#include <gf/Texture.h>

using namespace gf::literals;

namespace {

  constexpr gf::Vector2i InitialSize = { 1280,  978 };

  constexpr gf::Vector2f ScreenshotSize = { 680,  520 };

  class SpriteEntity : public gf::Entity {
  public:
    SpriteEntity(gf::Path path)
    : m_texture(path)
    {

    }

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override {
      gf::Sprite sprite(m_texture);
      target.draw(sprite, states);
    }

  private:
    gf::Texture m_texture;
  };


  enum class SegueType {
    None,
    Fade,
    Slide,
    Glitch,
  };

  struct World;
  void replaceScene(gf::SceneManager& scenes, World& world, gf::Id id, SegueType type);

  class Scene0 : public gf::Scene {
  public:
    Scene0(gf::SceneManager& scenes, World& world)
    : Scene(InitialSize)
    , m_scenes(scenes)
    , m_world(world)
    , m_segueAction("Segue")
    , m_noneSegueAction("None")
    , m_fadeSegueAction("Fade")
    , m_slideSegueAction("Slide")
    , m_glitchSegueAction("Glitch")
    , m_entity("assets/fb_menu.png")
    , m_segue(SegueType::None)
    {
      m_segueAction.addKeycodeKeyControl(gf::Keycode::Space);
      addAction(m_segueAction);

      m_noneSegueAction.addKeycodeKeyControl(gf::Keycode::Num1);
      addAction(m_noneSegueAction);
      m_fadeSegueAction.addKeycodeKeyControl(gf::Keycode::Num2);
      addAction(m_fadeSegueAction);
      m_slideSegueAction.addKeycodeKeyControl(gf::Keycode::Num3);
      addAction(m_slideSegueAction);
      m_glitchSegueAction.addKeycodeKeyControl(gf::Keycode::Num4);
      addAction(m_glitchSegueAction);

      addWorldEntity(m_entity);

      setWorldViewSize(ScreenshotSize);
      setWorldViewCenter(ScreenshotSize / 2);
    }

    void doHandleActions(gf::Window& window) override {
      if (m_segueAction.isActive()) {
        replaceScene(m_scenes, m_world, "Scene1"_id, m_segue);
      }

      if (m_noneSegueAction.isActive()) {
        std::cout << "Current segue: None\n";
        m_segue = SegueType::None;
      }

      if (m_fadeSegueAction.isActive()) {
        std::cout << "Current segue: Fade\n";
        m_segue = SegueType::Fade;
      }

      if (m_slideSegueAction.isActive()) {
        std::cout << "Current segue: Slide\n";
        m_segue = SegueType::Slide;
      }

      if (m_glitchSegueAction.isActive()) {
        std::cout << "Current segue: Glitch\n";
        m_segue = SegueType::Glitch;
      }
    }

  private:
    gf::SceneManager& m_scenes;
    World& m_world;
    gf::Action m_segueAction;

    gf::Action m_noneSegueAction;
    gf::Action m_fadeSegueAction;
    gf::Action m_slideSegueAction;
    gf::Action m_glitchSegueAction;

    SpriteEntity m_entity;

    SegueType m_segue;
  };

  class Scene1 : public gf::Scene {
  public:
    Scene1(gf::SceneManager& scenes, World& world)
    : Scene(InitialSize)
    , m_scenes(scenes)
    , m_world(world)
    , m_backAction("Back")
    , m_entity("assets/fb_game.png")
    {
      m_backAction.addKeycodeKeyControl(gf::Keycode::Backspace);
      addAction(m_backAction);

      addWorldEntity(m_entity);

      setWorldViewSize(ScreenshotSize);
      setWorldViewCenter(ScreenshotSize / 2);
    }

    void doHandleActions(gf::Window& window) override {
      if (m_backAction.isActive()) {
        replaceScene(m_scenes, m_world, "Scene0"_id, SegueType::None);
      }
    }

  private:
    gf::SceneManager& m_scenes;
    World& m_world;

    gf::Action m_backAction;

    SpriteEntity m_entity;
  };

  struct World {
    Scene0 scene0;
    Scene1 scene1;

    gf::FadeSegueEffect fade;
    gf::SlideSegueEffect slide;
    gf::GlitchSegueEffect glitch;

    World(gf::SceneManager& scenes)
    : scene0(scenes, *this)
    , scene1(scenes, *this)
    {

    }

    gf::Scene& operator()(gf::Id id) {
      switch (id) {
        case "Scene0"_id:
          return scene0;
        case "Scene1"_id:
          return scene1;
      }

      assert(false);
      return scene0;
    }

  };

  void replaceScene(gf::SceneManager& scenes, World& world, gf::Id id, SegueType type) {
    gf::Scene& scene = world(id);

    switch (type) {
      case SegueType::None:
        scenes.replaceScene(scene);
        break;
      case SegueType::Fade:
        scenes.replaceScene(scene, world.fade, gf::milliseconds(500));
        break;
      case SegueType::Slide:
        scenes.replaceScene(scene, world.slide, gf::milliseconds(500));
        break;
      case SegueType::Glitch:
        scenes.replaceScene(scene, world.glitch, gf::milliseconds(500));
        break;
    }
  }

}


int main() {
  std::cout << "Gamedev Framework (gf) example #33: Segues\n";
  std::cout << "This example shows various segues between two scenes.\n";
  std::cout << "How to use:\n";
  std::cout << "\tSpace: Start segue\n";
  std::cout << "\tBackspace: Rewind to first scene\n";
  std::cout << "\t1: No segue\n";
  std::cout << "\t2: Fade\n";
  std::cout << "\t3: Slide\n";
  std::cout << "\t4: Glitch\n";

  gf::SceneManager scenes("33_segues", InitialSize, ~gf::WindowHints::Resizable);
  World world(scenes);

  scenes.pushScene(world.scene0);
  scenes.run();

  return 0;
}
