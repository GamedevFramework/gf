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
  {
    m_basic.updateGeometry(m_vertices);
  }

  Sprite::Sprite(const Texture& texture)
  : m_basic(texture)
  {
    m_basic.updateGeometry(m_vertices);
  }

  Sprite::Sprite(const Texture& texture, const RectF& textureRect)
  : m_basic(texture, textureRect)
  {
    m_basic.updateGeometry(m_vertices);
  }

  void Sprite::setTexture(const Texture& texture, bool resetRect) {
    setTexture(texture, (resetRect) ? (RectF::fromSize({ 1.0f, 1.0f })) : m_basic.getTextureRect());
  }

  void Sprite::setTexture(const Texture& texture, const RectF& textureRect) {
    m_basic.setTexture(texture, textureRect);
    m_basic.updateGeometry(m_vertices);
  }

  void Sprite::unsetTexture() {
    m_basic.unsetTexture();
  }

  void Sprite::setTextureRect(const RectF& rect) {
    m_basic.setTextureRect(rect);
    m_basic.updateGeometry(m_vertices);
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
    return m_basic.getLocalBounds();
  }

  void Sprite::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  VertexBuffer Sprite::commitGeometry() const {
    return VertexBuffer(m_vertices, 4, PrimitiveType::TriangleStrip);
  }

  void Sprite::draw(RenderTarget& target, const RenderStates& states) {
    if (!m_basic.hasTexture()) {
      return;
    }

    RenderStates localStates = states;

    localStates.transform *= getTransform();
    localStates.texture[0] = &m_basic.getTexture();
    target.draw(m_vertices, 4, PrimitiveType::TriangleStrip, localStates);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
