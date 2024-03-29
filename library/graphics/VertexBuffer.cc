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
#include <gf/VertexBuffer.h>

#include <algorithm>
#include <stdexcept>

#include <gf/Log.h>
#include <gf/Vertex.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void GraphicsTrait<GraphicsTag::Buffer>::gen(int n, unsigned* resources) {
    GL_CHECK(glGenBuffers(n, resources));
  }

  void GraphicsTrait<GraphicsTag::Buffer>::del(int n, const unsigned* resources) {
    GL_CHECK(glDeleteBuffers(n, resources));
  }

  VertexBuffer::VertexBuffer()
  : m_vbo(gf::None)
  , m_ebo(gf::None)
  , m_size(0)
  , m_count(0)
  , m_type(PrimitiveType::Points)
  {
  }


  VertexBuffer::VertexBuffer(const Vertex *vertices, std::size_t count, PrimitiveType type)
  : VertexBuffer(vertices, sizeof(Vertex), count, type)
  {
  }

  VertexBuffer::VertexBuffer(const Vertex *vertices, const uint16_t *indices, std::size_t count, PrimitiveType type)
  : VertexBuffer(vertices, sizeof(Vertex), indices, count, type)
  {
  }

  VertexBuffer::VertexBuffer(const void *vertices, std::size_t size, std::size_t count, PrimitiveType type)
  : m_ebo(gf::None)
  , m_size(size)
  , m_count(count)
  , m_type(type)
  {
    if (vertices == nullptr || count == 0) {
      Log::error("Could not create the buffer, invalid input.\n");
      throw std::runtime_error("Could not create the buffer, invalid input.");
    }

    std::size_t vboSize = count * size;

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STATIC_DRAW));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, vboSize, vertices));

    GLint vboUploadedSize = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboUploadedSize));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    if(vboSize != static_cast<std::size_t>(vboUploadedSize)) {
      Log::error("Vertex array buffer size in not correct.\n");
      throw std::runtime_error("Vertex array buffer size in not correct.");
    }
  }


  VertexBuffer::VertexBuffer(const void *vertices, std::size_t size, const uint16_t *indices, std::size_t count, PrimitiveType type)
  : m_size(size)
  , m_count(count)
  , m_type(type)
  {
    if (vertices == nullptr || indices == nullptr || count == 0) {
      Log::error("Could not create the buffer, invalid input.\n");
      throw std::runtime_error("Could not create the buffer, invalid input.");
    }

    uint16_t maxIndex = *std::max_element(indices, indices + count);
    std::size_t vboSize = (maxIndex + 1) * size;

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STATIC_DRAW));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, vboSize, vertices));

    GLint vboUploadedSize = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboUploadedSize));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    if(vboSize != static_cast<std::size_t>(vboUploadedSize)) {
      Log::error("Vertex array buffer size in not correct.\n");
      throw std::runtime_error("Vertex array buffer size in not correct.");
    }

    std::size_t eboSize = count * sizeof(uint16_t);

    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, nullptr, GL_STATIC_DRAW));
    GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, eboSize, indices));

    GLint eboUploadedSize = 0;
    GL_CHECK(glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &eboUploadedSize));

    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    if(eboSize != static_cast<std::size_t>(eboUploadedSize)) {
      Log::error("Vertex element array buffer size in not correct.\n");
      throw std::runtime_error("Vertex element array buffer size in not correct.");
    }
  }

  void VertexBuffer::bind(const VertexBuffer *buffer) {
    if (buffer != nullptr) {
      if (buffer->m_vbo.isValid()) {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer->m_vbo));
      }

      if (buffer->m_ebo.isValid()) {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->m_ebo));
      }
    } else {
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
      GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
