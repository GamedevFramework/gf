#include <gf/Scene.h>

#include <gf/Unused.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Scene::Scene()
  : m_life(Life::Stopped)
  , m_status(Status::Paused)
  , m_visibility(Visibility::Hidden)
  {

  }

  Scene::~Scene() = default;

  void Scene::loop(Window& window, RenderWindow& target) {
  }

  void Scene::processEvent(Event& event) {
    m_views.processEvent(event);
    m_actions.processEvent(event);
    doProcessEvent(event);
  }

  void Scene::handleActions() {
    doHandleActions();
    m_actions.reset();
  }

  void Scene::update(Time time) {
    if (isPaused()) {
      return;
    }

    m_models.update(time);
    m_mainEntities.update(time);
    m_hudEntities.update(time);
    doUpdate(time);
  }

  void Scene::render(RenderTarget& target) {
    if (isHidden()) {
      return;
    }

    doRender(target);
  }

  void Scene::start() {
    m_life = Life::Started;
  }

  void Scene::stop() {
    m_life = Life::Stopped;
  }

  bool Scene::isStarted() const {
    return m_life == Life::Started;
  }

  void Scene::pause() {
    m_status = Status::Paused;
    doPause();
  }

  void Scene::resume() {
    m_status = Status::Resumed;
    doResume();
  }

  bool Scene::isPaused() const {
    return m_status == Status::Paused;
  }

  void Scene::hide() {
    m_visibility = Visibility::Hidden;
    doHide();
  }

  void Scene::show() {
    m_visibility = Visibility::Shown;
    doShow();
  }

  bool Scene::isHidden() const {
    return m_visibility == Visibility::Hidden;
  }

  void Scene::renderMainEntities(RenderTarget& target) {
    target.setView(m_mainView);
    m_mainEntities.render(target);
  }

  void Scene::renderHudEntities(RenderTarget& target) {
    target.setView(m_hudView);
    m_hudEntities.render(target);
  }

  void Scene::doProcessEvent(Event& event) {
    gf::unused(event);
    // nothing
  }

  void Scene::doHandleActions() {
    // nothing
  }

  void Scene::doUpdate(Time time) {
    gf::unused(time);
    // nothing
  }

  void Scene::doRender(RenderTarget& target) {
    renderMainEntities(target);
    renderHudEntities(target);
  }


  void Scene::doPause() {
    // nothing
  }

  void Scene::doResume() {
    // nothing
  }


  void Scene::doHide() {
    // nothing
  }

  void Scene::doShow() {
    // nothing
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
