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
#include <gf/Paths.h>

#include <SDL.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Path Paths::getPrefPath(StringRef org, StringRef app) {
    char *prefPath = SDL_GetPrefPath(org.getData(), app.getData());

    if (prefPath == nullptr) {
      return Path();
    }

    Path path(prefPath);
    SDL_free(prefPath);
    return path;
  }

  Path Paths::getBasePath() {
    char *basePath = SDL_GetBasePath();

    if (basePath == nullptr) {
      return Path();
    }

    Path path(basePath);
    SDL_free(basePath);
    return path;
  }

  Path Paths::getCurrentPath() {
    return boost::filesystem::current_path();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
