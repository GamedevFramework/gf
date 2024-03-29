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
#ifndef GF_VIEWS_H
#define GF_VIEWS_H

#include "GraphicsApi.h"
#include "View.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_views
   * @brief Stretch view
   *
   * This view assumes that the framebuffer is always the same size as the world.
   * The world will then be stretched to fit the framebuffer. There are no black
   * bars, but the aspect ratio may not be the same after the scaling took
   * place.
   *
   * ![Stretch view](@ref stretchview.png)
   *
   * @sa gf::AdaptativeView
   */
  class GF_GRAPHICS_API StretchView : public AdaptativeView {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    StretchView()
    : AdaptativeView()
    {
    }

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit StretchView(const RectF& rect)
    : AdaptativeView(rect)
    {
    }

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    StretchView(Vector2f center, Vector2f size)
    : AdaptativeView(center, size)
    {
    }

    void onFramebufferSizeChange(Vector2i framebufferSize) override;
  };

  /**
   * @ingroup graphics_views
   * @brief Fit view
   *
   * This view will always maintain the aspect ratio of the world, while
   * scaling it as much as possible to fit the framebuffer. One disadvantage
   * with this strategy is that there may appear black bars.
   *
   * ![Fit view](@ref fitview.png)
   *
   * @sa gf::AdaptativeView
   */
  class GF_GRAPHICS_API FitView : public AdaptativeView {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    FitView()
    : AdaptativeView()
    , m_localFramebufferSize(0, 0)
    , m_localViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
    }

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit FitView(const RectF& rect)
    : AdaptativeView(rect)
    , m_localFramebufferSize(0, 0)
    , m_localViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
    }

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    FitView(Vector2f center, Vector2f size)
    : AdaptativeView(center, size)
    , m_localFramebufferSize(0, 0)
    , m_localViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
    }

    void onFramebufferSizeChange(Vector2i framebufferSize) override;

  protected:
    void onSizeChange(Vector2f size) override;
    void onViewportChange(const RectF& viewport) override;

    void updateView();

  private:
    Vector2i m_localFramebufferSize;
    RectF m_localViewport;
  };

  /**
   * @ingroup graphics_views
   * @brief Fill view
   *
   * This view keeps the aspect ratio of the world, but it will always fill
   * the whole framebuffer which might result in parts of the world being cut off.
   *
   * ![Fill view](@ref fillview.png)
   *
   * @sa gf::AdaptativeView
   */
  class GF_GRAPHICS_API FillView : public AdaptativeView {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    FillView()
    : AdaptativeView()
    , m_localSize(1000.0f, 1000.0f)
    , m_localFramebufferSize(0, 0)
    {
    }

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit FillView(const RectF& rect)
    : AdaptativeView(rect)
    , m_localSize(rect.getSize())
    , m_localFramebufferSize(0, 0)
    {
    }

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    FillView(Vector2f center, Vector2f size)
    : AdaptativeView(center, size)
    , m_localSize(size)
    , m_localFramebufferSize(0, 0)
    {
    }

    void onFramebufferSizeChange(Vector2i framebufferSize) override;

  protected:
    void onSizeChange(Vector2f size) override;
    void onViewportChange(const RectF& viewport) override;

    void updateView();

  private:
    Vector2f m_localSize;
    Vector2i m_localFramebufferSize;
  };

  /**
   * @ingroup graphics_views
   * @brief Extend view
   *
   * This view keeps the world aspect ratio without black bars by extending
   * the world in one direction. The world is first scaled to fit within the
   * viewport, then the shorter dimension is lengthened to fill the viewport.
   *
   * ![Extend view](@ref extendview.png)
   *
   * @sa gf::AdaptativeView
   */
  class GF_GRAPHICS_API ExtendView : public AdaptativeView {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    ExtendView()
    : AdaptativeView()
    , m_localSize(1000.0f, 1000.0f)
    , m_localFramebufferSize(0, 0)
    {
    }

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit ExtendView(const RectF& rect)
    : AdaptativeView(rect)
    , m_localSize(rect.getSize())
    , m_localFramebufferSize(0, 0)
    {
    }

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    ExtendView(Vector2f center, Vector2f size)
    : AdaptativeView(center, size)
    , m_localSize(size)
    , m_localFramebufferSize(0, 0)
    {
    }

    void onFramebufferSizeChange(Vector2i framebufferSize) override;

  protected:
    void onSizeChange(Vector2f size) override;
    void onViewportChange(const RectF& viewport) override;

    void updateView();

  private:
    Vector2f m_localSize;
    Vector2i m_localFramebufferSize;
  };


  /**
   * @ingroup graphics_views
   * @brief Locked view
   *
   * This view keeps the world size constant and add black bars if the world
   * is smaller than the framebuffer size or make a zoom in the center of the
   * world if the world is bigger than the framebuffer size.
   *
   * ![Locked view](@ref lockedview.png)
   *
   * @sa gf::AdaptativeView
   */
  class GF_GRAPHICS_API LockedView : public AdaptativeView {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor creates a default view of @f$(0, 0, 1000, 1000)@f$.
     */
    LockedView()
    : AdaptativeView()
    , m_localSize(0.0f, 0.0f)
    , m_localFramebufferSize(0, 0)
    , m_localViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
    }

    /**
     * @brief Construct the view from a rectangle
     *
     * @param rect Rectangle defining the zone to display
     */
    explicit LockedView(const RectF& rect)
    : AdaptativeView(rect)
    , m_localSize(rect.getSize())
    , m_localViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
    }

    /**
     * @brief Construct the view from its center and size
     *
     * @param center Center of the zone to display
     * @param size Size of the zone to display
     */
    LockedView(Vector2f center, Vector2f size)
    : AdaptativeView(center, size)
    , m_localSize(size)
    , m_localViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
    }

    void onFramebufferSizeChange(Vector2i framebufferSize) override;

  protected:
    void onSizeChange(Vector2f size) override;
    void onViewportChange(const RectF& viewport) override;

    void updateView();

  private:
    Vector2f m_localSize;
    Vector2i m_localFramebufferSize;
    RectF m_localViewport;
  };

  /**
   * @ingroup graphics_views
   * @brief Screen view
   *
   * This view will always match the window size which means that no scaling
   * happens and no black bars appear. As a disadvantage this means that the
   * gameplay might change, because a player with a bigger framebuffer might see
   * more of the game, than a player with a smaller framebuffer size.
   *
   * This view can be used to display [HUD](https://en.wikipedia.org/wiki/HUD_%28video_gaming%29).
   *
   * ![Screen view](@ref screenview.png)
   *
   * @sa gf::AdaptativeView, gf::Coordinates
   */
  class GF_GRAPHICS_API ScreenView : public AdaptativeView {
  public:
    /**
     * @brief Default constructor
     */
    ScreenView()
    : AdaptativeView()
    , m_localFramebufferSize(0, 0)
    {
    }

    void onFramebufferSizeChange(Vector2i framebufferSize) override;
    void onViewportChange(const RectF& viewport) override;

    void updateView();

  private:
    Vector2i m_localFramebufferSize;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VIEWS_H
