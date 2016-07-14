/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <algorithm>

#include <gf/RenderTarget.h>
#include <gf/Transform.h>

namespace gf {
inline namespace v1 {

  constexpr int TileLayer::NoTile;

  TileLayer::TileLayer(Vector2u layerSize)
  : m_layerSize(layerSize)
  , m_tileSize(0, 0)
  , m_texture(nullptr)
  , m_margin(0)
  , m_spacing(0)
  , m_tiles(layerSize, NoTile)
  , m_rect(0, 0, 0, 0)
  , m_vertices(PrimitiveType::Triangles)
  {

  }

  void TileLayer::setTexture(const Texture& texture) {
    m_texture = &texture;
  }

  void TileLayer::setTileSize(Vector2u tileSize) {
    m_tileSize = tileSize;
  }

  void TileLayer::setMargin(unsigned margin) {
    m_margin = margin;
  }

  void TileLayer::setSpacing(unsigned spacing) {
    m_spacing = spacing;
  }

  void TileLayer::setTile(Vector2u position, int tile) {
    m_tiles(position) = tile;
  }

  int TileLayer::getTile(Vector2u position) const {
    return m_tiles(position);
  }

  void TileLayer::draw(RenderTarget& target, RenderStates states) {
    if (m_texture == nullptr) {
      return;
    }

    // compute the viewable part of the layer

    const View& view = target.getView();
    Vector2f size = view.getSize();
    Vector2f center = view.getCenter();

    size.width = size.height = gf::Sqrt2 * std::max(size.width, size.height);

    RectF world(center - size / 2, size);
    RectF local = gf::transform(getInverseTransform(), world).extend(std::max(m_tileSize.width, m_tileSize.height));

    RectF layer({ 0.0f, 0.0f }, m_layerSize * m_tileSize);

    RectU rect(0, 0, 0, 0);
    RectF intersection;

    if (local.intersects(layer, intersection)) {
      rect.position = intersection.position / m_tileSize + 0.5f;
      rect.size = intersection.size / m_tileSize + 0.5f;
    }

    // build vertex array (if necessary)

    if (rect != m_rect) {
      // std::printf("rect | position: %u,%u | size: %u,%u\n", rect.left, rect.top, rect.width, rect.height);
      m_rect = rect;
      updateGeometry();
    }

    // call draw

    states.transform *= getTransform();
    states.texture = m_texture;

    target.draw(m_vertices, states);
  }

  void TileLayer::updateGeometry() {
    m_vertices.clear();

    if (m_texture == nullptr || m_tileSize.width == 0 || m_tileSize.height == 0) {
      return;
    }

    m_vertices.reserve(m_rect.height * m_rect.width * 6);

    Vector2u tilesetSize = (m_texture->getSize() - 2 * m_margin + m_spacing) / (m_tileSize + m_spacing);

    Vector2u local;

    for (local.y = 0; local.y < m_rect.height; ++local.y) {
      for (local.x = 0; local.x < m_rect.width; ++local.x) {
        Vector2u cell(m_rect.position + local);
        int tile = m_tiles(cell);

        if (tile == NoTile) {
          continue;
        }

        assert(tile >= 0);

        // position

        RectF position(cell * m_tileSize, m_tileSize);

        // texture coords

        Vector2u tileCoords(tile % tilesetSize.width, tile / tilesetSize.width);
        assert(tileCoords.y < tilesetSize.height);

        RectU textureRect(tileCoords * m_tileSize + tileCoords * m_spacing + m_margin, m_tileSize);
        RectF textureCoords = m_texture->computeTextureCoords(textureRect);

        // vertices

        Vertex vertices[4];

        vertices[0].position = position.getTopLeft();
        vertices[1].position = position.getTopRight();
        vertices[2].position = position.getBottomLeft();
        vertices[3].position = position.getBottomRight();

        vertices[0].texCoords = textureCoords.getTopLeft();
        vertices[1].texCoords = textureCoords.getTopRight();
        vertices[2].texCoords = textureCoords.getBottomLeft();
        vertices[3].texCoords = textureCoords.getBottomRight();

        // first triangle

        m_vertices.append(vertices[0]);
        m_vertices.append(vertices[1]);
        m_vertices.append(vertices[2]);

        // second triangle

        m_vertices.append(vertices[2]);
        m_vertices.append(vertices[1]);
        m_vertices.append(vertices[3]);
      }
    }


  }

}
}
