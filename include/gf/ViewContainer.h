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
 */
#ifndef GF_VIEW_CONTAINER_H
#define GF_VIEW_CONTAINER_H

#include <vector>

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class AdaptativeView;
  struct Event;

  /**
   * @ingroup graphics
   * @brief A container of views
   *
   * A view manager handles several adaptive views. It can update all the
   * views at the same time. All it needs is the events that come from the
   * window.
   *
   * Here is a full example with two adaptive views:
   *
   * @snippet snippets/doc_class_view_container.cc view_container
   *
   * @sa gf::AdaptativeView
   */
  class GF_API ViewContainer {
  public:
    /**
     * @brief Add a view to the container
     *
     * @param view An adaptive view
     */
    void addView(AdaptativeView& view);

    /**
     * @brief Update the views thanks to the event
     *
     * Internally it calls onScreenSizeChange() if a resize event occurs.
     *
     * @param event An event
     */
    void processEvent(const Event& event);

    /**
     * @brief Update the views with the new screen size
     *
     * @param screenSize The new size of the screen
     *
     * @sa gf::AdaptativeView::onScreenSizeChange()
     */
    void onScreenSizeChange(Vector2u screenSize);

    /**
     * @brief Set the initial screen size
     *
     * @param screenSize The initial size of the screen
     */
    void setInitialScreenSize(Vector2u screenSize);

  private:
    std::vector<AdaptativeView*> m_views;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VIEW_CONTAINER_H
