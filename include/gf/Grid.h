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
#ifndef GF_GRID_H
#define GF_GRID_H

#include <memory>

#include "Cells.h"
#include "GraphicsApi.h"
#include "Transformable.h"
#include "Vector.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_drawables
   * @brief A grid of cells
   *
   * A grid is a set of regular cells. The cells can be orthogonal, staggered or hexagonal.
   *
   * The grid shows the limits of the cells and, optionnaly, the selected cell.
   * It can be used for debugging purpose.
   *
   * @see gf::Cells
   */
  class GF_GRAPHICS_API Grid : public gf::Transformable {
  public:
    /**
     * @brief Constructor
     */
    Grid();

    /**
     * @brief Create an orthogonal grid
     *
     * @param gridSize The size of the grid
     * @param cellSize The size of a cell
     */
    static Grid createOrthogonal(Vector2i gridSize, Vector2f cellSize);

    /**
     * @brief Create a staggered grid
     *
     * @param gridSize The size of the grid
     * @param cellSize The size of a cell
     * @param axis The cells axis
     * @param index The cells index
     */
    static Grid createStaggered(Vector2i gridSize, Vector2f cellSize, CellAxis axis, CellIndex index);

    /**
     * @brief Create a hexagonal grid
     *
     * @param gridSize The size of the grid
     * @param cellSize The size of a cell
     * @param sideLength The side length
     * @param axis The cells axis
     * @param index The cells index
     */
    static Grid createHexagonal(Vector2i gridSize, Vector2f cellSize, float sideLength, CellAxis axis, CellIndex index);

    /**
     * @brief Create a regular hexagonal grid
     *
     * @param gridSize The size of the grid
     * @param radius The radius of the regular hexagon
     * @param axis The cells axis
     * @param index The cells index
     */
    static Grid createHexagonal(Vector2i gridSize, float radius, CellAxis axis, CellIndex index);

    /**
     * @brief Get the underlying cells
     */
    const Cells& getCells() const {
      return *m_properties;
    }

    /**
     * @brief Set the grid size
     *
     * @param gridSize The new grid size
     */
    void setGridSize(Vector2i gridSize);

    /**
     * @brief Get the grid size
     *
     * @returns The current grid size
     */
    Vector2i getGridSize() const noexcept {
      return m_gridSize;
    }

    /**
     * @brief Set the color of the grid frame
     *
     * @param color The new color of the grid frame
     */
    void setColor(const Color4f& color);

    /**
     * @brief Get the color of the grid frame
     *
     * @returns The current color of the grid frame
     */
    const Color4f& getColor() const noexcept {
      return m_color;
    }

    /**
     * @brief Set the color of the selected cell
     *
     * @param color The new color of the selected cell
     */
    void setSelectedColor(const Color4f& color);

    /**
     * @brief Get the color of the selected cell
     *
     * @returns The current color of the selected cell
     */
    const Color4f& getSelectedColor() const noexcept {
      return m_selectedColor;
    }

    /**
     * @brief Set the width of the grid frame
     *
     * @param lineWidth The new width of the grid frame
     */
    void setLineWidth(float lineWidth) noexcept {
      m_lineWidth = lineWidth;
    }

    /**
     * @brief Get the width of the grid frame
     *
     * @returns The current width of the grid frame
     */
    float getLineWidth() const noexcept {
      return m_lineWidth;
    }

    void hover(Vector2f pointer);


    /**
     * @brief Get the local bounding rectangle of the entity
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

    void draw(RenderTarget& target, const RenderStates& states) override;

  private:
    Grid(Vector2i gridSize, std::unique_ptr<Cells> properties);

    void updateGeometry();

  private:
    std::unique_ptr<Cells> m_properties;
    Vector2i m_gridSize;
    Color4f m_color;
    float m_lineWidth;
    VertexArray m_vertices;
    Vector2i m_selected;
    Color4f m_selectedColor;
    VertexArray m_selectedVertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GRID_H
