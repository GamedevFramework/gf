#include <gf/SceneManager.h>

#include <cassert>

#include <gf/Color.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    void updateAndRenderScenes(Time time, std::vector<Ref<Scene>>& scenes, RenderTexture& target, const RenderStates& states) {
      target.setActive();

      for (Scene& scene : scenes) {
        scene.update(time);
      }

      target.clear(scenes.back().get().getClearColor());

      for (Scene& scene : scenes) {
        scene.render(target, states);
      }

      target.display();
    }

  }

  SceneManager::SceneManager(StringRef title, Vector2i size)
  : m_window(title, size)
  , m_renderer(m_window)
  , m_targetOldScenes(nullptr)
  , m_targetNewScenes(nullptr)
  , m_status(Status::Scene)
  {

  }

  void SceneManager::run(const RenderStates &states) {
    Clock clock;
    m_renderer.clear(Color::White);

    while (!m_scenes.empty() && m_window.isOpen()) {
      Scene& currentScene = m_scenes.back();
      currentScene.setRendererSize(m_renderer.getSize());
      currentScene.show();
      currentScene.resume();

      auto clearColor = currentScene.getClearColor();

      while (currentScene.isActive() && m_window.isOpen()) {
        Event event;

        while (m_window.pollEvent(event)) {
          for (Scene& scene : m_scenes) {
            scene.processEvent(event);
          }
        }

        for (Scene& scene : m_scenes) {
          scene.handleActions(m_window);
        }

        Time time = clock.restart();

        if (m_status == Status::Scene) {

          for (Scene& scene : m_scenes) {
            scene.update(time);
          }

          m_renderer.clear(clearColor);

          for (Scene& scene : m_scenes) {
            scene.render(m_renderer, states);
          }

          m_renderer.display();

        } else {
          assert(m_status == Status::Segue);

          m_segue.update(time);

          updateAndRenderScenes(time, m_oldScenes, *m_targetOldScenes, states);
          updateAndRenderScenes(time, m_scenes, *m_targetNewScenes, states);

          m_renderer.setActive();
          m_renderer.clear(currentScene.getClearColor());
//           m_renderer.setCanonicalScissorBox(Region{ 0, 0, m_renderer.getSize().x, m_renderer.getSize().y });
          m_renderer.draw(m_segue, states);
          m_renderer.display();

          if (!m_segue.isActive()) {
            m_targetOldScenes = nullptr;
            m_targetNewScenes = nullptr;
            m_status = Status::Scene;
          }
        }
      }
    }
  }

  namespace {

    void activate(Scene& scene) {
      scene.setActive(true);
    }

    void desactivate(Scene& scene) {
      scene.setActive(false);
    }

  }

  void SceneManager::pushScene(Scene& scene) {
    if (!m_scenes.empty()) {
      desactivate(m_scenes.back());
    }

    m_scenes.push_back(scene);
    activate(m_scenes.back());
  }

  void SceneManager::popScene() {
    assert(!m_scenes.empty());
    desactivate(m_scenes.back());
    m_scenes.pop_back();

    if (!m_scenes.empty()) {
      activate(m_scenes.back());
    }
  }


  void SceneManager::popAllScenes() {
    assert(!m_scenes.empty());

    while (!m_scenes.empty()) {
      desactivate(m_scenes.back());
      m_scenes.pop_back();
    }
  }

  void SceneManager::replaceScene(Scene& scene, SegueEffect& effect, Time duration) {
    setupSegue(effect, duration);
    popScene();
    pushScene(scene);
  }

  void SceneManager::replaceAllScenes(Scene& scene, SegueEffect& effect, Time duration) {
    setupSegue(effect, duration);
    popAllScenes();
    pushScene(scene);
  }

  void SceneManager::setupSegue(SegueEffect& effect, Time duration) {
    m_targetOldScenes = std::make_unique<RenderTexture>(m_renderer.getSize());
    m_targetNewScenes = std::make_unique<RenderTexture>(m_renderer.getSize());

    m_segue.setTextures(m_targetOldScenes->getTexture(), m_targetNewScenes->getTexture());
    m_segue.setEffect(effect);
    m_segue.start(duration);

    m_oldScenes = m_scenes;

    m_status = Status::Segue;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
