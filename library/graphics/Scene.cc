#include <gf/Scene.h>

#include <gf/Unused.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Scene::Scene(Vector2i initialSize)
  : m_active(false)
  , m_status(Status::Paused)
  , m_visibility(Visibility::Hidden)
  , m_closeWindowAction("Close")
  {
    m_views.addView(m_worldView);
    m_views.addView(m_hudView);
    m_views.setInitialScreenSize(initialSize);

    m_closeWindowAction.addCloseControl();
    m_actions.addAction(m_closeWindowAction);
  }

  Scene::~Scene() = default;

  void Scene::processEvent(Event& event) {
    m_views.processEvent(event);
    m_actions.processEvent(event);
    doProcessEvent(event);
  }

  void Scene::handleActions(Window& window) {
    if (m_closeWindowAction.isActive()) {
      window.close();
    }

    doHandleActions(window);
    m_actions.reset();
  }

  void Scene::update(Time time) {
    if (isPaused()) {
      return;
    }

    m_models.update(time);
    m_worldEntities.update(time);
    m_hudEntities.update(time);
    doUpdate(time);
  }

  void Scene::render(RenderTarget& target) {
    if (isHidden()) {
      return;
    }

    doRender(target);
  }

  void Scene::setActive(bool active) {
    m_active = active;
  }

  bool Scene::isActive() const {
    return m_active;
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

  void Scene::setWorldViewCenter(Vector2f center) {
    m_worldView.setCenter(center);
  }

  void Scene::setWorldViewSize(Vector2f size) {
    m_worldView.setSize(size);
  }

  void Scene::renderWorldEntities(RenderTarget& target) {
    target.setView(m_worldView);
    m_worldEntities.render(target);
  }

  void Scene::renderHudEntities(RenderTarget& target) {
    target.setView(m_hudView);
    m_hudEntities.render(target);
  }

  void Scene::doProcessEvent(Event& event) {
    gf::unused(event);
    // nothing
  }

  void Scene::doHandleActions(Window& window) {
    gf::unused(window);
    // nothing
  }

  void Scene::doUpdate(Time time) {
    gf::unused(time);
    // nothing
  }

  void Scene::doRender(RenderTarget& target) {
    renderWorldEntities(target);
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
