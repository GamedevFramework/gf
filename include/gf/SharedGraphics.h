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

#ifndef GF_SHARED_GRAPHICS_H
#define GF_SHARED_GRAPHICS_H

#include "GraphicsApi.h"
#include "Window.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_window_monitor
   * @brief A shared OpenGL context with the main thread
   *
   * This class is useful for loading textures asynchronously.
   * The context will automatically be attached to the current
   * thread at object instantiation and will be detached at its
   * destruction.
   */
  class GF_GRAPHICS_API SharedGraphics {
  public:
    /**
     * @brief Create a shared OpenGL context
     *
     * Attach a OpenGL context to the current thread.
     *
     * @param window A reference to the main window
     */
    SharedGraphics(Window& window);

    /**
     * @brief Destructor
     *
     * Detach the OpenGL context from the current thread
     */
    ~SharedGraphics();

    SharedGraphics(const SharedGraphics&) = delete;
    SharedGraphics(SharedGraphics&&) = delete;

    SharedGraphics& operator=(const SharedGraphics&) = delete;
    SharedGraphics& operator=(SharedGraphics&&) = delete;

  private:
    Window& m_window;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_SHARED_GRAPHICS_H
