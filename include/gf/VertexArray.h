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
#ifndef GF_VERTEX_ARRAY_H
#define GF_VERTEX_ARRAY_H

#include <cstddef>
#include <vector>

#include "Drawable.h"
#include "Portability.h"
#include "PrimitiveType.h"
#include "Rect.h"
#include "Vertex.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A set of primitives
   *
   * gf::VertexArray is a very simple wrapper around a dynamic
   * array of vertices and a primitive type.
   *
   * It inherits gf::Drawable, but unlike other drawables it
   * is not transformable.
   *
   * Example:
   *
   * ~~~{.cc}
   * gf::VertexArray lines(gf::PrimitiveType::LineStrip, 4);
   * lines[0].position = gf::Vector2f(10, 0);
   * lines[1].position = gf::Vector2f(20, 0);
   * lines[2].position = gf::Vector2f(30, 5);
   * lines[3].position = gf::Vector2f(40, 2);
   *
   * window.draw(lines);
   * ~~~
   *
   * @sa gf::Vertex
   */
  class GF_API VertexArray : public Drawable {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty vertex array. The default primitive type is
     * gf::PrimitiveType::Points.
     */
    VertexArray()
    : m_type(PrimitiveType::Points)
    {

    }

    /**
     * @brief Construct the vertex array with a type and an initial
     * number of vertices
     *
     * @param type Type of primitives
     * @param count Initial number of vertices in the array
     */
    VertexArray(PrimitiveType type, std::size_t count = 0)
    : m_type(type)
    , m_vertices(count)
    {

    }

    /**
     * @brief Return the vertex count
     *
     * @return Number of vertices in the array
     */
    std::size_t getVertexCount() const {
      return m_vertices.size();
    }

    /**
     * @brief Return the vertex data
     *
     * @return A pointer to the vertices in the array
     */
    const Vertex *getVertexData() const {
      return m_vertices.data();
    }

    /**
     * @brief Get a read-write access to a vertex by its index
     *
     * This function doesn't check `index`, it must be in range
     * @f$ [0, n - 1]@f$ where @f$ n @f$  is the vertex count. The
     * behavior is undefined otherwise.
     *
     * @param index Index of the vertex to get
     *
     * @return Reference to the index-th vertex
     *
     * @sa getVertexCount()
     */
    Vertex& operator[](std::size_t index) {
      return m_vertices[index];
    }

    /**
     * @brief Get a read-only access to a vertex by its index
     *
     * This function doesn't check `index`, it must be in range
     * @f$ [0, n - 1]@f$ where @f$ n @f$  is the vertex count. The
     * behavior is undefined otherwise.
     *
     * @param index Index of the vertex to get
     *
     * @return Const reference to the index-th vertex
     *
     * @sa getVertexCount()
     */
    const Vertex& operator[](std::size_t index) const {
      return m_vertices[index];
    }

    /**
     * @brief Check if the vertex array is empty
     *
     * @return True if the vertex array is empty
     */
    bool isEmpty() const {
      return m_vertices.empty();
    }

    /**
     * @brief Clear the vertex array
     *
     * This function removes all the vertices from the array.
     * It doesn't deallocate the corresponding memory, so that
     * adding new vertices after clearing doesn't involve
     * reallocating all the memory.
     */
    void clear() {
      m_vertices.clear();
    }

    /**
     * @brief Resize the vertex array
     *
     * If `count` is greater than the current size, the previous
     * vertices are kept and new (default-constructed) vertices are
     * added.
     *
     * If `count` is less than the current size, existing vertices
     * are removed from the array.
     *
     * @param count New size of the array (number of vertices)
     */
    void resize(std::size_t count) {
      m_vertices.resize(count);
    }

    /**
     * @brief Increase the capacity of the vertex array
     *
     * @param capacity New capacity of the array
     */
    void reserve(std::size_t capacity) {
      m_vertices.reserve(capacity);
    }

    /**
     * @brief Add a vertex to the array
     *
     * @param vertex The vertex to add
     */
    void append(const Vertex& vertex) {
      m_vertices.push_back(vertex);
    }

    /**
     * @brief Set the type of primitives to draw
     *
     * This function defines how the vertices must be interpreted
     * when it's time to draw them:
     *
     * - As points
     * - As lines
     * - As triangles
     *
     * The default primitive type is gf::PrimitiveType::Points.
     *
     * @param type Type of primitive
     */
    void setPrimitiveType(PrimitiveType type) {
      m_type = type;
    }

    /**
     * @brief Get the type of primitives drawn by the vertex array
     *
     * @return Primitive type
     */
    PrimitiveType getPrimitiveType() const {
      return m_type;
    }

    /**
     * @brief Compute the bounding rectangle of the vertex array
     *
     * This function returns the minimal axis-aligned rectangle
     * that contains all the vertices of the array.
     *
     * @return Bounding rectangle of the vertex array
     */
    RectF getBounds() const;

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    PrimitiveType m_type;
    std::vector<Vertex> m_vertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_VERTEX_ARRAY_H
