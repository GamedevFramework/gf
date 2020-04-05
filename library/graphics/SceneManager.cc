#include <gf/SceneManager.h>

#include <cassert>

#include <gf/Color.h>
#include <gf/Log.h>

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

  SceneManager::SceneManager(StringRef title, Vector2i size, Flags<WindowHints> hints)
  : m_window(title, size, hints)
  , m_renderer(m_window)
  , m_scenesChanged(false)
  , m_targetCurrScenes(size)
  , m_targetPrevScenes(size)
  , m_status(Status::Scene)
  {
    m_view.onFramebufferSizeChange(size);
  }

  void SceneManager::run(const RenderStates &states) {
    Clock clock;
    m_renderer.clear(Color::White);

    while (!m_currScenes.empty() && m_window.isOpen()) {
      std::vector<Ref<Scene>> scenes = m_currScenes; // make a copy to avoid iterator invalidation
      m_scenesChanged = false;

      Scene& currentScene = scenes.back();
      currentScene.show();
      currentScene.resume();

      auto size = m_renderer.getSize();

      for (Scene& scene : scenes) {
        scene.setFramebufferSize(size);
      }

      auto clearColor = currentScene.getClearColor();

      while (!m_scenesChanged && m_window.isOpen()) {
        bool transition = (m_status == Status::Segue);
        Event event;

        while (m_window.pollEvent(event)) {
          if (event.type == EventType::Resized) {
            m_targetPrevScenes.resize(event.size);
            m_targetCurrScenes.resize(event.size);
            m_view.onFramebufferSizeChange(event.size);
            m_segue.setTextures(m_targetPrevScenes.getTexture(), m_targetCurrScenes.getTexture());
          }

          for (Scene& scene : scenes) {
            scene.processEvent(event);
          }
        }

        for (Scene& scene : scenes) {
          scene.handleActions(m_window);
        }

        Time time = clock.restart();

        if (!transition) {
          for (Scene& scene : scenes) {
            scene.update(time);
          }

          m_renderer.clear(clearColor);

          for (Scene& scene : scenes) {
            scene.render(m_renderer, states);
          }

          m_renderer.display();
        } else {
          m_segue.update(time);

          updateAndRenderScenes(time, m_prevScenes, m_targetPrevScenes, states);
          updateAndRenderScenes(time, m_currScenes, m_targetCurrScenes, states);

          m_renderer.setActive();
          m_renderer.clear(currentScene.getClearColor());
          m_renderer.setView(m_view);
          m_renderer.draw(m_segue, states);
          m_renderer.display();

          if (!m_segue.isActive()) {
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
    if (m_status == Status::Segue) {
      gf::Log::warning("You should not push a scene during a transition.\n");
      return;
    }

    m_scenesChanged = true;

    if (!m_currScenes.empty()) {
      desactivate(m_currScenes.back());
    }

    m_currScenes.push_back(scene);
    activate(m_currScenes.back());
  }

  void SceneManager::popScene() {
    if (m_status == Status::Segue) {
      gf::Log::warning("You should not pop a scene during a transition.\n");
      return;
    }

    m_scenesChanged = true;

    assert(!m_currScenes.empty());
    desactivate(m_currScenes.back());
    m_currScenes.pop_back();

    if (!m_currScenes.empty()) {
      activate(m_currScenes.back());
    }
  }


  void SceneManager::popAllScenes() {
    if (m_status == Status::Segue) {
      gf::Log::warning("You should not pop all scenes during a transition.\n");
      return;
    }

    m_scenesChanged = true;

    assert(!m_currScenes.empty());

    while (!m_currScenes.empty()) {
      desactivate(m_currScenes.back());
      m_currScenes.pop_back();
    }
  }

  void SceneManager::replaceScene(Scene& scene, SegueEffect& effect, Time duration, Easing easing) {
    setupSegue(effect, duration, easing);
    popScene();
    pushScene(scene);
    m_status = Status::Segue;
  }

  void SceneManager::replaceAllScenes(Scene& scene, SegueEffect& effect, Time duration, Easing easing) {
    setupSegue(effect, duration, easing);
    popAllScenes();
    pushScene(scene);
    m_status = Status::Segue;
  }

  Vector2f SceneManager::computeWindowToGameCoordinates(Vector2i coords, const View& view) const {
    return m_renderer.mapPixelToCoords(coords, view);
  }

  Vector2i SceneManager::computeGameToWindowCoordinates(Vector2f coords, const View& view) const {
    return m_renderer.mapCoordsToPixel(coords, view);
  }

  void SceneManager::setupSegue(SegueEffect& effect, Time duration, Easing easing) {
    m_segue.setTextures(m_targetPrevScenes.getTexture(), m_targetCurrScenes.getTexture());
    m_segue.setEffect(effect);
    m_segue.setEasing(easing);
    m_segue.start(duration);

    m_prevScenes = m_currScenes;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
