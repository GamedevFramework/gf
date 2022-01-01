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
#include <gf/Shape.h>

#include <cassert>
#include <limits>

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Texture.h>
#include <gf/VectorOps.h>

#include <gfpriv/TextureCoords.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Shape::Shape()
  : m_texture(nullptr)
  , m_textureRect(gf::RectF::fromSize({ 1.0f, 1.0f }))
  , m_color(Color::White)
  , m_vertices(PrimitiveType::TriangleFan)
  , m_outlineColor(Color::White)
  , m_outlineThickness(0.0f)
  , m_outlineVertices(PrimitiveType::TriangleStrip)
  {

  }

  void Shape::setTexture(const Texture& texture, bool resetRect) {
    setTexture(texture, (resetRect) ? (RectF::fromSize({ 1.0f, 1.0f })) : m_textureRect);
  }

  void Shape::setTexture(const Texture& texture, const RectF& textureRect) {
    m_texture = &texture;

    m_textureRect = textureRect;
    updateTexCoords();
  }

  void Shape::unsetTexture() {
    m_texture = nullptr;
    m_textureRect = RectF();
  }

  void Shape::setTextureRect(const RectF& rect) {
    m_textureRect = rect;
    updateTexCoords();
  }

  void Shape::setColor(const Color4f& color) {
    m_color = color;
    updateColors();
  }

  void Shape::setOutlineColor(const Color4f& color) {
    m_outlineColor = color;
    updateOutlineColors();
  }

  void Shape::setOutlineThickness(float thickness) {
    m_outlineThickness = thickness;
    updateOutline();
  }

  RectF Shape::getLocalBounds() const {
    return m_bounds;
  }

  void Shape::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, m_bounds);
  }

  void Shape::draw(RenderTarget& target, const RenderStates& states) {
    RenderStates localStates = states;

    localStates.transform *= getTransform();

    if (m_outlineThickness > 0.0f) {
      localStates.texture[0] = nullptr;
      target.draw(m_outlineVertices, localStates);
    }

    localStates.texture[0] = m_texture;
    target.draw(m_vertices, localStates);
  }

  void Shape::updateGeometry() {
    std::size_t count = getPointCount();
    assert(count >= 3);

    m_vertices.resize(count + 2);

    Vector2f min = gf::vec(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector2f max = gf::vec(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (std::size_t i = 0; i < count; ++i) {
      auto point = getPoint(i);
      m_vertices[i + 1].position = point;
      min = gf::min(min, point);
      max = gf::max(max, point);
    }

    m_vertices[count + 1].position = m_vertices[1].position;
    m_vertices[0].position = (min + max) / 2;

    updateColors();
    updateTexCoords();

    if (m_outlineThickness > 0.0f) {
      updateOutline();
    }
  }

  void Shape::updateBounds(RectF bounds) {
    m_bounds = bounds;
    updateTexCoords();
  }

  void Shape::updateAutoBounds() {
    m_bounds = m_vertices.getBounds();
    updateTexCoords();
  }

  void Shape::updateColors() {
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      m_vertices[i].color = m_color;
    }
  }

  void Shape::updateTexCoords() {
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      Vector2f ratio(0.0f, 0.0f);

      if (!m_bounds.isEmpty()) {
        ratio = (m_vertices[i].position - m_bounds.getPosition()) / m_bounds.getSize();
      }

      m_vertices[i].texCoords = gf::priv::computeTextureCoords(m_textureRect.getPosition() + m_textureRect.getSize() * ratio);
    }
  }

  void Shape::updateOutline() {
    std::size_t count = getPointCount();
    m_outlineVertices.resize((count + 1) * 2);

    Vector2f center = m_vertices[0].position;

    for (std::size_t i = 0; i < count; ++i) {
      std::size_t j = i + 1;

      Vector2f prev = (i == 0) ? m_vertices[count].position : m_vertices[j - 1].position;
      Vector2f curr = m_vertices[j].position;
      Vector2f next = m_vertices[j + 1].position;

      Vector2f normalPrev = normalize(perp(curr - prev));
      Vector2f normalNext = normalize(perp(next - curr));

      if (dot(normalPrev, center - curr) > 0) {
        normalPrev = -normalPrev;
      }

      if (dot(normalNext, center - curr) > 0) {
        normalNext = -normalNext;
      }

      float factor = 1.0f + dot(normalPrev, normalNext);
      Vector2f normal = (normalPrev + normalNext) / factor;

      m_outlineVertices[i * 2 + 0].position = curr;
      m_outlineVertices[i * 2 + 1].position = curr + normal * m_outlineThickness;
    }

    m_outlineVertices[count * 2 + 0].position = m_outlineVertices[0].position;
    m_outlineVertices[count * 2 + 1].position = m_outlineVertices[1].position;

    updateOutlineColors();
  }

  void Shape::updateOutlineColors() {
    for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i) {
      m_outlineVertices[i].color = m_outlineColor;
    }
  }

  VertexBuffer Shape::commitGeometry() const {
    return VertexBuffer(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
  }

  VertexBuffer Shape::commitOutlineGeometry() const {
    return VertexBuffer(m_outlineVertices.getVertexData(), m_outlineVertices.getVertexCount(), m_outlineVertices.getPrimitiveType());
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
