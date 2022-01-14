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
#ifndef GF_PATHS_H
#define GF_PATHS_H

#include <string>

#include "GraphicsApi.h"
#include "Path.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_resources
   * @brief Special paths
   *
   * @sa gf::SystemInfo
   */
  struct GF_GRAPHICS_API Paths {
    /**
     * @brief Deleted constructor
     */
    Paths() = delete;

    /**
     * @brief Get the preferences path
     *
     * The preferences path is where an application can save data. It depends
     * on the user and the application.
     *
     * @param org The name of your organization
     * @param app The name of your application
     * @returns The preferences path
     */
    static Path getPrefPath(const std::string& org, const std::string& app);

    /**
     * @brief Get the base path
     *
     * The base path is the path where the application is run.
     * @return The base path
     */
    static Path getBasePath();

    /**
     * @brief Get the current path
     *
     * @returns The current path
     */
    static Path getCurrentPath();

    /**
     * @brief Get a temporary directory
     *
     * @returns A temporary directory
     */
    static Path getTemporaryDirectory();
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PATHS_H
