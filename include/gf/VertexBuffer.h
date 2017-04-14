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
#ifndef GF_VERTEX_BUFFER_H
#define GF_VERTEX_BUFFER_H

#include <cstddef>
#include <cstdint>

#include "Portability.h"
#include "PrimitiveType.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct Vertex;

  /**
   * @ingroup graphics
   * @brief Data in the graphics memory
   *
   * A vertex buffer is a buffer that resides directly in the graphics memory.
   * The advantage is that the draw operations are faster than uploading data
   * each time. The drawback is that the data must not change, i.e. data can
   * be loaded only once.
   *
   * In gf, a vertex buffer can be used directly. But the main usage is for
   * drawable entities that can upload the final geometry and give the
   * corresponding vertex buffer.
   *
   * Usage:
   *
   * ~~~{.cc}
   * gf::Texture texture;
   *
   * gf::Sprite sprite;
   *
   * sprite.setTexture(texture);
   * sprite.setTextureRect({ 0.5f, 0.5f, 0.25f, 0.25f });
   * gf::VertexBuffer buffer = sprite.commitGeometry();
   *
   * gf::BufferedGeometry geometry;
   * geometry.setBuffer(buffer);
   * geometry.setTexture(texture);
   *
   * renderer.draw(geometry);
   * ~~~
   *
   * @sa gf::BufferedGeometry
   */
  class GF_API VertexBuffer {
  public:
    /**
     * @brief Default constructor
     */
    VertexBuffer();

    /**
     * @brief Destructor.
     */
    ~VertexBuffer();

    /**
     * @brief Deleted copy constructor
     */
    VertexBuffer(const VertexBuffer&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    /**
     * @brief Move constructor
     */
    VertexBuffer(VertexBuffer&& other);

    /**
     * @brief Move assignment
     */
    VertexBuffer& operator=(VertexBuffer&& other);

    /**
     * @brief Load an array of vertices
     *
     * @param vertices Pointer to the vertices
     * @param count Number of vertices in the array
     * @param type Type of primitives to draw
     */
    void load(const Vertex *vertices, std::size_t count, PrimitiveType type);

    /**
     * @brief Load an array of vertices and their indices
     *
     * @param vertices Pointer to the vertices
     * @param indices Pointer to the indices
     * @param count Number of indices in the array
     * @param type Type of primitives to draw
     */
    void load(const Vertex *vertices, const uint16_t *indices, std::size_t count, PrimitiveType type);

    /**
     * @brief Check if there is an array buffer
     *
     * @return True if an array buffer is defined
     */
    bool hasArrayBuffer() const {
      return m_vbo != 0;
    }

    /**
     * @brief Check if there is an element array buffer
     *
     * @return True if an array element buffer is defined
     */
    bool hasElementArrayBuffer() const {
      return m_ebo != 0;
    }

    /**
     * @brief Get the count of vertices or indices
     *
     * This function returns the `count` parameter given in
     * VertexBuffer::load().
     *
     * @return The count of vertices or indices
     * @sa load()
     */
    std::size_t getCount() const {
      return m_count;
    }

    /**
     * @brief Get the primitive type of the data in the buffer
     *
     * This function returns the `type` parameter given in
     * VertexBuffer::load().
     *
     * @return The primitive type
     * @sa load()
     */
    PrimitiveType getPrimitiveType() const {
      return m_type;
    }

    /**
     * @brief Binds a vertex buffer
     *
     * This function is for internal use only.
     *
     * @param buffer A pointer to a buffer or `nullptr`
     */
    static void bind(const VertexBuffer *buffer);

  private:
    unsigned m_vbo;
    unsigned m_ebo;
    std::size_t m_count;
    PrimitiveType m_type;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VERTEX_BUFFER_H
