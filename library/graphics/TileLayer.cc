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
#include <gf/TileLayer.h>

#include <cassert>
#include <cstdio>
#include <algorithm>

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include <gfpriv/TextureCoords.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  constexpr int TileLayer::NoTile;

  TileLayer::TileLayer()
  : m_orientation(CellOrientation::Unknown)
  , m_properties(nullptr)
  , m_layerSize(0, 0)
  , m_rect(RectI::empty())
  {
  }

  TileLayer::TileLayer(Vector2i layerSize, CellOrientation orientation, std::unique_ptr<Cells> properties)
  : m_orientation(orientation)
  , m_properties(std::move(properties))
  , m_layerSize(layerSize)
  , m_rect(RectI::empty())
  , m_tiles(layerSize)
  {
    clear();
  }

  TileLayer TileLayer::createOrthogonal(Vector2i layerSize, Vector2i tileSize) {
    return TileLayer(layerSize, CellOrientation::Orthogonal, std::make_unique<OrthogonalCells>(tileSize));
  }

  TileLayer TileLayer::createStaggered(Vector2i layerSize, Vector2i tileSize, CellAxis axis, CellIndex index) {
    return TileLayer(layerSize, CellOrientation::Staggered, std::make_unique<StaggeredCells>(tileSize, axis, index));
  }

  TileLayer TileLayer::createHexagonal(Vector2i layerSize, Vector2i tileSize, int sideLength, CellAxis axis, CellIndex index) {
    return TileLayer(layerSize, CellOrientation::Hexagonal, std::make_unique<HexagonalCells>(tileSize, sideLength, axis, index));
  }

  std::size_t TileLayer::createTilesetId() {
    std::size_t id = m_sheets.size();
    m_sheets.push_back({ Tileset{}, VertexArray(PrimitiveType::Triangles) });
    return id;
  }

  Tileset& TileLayer::getTileset(std::size_t id) {
    assert(id < m_sheets.size());
    return m_sheets[id].tileset;
  }

  const Tileset& TileLayer::getTileset(std::size_t id) const {
    assert(id < m_sheets.size());
    return m_sheets[id].tileset;
  }

  void TileLayer::setTilesetSmooth(bool smooth) {
    for (auto & sheet : m_sheets) {
      sheet.tileset.setSmooth(smooth);
    }
  }


  void TileLayer::setTile(Vector2i position, std::size_t tileset, int tile, Flags<Flip> flip) {
    assert(m_tiles.isValid(position));
    m_tiles(position) = { tileset, tile, flip };
  }

  int TileLayer::getTile(Vector2i position) const {
    assert(m_tiles.isValid(position));
    return m_tiles(position).tile;
  }

  Flags<Flip> TileLayer::getFlip(Vector2i position) const {
    assert(m_tiles.isValid(position));
    return m_tiles(position).flip;
  }

  std::size_t TileLayer::getTileTileset(Vector2i position) const {
    assert(m_tiles.isValid(position));
    return m_tiles(position).tileset;
  }

  void TileLayer::clear() {
    for (auto& cell : m_tiles) {
      cell.tileset = std::size_t(-1);
      cell.tile = NoTile;
      cell.flip = None;
    }
  }

  RectF TileLayer::getLocalBounds() const {
    assert(m_properties);
    return m_properties->computeBounds(m_layerSize);
  }

  void TileLayer::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  VertexBuffer TileLayer::commitGeometry() const {
    // TODO: there is more than one geometry
    VertexArray vertices(PrimitiveType::Triangles);
//     RectI rect = RectI::fromSize(m_layerSize);
//     fillVertexArray(vertices, rect);
    return VertexBuffer(vertices.getVertexData(), vertices.getVertexCount(), vertices.getPrimitiveType());
  }

  void TileLayer::draw(RenderTarget& target, const RenderStates& states) {
    if (m_sheets.empty() || m_orientation == CellOrientation::Unknown || m_properties == nullptr) {
      return;
    }

    auto inverseTransform = getInverseTransform();

    auto toLocal = [&](Vector2i point) {
      return gf::transform(inverseTransform, target.mapPixelToCoords(point));
    };

    RectI screen = RectI::fromSize(target.getSize());

    RectF local = RectF::empty();
    local.extend(toLocal(screen.getTopLeft()));
    local.extend(toLocal(screen.getTopRight()));
    local.extend(toLocal(screen.getBottomLeft()));
    local.extend(toLocal(screen.getBottomRight()));

    RectI layer = gf::RectI::fromSize(m_layerSize - 1);
    RectI visible = m_properties->computeVisibleArea(local);
    RectI rect = visible.getIntersection(layer);

    // TODO: handle offsets of tilesets

    // build vertex array (if necessary)

    if (!m_rect.contains(rect)) {
      m_rect = rect.grow(5).getIntersection(layer);
//       std::printf("local  | min: %g,%g | max: %g,%g\n", local.min.x, local.min.y, local.max.x, local.max.y);
//       std::printf("rect   | min: %i,%i | max: %i,%i\n", m_rect.min.x, m_rect.min.y, m_rect.max.x, m_rect.max.y);
      updateGeometry();
    }

    // call draw

    RenderStates localStates = states;

    localStates.transform *= getTransform();

    for (auto& sheet : m_sheets) {
      localStates.texture[0] = &sheet.tileset.getTexture();
      target.draw(sheet.vertices, localStates);
    }

    // TODO: maybe do not draw tileset by tileset, but row by row with a batch
  }

  void TileLayer::fillVertexArray(std::vector<Sheet>& sheets, RectI rect) const {
    assert(m_properties);

    for (auto& sheet : sheets) {
      sheet.vertices.clear();
    }

    Vector2i coords;

    for (coords.y = rect.min.y; coords.y <= rect.max.y; ++coords.y) {
      for (coords.x = rect.min.x; coords.x <= rect.max.x; ++coords.x) {
        assert(m_tiles.isValid(coords));
        const Cell& cell = m_tiles(coords);

        if (cell.tile == NoTile) {
          continue;
        }

        assert(cell.tile >= 0);

        assert(cell.tileset < sheets.size());
        Sheet& sheet = sheets[cell.tileset];

        // position

        RectF bounds = m_properties->computeCellBounds(coords);
        Vector2f position = bounds.getPosition();
        position += sheet.tileset.getOffset();
        Vector2f boxSize = bounds.getSize();

        // Keep the tile ratio
        const Vector2f tileSize = sheet.tileset.getTileSize();
        if ((tileSize.width / tileSize.height) != 1.0f)  {
          const Vector2f ratio = tileSize / boxSize;
          boxSize *= ratio;
        }

        RectF box = RectF::fromPositionSize(position, boxSize);

        // texture coords

        RectF textureCoords = sheet.tileset.computeTextureCoords(cell.tile);

        // vertices

        Vertex vertices[4];

        vertices[0].position = box.getTopLeft();
        vertices[1].position = box.getTopRight();
        vertices[2].position = box.getBottomLeft();
        vertices[3].position = box.getBottomRight();

        vertices[0].texCoords = gf::priv::computeTextureCoords(textureCoords.getTopLeft());
        vertices[1].texCoords = gf::priv::computeTextureCoords(textureCoords.getTopRight());
        vertices[2].texCoords = gf::priv::computeTextureCoords(textureCoords.getBottomLeft());
        vertices[3].texCoords = gf::priv::computeTextureCoords(textureCoords.getBottomRight());

        auto flip = cell.flip;

        // order of flip matters:
        // http://docs.mapeditor.org/en/latest/reference/tmx-map-format/#tile-flipping

        if (flip.test(Flip::Diagonally)) {
          std::swap(vertices[1].texCoords, vertices[2].texCoords);
        }

        if (flip.test(Flip::Horizontally)) {
          std::swap(vertices[0].texCoords, vertices[1].texCoords);
          std::swap(vertices[2].texCoords, vertices[3].texCoords);
        }

        if (flip.test(Flip::Vertically)) {
          std::swap(vertices[0].texCoords, vertices[2].texCoords);
          std::swap(vertices[1].texCoords, vertices[3].texCoords);
        }

        // first triangle

        sheet.vertices.append(vertices[0]);
        sheet.vertices.append(vertices[1]);
        sheet.vertices.append(vertices[2]);

        // second triangle

        sheet.vertices.append(vertices[2]);
        sheet.vertices.append(vertices[1]);
        sheet.vertices.append(vertices[3]);
      }
    }

  }

  void TileLayer::updateGeometry() {
    if (m_sheets.empty() || m_properties == nullptr) {
      return;
    }

    fillVertexArray(m_sheets, m_rect);
  }

  RectI TileLayer::computeOffsets() const {
    RectI offsets;

    for (auto& sheet : m_sheets) {
      offsets.extend(sheet.tileset.getOffset());
    }

    return offsets;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
