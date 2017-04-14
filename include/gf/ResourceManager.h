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
#ifndef GF_RESOURCE_MANAGER_H
#define GF_RESOURCE_MANAGER_H

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

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
   * @brief A generic cache for resources
   *
   * This function is a low-level class that is used in gf::ResourceManager.
   * It is generic enough so that you can use it for your own purpose.
   *
   * @sa gf::ResourceManager
   */
  template<typename T>
  class ResourceCache {
  public:
    /**
     * @brief A resource loader
     */
    using Loader = std::function<std::unique_ptr<T>(const Path&)>;

    /**
     * @brief Constructor
     *
     * @param loader A resource loader
     */
    explicit ResourceCache(Loader loader)
    : m_loader(std::move(loader))
    {

    }

    /**
     * @brief Deleted copy constructor
     */
    ResourceCache(const ResourceCache&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    ResourceCache& operator=(const ResourceCache&) = delete;

    /**
     * @brief Get a resource
     *
     * If the resource exists in the cache, it is returned immediately.
     * Otherwise, it is searched thanks to an asset manager and put in the
     * cache. If the resource is not found, an exception is thrown.
     *
     * @param assetManager An asset manager
     * @param filename The filename of the resource
     * @return A reference to the resource
     * @throw std::runtime_error If the resource is not found
     */
    T& getResource(AssetManager& assetManager, const Path& filename) {
      std::size_t h = boost::filesystem::hash_value(filename);

      auto it = m_cache.find(h);

      if (it != m_cache.end()) {
        return *it->second;
      }

      Path absolutePath = assetManager.getAbsolutePath(filename);

      if (absolutePath.empty()) {
        throw std::runtime_error("Path not found");
      }

      auto ptr = m_loader(absolutePath);

      if (!ptr) {
        throw std::runtime_error("Resource not loaded");
      }

      auto inserted = m_cache.emplace(h, std::move(ptr));

      if (inserted.second) {
        return *inserted.first->second;
      }

      throw std::runtime_error("Resource not inserted in the cache");
    }

  private:
    Loader m_loader;
    std::map<std::size_t, std::unique_ptr<T>> m_cache;
  };


  /**
   * @ingroup game
   * @brief A resource manager
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
     * @return A reference to the texture
     * @throw std::runtime_error If the texture is not found
     */
    Texture& getTexture(const Path& path) {
      return m_textures.getResource(*this, path);
    }

    /**
     * @brief Get a font
     *
     * @param path A path to the font
     * @return A reference to the font
     * @throw std::runtime_error If the font is not found
     */
    Font& getFont(const Path& path) {
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
