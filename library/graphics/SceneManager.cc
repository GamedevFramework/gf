#include <gf/SceneManager.h>

#include <cassert>

#include <gf/Color.h>
#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SceneManager::SceneManager(StringRef title, Vector2i size)
  : m_window(title, size)
  , m_renderer(m_window)
  {
    if (s_instance != nullptr) {
      gf::Log::warning("An instance of SceneManager is already registered as the current instance. You should not use SceneManager::get().");
    }

    s_instance = this;
  }

  SceneManager::~SceneManager() {
    s_instance = nullptr;
  }

  void SceneManager::run() {
    Clock clock;
    m_renderer.clear(Color::White);

    while (!m_scenes.empty() && m_window.isOpen()) {
      Scene& currentScene = m_scenes.back();
      currentScene.show();
      currentScene.resume();

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

        for (Scene& scene : m_scenes) {
          scene.update(time);
        }

        m_renderer.clear();

        for (Scene& scene : m_scenes) {
          scene.render(m_renderer);
        }

        m_renderer.display();
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

  SceneManager& SceneManager::get() {
    return *s_instance;
  }

  SceneManager *SceneManager::s_instance = nullptr;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
