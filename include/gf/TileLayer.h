/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#ifndef GF_TILE_LAYER_H
#define GF_TILE_LAYER_H

#include "Array2D.h"
#include "Flags.h"
#include "Flip.h"
#include "Portability.h"
#include "Stagger.h"
#include "Transformable.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  /**
   * @ingroup graphics
   * @brief A tile layer
   *
   * A tile layer represents a map made of tiles. gf::TileLayer makes it easy
   * to draw a tile map.
   *
   * A tile layer is associated to a single tileset. A tileset is a texture
   * that contains all the tiles, ordered in a grid. The tileset has several
   * parameters that are inspired by the parameters in
   * [Tiled](http://www.mapeditor.org/):
   *
   * - tile size: the size of a tile (`setTileSize()`, `getTileSize()`)
   * - margin: the margin around the tiles (`setMargin()`, `getMargin()`), default: 0
   * - spacing: the spacing between the tiles (`setSpacing()`, `getSpacing()`), default: 0
   *
   * The tile layer is given with an array of indices. Each index correspond
   * to a tile in the tileset. Tile 0 correspond to the tile at the top left
   * in the tileset. Then tile are numbered from left to right, and then from
   * top to bottom. If a tile is not present in the tile layer, the constant
   * gf::TileLayer::NoTile can be used.
   *
   */
  class GF_API TileLayer : public Transformable {
  public:
    /**
     * @brief A constant meaning that there is no tile
     */
    static constexpr int NoTile = -1;

    /**
     * @brief The type of tile layer
     */
    enum Type {
      Orthogonal, ///< An orthogonal tile layer
      Staggered,  ///< A staggered tile layer
    };

    /**
     * @brief Constructor
     *
     * @param layerSize the size of the layer, in number of tiles
     */
    TileLayer(Vector2u layerSize, Type type = Orthogonal);

    /**
     * @brief Get the size of the layer
     *
     * @returns The size of the layer, in number of tiles
     */
    Vector2u getMapSize() const {
      return m_tiles.getSize();
    }

    /**
     * @name Tileset parameters
     * @{
     */

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
    void setTexture(const Texture& texture);

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
    void setTileSize(Vector2u tileSize);

    /**
     * @brief Get the tile size in the tileset
     *
     * @return The tile size, in pixels
     * @sa setTileSize()
     */
    Vector2u getTileSize() const {
      return m_tileSize;
    }

    /**
     * @brief Set the margin of the tileset
     *
     * @param margin The margin, in pixels
     * @sa getMargin()
     */
    void setMargin(unsigned margin) {
      setMargin({ margin, margin });
    }

    /**
     * @brief Set the margin of the tileset
     *
     * @param margin The margin, in pixels
     * @sa getMargin()
     */
    void setMargin(Vector2u margin);

    /**
     * @brief Get the margin of the tileset
     *
     * @return The margin, in pixels
     * @sa setMargin()
     */
    Vector2u getMargin() const {
      return m_margin;
    }

    /**
     * @brief Set the spacing of the tileset
     *
     * @param spacing The spacing, in pixels
     * @sa getSpacing()
     */
    void setSpacing(unsigned spacing) {
      setSpacing({ spacing, spacing });
    }

    /**
     * @brief Set the spacing of the tileset
     *
     * @param spacing The spacing, in pixels
     * @sa getSpacing()
     */
    void setSpacing(Vector2u spacing);

    /**
     * @brief Get the spacing of the tileset
     *
     * @return The spacing, in pixels
     * @sa setSpacing()
     */
    Vector2u getSpacing() const {
      return m_spacing;
    }

    /** @} */

    /**
     * @name Tile definition
     * @{
     */

    /**
     * @brief Set the block size
     *
     * The block size is the size of tiles in the layer. If not specified,
     * it is the same as the tile size.
     *
     * @param blockSize The new size of the block, in pixels
     * @sa getBlockSize()
     */
    void setBlockSize(Vector2u blockSize);

    /**
     * @brief Get the block size
     *
     * The block size is the size of tiles in the layer. If not specified,
     * it is the same as the tile size.
     *
     * @return The block size
     */
    Vector2u getBlockSize() const;

    /**
     * @brief Set a tile
     *
     * @param position The position of the tile in the tile layer
     * @param tile The number of the tile in the tileset or `gf::TileLayer::NoTile`
     * @param flip The flip property of the tile
     * @sa getTile()
     */
    void setTile(Vector2u position, int tile, Flags<Flip> flip = None);

    /**
     * @brief Get a tile
     *
     * @param position The position of the tile in the tile layer
     * @return The number of the tile in the tileset or `gf::TileLayer::NoTile`
     * @sa setTile()
     */
    int getTile(Vector2u position) const;


    /**
     * @brief Get the flip properties of a tile
     *
     * @param position The position of the tile in the tile layer
     * @return A flag to indicate how the tile is flipped
     * @sa setTile()
     */
    Flags<Flip> getFlip(Vector2u position) const;

    /**
     * @brief Remove all the tiles
     */
    void clear();

    /** @} */

    /**
     * @brief Get the local bounding rectangle of the layer
     *
     * The returned rectangle is in local coordinates, which means
     * that it ignores the transformations (translation, rotation,
     * scale, ...) that are applied to the entity.
     * In other words, this function returns the bounds of the
     * entity in the entity's coordinate system.
     *
     * @return Local bounding rectangle of the entity
     */
    RectF getLocalBounds() const;

    /**
     * @brief Set the anchor origin of the entity
     *
     * Compute the origin of the entity based on the local bounds and
     * the specified anchor. Internally, this function calls
     * `Transformable::setOrigin()`.
     *
     * @param anchor The anchor of the entity
     * @sa getLocalBounds(), Transformable::setOrigin()
     */
    void setAnchor(Anchor anchor);

    /**
     * @brief Create a buffer with the current geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current geometry
     */
    VertexBuffer commitGeometry() const;

    virtual void draw(RenderTarget& target, const RenderStates& states) override;

  private:
    struct Cell {
      int tile;
      Flags<Flip> flip;
    };

  private:
    void fillVertexArray(VertexArray& array, RectU rect) const;
    void updateGeometry();

  private:
    Type m_type;
    StaggerIndex m_staggerIndex;
    StaggerAxis m_staggerAxis;

    Vector2u m_layerSize;
    Vector2u m_blockSize;

    const Texture *m_texture;
    Vector2u m_tileSize;
    Vector2u m_margin;
    Vector2u m_spacing;

    Array2D<Cell> m_tiles;

    RectU m_rect;
    VertexArray m_vertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TILE_LAYER_H
