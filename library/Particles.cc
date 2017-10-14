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
#include <gf/Particles.h>

#include <gf/RenderTarget.h>
#include <gf/Texture.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  PointParticles::PointParticles()
  : m_vertices(PrimitiveType::Points)
  {

  }

  void PointParticles::addPoint(Vector2f position, Color4f color) {
    Vertex vertex;
    vertex.position = position;
    vertex.color = color;
    m_vertices.append(vertex);
  }

  void PointParticles::draw(RenderTarget& target, RenderStates states) {
    states.transform *= getTransform();
    target.draw(m_vertices, states);
  }

  ShapeParticles::ShapeParticles()
  : m_vertices(PrimitiveType::Triangles)
  {

  }

  void ShapeParticles::addCircle(Vector2f center, float radius, Color4f color, std::size_t pointCount) {
    Vector2f prev = { center.x + radius, center.y };

    Vertex vertices[3];
    vertices[0].position = center;
    vertices[0].color = vertices[1].color = vertices[2].color = color;

    for (std::size_t i = 1; i <= pointCount; ++i) {
      float angle = i * 2.0f * gf::Pi / pointCount;
      Vector2f curr = center + radius * gf::unit(angle);

      vertices[1].position = prev;
      vertices[2].position = curr;

      m_vertices.append(vertices[0]);
      m_vertices.append(vertices[1]);
      m_vertices.append(vertices[2]);

      prev = curr;
    }
  }

  void ShapeParticles::addCircle(const CircF& circ, Color4f color, std::size_t pointCount) {
    addCircle(circ.center, circ.radius, color, pointCount);
  }

  void ShapeParticles::addRectangle(Vector2f position, Vector2f size, Color4f color) {
    addRectangle(RectF(position, size), color);
  }

  void ShapeParticles::addRectangle(const RectF& rect, Color4f color) {
    Vertex vertices[4];

    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

    vertices[0].position = rect.getTopLeft();
    vertices[1].position = rect.getTopRight();
    vertices[2].position = rect.getBottomLeft();
    vertices[3].position = rect.getBottomRight();

    // first triangle
    m_vertices.append(vertices[0]);
    m_vertices.append(vertices[1]);
    m_vertices.append(vertices[2]);

    // second triangle
    m_vertices.append(vertices[2]);
    m_vertices.append(vertices[1]);
    m_vertices.append(vertices[3]);
  }

  void ShapeParticles::draw(RenderTarget& target, RenderStates states) {
    states.transform *= getTransform();
    target.draw(m_vertices, states);
  }

  SpriteParticles::SpriteParticles()
  : m_texture(nullptr)
  , m_vertices(PrimitiveType::Triangles)
  {

  }

  SpriteParticles::SpriteParticles(const Texture& texture)
  : m_texture(&texture)
  , m_vertices(PrimitiveType::Triangles)
  {

  }

  void SpriteParticles::setTexture(const Texture& texture) {
    m_texture = &texture;
  }

  void SpriteParticles::addSprite(Vector2f position, const RectF& textureRect, Color4f color) {
    if (m_texture == nullptr) {
      return;
    }

    Vector2f size = textureRect.size * m_texture->getSize();
    RectF box(position - size / 2, size);

    Vertex vertices[4];

    vertices[0].position = box.getTopLeft();
    vertices[1].position = box.getTopRight();
    vertices[2].position = box.getBottomLeft();
    vertices[3].position = box.getBottomRight();

    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

    vertices[0].texCoords = textureRect.getTopLeft();
    vertices[1].texCoords = textureRect.getTopRight();
    vertices[2].texCoords = textureRect.getBottomLeft();
    vertices[3].texCoords = textureRect.getBottomRight();

    m_vertices.append(vertices[0]);
    m_vertices.append(vertices[1]);
    m_vertices.append(vertices[2]);

    m_vertices.append(vertices[2]);
    m_vertices.append(vertices[1]);
    m_vertices.append(vertices[3]);
  }

  void SpriteParticles::draw(RenderTarget& target, RenderStates states) {
    if (m_texture == nullptr) {
      return;
    }

    states.transform *= getTransform();
    states.texture = m_texture;
    target.draw(m_vertices, states);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
