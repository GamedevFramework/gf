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
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <vector>

#include "Path.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief An asset manager
   *
   * gf::AssetManager is a base class for searching files in some seach
   * directories. If you are looking for some more sophisticated manager,
   * you should look at gf::ResourceManager.
   *
   * @sa ResourceManager
   */
  class GF_API AssetManager {
  public:
    /**
     * @brief Add a search directory
     *
     * If the provided path is not a directory, it is not added to the current
     * search directories.
     *
     * @param path The path of the directory
     */
    void addSearchDir(Path path);

    /**
     * @brief Search a file in the search directories
     *
     * This function tries to build absolute paths by joining a search
     * directory and the relative path to the file.
     *
     * If the provided path is absolute, then it is returned without searching
     * in the search directories.
     *
     * @param relativePath A path to the file, relative to a search directory
     * @return The absolute path to the file if it has been found or an empty path otherwise
     */
    Path getAbsolutePath(const Path& relativePath) const;

  private:
    std::vector<Path> m_searchdirs;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // ASSET_MANAGER_H
