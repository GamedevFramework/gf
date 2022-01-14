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
#include <gf/SpriteBatch.h>

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Transform.h>
#include <gf/Texture.h>
#include <gf/VectorOps.h>

#include <gfpriv/TextureCoords.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SpriteBatch::SpriteBatch(RenderTarget& target)
  : m_target(target)
  , m_count(0)
  {

  }

  void SpriteBatch::begin() {
    m_count = 0;
  }

  namespace {

    bool areStatesSimilar(const RenderStates& lhs, const RenderStates& rhs) {
      return lhs.mode == rhs.mode && lhs.transform == rhs.transform && lhs.shader == rhs.shader;
    }

  } // anonymous namespace

  void SpriteBatch::draw(Sprite& sprite, const RenderStates& states) {
    if (!sprite.hasTexture()) {
      return;
    }

    const Texture& texture = sprite.getTexture();
    RectF textureRect = sprite.getTextureRect();
    Matrix3f transform = sprite.getTransform();
    Color4f color = sprite.getColor();

    if (m_count == 0) {
      m_currentRenderStates.mode = states.mode;
      m_currentRenderStates.transform = states.transform;
      m_currentRenderStates.texture[0] = &texture;
      m_currentRenderStates.shader = states.shader;
    } else {
      if (m_count == MaxSpriteCount || m_currentRenderStates.texture[0] != &texture || !areStatesSimilar(m_currentRenderStates, states)) {
        renderBatch();

        m_currentRenderStates.mode = states.mode;
        m_currentRenderStates.transform = states.transform;
        m_currentRenderStates.texture[0] = &texture;
        m_currentRenderStates.shader = states.shader;
      }
    }

    Vertex vertices[4];

    // compute sprite position

    Vector2i textureSize = texture.getSize();
    Vector2f spriteSize = textureSize * textureRect.getSize();
    RectF bounds = RectF::fromSize(spriteSize);

    vertices[0].position = bounds.getTopLeft();
    vertices[1].position = bounds.getTopRight();
    vertices[2].position = bounds.getBottomLeft();
    vertices[3].position = bounds.getBottomRight();

    for (auto& vertex : vertices) {
      // apply transform as it is different for every sprite
      vertex.position = gf::transform(transform, vertex.position);
    }

    // set sprite color

    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

    // compute sprite texture coordinates

    vertices[0].texCoords = gf::priv::computeTextureCoords(textureRect.getTopLeft());
    vertices[1].texCoords = gf::priv::computeTextureCoords(textureRect.getTopRight());
    vertices[2].texCoords = gf::priv::computeTextureCoords(textureRect.getBottomLeft());
    vertices[3].texCoords = gf::priv::computeTextureCoords(textureRect.getBottomRight());

    // add first triangle

    std::size_t index = m_count * VerticesPerSprite;

    m_vertices[index + 0] = vertices[0];
    m_vertices[index + 1] = vertices[1];
    m_vertices[index + 2] = vertices[2];

    // add second triangle

    m_vertices[index + 3] = vertices[2];
    m_vertices[index + 4] = vertices[1];
    m_vertices[index + 5] = vertices[3];

    m_count++;
  }


  void SpriteBatch::draw(const Texture& texture, Vector2f position, const RenderStates& states) {
    gf::Sprite sprite(texture);
    sprite.setPosition(position);
    draw(sprite, states);
  }

  void SpriteBatch::draw(const Texture& texture, const RectF& textureRect, Vector2f position, const RenderStates& states) {
    gf::Sprite sprite(texture, textureRect);
    sprite.setPosition(position);
    draw(sprite, states);
  }

  void SpriteBatch::end() {
    renderBatch();
  }

  void SpriteBatch::renderBatch() {
    if (m_count == 0) {
      return;
    }

    // Log::debug(Log::Graphics, "Batch %zu sprites...\n", m_count);

    m_target.draw(m_vertices.data(), m_count * VerticesPerSprite, PrimitiveType::Triangles, m_currentRenderStates);
    m_count = 0;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
