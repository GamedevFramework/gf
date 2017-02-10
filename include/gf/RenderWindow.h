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
#ifndef GF_RENDER_WINDOW_H
#define GF_RENDER_WINDOW_H

#include "Portability.h"
#include "RenderTarget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Window;

  /**
   * @ingroup graphics
   * \brief A window that can serve as a target for 2D drawing
   *
   * gf::RenderWindow is the main class of the graphics module.
   * It defines an OS window that can be painted using the other
   * classes of the graphics module.
   *
   * Here is a typical rendering and event loop with a gf::RenderWindow:
   *
   * ~~~{.cc}
   * // Declare and create a new render-window
   * gf::Window window("New window", { 800, 600 });
   * gf::RenderWindow renderer(window);
   *
   * // The main loop - ends as soon as the window is closed
   * while (window.isOpen()) {
   *   // Event processing
   *   gf::Event event;
   *
   *   while (window.pollEvent(event)) {
   *     // Request for closing the window
   *     if (event.type == gf::EventType::Closed) {
   *       window.close();
   *     }
   *   }
   *
   *   // Clear the whole window before rendering a new frame
   *   renderer.clear();
   *
   *   // Draw some graphical entities
   *   renderer.draw(sprite);
   *   renderer.draw(circle);
   *   renderer.draw(text);
   *
   *   // End the current frame and display its contents on screen
   *   renderer.display();
   * }
   * ~~~
   *
   * \see gf::Window, gf::RenderTarget, gf::RenderTexture, gf::View
   *
   */
  class GF_API RenderWindow : public RenderTarget {
  public:

    /**
     * @brief Constructor
     *
     * @param window The window to render to
     */
    RenderWindow(Window& window);

    virtual Vector2u getSize() const override;

    /**
     * @brief Activate the target for rendering
     *
     * This function activates the render-window so that all draw calls
     * are targeted to the window. You should call this function before
     * you want to draw something to the target.
     */
    void setActive();

    /**
     * @brief Display on screen what has been rendered to the window so far
     *
     * This function is typically called after all OpenGL rendering
     * has been done for the current frame, in order to show
     * it on screen.
     *
     * @sa Window::display()
     */
    void display();

    /**
     * @brief Copy the current contents of the render window to an image
     *
     * This is a slow operation, whose main purpose is to make screenshots of
     * the application.
     */
    Image capture() const;

  private:
    Window& m_window;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RENDER_WINDOW_H
