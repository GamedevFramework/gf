#include <gf/SceneManager.h>

#include <cassert>

#include <gf/Color.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SceneManager::SceneManager(StringRef title, Vector2i size)
  : m_window(title, size)
  , m_renderer(m_window)
  {

  }

  void SceneManager::run() {
    Clock clock;
    m_renderer.clear(Color::White);

    while (!m_scenes.empty()) {
      Scene& currentScene = m_scenes.back();
      currentScene.start();
      currentScene.show();
      currentScene.resume();

      while (currentScene.isStarted() && m_window.isOpen()) {
        Event event;

        while (m_window.pollEvent(event)) {
          currentScene.processEvent(event);
        }

        currentScene.handleActions();

        Time time = clock.restart();
        currentScene.update(time);

        m_renderer.clear();
        currentScene.render(m_renderer);
        m_renderer.display();
      }

      currentScene.stop();
    }
  }

  void SceneManager::pushScene(Scene& scene) {
    m_scenes.push_back(scene);
  }

  void SceneManager::popScene() {
    assert(!m_scenes.empty());
    m_scenes.pop_back();
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
