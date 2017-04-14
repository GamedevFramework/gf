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
#include <gf/BufferedGeometry.h>

#include <gf/Matrix.h>
#include <gf/RenderTarget.h>
#include <gf/Transformable.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  BufferedGeometry::BufferedGeometry()
  : m_buffer(nullptr)
  , m_outlineBuffer(nullptr)
  , m_texture(nullptr)
  , m_bounds(0, 0, 0, 0)
  {

  }

  BufferedGeometry::BufferedGeometry(const VertexBuffer& buffer)
  : m_buffer(&buffer)
  , m_outlineBuffer(nullptr)
  , m_texture(nullptr)
  , m_bounds(0, 0, 0, 0)
  {

  }

  BufferedGeometry::BufferedGeometry(const VertexBuffer& buffer, const VertexBuffer& outlineBuffer)
  : m_buffer(&buffer)
  , m_outlineBuffer(&outlineBuffer)
  , m_texture(nullptr)
  , m_bounds(0, 0, 0, 0)
  {

  }

  void BufferedGeometry::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, m_bounds);
  }

  void BufferedGeometry::draw(RenderTarget& target, RenderStates states) {
    states.texture = m_texture;
    states.transform *= getTransform();

    if (m_outlineBuffer != nullptr) {
      target.draw(*m_outlineBuffer, states);
    }

    if (m_buffer != nullptr) {
      target.draw(*m_buffer, states);
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
