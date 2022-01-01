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
#include <gf/ResourceManager.h>

#include <SDL.h>

#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    template<typename T>
    class ResourceLoader {
    public:
      std::unique_ptr<T> operator()(const Path& filename) {
        return std::make_unique<T>(filename);
      }
    };

  }

  ResourceManager::ResourceManager()
  : m_images(ResourceLoader<Image>())
  , m_textures(ResourceLoader<Texture>())
  , m_fonts(ResourceLoader<Font>())
  {

  }

  ResourceManager::ResourceManager(std::initializer_list<Path> paths)
  : ResourceManager()
  {
    for (auto path : paths) {
      addSearchDir(path);
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
