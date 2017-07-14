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
#ifndef GF_MONITOR_H
#define GF_MONITOR_H

#include <string>
#include <vector>

#include "Library.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief A video mode
   *
   */
  struct GF_API VideoMode {
    Vector2u size; ///< Video mode size, in pixels
    unsigned bitsPerPixel; ///< Video mode pixel depth, in bits per pixel
    int refreshRate; ///< Video mode refresh rate, in Hz
  };

  /**
   * @ingroup window
   * @brief A monitor
   */
  class GF_API Monitor {
  public:
    /**
     * @brief Get the primary monitor
     *
     * @return The primary monitor
     */
    static Monitor getPrimaryMonitor();

    /**
     * @brief Get all the available monitors
     */
    static std::vector<Monitor> getAvailableMonitors();

    /**
     * @brief Deleted copy constructor
     */
    Monitor(const Monitor&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Monitor& operator=(const Monitor&) = delete;

    /**
     * @brief Move constructor
     */
    Monitor(Monitor&& other) = default;

    /**
     * @brief Move assignement
     */
    Monitor& operator=(Monitor&& other) = default;

    /**
     * @brief Get the name of the monitor
     *
     * @return A string containing the name of the monitor
     */
    std::string getName() const;

    /**
     * @brief Get the position of the monitor
     *
     * @return The position of the monitor
     */
    Vector2i getPosition() const;

    /**
     * @brief Get the physical size of the monitor
     *
     * @return The physical size of the monitor
     */
    Vector2u getPhysicalSize() const;

    /**
     * @brief Get the available video modes for this monitor
     *
     * @return The video modes for this monitor
     */
    std::vector<VideoMode> getAvailableVideoModes() const;

    /**
     * @brief Get the current video mode
     *
     * @return The current video mode
     */
    VideoMode getCurrentVideoMode() const;

  private:
    friend class Window;

    explicit Monitor(int index);

    Library m_lib; // to automatically initialize SDL
    int m_index;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GL_MONITOR_H
