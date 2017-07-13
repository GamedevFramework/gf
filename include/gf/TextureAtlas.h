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
#ifndef GF_TEXTURE_ATLAS_H
#define GF_TEXTURE_ATLAS_H

#include <map>
#include <string>

#include "Path.h"
#include "Portability.h"
#include "Rect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;
  class ResourceManager;

  /**
   * @ingroup game
   * @brief A collection of sub-texture
   *
   * A texture atlas is a collection of sub-texture that have been packed
   * together in a single texture. The description of the sub-textures is done
   * in a XML file. This description is a name and the texture coordinates in
   * the texture.
   *
   * Here is an example of a XML file containing an atlas:
   *
   * ~~~{.xml}
   * <TextureAtlas imagePath="bricks.png">
   *   <SubTexture name="brickBlack01" x="718" y="500" width="32" height="32"/>
   *   <SubTexture name="brickBlack02" x="716" y="66" width="64" height="32"/>
   *   ...
   * </TextureAtlas>
   * ~~~
   *
   * gf::TextureAtlas is able to read the XML file and give the coordinates
   * thanks to the name of the sub-texture, either in pixels or in texture
   * normalized coordinates.
   *
   * @sa gf::Texture
   * @sa [Texture Atlas (wikipedia)](https://en.wikipedia.org/wiki/Texture_atlas)
   */
  class GF_API TextureAtlas {
  public:
    /**
     * @brief Default constructor
     */
    TextureAtlas()
    : m_texture(nullptr)
    {

    }

    /**
     * @brief Load an atlas from a XML file
     *
     * Load the data about the sub-textures from a XML file.
     *
     * @param filename The filename of the XML file
     * @return True if the file has been loaded
     */
    bool loadFromFile(const Path& filename);

    /**
     * @brief Load an atlas from a XML file
     *
     * Load the data about the sub-textures from a XML file. In addition, load
     * the texture thanks to a resource manager.
     *
     * @param filename The filename of the XML file
     * @param resources A resource manager
     * @return True if the file has been loaded
     */
    bool loadFromFile(const Path& filename, ResourceManager& resources);

    /**
     * @brief Set the texture path
     *
     * The texture path comes from the XML file when it is loaded.
     *
     * @param path The path to the texture
     * @sa getTexturePath()
     */
    void setTexturePath(const Path& path) {
      m_texturePath = path;
    }

    /**
     * @brief Get the texture path
     *
     * @return The path to the texture
     * @sa setTexturePath()
     */
    const Path& getTexturePath() const {
      return m_texturePath;
    }

    /**
     * @brief Change the source texture of the atlas
     *
     * The texture must exist as long as the atlas uses it. Indeed, the
     * atlas doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the atlas tries to
     * use it, the behavior is undefined.
     *
     * @param texture New texture
     *
     * @sa getTexture(), getTexturePath()
     */
    void setTexture(const Texture& texture) {
      m_texture = &texture;
    }

    /**
     * @brief Get the source texture of the atlas
     *
     * The returned reference is const, which means that you can't
     * modify the texture when you retrieve it with this function.
     *
     * @return A reference to the atlas' texture
     *
     * @sa setTexture()
     */
    const Texture& getTexture() const {
      return *m_texture;
    }

    /**
     * @brief Check if a texture is set
     *
     * @returns True if a texture is already set
     *
     * @sa setTexture(), getTexture()
     */
    bool hasTexture() const {
      return m_texture != nullptr;
    }

    /**
     * @brief Unset the source texture of the atlas
     *
     * After a call to this function, the atlas has no source texture.
     *
     * @sa setTexture()
     */
    void unsetTexture() {
      m_texture = nullptr;
    }

    /**
     * @brief Add a sub-texture to the atlas
     *
     * @param name The name of the sub-texture
     * @param rect The rectangle inside the texture
     */
    void addSubTexture(std::string name, const RectU& rect);

    /**
     * @brief Get the sub-texture rectangle
     *
     * @param name The name of the sub-texture
     * @return The rectangle corresponding to the name
     * @sa getTextureRect()
     */
    RectU getSubTexture(const std::string& name) const;

    /**
     * @brief Get the texture rectangle in normalized coordinates
     *
     * @param name The name of the sub-texture
     * @return The texture rectangle in normalized coordinates
     * @sa getSubTexture()
     */
    RectF getTextureRect(const std::string& name) const;

  private:
    Path m_texturePath;
    const Texture *m_texture;
    std::map<std::string, RectU> m_rects;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TEXTURE_ATLAS_H
