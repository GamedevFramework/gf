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
#ifndef GF_SYSTEM_INFO_H
#define GF_SYSTEM_INFO_H

#include <string>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A class for system info
   *
   * This class only contains static methods. You can get:
   *
   * - the plaform name (e.g. "Linux")
   * - the CPU cache line size
   * - the number of CPU cores
   * - the amount of memory (RAM)
   *
   */
  class GF_API SystemInfo {
  public:
    /**
     * @brief Deleted default constructor
     */
    SystemInfo() = delete;

    /**
     * @brief Get the plaform name
     *
     * The plaform can be:
     *
     * - Windows
     * - Mac OS X
     * - Linux
     * - iOS
     * - Android
     * - Unknown
     *
     * @return The plaform name (e.g. "Linux")
     */
    static std::string getPlatformName();

    /**
     * @brief Get the CPU L1 cache line size
     *
     * @return The L1 cache line size of the CPU, in kilobytes
     */
    static int getCpuCacheLineSize();

    /**
     * @brief Get the number of CPU cores
     *
     * This function returns the number of logical CPU cores.
     *
     * @return The number of CPU cores
     */
    static int getCpuCount();

    /**
     * @brief Get the amount of memory
     *
     * @return The amount of memory (RAM), in megabytes
     */
    static int getSystemRamSize();

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SYSTEM_INFO_H
