/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_VIEW_H
#define GF_VIEW_H

#include "Matrix.h"
#include "Portability.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  struct Event;
  class RenderTarget;

  /**
   * @ingroup graphics
   * @brief 2D camera that defines what region is shown on screen
   *
   * gf::View defines a camera in the 2D scene. This is a
   * very powerful concept: you can scroll, rotate or zoom
   * the entire scene without altering the way that your
   * drawable objects are drawn.
   *
   * A view is composed of a source rectangle, which defines
   * what part of the 2D scene is shown, and a target viewport,
   * which defines where the contents of the source rectangle
   * will be displayed on the render target (window or texture).
   *
   * The viewport allows to map the scene to a custom part
   * of the render target, and can be used for split-screen
   * or for displaying a minimap, for example. If the source
   * rectangle has not the same size as the viewport, its
   * contents will be stretched to fit in.
   *
   * To apply a view, you have to assign it to the render target.
   * Then, every objects drawn in this render target will be
   * affected by the view until you use another view.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::RenderWindow renderer;
   * gf::View view;
   *
   * // Initialize the view to a rectangle located at (100, 100) and
   * // with a size of 400x200
   * view.reset({ 100.0f, 100.0f, 400.0f, 200.0f });
   *
   * // Rotate it by 45 degrees
   * view.rotate(gf::Pi / 4);
   *
   * // Set its target viewport to be half of the window
   * view.setViewport({ 0.f, 0.f, 0.5f, 1.f });
   *
   * // Apply it
   * renderer.setView(view);
   *
   * // Render stuff
   * renderer.draw(someSprite);
   *
   * // Set the default view back
   * renderer.setView(renderer.getDefaultView());
   *
   * // Render stuff not affected by the view
   * renderer.draw(someOtherSprite);
   * ~~~
   *
   * @sa gf::RenderTarget, gf::AdaptiveView
   */
  class GF_API View {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    View();

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit View(const RectF& rect);

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    View(Vector2f center, Vector2f size);

    /**
     * @brief Destructor
     */
    virtual ~View();

    /**
     * @brief Set the center of the view
     *
     * @param center New center
     *
     * @sa getCenter()
     */
    void setCenter(Vector2f center) {
      m_center = center;
    }

    /**
     * @brief Get the center of the view
     *
     * @return Center of the view
     *
     * @sa setCenter()
     */
    Vector2f getCenter() const {
      return m_center;
    }

    /**
     * @brief Set the size of the view
     *
     * @param size New size
     *
     * @sa getSize()
     */
    void setSize(Vector2f size) {
      m_size = size;
      onSizeChange(m_size);
    }

    /**
     * @brief Get the size of the view
     *
     * @return Size of the view
     *
     * @sa setSize()
     */
    Vector2f getSize() const {
      return m_size;
    }

    /**
     * @brief Set the orientation of the view
     *
     * The default rotation of a view is 0 degree.
     *
     * @param rotation New angle, in radians
     *
     * @sa getRotation()
     */
    void setRotation(float rotation) {
      m_rotation = rotation;
    }

    /**
     * @brief Get the current orientation of the view
     *
     * @return Rotation angle of the view, in radians
     *
     * @sa setRotation()
     */
    float getRotation() const {
      return m_rotation;
    }

    /**
     * @brief Set the target viewport
     *
     * The viewport is the rectangle into which the contents of the
     * view are displayed, expressed as a factor (between 0 and 1)
     * of the size of the RenderTarget to which the view is applied.
     *
     * For example, a view which takes the left side of the target would
     * be defined with:
     *
     * ~~~{.cc}
     * view.setViewport({0.0f, 0.0f, 0.5f, 1.0f}).
     * ~~~
     *
     * By default, a view has a viewport which covers the entire target.
     *
     * @param viewport New viewport rectangle
     *
     * @sa getViewport()
     */
    void setViewport(const RectF& viewport);

    /**
     * @brief Get the target viewport rectangle of the view
     *
     * @return Viewport rectangle, expressed as a factor of the target size
     *
     * @sa setViewport()
     */
    const RectF& getViewport() const {
      return m_viewport;
    }

    /**
     * @brief Reset the view to the given rectangle
     *
     * Note that this function resets the rotation angle to 0.
     *
     * @param rect Rectangle defining the zone to display
     *
     * @sa setCenter(), setSize(), setRotation()
     */
    void reset(const RectF& rect);

    /**
     * @brief Move the view relatively to its current position
     *
     * @param offset Move offset
     *
     * @sa setCenter(), rotate(), zoom()
     */
    void move(Vector2f offset);

    /**
     * @brief Rotate the view relatively to its current orientation
     *
     * @param angle Angle to rotate, in radians
     *
     * @sa setRotation(), move(), zoom()
     */
    void rotate(float angle);

    /**
     * @brief Resize the view rectangle relatively to its current size
     *
     * Resizing the view simulates a zoom, as the zone displayed on
     * screen grows or shrinks. `factor` is a multiplier:
     *
     * - @f$ = 1 @f$ keeps the size unchanged
     * - @f$ > 1 @f$ makes the view bigger (objects appear smaller)
     * - @f$ < 1 @f$ makes the view smaller (objects appear bigger)
     *
     * @param factor Zoom factor to apply
     *
     * @sa setSize(), move(), rotate()
     */
    void zoom(float factor);

    /**
     * @brief Resize the view rectangle relatively to its current size and a fixed point
     *
     * Resizing the view simulates a zoom, as the zone displayed on
     * screen grows or shrinks. `factor` is a multiplier:
     *
     * - @f$ = 1 @f$ keeps the size unchanged
     * - @f$ > 1 @f$ makes the view bigger (objects appear smaller)
     * - @f$ < 1 @f$ makes the view smaller (objects appear bigger)
     *
     * Additionally, a fixed point is used as the center of the zoom. It is the
     * only point that stays at the same place in the view.
     *
     * @param factor Zoom factor to apply
     * @param fixed The center of the zoom
     *
     * @sa setSize(), move(), rotate()
     */
    void zoom(float factor, Vector2f fixed);

    /**
     * @brief Get the projection transform of the view
     *
     * This function is meant for internal use only.
     *
     * @return Projection transform defining the view
     *
     * @sa getInverseTransform()
     */
    Matrix3f getTransform() const;

    /**
     * @brief Get the inverse projection transform of the view
     *
     * This function is meant for internal use only.
     *
     * @return Inverse of the projection transform defining the view
     *
     * @sa getTransform
     */
    Matrix3f getInverseTransform() const;

  protected:
    /**
     * @brief Set the world size, without calling onSizeChange()
     *
     * This function is meant for adaptative views so that they can
     * adapt the world size without having a callback infinite loop.
     *
     * @param size The new world size
     */
    void setSizeNoCallback(Vector2f size) {
      m_size = size;
    }

    /**
     * @brief Callback when the world has just been resized
     *
     * This callback is called when setSize() is called.
     *
     * @param size The new size of the visible world
     */
    virtual void onSizeChange(Vector2f size);

    /**
     * @brief Set the viewport, without calling onViewportChange()
     *
     * This function is meant for adaptative views so that they can
     * adapt the viewport without having a callback infinite loop.
     *
     * @param viewport The new viewport
     */
    void setViewportNoCallback(const RectF& viewport);

    /**
     * @brief Callback when the viewport has just been changed
     *
     * @param viewport The new viewport
     */
    virtual void onViewportChange(const RectF& viewport);

  private:
    Vector2f m_center;
    Vector2f m_size;
    float m_rotation;
    RectF m_viewport;
  };


  /**
   * @ingroup graphics
   * @brief Adaptative view
   *
   * An adaptative view is a view that adapts automatically to screen
   * resolution change.
   *
   * There are several kinds of adaptative views, according to the policy
   * that is adopted when the resolution changes. In the examples below,
   * The screen is represented by the black rectangle and the world is
   * the red square. If red dashed lines appears, it means that the world
   * has been modified.
   *
   * | Class           | Example                     |
   * |-----------------|-----------------------------|
   * | gf::StretchView | @image html stretchview.png |
   * | gf::FitView     | @image html fitview.png     |
   * | gf::FillView    | @image html fillview.png    |
   * | gf::ExtendView  | @image html extendview.png  |
   * | gf::LockedView  | @image html lockedview.png  |
   * | gf::ScreenView  | @image html screenview.png  |
   *
   * @sa gf::ViewContainer
   */
  class GF_API AdaptativeView : public View {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    AdaptativeView()
    : View()
    {

    }

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit AdaptativeView(const RectF& rect)
    : View(rect)
    {

    }

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    AdaptativeView(Vector2f center, Vector2f size)
    : View(center, size)
    {

    }

    /**
     * @brief Set the initial screen size
     *
     * @param screenSize The initial size of the screen
     */
    void setInitialScreenSize(Vector2u screenSize);

    /**
     * @brief Callback when the screen has just been resized
     *
     * @param screenSize The new size of the screen
     */
    virtual void onScreenSizeChange(Vector2u screenSize) = 0;

  };

  /**
   * @ingroup graphics
   * @brief A view adaptor for zooming/moving with the mouse
   */
  class GF_API ZoomingViewAdaptor {
  public:
    /**
     * @brief Constructor
     *
     * @param target The rendering target
     * @param view The original view to zoom/move
     */
    ZoomingViewAdaptor(const RenderTarget& target, View& view);

    /**
     * @brief Update the original view thanks to the event
     *
     * @param event An event
     */
    void processEvent(const Event& event);

  private:
    const RenderTarget& m_target;
    View& m_view;
    gf::Vector2i m_mousePosition;

    enum class State {
      Stationary,
      Moving,
    };

    State m_state;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VIEW_H
