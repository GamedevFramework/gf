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
#ifndef GF_BUFFERED_GEOMETRY_H
#define GF_BUFFERED_GEOMETRY_H

#include "Portability.h"
#include "Rect.h"
#include "Texture.h"
#include "Transformable.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;
  class VertexBuffer;

  /**
   * @ingroup graphics
   * @brief A drawable for buffers
   *
   * A buffered geometry is a lightweight object to draw vertex buffers. It
   * can handle two vertex buffers, one for the main object and one for the
   * outline when it exists. The outline is drawn first and the main object
   * second.
   *
   * In addition to the geometry, a buffered geometry can store a texture if
   * needed because a vertex buffer has no notion of texture.
   *
   * Finally, a buffered geometry can remember the local bounds of the
   * original object, so that it can be used for setting an anchor.
   *
   * @sa gf::VertexBuffer
   */
  class GF_API BufferedGeometry : public Transformable {
  public:
    /**
     * @brief Default constructor
     */
    BufferedGeometry();

    /**
     * @brief Constructor with a buffer
     *
     * @param buffer A vertex buffer
     */
    BufferedGeometry(const VertexBuffer& buffer);

    /**
     * @brief Constructor with a buffer and an outline buffer
     *
     * @param buffer A vertex buffer
     * @param outlineBuffer An outline buffer
     * @sa setBuffer(), setOutlineBuffer()
     */
    BufferedGeometry(const VertexBuffer& buffer, const VertexBuffer& outlineBuffer);

    /**
     * @brief Set the vertex buffer
     *
     * @param buffer A vertex buffer
     * @sa setOutlineBuffer()
     */
    void setBuffer(const VertexBuffer& buffer) {
      m_buffer = &buffer;
    }

    /**
     * @brief Set the outline buffer, if any
     *
     * @param outlineBuffer An outline buffer
     * @sa setBuffer()
     */
    void setOutlineBuffer(const VertexBuffer& outlineBuffer) {
      m_outlineBuffer = &outlineBuffer;
    }

    /**
     * @brief Change the source texture of the geometry
     *
     * The texture must exist as long as the geometry uses it. Indeed, the
     * geometry doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the geometry tries to
     * use it, the behavior is undefined.
     *
     * @param texture The texture
     *
     * @sa getTexture()
     */
    void setTexture(const BareTexture& texture) {
      m_texture = &texture;
    }

    /**
     * @brief Get the source texture of the geometry
     *
     * The returned reference is const, which means that you can't
     * modify the texture when you retrieve it with this function.
     *
     * @return Reference to the geometry's texture
     *
     * @sa setTexture()
     */
    const BareTexture& getTexture() const {
      return *m_texture;
    }

    /**
     * @brief Check if a texture is set
     *
     * @returns True if a texture is already set
     *
     * @sa setTexture(), getTexture()
     */
    bool hasTexture() const {
      return m_texture != nullptr;
    }

    /**
     * @brief Unset the source texture of the geometry
     *
     * After a call to this function, the geometry has no source texture.
     *
     * @sa setTexture()
     */
    void unsetTexture() {
      m_texture = nullptr;
    }

    /**
     * @brief Set the local bounds of the geometry
     *
     * There is no way to compute the bounds of the object with the vertex
     * buffer. So you have to set it directly with this function when you
     * create the geometry.
     *
     * @param bounds The local bounds of the object
     */
    void setLocalBounds(const RectF& bounds) {
      m_bounds = bounds;
    }

    /**
     * @brief Get the local bounding rectangle of the entity
     *
     * The returned rectangle is in local coordinates, which means
     * that it ignores the transformations (translation, rotation,
     * scale, ...) that are applied to the entity.
     * In other words, this function returns the bounds of the
     * entity in the entity's coordinate system.
     *
     * @return Local bounding rectangle of the entity
     */
    RectF getLocalBounds() const {
      return m_bounds;
    }

    /**
     * @brief Set the anchor origin of the entity
     *
     * Compute the origin of the entity based on the local bounds and
     * the specified anchor. Internally, this function calls
     * `Transformable::setOrigin()`.
     *
     * @param anchor The anchor of the entity
     * @sa getLocalBounds(), Transformable::setOrigin()
     */
    void setAnchor(Anchor anchor);

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    const VertexBuffer *m_buffer;
    const VertexBuffer *m_outlineBuffer;
    const BareTexture *m_texture;
    RectF m_bounds;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BUFFERED_GEOMETRY_H
