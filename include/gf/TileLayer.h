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
#ifndef GF_TILE_LAYER_H
#define GF_TILE_LAYER_H

#include <memory>
#include <vector>

#include "Array2D.h"
#include "Cells.h"
#include "CellTypes.h"
#include "Flags.h"
#include "GraphicsApi.h"
#include "Tileset.h"
#include "Transformable.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  /**
   * @ingroup graphics_tiles
   * @brief A tile layer
   *
   * A tile layer represents a map made of tiles. gf::TileLayer makes it easy
   * to draw a tile map.
   *
   * The tile layer is given with an array of indices. Each index correspond
   * to a tile in the tileset. Tile 0 correspond to the tile at the top left
   * in the tileset. Then tile are numbered from left to right, and then from
   * top to bottom. If a tile is not present in the tile layer, the constant
   * gf::TileLayer::NoTile can be used.
   *
   */
  class GF_GRAPHICS_API TileLayer : public Transformable {
  public:
    /**
     * @brief A constant meaning that there is no tile
     */
    static constexpr int NoTile = -1;

    /**
     * @brief Constructor
     */
    TileLayer();

    /**
     * @brief Create an orthogonal tile layer
     *
     * @param layerSize The size of the layer
     * @param tileSize The size of a tile
     */
    static TileLayer createOrthogonal(Vector2i layerSize, Vector2i tileSize);

    /**
     * @brief Create a staggered tile layer
     *
     * @param layerSize The size of the layer
     * @param tileSize The size of a tile
     * @param axis The cells axis
     * @param index The cells index
     */
    static TileLayer createStaggered(Vector2i layerSize, Vector2i tileSize, CellAxis axis, CellIndex index);

    /**
     * @brief Create a hexagonal tile layer
     *
     * @param layerSize The size of the layer
     * @param tileSize The size of a tile
     * @param sideLength The side length
     * @param axis The cells axis
     * @param index The cells index
     */
    static TileLayer createHexagonal(Vector2i layerSize, Vector2i tileSize, int sideLength, CellAxis axis, CellIndex index);

    /**
     * @brief Get the size of the layer
     *
     * @returns The size of the layer, in number of tiles
     */
    Vector2i getMapSize() const {
      return m_tiles.getSize();
    }

    /**
     * @name Tileset parameters
     * @{
     */

    /**
     * @brief Create a tileset id
     *
     * @returns A new tileset id
     */
    std::size_t createTilesetId();

    /**
     * @brief Get a tileset with a tileset id
     *
     * @param id A valid tileset id
     * @returns A reference to the tileset
     */
    Tileset& getTileset(std::size_t id);

    /**
     * @brief Get a tileset with a tileset id
     *
     * @param id A valid tileset id
     * @returns A reference to the tileset
     */
    const Tileset& getTileset(std::size_t id) const;

    /**
     * @brief Enable or disable the smooth filter on the texture of tilesets
     *
     * @param smooth True to enable smoothing, false to disable it
     *
     * @see Tileset::setSmooth()
     */
    void setTilesetSmooth(bool smooth = true);

    /** @} */

    /**
     * @name Tile definition
     * @{
     */

    /**
     * @brief Set a tile
     *
     * @param position The position of the tile in the tile layer
     * @param tileset The tileset id of the tile
     * @param tile The number of the tile in the tileset or `gf::TileLayer::NoTile`
     * @param flip The flip property of the tile
     * @sa getTile(), getFlip(), getTileTileset()
     */
    void setTile(Vector2i position, std::size_t tileset, int tile, Flags<Flip> flip = None);

    /**
     * @brief Get a tile
     *
     * @param position The position of the tile in the tile layer
     * @return The number of the tile in the tileset or `gf::TileLayer::NoTile`
     * @sa setTile()
     */
    int getTile(Vector2i position) const;

    /**
     * @brief Get the flip properties of a tile
     *
     * @param position The position of the tile in the tile layer
     * @return A flag to indicate how the tile is flipped
     * @sa setTile()
     */
    Flags<Flip> getFlip(Vector2i position) const;

    /**
     * @brief Get the tileset property of a tile
     *
     * @param position The position of the tile in the tile layer
     * @return The tileset id of the tile
     * @sa setTile()
     */
    std::size_t getTileTileset(Vector2i position) const;

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
    TileLayer(Vector2i layerSize, CellOrientation orientation, std::unique_ptr<Cells> properties);

  private:
    struct Cell {
      std::size_t tileset = std::size_t(-1);
      int tile = NoTile;
      Flags<Flip> flip = gf::None;
    };

    struct Sheet {
      Tileset tileset;
      VertexArray vertices;
    };

  private:
    void fillVertexArray(std::vector<Sheet>& sheets, RectI rect) const;
    void updateGeometry();
    RectI computeOffsets() const;

  private:
    CellOrientation m_orientation;
    std::unique_ptr<Cells> m_properties;

    Vector2i m_layerSize;

    std::vector<Sheet> m_sheets;
    RectI m_rect;

    Array2D<Cell> m_tiles;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TILE_LAYER_H
