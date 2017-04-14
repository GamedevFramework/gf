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
#include <gf/VertexBuffer.h>

#include <algorithm>

#include <glad/glad.h>

#include <gf/Log.h>
#include <gf/Vertex.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  VertexBuffer::VertexBuffer()
  : m_vbo(0)
  , m_ebo(0)
  , m_count(0)
  , m_type(PrimitiveType::Points)
  {

  }

  VertexBuffer::~VertexBuffer() {
    if (m_vbo != 0) {
      glCheck(glDeleteBuffers(1, &m_vbo));
    }

    if (m_ebo != 0) {
      glCheck(glDeleteBuffers(1, &m_ebo));
    }
  }

  VertexBuffer::VertexBuffer(VertexBuffer&& other)
  : m_vbo(other.m_vbo)
  , m_ebo(other.m_ebo)
  , m_count(other.m_count)
  , m_type(other.m_type)
  {
    other.m_vbo = other.m_ebo = 0;
  }

  VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    std::swap(m_vbo, other.m_vbo);
    std::swap(m_ebo, other.m_ebo);
    std::swap(m_count, other.m_count);
    std::swap(m_type, other.m_type);
    return *this;
  }

  void VertexBuffer::load(const Vertex *vertices, std::size_t count, PrimitiveType type) {
    if (vertices == nullptr || count == 0) {
      return;
    }

    if (m_vbo != 0 || m_ebo != 0) {
      Log::warning("Vertex buffer can not be loaded twice.\n");
      return;
    }

    std::size_t vboSize = count * sizeof(Vertex);

    glCheck(glGenBuffers(1, &m_vbo));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    glCheck(glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STATIC_DRAW));
    glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, vboSize, vertices));

    GLint vboUploadedSize = 0;
    glCheck(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboUploadedSize));

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

    if(vboSize != static_cast<std::size_t>(vboUploadedSize)) {
      Log::error("Vertex array buffer size in not correct.\n");
      glCheck(glDeleteBuffers(1, &m_vbo));
      m_vbo = 0;
      return;
    }

    m_count = count;
    m_type = type;
  }

  void VertexBuffer::load(const Vertex *vertices, const uint16_t *indices, std::size_t count, PrimitiveType type) {
    if (vertices == nullptr || indices == nullptr || count == 0) {
      return;
    }

    if (m_vbo != 0 || m_ebo != 0) {
      Log::warning("Vertex buffer can not be loaded twice.\n");
      return;
    }

    uint16_t maxIndex = *std::max_element(indices, indices + count);
    std::size_t vboSize = maxIndex * sizeof(Vertex);

    glCheck(glGenBuffers(1, &m_vbo));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    glCheck(glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STATIC_DRAW));
    glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, vboSize, vertices));

    GLint vboUploadedSize = 0;
    glCheck(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboUploadedSize));

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

    if(vboSize != static_cast<std::size_t>(vboUploadedSize)) {
      Log::error("Vertex array buffer size in not correct.\n");
      glCheck(glDeleteBuffers(1, &m_vbo));
      m_vbo = 0;
      return;
    }

    std::size_t eboSize = count * sizeof(uint16_t);

    glCheck(glGenBuffers(1, &m_ebo));
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, nullptr, GL_STATIC_DRAW));
    glCheck(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, eboSize, indices));

    GLint eboUploadedSize = 0;
    glCheck(glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &eboUploadedSize));

    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    if(eboSize != static_cast<std::size_t>(eboUploadedSize)) {
      Log::error("Vertex element array buffer size in not correct.\n");
      glCheck(glDeleteBuffers(1, &m_vbo));
      m_vbo = 0;
      glCheck(glDeleteBuffers(1, &m_ebo));
      m_ebo = 0;
      return;
    }

    m_count = count;
    m_type = type;
  }

  void VertexBuffer::bind(const VertexBuffer *buffer) {
    if (buffer != nullptr) {
      if (buffer->m_vbo != 0) {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, buffer->m_vbo));
      }

      if (buffer->m_ebo != 0) {
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->m_ebo));
      }
    } else {
      glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
      glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
