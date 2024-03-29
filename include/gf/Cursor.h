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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_CURSOR_H
#define GF_CURSOR_H

#include <cstdint>

#include "GraphicsApi.h"
#include "Path.h"
#include "Vector.h"

struct SDL_Cursor;
struct SDL_Surface;

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Image;

  /**
   * @ingroup graphics_window_monitor
   * @brief A mouse cursor
   *
   * This class abstracts the operating system resources
   * associated with either a native system cursor or a custom
   * cursor.
   *
   * After construction, the cursor can be changed with
   * gf::Window::setMouseCursor().
   *
   * The behaviour is undefined if the cursor is destroyed while
   * in use by the window.
   *
   * Usage example:
   *
   * @snippet snippets/doc_class_cursor.cc cursor
   *
   * @see gf::Window::setMouseCursor()
   */
  class GF_GRAPHICS_API Cursor {
  public:

    /**
     * @brief Enumeration of the native system cursor types
     */
    enum Type {
      Arrow,                  ///< Arrow cursor (default)
      ArrowWait,              ///< Busy arrow cursor
      Wait,                   ///< Busy cursor
      Text,                   ///< I-beam, cursor when hovering over a field allowing text entry
      Hand,                   ///< Pointing hand cursor
      SizeHorizontal,         ///< Horizontal double arrow cursor
      SizeVertical,           ///< Vertical double arrow cursor
      SizeTopLeftBottomRight, ///< Double arrow cursor going from top-left to bottom-right
      SizeBottomLeftTopRight, ///< Double arrow cursor going from bottom-left to top-right
      SizeAll,                ///< Combination of SizeHorizontal and SizeVertical
      Cross,                  ///< Crosshair cursor
      NotAllowed              ///< Action not allowed cursor
    };

    /**
     * @brief Default constructor
     *
     * This constructor doesn't actually create the cursor.
     */
    Cursor();

    /**
     * @brief Create a cursor with the provided pixels
     *
     * @a pixels must be an array of @a width by @a height pixels
     * in 32-bit RGBA format. If not, this will cause undefined behavior.
     *
     * If @a pixels is null or either @a width or @a height are 0,
     * the current cursor is left unchanged and the function will
     * return false.
     *
     * In addition to specifying the pixel data, you can also
     * specify the location of the hotspot of the cursor. The
     * hotspot is the pixel coordinate within the cursor image
     * which will be located exactly where the mouse pointer
     * position is. Any mouse actions that are performed will
     * return the window/screen location of the hotspot.
     *
     *
     * @param pixels An array of pixels of the image
     * @param size The width and height of the image
     * @param hotspot The location of the hotspot
     */
    Cursor(const uint8_t* pixels, Vector2i size, Vector2i hotspot);

    /**
     * @brief Create a cursor from the provided image
     *
     * @param image The image
     * @param hotspot THe location of the hotspot
     */
    Cursor(const Image& image, Vector2i hotspot);

    /**
     * @brief Create a cursor from an image stored in a file
     *
     * @param path The path to the image
     * @param hotspot THe location of the hotspot
     */
    Cursor(const Path& path, Vector2i hotspot);

    /**
     * @brief Create a native system cursor
     *
     * @param type The native system cursor type
     */
    Cursor(Type type);

    /**
     * @brief Deleted copy constructor
     */
    Cursor(const Cursor&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Cursor& operator=(const Cursor&) = delete;

    /**
     * @brief Move constructor
     */
    Cursor(Cursor&& other) noexcept;

    /**
     * @brief Move assignment
     */
    Cursor& operator=(Cursor&& other) noexcept;

    /**
     * @brief Destructor
     *
     * This destructor releases the system resources
     * associated with this cursor, if any.
     */
    ~Cursor();

  private:
    friend class Window;
    SDL_Cursor *m_cursor;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CURSOR_H
