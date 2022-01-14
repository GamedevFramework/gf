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
#ifndef GF_VIEW_CONTAINER_H
#define GF_VIEW_CONTAINER_H

#include <vector>

#include "GraphicsApi.h"
#include "Ref.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class AdaptativeView;
  struct Event;

  /**
   * @ingroup graphics_views
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
  class GF_GRAPHICS_API ViewContainer {
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
     * Internally it calls onFramebufferSizeChange() if a resize event occurs.
     *
     * @param event An event
     */
    void processEvent(const Event& event);

    /**
     * @brief Update the views with the new framebuffer size
     *
     * @param framebufferSize The new size of the framebuffer
     *
     * @sa gf::AdaptativeView::onFramebufferSizeChange()
     */
    void onFramebufferSizeChange(Vector2i framebufferSize);

    /**
     * @brief Set the initial framebuffer size
     *
     * @param framebufferSize The initial size of the framebuffer
     */
    void setInitialFramebufferSize(Vector2i framebufferSize);

    /**
     * @brief Set the initial screen size
     *
     * @param screenSize The initial size of the screen
     *
     * @deprecated You should use setInitialFramebufferSize() instead
     */
    [[deprecated("You should use setInitialFramebufferSize() instead")]]
    void setInitialScreenSize(Vector2i screenSize) {
      setInitialFramebufferSize(screenSize);
    }
  private:
    std::vector<Ref<AdaptativeView>> m_views;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VIEW_CONTAINER_H
