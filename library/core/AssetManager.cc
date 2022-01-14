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
#include <gf/AssetManager.h>

#include <cassert>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void AssetManager::addSearchDir(Path path) {
    if (!path.is_absolute()) {
      Path absolute = std::filesystem::current_path() / path;
      Log::warning("Directory '%s' is relative, using '%s' instead.\n", path.string().c_str(), absolute.string().c_str());
      std::swap(path, absolute);
    }

    if (!std::filesystem::is_directory(path)) {
      Log::info("Directory not found: '%s'\n", path.string().c_str());
      return;
    }

    Log::info("Added a new search directory: '%s'\n", path.string().c_str());
    m_searchdirs.emplace_back(std::move(path));
  }

  Path AssetManager::getAbsolutePath(const Path& relativePath) const {
    if (relativePath.is_absolute()) {
      assert(std::filesystem::is_regular_file(relativePath));
      Log::info("Found a resource file: '%s'\n", relativePath.string().c_str());
      return relativePath;
    }

    for (const Path& base : m_searchdirs) {
      Path absolutePath = base / relativePath;

      if (std::filesystem::is_regular_file(absolutePath)) {
        Log::info("Found a resource file ['%s']: '%s'\n", base.string().c_str(), relativePath.string().c_str());
        return absolutePath;
      }
    }

    Log::error("File not found: %s\n", relativePath.string().c_str());
    return Path();
  }

  Path AssetManager::getRelativePath(const Path& absolutePath) const {
    if (!absolutePath.is_absolute()) {
      return absolutePath;
    }

    for (const Path& base : m_searchdirs) {
      auto relativePath = absolutePath.lexically_proximate(base);

      if (relativePath != absolutePath) {
        assert(!relativePath.is_absolute());
        return relativePath;
      }
    }

    return Path();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
