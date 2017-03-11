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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Sprite.h>

#include <gf/RenderTarget.h>
#include <gf/Texture.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Sprite::Sprite()
  : m_texture(nullptr)
  , m_textureRect(0, 0, 1, 1)
  , m_bounds(0, 0, 0, 0)
  {
    updateTexCoords();
  }

  Sprite::Sprite(const Texture& texture)
  : m_texture(&texture)
  , m_textureRect(0, 0, 1, 1)
  , m_bounds(0, 0, 0, 0)
  {
    updateTexCoords();
    updatePositions();
  }

  Sprite::Sprite(const Texture& texture, const RectF& textureRect)
  : m_texture(&texture)
  , m_textureRect(textureRect)
  , m_bounds(0, 0, 0, 0)
  {
    updateTexCoords();
    updatePositions();
  }

  void Sprite::setTexture(const Texture& texture, bool resetRect) {
    m_texture = &texture;
    updatePositions();

    if (resetRect) {
      m_textureRect = { 0.0f, 0.0f, 1.0f, 1.0f };
      updateTexCoords();
    }
  }

  void Sprite::unsetTexture() {
    m_texture = nullptr;
  }

  void Sprite::setTextureRect(const RectF& rect) {
    m_textureRect = rect;
    updateTexCoords();
    updatePositions();
  }

  void Sprite::setColor(const Color4f& color) {
    for (Vertex& vertex : m_vertices) {
      vertex.color = color;
    }
  }

  const Color4f& Sprite::getColor() const {
    return m_vertices[0].color;
  }

  RectF Sprite::getLocalBounds() const {
    return m_bounds;
  }

  void Sprite::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, m_bounds);
  }

  VertexBuffer Sprite::commitGeometry() const {
    VertexBuffer buffer;
    buffer.load(m_vertices, 4, PrimitiveType::TriangleStrip);
    return buffer;
  }

  void Sprite::draw(RenderTarget& target, RenderStates states) {
    if (m_texture == nullptr) {
      return;
    }

    states.transform *= getTransform();
    states.texture = m_texture;
    target.draw(m_vertices, 4, PrimitiveType::TriangleStrip, states);
  }

  void Sprite::updatePositions() {
    if (m_texture == nullptr) {
      return;
    }

    Vector2u textureSize = m_texture->getSize();
    Vector2f spriteSize = textureSize * m_textureRect.size;

    m_vertices[0].position = {  0.0f,            0.0f };
    m_vertices[1].position = { spriteSize.width, 0.0f };
    m_vertices[2].position = {  0.0f,            spriteSize.height };
    m_vertices[3].position = { spriteSize.width, spriteSize.height };

    m_bounds.size = spriteSize;
  }

  void Sprite::updateTexCoords() {
    m_vertices[0].texCoords = m_textureRect.getTopLeft();
    m_vertices[1].texCoords = m_textureRect.getTopRight();
    m_vertices[2].texCoords = m_textureRect.getBottomLeft();
    m_vertices[3].texCoords = m_textureRect.getBottomRight();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
