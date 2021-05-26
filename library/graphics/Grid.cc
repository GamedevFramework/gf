/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#include <gf/Grid.h>

#include <gf/RenderTarget.h>

#include <gf/Stagger.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SquareGrid::SquareGrid(Vector2i gridSize, Vector2f cellSize, const Color4f& color, float lineWidth)
  : m_gridSize(gridSize)
  , m_cellSize(cellSize)
  , m_color(color)
  , m_lineWidth(lineWidth)
  , m_vertices(PrimitiveType::Lines)
  {
    updateGeometry();
  }

  void SquareGrid::setGridSize(Vector2i gridSize) {
    m_gridSize = gridSize;
    updateGeometry();
  }


  void SquareGrid::setCellSize(Vector2f cellSize) {
    m_cellSize = cellSize;
    updateGeometry();
  }

  void SquareGrid::setColor(const Color4f& color) {
    m_color = color;

    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      m_vertices[i].color = m_color;
    }
  }

  RectF SquareGrid::getLocalBounds() const {
    return RectF::fromSize(m_gridSize * m_cellSize);
  }

  void SquareGrid::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  VertexBuffer SquareGrid::commitGeometry() const {
    return VertexBuffer(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
  }

  void SquareGrid::draw(RenderTarget& target, const RenderStates& states) {
    RenderStates localStates = states;
    localStates.transform *= getTransform();
    localStates.lineWidth = m_lineWidth;
    target.draw(m_vertices, localStates);
  }

  void SquareGrid::updateGeometry() {
    m_vertices.clear();
    Vector2f max = m_gridSize * m_cellSize;

    Vertex vertices[2];
    vertices[0].color = vertices[1].color = m_color;

    for (int i = 0; i <= m_gridSize.width; ++i) {
      float x = i * m_cellSize.width;
      vertices[0].position = { x, 0.0f };
      vertices[1].position = { x, max.y };

      m_vertices.append(vertices[0]);
      m_vertices.append(vertices[1]);
    }

    for (int j = 0; j <= m_gridSize.height; ++j) {
      float y = j * m_cellSize.height;
      vertices[0].position = { 0.0f, y };
      vertices[1].position = { max.x, y };

      m_vertices.append(vertices[0]);
      m_vertices.append(vertices[1]);
    }

  }

  HexagonGrid::HexagonGrid(MapCellAxis axis, MapCellIndex index, Vector2i gridSize, float radius, const Color4f& color, float lineWidth)
  : m_gridSize(gridSize)
  , m_radius(radius)
  , m_helper(axis, index, 0) // TODO: compute sideLength
  , m_color(color)
  , m_lineWidth(lineWidth)
  , m_vertices(PrimitiveType::Lines)
  {
    updateGeometry();
  }

  void HexagonGrid::setGridSize(Vector2i gridSize) {
    m_gridSize = gridSize;
    updateGeometry();
  }


  void HexagonGrid::setRadius(float radius) {
    m_radius = radius;
    updateGeometry();
  }

  void HexagonGrid::setColor(const Color4f& color) {
    m_color = color;

    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      m_vertices[i].color = m_color;
    }
  }

  RectF HexagonGrid::getLocalBounds() const {
    auto bounds = m_helper.computeBounds(m_gridSize, m_radius);
    return RectF::fromPositionSize({ -m_lineWidth, -m_lineWidth }, bounds.getSize() + 2.0f * m_lineWidth);
  }

  void HexagonGrid::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  VertexBuffer HexagonGrid::commitGeometry() const {
    return VertexBuffer(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
  }

  void HexagonGrid::draw(RenderTarget& target, const RenderStates& states) {
    RenderStates localStates = states;
    localStates.transform *= getTransform();
    localStates.lineWidth = m_lineWidth;
    target.draw(m_vertices, localStates);
  }

  void HexagonGrid::updateGeometry() {
    m_vertices.clear();

    Vertex vertices[2];
    vertices[0].color = vertices[1].color = m_color;

    for (int i = 0; i < m_gridSize.width; ++i) {
      for (int j = 0; j < m_gridSize.height; ++j) {
        auto corners = m_helper.computeCorners({ i, j }, m_radius);

        for (unsigned k = 0; k < corners.size() - 1; ++k) {
          vertices[0].position = corners[k];
          vertices[1].position = corners[k + 1];
          m_vertices.append(vertices[0]);
          m_vertices.append(vertices[1]);
        }

        vertices[0].position = corners[5];
        vertices[1].position = corners[0];
        m_vertices.append(vertices[0]);
        m_vertices.append(vertices[1]);
      }
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
