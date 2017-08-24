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
#ifndef GF_WINDOW_H
#define GF_WINDOW_H

#include <string>

#include "Clock.h"
#include "Flags.h"
#include "Library.h"
#include "Portability.h"
#include "StringRef.h"
#include "Time.h"
#include "Vector.h"

struct SDL_Window;

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct Event;
  class Cursor;

  /**
   * @ingroup window
   * @brief Hints for window creation
   * @sa gf::WindowFlags
   */
  enum class WindowHints : uint32_t {
    Resizable = 0x0001, ///< Is the window resizable?
    Visible   = 0x0002, ///< Is the window visible?
    Decorated = 0x0004, ///< Is the window decorated?
  };

  /**
   * @ingroup window
   * @brief Flags for window creation
   * @sa gf::Flags, gf::WindowHints
   */
  using WindowFlags = Flags<WindowHints>;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
}

template<>
struct EnableBitmaskOperators<WindowHints> {
  static constexpr bool value = true;
};

inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief An OS window
   *
   * The gf::Window class provides a simple interface for manipulating
   * the window: move, resize, show/hide, control mouse cursor, etc.
   * It also provides event handling through its pollEvent() and waitEvent()
   * functions.
   *
   * @snippet snippets/doc_class_window.cc window
   *
   * @sa gf::RenderWindow
   */
  class GF_API Window {
  public:

    /**
     * @brief Create a new window
     *
     * This constructor creates the window with the size defined in `size`.
     * Additional parameters can be passed with `hints` (resizable, visible,
     * decorated).
     *
     * @param title The title of the window
     * @param size The initial size of the window
     * @param hints Some hints for the creation of the window
     * @sa gf::WindowHints
     */
    Window(StringRef title, Vector2u size, WindowFlags hints = WindowFlags(All));

    /**
     * @brief Destructor
     *
     * Actually destroy the window.
     */
    ~Window();

    /**
     * @brief Deleted copy constructor
     */
    Window(const Window&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Window& operator=(const Window&) = delete;

    /**
     * @name Window's lifecycle
     * @{
     */

    /**
     * @brief Tell whether or not closing has been requested
     *
     * @return True if the window is open, false if closing has been requested
     * @sa close()
     */
    bool isOpen();

    /**
     * @brief Request for closing
     *
     * This function does not close the window immediately. It only requests
     * the window to close. Actual closing is done when the object is destroyed.
     *
     * @sa isOpen()
     */
    void close();

    /** @} */

    /**
     * @name Window's positon and size
     * @{
     */

    /**
     * @brief Change the title of the window
     *
     * @param title New title
     */
    void setTitle(StringRef title);

    /**
     * @brief Get the position of the window
     *
     * @return The position of the window, in pixels
     * @sa setPosition()
     */
    Vector2i getPosition() const;

    /**
     * @brief Change the position of the window on screen
     *
     * @param position New position, in pixels
     * @sa getPosition()
     */
    void setPosition(Vector2i position);

    /**
     * @brief Get the size of the rendering region of the window
     *
     * @return The size in pixels
     * @sa setSize(), getFramebufferSize()
     */
    Vector2u getSize() const;

    /**
     * @brief Change the size of the rendering region of the window
     *
     * @param size New size, in pixels
     * @sa getSize(), getFramebufferSize()
     */
    void setSize(Vector2u size);

    /**
     * @brief Get the size of the underlying framebuffer
     *
     * This size can differ from the size returned by getSize() for high-DPI screens.
     *
     * @return getSize(), setSize()
     */
    Vector2u getFramebufferSize() const;

    /**
     * @brief Change the window state to fullscreen or not
     *
     * @param full True if the window must be in fullscreen
     */
    void setFullscreen(bool full = true);

    /**
     * @brief Toggle the fullscreen state
     */
    void toggleFullscreen();

    /**
     * @brief Check if the window is fullscreen or not
     *
     * @returns True if the window is fullscreen
     */
    bool isFullscreen() const {
      return m_isFullscreen;
    }

    /** @} */

    /**
     * @name Window's state
     * @{
     */

    /**
     * @brief Minimize the window
     */
    void minimize();

    /**
     * @brief Restore the window
     *
     * Restore the size and position of a minimized or maximized window.
     */
    void restore();

    /**
     * @brief Maximize the window
     */
    void maximize();

    /**
     * @brief Show a window
     * @sa hide()
     */
    void show();

    /**
     * @brief Hide a window
     * @sa show()
     */
    void hide();

    /**
     * @brief Show or hide the window
     *
     * The window is shown by default.
     *
     * @param visible True to show the window, false to hide it
     * @sa show(), hide(), isVisible()
     */
    void setVisible(bool visible = true);

    /**
     * @brief Show or hide the decoration of the window
     *
     * @param decorated True to show decoration
     * @sa isDecorated()
     */
    void setDecorated(bool decorated = true);

    /**
     * @brief Check if the window is focused
     *
     * @return True if the window is focused
     */
    bool isFocused() const;

    /**
     * @brief Check if the window is minimized
     *
     * @return True if the window is minimized
     */
    bool isMinimized() const;

    /**
     * @brief Check if the window is resizable
     *
     * @return True if the window is resizable
     */
    bool isResizable() const;

    /**
     * @brief Check if the window is visible
     *
     * @return True if the window is visible
     */
    bool isVisible() const;

    /**
     * @brief Check if the window is decorated
     *
     * @return True if the window is decorated
     */
    bool isDecorated() const;

    /** @} */

    /**
     * @name Event handling
     * @{
     */

    /**
     * @brief Pop the event on top of the event queue, if any, and return it
     *
     * This function is not blocking: if there's no pending event then
     * it will return false and leave `event` unmodified.
     * Note that more than one event may be present in the event queue,
     * thus you should always call this function in a loop
     * to make sure that you process every pending event.
     *
     * ~~~{.cc}
     * gf::Event event;
     *
     * while (window.pollEvent(event)) {
     *    // process event...
     * }
     * ~~~
     *
     * @param event Event to be returned
     * @return True if an event was returned, or false if the event queue was empty
     * @sa waitEvent()
     */
    bool pollEvent(Event& event);

    /**
     * @brief Wait for an event and return it
     *
     * This function is blocking: if there's no pending event then
     * it will wait until an event is received.
     * After this function returns (and no error occurred),
     * the `event` object is always valid and filled properly.
     * This function is typically used when you have a thread that
     * is dedicated to events handling: you want to make this thread
     * sleep as long as no new event is received.
     *
     * ~~~{.cc}
     * gf::Event event;
     *
     * if (window.waitEvent(event))
     * {
     *    // process event...
     * }
     * ~~~
     *
     * @param event Event to be returned
     * @return False if any error occurred
     * @sa pollEvent()
     */
    bool waitEvent(Event& event);

    /** @} */

    /**
     * @name Display
     * @{
     */

    /**
     * @brief Enable or disable vertical synchronization
     *
     * Activating vertical synchronization will limit the number
     * of frames displayed to the refresh rate of the monitor.
     * This can avoid some visual artifacts, and limit the framerate
     * to a good value (but not constant across different computers).
     *
     * @param enabled True to enable v-sync, false to deactivate it
     * @sa isVerticalSyncEnabled()
     */
    void setVerticalSyncEnabled(bool enabled);

    /**
     * @brief Check if the vertical synchronization is enabled
     *
     * @returns True if the v-sync is enabled
     * @sa setVerticalSyncEnabled()
     */
    bool isVerticalSyncEnabled() const;

    /**
     * @brief Limit the framerate to a maximum fixed frequency
     *
     * If a limit is set, the window will use a small delay after
     * each call to display() to ensure that the current frame
     * lasted long enough to match the framerate limit.
     *
     * @param limit Framerate limit, in frames per seconds (use 0 to disable limit)
     */
    void setFramerateLimit(unsigned int limit);

    /**
     * @brief Display on screen what has been rendered to the window so far
     *
     * This function is typically called after all OpenGL rendering
     * has been done for the current frame, in order to show
     * it on screen.
     *
     * @sa RenderWindow::display()
     */
    void display();

    /** @} */


    /**
     * @name Input management
     * @{
     */

    /**
     * @brief Show or hide the mouse cursor
     *
     * The mouse cursor is visible by default.
     *
     * @param visible True to show the mouse cursor, false to hide it
     */
    void setMouseCursorVisible(bool visible);

    /**
     * @brief Grab or release the mouse cursor
     *
     * If set, grabs the mouse cursor inside this window's client
     * area so it may no longer be moved outside its bounds.
     * Note that grabbing is only active while the window has
     * focus and calling this function for fullscreen windows
     * won't have any effect (fullscreen windows always grab the
     * cursor).
     *
     * @param grabbed True to enable, false to disable
     */
    void setMouseCursorGrabbed(bool grabbed);


    /**
     * @brief Set the displayed cursor to a native system cursor
     *
     * Upon window creation, the arrow cursor is used by default.
     *
     * @warning The cursor must not be destroyed while in use by the window.
     *
     * @param cursor Native system cursor type to display
     */
    void setMouseCursor(const Cursor& cursor);

    /** @} */


  private:
    Library m_lib; // to automatically initialize SDL

  private:
    SDL_Window *m_window;
    void *m_context;
    bool m_shouldClose;
    bool m_isFullscreen;

    // framerate limit handling
    Clock m_clock;
    Time m_duration;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif

}

#endif // GL_WINDOW_H
