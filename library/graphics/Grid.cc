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
#include <gf/Grid.h>

#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Grid::Grid()
  : m_properties(nullptr)
  , m_gridSize(0, 0)
  , m_color(gf::Color::Black)
  , m_lineWidth(1.0f)
  , m_vertices(PrimitiveType::Lines)
  {
  }

  Grid Grid::createOrthogonal(Vector2i gridSize, Vector2f cellSize) {
    return Grid(gridSize, std::make_unique<OrthogonalCells>(cellSize));
  }

  Grid Grid::createStaggered(Vector2i gridSize, Vector2f cellSize, CellAxis axis, CellIndex index) {
    return Grid(gridSize, std::make_unique<StaggeredCells>(cellSize, axis, index));
  }

  Grid Grid::createHexagonal(Vector2i gridSize, Vector2f cellSize, float sideLength, CellAxis axis, CellIndex index) {
    return Grid(gridSize, std::make_unique<HexagonalCells>(cellSize, sideLength, axis, index));
  }

  Grid Grid::createHexagonal(Vector2i gridSize, float radius, CellAxis axis, CellIndex index) {
    return Grid(gridSize, std::make_unique<HexagonalCells>(radius, axis, index));
  }

  void Grid::setGridSize(Vector2i gridSize) {
    m_gridSize = gridSize;
    updateGeometry();
  }

  void Grid::setColor(const Color4f& color) {
    m_color = color;

    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      m_vertices[i].color = m_color;
    }
  }
  
  void Grid::setSelectedColor(const Color4f& color){
    m_selectedColor=color;
  }

  void Grid::hover(Vector2f pointer) {
    if (!m_properties) {
      return;
    }

    Vector2f local = gf::transform(getInverseTransform(), pointer);
    Vector2i selected = m_properties->computeCoordinates(local);

    if (selected == m_selected) {
      return;
    }

    m_selected = selected;
    m_selectedVertices.clear();

    Vertex vertices[2];
    vertices[0].color = vertices[1].color = m_selectedColor;

    auto polyline = m_properties->computePolyline(m_selected);
    assert(polyline.isLoop());

    for (std::size_t k = 0; k < polyline.getPointCount(); ++k) {
      vertices[0].position = polyline.getPoint(k);
      vertices[1].position = polyline.getNextPoint(k);
      m_selectedVertices.append(vertices[0]);
      m_selectedVertices.append(vertices[1]);
    }
  }


  RectF Grid::getLocalBounds() const {
    if (!m_properties) {
      return RectF();
    }

    return m_properties->computeBounds(m_gridSize);
  }

  void Grid::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  VertexBuffer Grid::commitGeometry() const {
    return VertexBuffer(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
  }

  void Grid::draw(RenderTarget& target, const RenderStates& states) {
    RenderStates localStates = states;
    localStates.transform *= getTransform();
    localStates.lineWidth = m_lineWidth;
    target.draw(m_vertices, localStates);

    if (RectI::fromSize(m_gridSize).contains(m_selected)) {
      target.draw(m_selectedVertices, localStates);
    }
  }

  Grid::Grid(Vector2i gridSize, std::unique_ptr<Cells> properties)
  : m_properties(std::move(properties))
  , m_gridSize(gridSize)
  , m_color(Color::Black)
  , m_lineWidth(1.0f)
  , m_vertices(PrimitiveType::Lines)
  , m_selected(-1, -1)
  , m_selectedColor(Color::Red)
  , m_selectedVertices(PrimitiveType::Lines)
  {
    updateGeometry();
  }

  void Grid::updateGeometry() {
    if (!m_properties) {
      return;
    }

    m_vertices.clear();

    Vertex vertices[2];
    vertices[0].color = vertices[1].color = m_color;

    for (int i = 0; i < m_gridSize.width; ++i) {
      for (int j = 0; j < m_gridSize.height; ++j) {
        auto polyline = m_properties->computePolyline({ i, j });
        assert(polyline.isLoop());

        for (std::size_t k = 0; k < polyline.getPointCount(); ++k) {
          vertices[0].position = polyline.getPoint(k);
          vertices[1].position = polyline.getNextPoint(k);
          m_vertices.append(vertices[0]);
          m_vertices.append(vertices[1]);
        }
      }
    }

  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
