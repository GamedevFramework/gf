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
#include <gf/Curve.h>

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Curve::Curve()
  : m_type(Type::Outlined)
  , m_closed(false)
  , m_color(Color::White)
  , m_width(1)
  , m_vertices(PrimitiveType::TriangleStrip)
  , m_bounds(0, 0, 0, 0)
  , m_outlineColor(Color::White)
  , m_outlineThickness(0.0f)
  , m_outlineVertices(PrimitiveType::TriangleStrip)
  {
  }

  void Curve::setType(Type type) {
    if (m_type == type) {
      return;
    }

    m_type = type;

    switch (m_type) {
      case Type::Simple:
        m_vertices.setPrimitiveType(m_closed ? PrimitiveType::LineLoop : PrimitiveType::LineStrip);
        m_outlineVertices.clear();
        break;

      case Type::Outlined:
        m_vertices.setPrimitiveType(PrimitiveType::TriangleStrip);
        break;
    }

    updateGeometry();
  }

  void Curve::setColor(const Color4f& color) {
    m_color = color;
    updateColors();
  }

  void Curve::setWidth(float width) {
    m_width = width;
    updateGeometry();
  }

  void Curve::setOutlineColor(const Color4f& color) {
    m_outlineColor = color;
    updateOutlineColors();
  }

  void Curve::setOutlineThickness(float thickness) {
    m_outlineThickness = thickness;
    updateOutline();
  }

  RectF Curve::getLocalBounds() const {
    return m_bounds;
  }

  void Curve::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, m_bounds);
  }

  VertexBuffer Curve::commitGeometry() const {
    VertexBuffer buffer;
    buffer.load(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
    return buffer;
  }

  VertexBuffer Curve::commitOutlineGeometry() const {
    VertexBuffer buffer;
    buffer.load(m_outlineVertices.getVertexData(), m_outlineVertices.getVertexCount(), m_outlineVertices.getPrimitiveType());
    return buffer;
  }

  void Curve::draw(RenderTarget& target, RenderStates states) {
    states.transform *= getTransform();

    if (m_type == Type::Simple) {
      states.lineWidth = m_width;
    }

    if (m_type == Type::Outlined && m_outlineThickness > 0.0f) {
      target.draw(m_outlineVertices, states);
    }

    target.draw(m_vertices, states);
  }

  void Curve::setClosed(bool closed) {
    if (m_closed == closed) {
      return;
    }

    m_closed = closed;

    if (m_type == Type::Simple) {
      m_vertices.setPrimitiveType(PrimitiveType::LineLoop);
    }

    updateGeometry();
  }

  void Curve::updateGeometry() {
    if (getPointCount() < 2) {
      return;
    }

    switch (m_type) {
      case Type::Simple:
        m_vertices.resize(getPointCount());

        for (std::size_t i = 0; i < getPointCount(); ++i) {
          m_vertices[i].position = getPoint(i);
        }

        break;

      case Type::Outlined:
        computeVertices(m_vertices, m_width / 2);
        break;
    }

    m_bounds = m_vertices.getBounds();

    updateColors();

    if (m_type == Type::Outlined && m_outlineThickness > 0.0f) {
      updateOutline();
    }
  }

  void Curve::updateColors() {
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      m_vertices[i].color = m_color;
    }
  }

  void Curve::updateOutline() {
    computeVertices(m_outlineVertices, m_width / 2 + m_outlineThickness);
    updateOutlineColors();
  }

  void Curve::updateOutlineColors() {
    for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i) {
      m_outlineVertices[i].color = m_outlineColor;
    }
  }

  static Vector2f computeNormal(Vector2f prev, Vector2f curr, Vector2f next) {
    Vector2f normalPrev = normalize(perp(curr - prev));
    Vector2f normalNext = normalize(perp(next - curr));

    float factor = 1.0f + dot(normalPrev, normalNext);
    return (normalPrev + normalNext) / factor;
  }

  void Curve::computeVertices(VertexArray& vertices, float halfWidth) {
    std::size_t count = getPointCount();
    assert(count >= 2);

    if (m_closed) {
      vertices.resize(2 * count + 2);
    } else {
      vertices.resize(2 * count);
    }

    // first point

    Vector2f curr = getPoint(0);
    Vector2f next = getPoint(1);

    if (m_closed) {
      Vector2f prev = getPoint(count - 1);
      Vector2f normal = computeNormal(prev, curr, next);
      vertices[0].position = curr - normal * halfWidth;
      vertices[1].position = curr + normal * halfWidth;
    } else {
      Vector2f normal = normalize(perp(next - curr));
      vertices[0].position = curr - normal * halfWidth;
      vertices[1].position = curr + normal * halfWidth;
    }

    // middle points

    for (std::size_t i = 1; i < count - 1; ++i) {
      Vector2f prev = curr;
      curr = next;
      next = getPoint(i + 1);

      Vector2f normal = computeNormal(prev, curr, next);
      vertices[i * 2 + 0].position = curr - normal * halfWidth;
      vertices[i * 2 + 1].position = curr + normal * halfWidth;
    }

    // last point

    Vector2f prev = curr;
    curr = getPoint(count - 1);

    if (m_closed) {
      next = getPoint(0);
      Vector2f normal = computeNormal(prev, curr, next);
      vertices[2 * count - 2].position = curr - normal * halfWidth;
      vertices[2 * count - 1].position = curr + normal * halfWidth;
      vertices[2 * count + 0].position = vertices[0].position;
      vertices[2 * count + 1].position = vertices[1].position;
    } else {
      Vector2f normal = normalize(perp(curr - prev));
      vertices[2 * count - 2].position = curr - normal * halfWidth;
      vertices[2 * count - 1].position = curr + normal * halfWidth;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
