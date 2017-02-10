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
#ifndef GF_VERTEX_H
#define GF_VERTEX_H

#include "Color.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A point associated with a color and a texture coordinate
   *
   * gf::Vertex represents the association between a position in the world, a
   * color and texture coordinates.
   *
   * A vertex is the building block for drawing. Everything which is visible
   * on screen is made of vertices. They are grouped as 2D primitives
   * (triangles, etc), and these primitives are grouped to create even more
   * complex 2D entities such as sprites, shapes, etc.
   *
   * If you use the graphical entities of gf (sprite, shape), you won't have
   * to deal with vertices directly. But if you want to define your own 2D
   * entities, using vertices will allow you to get maximum performances.
   *
   * The texture coordinates are in the @f$ [0, 1] @f$ range. @f$ (0,0) @f$
   * is the top-left of the texture while @f$ (1,1) @f$ is the bottom right
   * of the texture. If a coordinate is outside the @f$ [0, 1] @f$ range, the
   * texture is clamped or repeated (see Texture::setRepeated).
   *
   * Example:
   *
   * ~~~{.cc}
   * // define a triangle
   * gf::Vertex triangle[3];
   * triangle[0].position = { 0.0f, 0.5f };
   * triangle[0].color = gf::Color::Red;
   * triangle[1].position = { 0.5f, -0.5f };
   * triangle[1].color = gf::Color::Green;
   * triangle[2].position = { -0.5f, -0.5f };
   * triangle[2].color = gf::Color::Yellow;
   *
   * // and draw it
   * renderer.draw(triangle, 3, gf::PrimitiveType::Triangles);
   * ~~~
   *
   * @sa gf::PrimitiveType, gf::RenderTarget::draw
   */
  struct GF_API Vertex {
    Vector2f position; ///< Position of the vertex in world coordinates
    Color4f color = Color::White; ///< %Color of the vertex (default: white)
    Vector2f texCoords = Vector2f{ 0.0f, 0.0f }; ///< Coordinates of the texture
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VERTEX_H
