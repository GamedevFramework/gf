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
#include <gf/BasicSprite.h>

#include <gf/Texture.h>
#include <gf/VectorOps.h>
#include <gf/Vertex.h>

#include <gfpriv/TextureCoords.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  BasicSprite::BasicSprite()
  : m_texture(nullptr)
  , m_textureRect(RectF::fromSize({ 1.0f, 1.0f }))
  {

  }

  BasicSprite::BasicSprite(const Texture& texture)
  : m_texture(&texture)
  , m_textureRect(RectF::fromSize({ 1.0f, 1.0f }))
  {

  }

  BasicSprite::BasicSprite(const Texture& texture, const RectF& textureRect)
  : m_texture(&texture)
  , m_textureRect(textureRect)
  {

  }

  void BasicSprite::setTexture(const Texture& texture, bool resetRect) {
    setTexture(texture, (resetRect) ? (RectF::fromSize({ 1.0f, 1.0f })) : m_textureRect);
  }

  void BasicSprite::setTexture(const Texture& texture, const RectF& textureRect) {
    m_texture = &texture;
    m_textureRect = textureRect;
  }

  void BasicSprite::unsetTexture() {
    m_texture = nullptr;
  }

  void BasicSprite::setTextureRect(const RectF& rect) {
    m_textureRect = rect;
  }

  RectF BasicSprite::getLocalBounds() const {
    return m_bounds;
  }

  void BasicSprite::updateGeometry(Span<Vertex> vertices) {
    assert(vertices.getSize() >= 4);

    vertices[0].texCoords = gf::priv::computeTextureCoords(m_textureRect.getTopLeft());
    vertices[1].texCoords = gf::priv::computeTextureCoords(m_textureRect.getTopRight());
    vertices[2].texCoords = gf::priv::computeTextureCoords(m_textureRect.getBottomLeft());
    vertices[3].texCoords = gf::priv::computeTextureCoords(m_textureRect.getBottomRight());

    if (m_texture == nullptr) {
      return;
    }

    Vector2i textureSize = m_texture->getSize();
    Vector2f spriteSize = textureSize * m_textureRect.getSize();

    m_bounds = RectF::fromSize(spriteSize);

    vertices[0].position = m_bounds.getTopLeft();
    vertices[1].position = m_bounds.getTopRight();
    vertices[2].position = m_bounds.getBottomLeft();
    vertices[3].position = m_bounds.getBottomRight();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
