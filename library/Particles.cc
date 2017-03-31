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

  void ShapeParticles::addCircle(Vector2f position, float radius, Color4f color, std::size_t pointCount) {
    Vector2f prev = { position.x + radius, position.y };

    Vertex vertices[3];
    vertices[0].position = position;
    vertices[0].color = vertices[1].color = vertices[2].color = color;

    for (std::size_t i = 1; i <= pointCount; ++i) {
      float angle = i * 2.0f * gf::Pi / pointCount;
      Vector2f curr = position + radius * gf::unit(angle);

      vertices[1].position = prev;
      vertices[2].position = curr;

      m_vertices.append(vertices[0]);
      m_vertices.append(vertices[1]);
      m_vertices.append(vertices[2]);

      prev = curr;
    }
  }

  void ShapeParticles::draw(RenderTarget& target, RenderStates states) {
    states.transform *= getTransform();
    target.draw(m_vertices, states);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
