/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef GF_WINDOW_GEOMETRY_TRACKER_H
#define GF_WINDOW_GEOMETRY_TRACKER_H

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  struct Event;

  /**
   * @ingroup window
   * @brief A window geometry tracker
   *
   * The window geometry tracker computes position relative to the borders of
   * the screen. It tracks the changes of the window geometry (especially the
   * size of the window).
   *
   * This class is very useful for computing HUD elements' position.
   */
  class GF_API WindowGeometryTracker {
  public:
    /**
     * @brief Default constructor
     */
    WindowGeometryTracker();

    /**
     * @brief Compute `x` for a centered element
     *
     * Compute the x coordinate of an element of size `width` that must be
     * centered on the window.
     *
     * @param width The width of the element
     * @return The computed `x` coordinate
     * @sa getXRatio()
     */
    float getXCentered(float width) const;

    /**
     * @brief Compute `x` for a right aligned element
     *
     * Compute the x coordinate of an element of size `width` that must be
     * right aligned on the window.
     *
     * @param width The width of the element
     * @return The computed `x` coordinate
     */
    float getXFromRight(float width) const;

    /**
     * @brief Compute `x` for an element at some percent from the left
     *
     * Compute the x coordinate of an element of size `width` that must be
     * at a given percentage from the left. @f$ 0 @f$ puts the element on the
     * left, @f$ 0.5 @f$ puts the element in the center (see getXCentered()),
     * @f$ 1 @f$ puts the element on the right (see getXFromRight()).
     *
     * @param ratio The percentage, in the interval @f$ [0,1] @f$
     * @param width The width of the element
     * @return The computed `x` coordinate
     */
    float getXRatio(float ratio, float width) const;


    /**
     * @brief Compute `y` for a centered element
     *
     * Compute the y coordinate of an element of size `height` that must be
     * centered on the window.
     *
     * @param height The height of the element
     * @sa getYRatio()
     * @return The computed `y` coordinate
     */
    float getYCentered(float height) const;

    /**
     * @brief Compute `y` for a bottom aligned element
     *
     * Compute the x coordinate of an element of size `height` that must be
     * bottom aligned on the window.
     *
     * @param height The height of the element
     * @return The computed `y` coordinate
     */
    float getYFromBottom(float height) const;

    /**
     * @brief Compute `y` for an element at some percent from the top
     *
     * Compute the y coordinate of an element of size `height` that must be
     * at a given percentage from the top. @f$ 0 @f$ puts the element on the
     * top, @f$ 0.5 @f$ puts the element in the center (see getYCentered()),
     * @f$ 1 @f$ puts the element on the bottom (see getYFromBottom()).
     *
     * @param ratio The percentage, in the interval @f$ [0,1] @f$
     * @param height The height of the element
     * @return The computed `y` coordinate
     */
    float getYRatio(float ratio, float height) const;

    /**
     * @brief Get a position relative to a corner
     *
     * If the position has a negative coordinate, it is considered on the
     * other side than a positive coordinate.
     *
     * | `pos.x`     | `pos.y`     | Final position      |
     * |-------------|-------------|---------------------|
     * | @f$ > 0 @f$ | @f$ > 0 @f$ | top-left corner     |
     * | @f$ < 0 @f$ | @f$ > 0 @f$ | top-right corner    |
     * | @f$ > 0 @f$ | @f$ < 0 @f$ | bottom-left corner  |
     * | @f$ < 0 @f$ | @f$ < 0 @f$ | bottom-right corner |
     *
     * @param pos The position
     * @return The computed position
     */
    Vector2f getCornerPosition(const Vector2f& pos) const;

    /**
     * @brief Update the geometry thanks to the event
     *
     * Internally it calls onScreenResize() if a resize event occurs.
     *
     * @param event An event
     */
    void processEvent(const Event& event);

    /**
     * @brief Update the geometry with the new screen size
     *
     * @param screenSize The new size of the screen
     */
    void onScreenResize(Vector2u screenSize);

    /**
     * @brief Set the initial screen size
     *
     * @param screenSize The initial size of the screen
     */
    void setInitialScreenSize(Vector2u screenSize);

  private:
    Vector2u m_screenSize;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GL_WINDOW_GEOMETRY_TRACKER_H
