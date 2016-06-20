/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef GF_RESOURCE_MANAGER_H
#define GF_RESOURCE_MANAGER_H

#include <functional>
#include <map>
#include <memory>

#include "AssetManager.h"
#include "Font.h"
#include "Portability.h"
#include "Texture.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief A generic cache for ressources
   *
   * This function is a low-level class that is used in gf::ResourceManager.
   * It is generic enough so that you can use it for your own purpose.
   *
   * @sa gf::ResourceManager
   */
  template<typename T>
  class GF_API ResourceCache {
  public:
    /**
     * @brief A ressource loader
     */
    typedef std::function<std::unique_ptr<T>(const Path&)> Loader;

    /**
     * @brief Constructor
     *
     * @param loader A ressource loader
     */
    explicit ResourceCache(Loader loader)
    : m_loader(std::move(loader))
    {

    }

    /**
     * @brief Get a ressource
     *
     * If the ressource exists in the cache, it is returned immediately.
     * Otherwise, it is searched thanks to an asset manager and put in the
     * cache.
     *
     * @param assetManager An asset manager
     * @param filename The filename of the ressource
     * @return The ressource or `nullptr` if it has not been found
     */
    T *getResource(AssetManager& assetManager, const Path& filename) {
      auto it = m_cache.find(filename);

      if (it != m_cache.end()) {
        return it->second.get();
      }

      Path absolutePath = assetManager.getAbsolutePath(filename);

      if (absolutePath.empty()) {
        return nullptr;
      }

      auto ptr = m_loader(absolutePath);

      if (!ptr) {
        return nullptr;
      }

      auto inserted = m_cache.emplace(filename, std::move(ptr));

      if (inserted.second) {
        return inserted.first->second.get();
      }

      return nullptr;
    }

  private:
    Loader m_loader;
    std::map<Path, std::unique_ptr<T>> m_cache;
  };


  /**
   * @ingroup game
   * @brief A ressource manager
   *
   * @sa gf::ResourceCache
   */
  class GF_API ResourceManager : public AssetManager {
  public:
    /**
     * @brief Default constructor
     */
    ResourceManager();

    /**
     * @brief Get a texture
     *
     * @param path A path to the texture
     * @return A texture or `nullptr` if it has not been found
     */
    Texture *getTexture(const Path& path) {
      return m_textures.getResource(*this, path);
    }

    /**
     * @brief Get a font
     *
     * @param path A path to the font
     * @return A font or `nullptr` if it has not been found
     */
    Font *getFont(const Path& path) {
      return m_fonts.getResource(*this, path);
    }

  private:
    ResourceCache<Texture> m_textures;
    ResourceCache<Font> m_fonts;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RESOURCE_MANAGER_H
