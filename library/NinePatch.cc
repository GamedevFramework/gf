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
#include <gf/NinePatch.h>

#include <gf/RenderTarget.h>
#include <gf/Texture.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  NinePatch::NinePatch()
  : m_texture(nullptr)
  , m_textureRect(0, 0, 1, 1)
  , m_top(0)
  , m_bottom(1)
  , m_left(0)
  , m_right(1)
  , m_size({ 0, 0 })
  {
    updatePositions();
    updateTexCoords();
  }

  NinePatch::NinePatch(const Texture& texture)
  : m_texture(&texture)
  , m_textureRect(0, 0, 1, 1)
  , m_top(0)
  , m_bottom(1)
  , m_left(0)
  , m_right(1)
  , m_size({ 0, 0 })
  {
    updatePositions();
    updateTexCoords();
  }

  NinePatch::NinePatch(const Texture& texture, const RectF& textureRect)
  : m_texture(&texture)
  , m_textureRect(textureRect)
  , m_top(0)
  , m_bottom(1)
  , m_left(0)
  , m_right(1)
  , m_size({ 0, 0 })
  {
    updatePositions();
    updateTexCoords();
  }

  void NinePatch::setTexture(const Texture& texture, bool resetRect) {
    m_texture = &texture;
    updatePositions();

    if (resetRect) {
      m_textureRect = { 0.0f, 0.0f, 1.0f, 1.0f };
      updateTexCoords();
    }
  }

  void NinePatch::unsetTexture() {
    m_texture = nullptr;
  }

  void NinePatch::setTextureRect(const RectF& rect) {
    m_textureRect = rect;
    updateTexCoords();
  }

  void NinePatch::setColor(const Color4f& color) {
    for (Vertex& vertex : m_vertices) {
      vertex.color = color;
    }
  }

  const Color4f& NinePatch::getColor() const {
    return m_vertices[0].color;
  }

  void NinePatch::setLimits(float top, float bottom, float left, float right) {
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;

    updatePositions();
    updateTexCoords();
  }

  void NinePatch::setVerticalLimits(float top, float bottom) {
    m_top = top;
    m_bottom = bottom;

    updatePositions();
    updateTexCoords();
  }

  void NinePatch::setHorizontalLimits(float left, float right) {
    m_left = left;
    m_right = right;

    updatePositions();
    updateTexCoords();
  }

  void NinePatch::setSize(Vector2f size) {
    m_size = size;
    updatePositions();
  }

  RectF NinePatch::getLocalBounds() const {
    return RectF({ 0.0f, 0.0f }, m_size);
  }

  void NinePatch::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  /*
   *  0---4---8--12
   *  |   |   |   |
   *  1---5---9--13
   *  |   |   |   |
   *  2---6--10--14
   *  |   |   |   |
   *  3---7--11--15
   *
   */

  constexpr uint16_t gIndices[24] = {
      0,  1,  4,  5,  8,  9, 12, 13,
     13, 14,  9, 10,  5,  6,  1,  2,
      2,  3,  6,  7, 10, 11, 14, 15,
  }; // warning, there are four empty triangles

  VertexBuffer NinePatch::commitGeometry() const {
    VertexBuffer buffer;
    buffer.load(m_vertices, gIndices, 24, PrimitiveType::TriangleStrip);
    return buffer;
  }

  void NinePatch::draw(RenderTarget& target, RenderStates states) {
    if (m_texture == nullptr) {
      return;
    }

    states.transform *= getTransform();
    states.texture = m_texture;

    target.draw(m_vertices, gIndices, 24, PrimitiveType::TriangleStrip, states);
  }

  void NinePatch::updatePositions() {
    if (m_texture == nullptr) {
      return;
    }

    auto texSize = m_texture->getSize();

    float x0 = 0.0f;
    float x1 = m_left * texSize.width;
    float x2 = m_size.width - m_right * texSize.width;
    float x3 = m_size.width;

    float y0 = 0.0f;
    float y1 = m_top * texSize.height;
    float y2 = m_size.height - m_bottom * texSize.height;
    float y3 = m_size.height;

    m_vertices[ 0].position = { x0, y0 };
    m_vertices[ 1].position = { x0, y1 };
    m_vertices[ 2].position = { x0, y2 };
    m_vertices[ 3].position = { x0, y3 };
    m_vertices[ 4].position = { x1, y0 };
    m_vertices[ 5].position = { x1, y1 };
    m_vertices[ 6].position = { x1, y2 };
    m_vertices[ 7].position = { x1, y3 };
    m_vertices[ 8].position = { x2, y0 };
    m_vertices[ 9].position = { x2, y1 };
    m_vertices[10].position = { x2, y2 };
    m_vertices[11].position = { x2, y3 };
    m_vertices[12].position = { x3, y0 };
    m_vertices[13].position = { x3, y1 };
    m_vertices[14].position = { x3, y2 };
    m_vertices[15].position = { x3, y3 };
  }

  void NinePatch::updateTexCoords() {
    float x0 = m_textureRect.left;
    float x1 = m_textureRect.left + m_left * m_textureRect.width;
    float x2 = m_textureRect.left + (1 - m_right) * m_textureRect.width;
    float x3 = m_textureRect.left + m_textureRect.width;

    float y0 = m_textureRect.top;
    float y1 = m_textureRect.top + m_top * m_textureRect.height;
    float y2 = m_textureRect.top + (1 - m_bottom) * m_textureRect.height;
    float y3 = m_textureRect.top + m_textureRect.height;

    m_vertices[ 0].texCoords = { x0, y0 };
    m_vertices[ 1].texCoords = { x0, y1 };
    m_vertices[ 2].texCoords = { x0, y2 };
    m_vertices[ 3].texCoords = { x0, y3 };
    m_vertices[ 4].texCoords = { x1, y0 };
    m_vertices[ 5].texCoords = { x1, y1 };
    m_vertices[ 6].texCoords = { x1, y2 };
    m_vertices[ 7].texCoords = { x1, y3 };
    m_vertices[ 8].texCoords = { x2, y0 };
    m_vertices[ 9].texCoords = { x2, y1 };
    m_vertices[10].texCoords = { x2, y2 };
    m_vertices[11].texCoords = { x2, y3 };
    m_vertices[12].texCoords = { x3, y0 };
    m_vertices[13].texCoords = { x3, y1 };
    m_vertices[14].texCoords = { x3, y2 };
    m_vertices[15].texCoords = { x3, y3 };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
