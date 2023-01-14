/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
#include "GraphicsApi.h"
#include "ModelContainer.h"
#include "RenderWindow.h"
#include "RenderStates.h"
#include "Time.h"
#include "ViewContainer.h"
#include "Views.h"
#include "Window.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_scenes
   * @brief A scene in the game
   *
   * A scene is a set of entities that are handled together in the game. A
   * scene is associated with a set of user actions. A scene is responsible
   * for handling the steps of a game: handling the input (and the actions),
   * updating the entities, rendering the entities.
   *
   * A scene can be associated to a scene manager, with other scenes. A scene
   * is active when it is the top scene on the stack of scenes. The scene
   * manager is responsible for activating and desactivating the scenes.
   *
   * A scene can be either paused or resumed. A paused scene is not updated.
   * In other words, the update step is discarded. It is the user's
   * responsability to pause or resume a scene. An active scene is always
   * resumed when becoming active.
   *
   * A scene can be either hidden or shown. A hidden scene is not rendererd.
   * In other words, the render step is discarded. It is the user's
   * responsability to hide or show a scene. An active scene is always shown
   * when becoming active.
   *
   * @sa gf::SceneManager
   */
  class GF_GRAPHICS_API Scene {
  public:
    /**
     * @brief Constructor
     *
     * At construction, a scene is paused, hidden and inactive.
     *
     * @param initialSize The initial screen size
     *
     * @sa gf::ViewContainer::setInitialScreenSize()
     */
    Scene(Vector2i initialSize);

    /**
     * @brief Destructor
     */
    virtual ~Scene();

    /**
     * @name Game loop
     * @{
     */

    /**
     * @brief Process an event
     *
     * All the registered views and actions are updated with the event.
     *
     * This function can be customized with doProcessEvent() that is called
     * after all other event processing.
     *
     * @param event The event
     *
     * @sa gf::ActionContainer::processEvent(), gf::ViewContainer::processEvent()
     */
    void processEvent(Event& event);

    /**
     * @brief Handle actions
     *
     * This function must be customized with doHandleActions() in order to
     * handle the actions that have been updated with the event processing.
     *
     * At the end of this function, all the actions are resetted.
     *
     * @param window The current window
     *
     * @sa gf::ActionContainer::reset()
     */
    void handleActions(Window& window);

    /**
     * @brief Update the scene
     *
     * This function updates all the models and entities of the scene, if the
     * scene is not paused. It can be customized with doUpdate() that is called
     * after all other updates.
     *
     * @param time The time since the last frame
     *
     * @sa gf::ModelContainer::update(), gf::EntityContainer::update()
     */
    void update(Time time);

    /**
     * @brief Render the scene
     *
     * This function render all the entities of the scene, if the scene is not
     * hidden. It can be customized with doRender().
     *
     * By default, it first renders the world entities (with a gf::ExtendView)
     * through a call to renderWorldEntities() and then it renders the HUD
     * entities (with a gf::ScreenView) through a call to renderHudEntities().
     *
     * @param target The target to render the scene
     * @param states The render states to use for drawing
     *
     * @sa gf::EntityContainer::render()
     */
    void render(RenderTarget& target, const RenderStates &states = RenderStates());

    /**
     * @brief Update the framebuffer size
     *
     * This function is called to update the views in the scene.
     *
     * @param size The new size of the framebuffer
     */
    void setFramebufferSize(gf::Vector2i size);

    /**
     * @brief Set the new clear color
     *
     * @param color The new clear color
     * @sa getClearColor()
     */
    void setClearColor(gf::Color4f color) {
      m_clear = color;
    }

    /**
     * @brief Get the current clear color
     *
     * @returns The current clear color
     * @sa setClearColor();
     */
    gf::Color4f getClearColor() const {
      return m_clear;
    }

    /**
     * @}
     */

    /**
     * @name Scene properties
     * @{
     */

    /**
     * @brief Change the active state of the scene
     *
     * @param active The new state of the scene
     *
     * @sa isActive()
     */
    void setActive(bool active = true);

    /**
     * @brief Check if the scene is active
     *
     * @returns True if the scene is active, and false otherwise
     *
     * @sa setActive()
     */
    bool isActive() const;

    /**
     * @brief Pause the scene
     *
     * When a scene is paused, the entities are not updated.
     *
     * @sa resume(), isPaused()
     */
    void pause();

    /**
     * @brief Resume the scene
     *
     * @sa pause(), isPaused()
     */
    void resume();

    /**
     * @brief Check if the scene is paused
     *
     * @returns True if the scene is paused, and false otherwise
     *
     * @sa pause(), resume()
     */
    bool isPaused() const;

    /**
     * @brief Hide the scene
     *
     * When a scene is hidden, the entities are not rendered.
     *
     * @sa show(), isHidden()
     */
    void hide();

    /**
     * @brief Show the scene
     *
     * @sa hide(), isHidden()
     */
    void show();

    /**
     * @brief Check if the scene is hidden.
     *
     * @returns True if the scene is hidden
     *
     * @sa hide(), show()
     */
    bool isHidden() const;

    /**
     * @}
     */

    /**
     * @name Scene content
     * @{
     */

    /**
     * @brief Add a view to the scene
     *
     * @param view A custom view
     */
    void addView(AdaptativeView& view) {
      m_views.addView(view);
    }

    /**
     * @brief Add an action to the scene
     *
     * @param action The action
     */
    void addAction(Action& action) {
      m_actions.addAction(action);
    }

    /**
     * @brief Add a model to the scene
     *
     * @param model The  model
     */
    void addModel(Model& model) {
      m_models.addModel(model);
    }

    /**
     * @brief Add a world entity to the scene
     *
     * @param entity The entity
     */
    void addWorldEntity(Entity& entity) {
      m_worldEntities.addEntity(entity);
    }

    /**
     * @brief Remove a world entity from the scene
     *
     * @param entity The entity
     */
    Entity* removeWorldEntity(Entity* entity) {
      return m_worldEntities.removeEntity(entity);
    }

    /**
     * @brief Add a HUD entity to the scene
     *
     * @param entity The entity
     */
    void addHudEntity(Entity& entity) {
      m_hudEntities.addEntity(entity);
    }

    /**
     * @brief Remove a HUD entity from the scene
     *
     * @param entity The entity
     */
    Entity* removeHudEntity(Entity* entity) {
      return m_hudEntities.removeEntity(entity);
    }

    /**
     * @}
     */

    /**
     * @name World view management
     * @{
     */

    /**
     * @brief Set the center of the world view
     *
     * @param center The new center
     */
    void setWorldViewCenter(Vector2f center);

    /**
     * @brief Set the size of the world view
     *
     * @param size The new size
     */
    void setWorldViewSize(Vector2f size);

    /**
     * @}
     */

  protected:
    /**
     * @brief Render the world entities
     */
    void renderWorldEntities(RenderTarget& target, const RenderStates &states);

    /**
     * @brief Render the HUD entities
     */
    void renderHudEntities(RenderTarget& target, const RenderStates &states);

    /**
     * @brief Get the world view
     */
    View& getWorldView() {
      return m_worldView;
    }

    /**
     * @brief Get the HUD view
     */
    View& getHudView() {
      return m_hudView;
    }

    /**
     * @brief Get the world entities
     */
    EntityContainer& getWorldEntities() {
      return m_worldEntities;
    }

    /**
     * @brief Get the HUD entities
     */
    EntityContainer& getHudEntities() {
      return m_hudEntities;
    }

    /**
     * @brief Customization point for processEvent()
     *
     * @returns True if the event should not be passed to the other actions
     */
    virtual bool doEarlyProcessEvent(Event& event);

    /**
     * @brief Customization point for processEvent()
     */
    virtual void doProcessEvent(Event& event);

    /**
     * @brief Customization point for handleActions()
     */
    virtual void doHandleActions(Window& window);

    /**
     * @brief Customization point for update()
     */
    virtual void doUpdate(Time time);

    /**
     * @brief Customization point for render()
     */
    virtual void doRender(RenderTarget& target, const RenderStates &states);

    /**
     * @brief Customization point for pause()
     */
    virtual void doPause();

    /**
     * @brief Customization point for resume()
     */
    virtual void doResume();

    /**
     * @brief Customization point for hide()
     */
    virtual void doHide();

    /**
     * @brief Customization point for show()
     */
    virtual void doShow();

    /**
     * @brief Callback when the scene becomes active or inactive
     *
     * @param active The new status of the scene
     */
    virtual void onActivityChange(bool active);

  private:
    friend class SceneManager;

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
    Action m_closeWindowAction;

    ModelContainer m_models;

    ExtendView m_worldView;
    ScreenView m_hudView;
    ViewContainer m_views;

    EntityContainer m_worldEntities;
    EntityContainer m_hudEntities;

    Color4f m_clear;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_SCENE_H
