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
  , m_clearColor(Color::White)
  , m_targetOldScenes(nullptr)
  , m_targetNewScenes(nullptr)
  {

  }

  void SceneManager::run(const RenderStates &states) {
    Clock clock;
    m_renderer.clear(m_clearColor);

    while (!m_scenes.empty() && m_window.isOpen()) {
      Scene& currentScene = m_scenes.back();
      currentScene.m_views.setInitialScreenSize(m_renderer.getSize());
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

        if (m_transition.isActive())
        {
          for (Scene& scene : m_oldScenes) {
            scene.update(time);
          }
        }

        for (Scene& scene : m_scenes) {
          scene.update(time);
        }

        if (m_transition.isActive())
        {
          m_transition.update(time.asSeconds());

          m_targetOldScenes->setActive();

          for (Scene& scene : m_oldScenes) {
            scene.update(time);
          }

          m_targetOldScenes->clear(m_clearColor);

          for (Scene& scene : m_oldScenes) {
            scene.render(*m_targetOldScenes, states);
          }

          m_targetNewScenes->setActive();

          for (Scene& scene : m_scenes) {
            scene.update(time);
          }

          m_targetNewScenes->clear(m_clearColor);

          for (Scene& scene : m_scenes) {
            scene.render(*m_targetNewScenes, states);
          }

          m_renderer.setActive();

          m_renderer.clear();

          m_renderer.setCanonicalScissorBox(Region{ 0, 0, m_renderer.getSize().x, m_renderer.getSize().y });

          m_transition.draw(m_renderer, states);

          if (!m_transition.isActive())
          {
            delete m_targetOldScenes;
            delete m_targetNewScenes;
          }
        }
        else
        {
          m_renderer.clear(m_clearColor);

          for (Scene& scene : m_scenes) {
            scene.render(m_renderer, states);
          }
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

  void SceneManager::transitionToScene(Scene& scene, float time, TransitionEffect& effect) {
    m_targetOldScenes = new RenderTexture(m_window.getSize());
    m_targetNewScenes = new RenderTexture(m_window.getSize());

    m_transition.setTextures(m_targetOldScenes->getTexture(), m_targetNewScenes->getTexture());
    m_transition.setEffect(effect);
    m_transition.start(time);

    m_oldScenes = m_scenes;

    while (!m_scenes.empty())
    {
      desactivate(m_scenes.back());
      m_scenes.pop_back();
    }

    m_scenes.push_back(scene);
    activate(m_scenes.back());
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
