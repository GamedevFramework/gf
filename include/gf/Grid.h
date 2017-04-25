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
#ifndef GF_GRID_H
#define GF_GRID_H

#include "Portability.h"
#include "Transformable.h"
#include "Vector.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A square grid
   */
  class GF_API SquareGrid : public gf::Transformable {
  public:
    /**
     * @brief Constructor
     *
     * @param gridSize The size of the grid
     * @param cellSize The size of a cell in the grid
     * @param color The color of the grid frame
     * @param lineWidth The width of the grid frame
     */
    SquareGrid(Vector2u gridSize, Vector2f cellSize, const Color4f& color, float lineWidth = 1.0f);

    /**
     * @brief Set the grid size
     *
     * @param gridSize The new grid size
     */
    void setGridSize(Vector2u gridSize);

    /**
     * @brief Get the grid size
     *
     * @returns The current grid size
     */
    Vector2u getGridSize() const noexcept {
      return m_gridSize;
    }

    /**
     * @brief Set the cell size
     *
     * @param cellSize The new cell size
     */
    void setCellSize(Vector2f cellSize);

    /**
     * @brief Get the cell size
     *
     * @returns The current cell size
     */
    Vector2f getCellSize() const noexcept {
      return m_cellSize;
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

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    void updateGeometry();

  private:
    Vector2u m_gridSize;
    Vector2f m_cellSize;
    Color4f m_color;
    float m_lineWidth;
    VertexArray m_vertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GRID_H
