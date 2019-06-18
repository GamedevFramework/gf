/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#ifndef GF_SCENE_H
#define GF_SCENE_H

#include "Action.h"
#include "EntityContainer.h"
#include "Event.h"
#include "ModelContainer.h"
#include "RenderWindow.h"
#include "Time.h"
#include "ViewContainer.h"
#include "Views.h"
#include "Window.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Scene {
  public:
    Scene();
    virtual ~Scene();

    void processEvent(Event& event);
    void handleActions();
    void update(Time time);
    void render(RenderTarget& target);

    void setActive(bool active = true);
    bool isActive() const;

    void pause();
    void resume();

    bool isPaused() const;

    void hide();
    void show();

    bool isHidden() const;

    void addView(AdaptativeView& view) {
      m_views.addView(view);
    }

    void addAction(Action& action) {
      m_actions.addAction(action);
    }

    void addModel(Model& model) {
      m_models.addModel(model);
    }

    void addMainEntity(Entity& entity) {
      m_mainEntities.addEntity(entity);
    }

    void addHudEntity(Entity& entity) {
      m_hudEntities.addEntity(entity);
    }

  protected:
    void renderMainEntities(RenderTarget& target);
    void renderHudEntities(RenderTarget& target);

  private:
    virtual void doProcessEvent(Event& event);
    virtual void doHandleActions();
    virtual void doUpdate(Time time);
    virtual void doRender(RenderTarget& target);

    virtual void doPause();
    virtual void doResume();

    virtual void doHide();
    virtual void doShow();

  private:
    bool m_active;

    enum class Status {
      Paused,
      Resumed,
    };

    Status m_status;

    enum class Visibility {
      Shown,
      Hidden,
    };

    Visibility m_visibility;

    ActionContainer m_actions;

    ModelContainer m_models;

    ExtendView m_mainView;
    ScreenView m_hudView;
    ViewContainer m_views;

    EntityContainer m_mainEntities;
    EntityContainer m_hudEntities;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_SCENE_H
