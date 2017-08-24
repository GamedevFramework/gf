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
#ifndef GF_CLIPBOARD_H
#define GF_CLIPBOARD_H

#include <string>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief gf::Clipboard provides an interface for getting and setting the
   * contents of the system clipboard.
   *
   * Usage example:
   * @code
   * // get the clipboard content as a string
   * std::string string = gf::Clipboard::getString();
   *
   * // or use it in the event loop
   * gf::Event event;
   *
   * while (window.pollEvent(event)) {
   *   if(event.type == gf::EventType::Closed) {
   *     window.close();
   *   }
   *
   *   if (event.type == gf::EventType::KeyPressed) {
   *     // Using Ctrl + V to paste a string
   *     if (event.key.modifiers.test(gf::Mod::Control) && event.key.keycode == gf::Keycode::V) {
   *       string = gf::Clipboard::getString();
   *     }
   *   }
   * }
   *
   * // set the clipboard to a string
   * gf::Clipboard::setString("Hello World!");
   * @endcode
   *
   */
  class GF_API Clipboard {
  public:

    /**
     * @brief Get the content of the clipboard as string data
     *
     * This function returns the content of the clipboard
     * as a string. If the clipboard does not contain string
     * it returns an empty string.
     *
     * @returns The clipboard contents
     */
    static std::string getString();

    /**
     * @brief Set the content of the clipboard as string data
     *
     * This function sets the content of the clipboard as a
     * string.
     *
     * @param text A string containing the data to be sent to the clipboard
     */
    static void setString(const std::string& text);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CLIPBOARD_H
