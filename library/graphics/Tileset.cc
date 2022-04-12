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
#include <gf/Tileset.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Tileset::Tileset()
  : m_texture(nullptr)
  , m_tileSize(0, 0)
  , m_margin(0, 0)
  , m_spacing(0, 0)
  , m_offset(0, 0)
  , m_size(0, 0)
  {
  }

  void Tileset::setTexture(Texture& texture) {
    m_texture = &texture;
    updateSize();
  }

  void Tileset::unsetTexture() {
    m_texture = nullptr;
    updateSize();
  }

  void Tileset::setTileSize(Vector2i tileSize) {
    m_tileSize = tileSize;
    updateSize();
  }

  void Tileset::setMargin(Vector2i margin) {
    m_margin = margin;
    updateSize();
  }

  void Tileset::setSpacing(Vector2i spacing) {
    m_spacing = spacing;
    updateSize();
  }

  void Tileset::setOffset(Vector2i offset) {
    m_offset = offset;
  }

  RectF Tileset::computeTextureCoords(int tile) const {
    return computeTextureCoords({ tile % m_size.width, tile / m_size.width });
  }

  RectF Tileset::computeTextureCoords(Vector2i coords) const {
    assert(m_texture != nullptr);
    assert(coords.x < m_size.width);
    assert(coords.y < m_size.height);
    RectI rect = RectI::fromPositionSize(coords * m_tileSize + coords * m_spacing + m_margin, m_tileSize);
    return m_texture->computeTextureCoords(rect);
  }

  void Tileset::updateSize() {
    if (m_texture == nullptr || m_tileSize.width == 0 || m_tileSize.height == 0) {
      m_size = gf::vec(0, 0);
      return;
    }

    m_size = (m_texture->getSize() - 2 * m_margin + m_spacing) / (m_tileSize + m_spacing);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
