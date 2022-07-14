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
#ifndef GF_TILESET_H
#define GF_TILESET_H

#include "GraphicsApi.h"
#include "Texture.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_tiles
   * @brief A tileset
   *
   * A tileset is a texture that contains all the tiles, ordered in a grid.
   * The tileset has several parameters that are inspired by the parameters in
   * [Tiled](http://www.mapeditor.org/):
   *
   * - tile size: the size of a tile (`setTileSize()`, `getTileSize()`)
   * - margin: the margin around the tiles (`setMargin()`, `getMargin()`), default: @f$ (0,0) @f$
   * - spacing: the spacing between the tiles (`setSpacing()`, `getSpacing()`), default: @f$ (0,0) @f$
   * - offset: the offset of the tileset when drawing a tile, default: @f$ (0,0) @f$
   *
   * @sa gf::TileLayer
   */
  class GF_GRAPHICS_API Tileset {
  public:
    Tileset();

    /**
     * @brief Change the source texture of the tileset
     *
     * The texture must exist as long as the tile layer uses it. Indeed, the
     * tile layer doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the tile layer tries to
     * use it, the behavior is undefined.
     *
     * @param texture New texture
     * @sa getTexture()
     */
    void setTexture(Texture& texture);

    /**
     * @brief Get the source texture of the tileset
     *
     * The returned reference is const, which means that you can't
     * modify the texture when you retrieve it with this function.
     *
     * @return Reference to the tileset's texture
     * @sa setTexture()
     */
    const Texture& getTexture() const {
      return *m_texture;
    }

    /**
     * @brief Enable or disable the smooth filter on the texture
     *
     * @param smooth True to enable smoothing, false to disable it
     *
     * @see BareTexture::setSmooth()
     */
    void setSmooth(bool smooth = true) {
      m_texture->setSmooth(smooth);
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
     * @brief Unset the source texture of the tile layer
     *
     * After a call to this function, the tile layer has no source texture.
     *
     * @sa setTexture()
     */
    void unsetTexture();

    /**
     * @brief Set the tile size in the tileset
     *
     * @param tileSize The new tile size, in pixels
     * @sa getTileSize()
     */
    void setTileSize(Vector2i tileSize);

    /**
     * @brief Get the tile size in the tileset
     *
     * @return The tile size, in pixels
     * @sa setTileSize()
     */
    Vector2i getTileSize() const {
      return m_tileSize;
    }

    /**
     * @brief Set the margin of the tileset
     *
     * @param margin The margin, in pixels
     * @sa getMargin()
     */
    void setMargin(int margin) {
      setMargin({ margin, margin });
    }

    /**
     * @brief Set the margin of the tileset
     *
     * @param margin The margin, in pixels
     * @sa getMargin()
     */
    void setMargin(Vector2i margin);

    /**
     * @brief Get the margin of the tileset
     *
     * @return The margin, in pixels
     * @sa setMargin()
     */
    Vector2i getMargin() const {
      return m_margin;
    }

    /**
     * @brief Set the spacing of the tileset
     *
     * @param spacing The spacing, in pixels
     * @sa getSpacing()
     */
    void setSpacing(int spacing) {
      setSpacing({ spacing, spacing });
    }

    /**
     * @brief Set the spacing of the tileset
     *
     * @param spacing The spacing, in pixels
     * @sa getSpacing()
     */
    void setSpacing(Vector2i spacing);

    /**
     * @brief Get the spacing of the tileset
     *
     * @return The spacing, in pixels
     * @sa setSpacing()
     */
    Vector2i getSpacing() const {
      return m_spacing;
    }

    /**
     * @brief Set the offset of the tileset
     *
     * @param offset The offset, in pixels
     * @sa getOffset();
     */
    void setOffset(Vector2i offset);

    /**
     * @brief Get the offset of the tileset
     *
     * @return The offset, in pixels
     * @sa setOffset()
     */
    Vector2i getOffset() const {
      return m_offset;
    }

    /**
     * @brief Get the size of the tileset
     *
     * @return The size of the tileset in number of tiles
     */
    Vector2i getSize() const {
      return m_size;
    }

    /**
     * @brief Get the texture coordinates for a tile
     *
     * @param tile The tile number
     * @return The texture coordinates
     */
    RectF computeTextureCoords(int tile) const;

    /**
     * @brief Get the texture coordinates for a tile
     *
     * @param coords The tile coordinates
     * @return The texture coordinates
     */
    RectF computeTextureCoords(Vector2i coords) const;

  private:
    void updateSize();

  private:
    Texture *m_texture;
    Vector2i m_tileSize;
    Vector2i m_margin;
    Vector2i m_spacing;
    Vector2i m_offset;
    Vector2i m_size;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TILESET_H
