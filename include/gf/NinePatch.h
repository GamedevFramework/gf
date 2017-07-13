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
#ifndef GF_NINE_PATCH_H
#define GF_NINE_PATCH_H

#include "Portability.h"
#include "Transformable.h"
#include "Vertex.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  /**
   * @ingroup graphics
   * @brief A nine-patch
   *
   * A nine-patch is a stretchable image that can be automatically resized. You
   * just have to indicate the position of the stretchable area and that's it.
   *
   * @sa [Android Developers: Draw 9-patch](http://developer.android.com/tools/help/draw9patch.html)
   * @sa [Android Developers: Nine-patch](http://developer.android.com/guide/topics/graphics/2d-graphics.html#nine-patch)
   */
  class GF_API NinePatch : public Transformable {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty nine-patch with no source texture
     */
    NinePatch();

    /**
     * @brief Construct the nine-patch from a source texture
     *
     * @param texture Source texture
     *
     * @sa setTexture()
     */
    NinePatch(const Texture& texture);

    /**
     * @brief Construct the nine-patch from a sub-rectangle of a source texture
     *
     * @param texture Source texture
     * @param textureRect Sub-rectangle of the texture to assign to the nine-patch
     *
     * @sa setTexture(), setTextureRect()
     */
    NinePatch(const Texture& texture, const RectF& textureRect);

    /**
     * @brief Change the source texture of the nine-patch
     *
     * The texture must exist as long as the nine-patch uses it. Indeed, the
     * nine-patch doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the nine-patch tries to
     * use it, the behavior is undefined.
     *
     * If `resetRect` is true, the texture rect property of
     * the nine-patch is automatically adjusted to the size of the new
     * texture. If it is false, the texture rect is left unchanged.
     *
     * @param texture New texture
     * @param resetRect Should the texture rect be reset to the size of the new texture?
     *
     * @sa getTexture(), setTextureRect()
     */
    void setTexture(const Texture& texture, bool resetRect = false);

    /**
     * @brief Get the source texture of the nine-patch
     *
     * The returned reference is const, which means that you can't
     * modify the texture when you retrieve it with this function.
     *
     * @return Reference to the nine-patch's texture
     *
     * @sa setTexture()
     */
    const Texture& getTexture() const {
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
     * @brief Unset the source texture of the nine-patch
     *
     * After a call to this function, the nine-patch has no source texture.
     *
     * @sa setTexture()
     */
    void unsetTexture();

    /**
     * @brief Set the sub-rectangle of the texture that the nine-patch will display
     *
     * The texture rect is useful when you don't want to display
     * the whole texture, but rather a part of it.
     * By default, the texture rect covers the entire texture.
     *
     * The rectangle is given in texture coordinates, meaning that
     * @f$(0,0)@f$ is the top left corner and @f$(1,1)@f$ is the
     * bottom right corner.
     *
     * @param rect Rectangle defining the region of the texture to display
     *
     * @sa getTextureRect(), setTexture()
     */
    void setTextureRect(const RectF& rect);

    /**
     * @brief Get the sub-rectangle of the texture displayed by the nine-patch
     *
     * @return Texture rectangle of the nine-patch in texture coordinates
     *
     * @sa setTextureRect()
     */
    const RectF& getTextureRect() const {
      return m_textureRect;
    }

    /**
     * @brief Set the global color of the nine-patch
     *
     * This color is modulated (multiplied) with the nine-patch's
     * texture. It can be used to colorize the nine-patch, or change
     * its global opacity.
     *
     * By default, the nine-patch's color is opaque white.
     *
     * @param color New color of the nine-patch
     *
     * @sa getColor()
     */
    void setColor(const Color4f& color);

    /**
     * @brief Get the global color of the nine-patch
     *
     * @return Global color of the nine-patch
     *
     * @sa setColor()
     */
    const Color4f& getColor() const;

    /**
     * @brief Set the limits of the stretchable area
     *
     * The limits are given in normalized coordinates
     *
     * @param top The top limit
     * @param bottom The bottom limit
     * @param left The left limit
     * @param right The right limit
     */
    void setLimits(float top, float bottom, float left, float right);

    /**
     * @brief Set the vertical limits of the stretchable area
     *
     * The limits are given in normalized coordinates
     *
     * @param top The top limit
     * @param bottom The bottom limit
     */
    void setVerticalLimits(float top, float bottom);

    /**
     * @brief Set the horizontal limits of the stretchable area
     *
     * The limits are given in normalized coordinates
     *
     * @param left The left limit
     * @param right The right limit
     */
    void setHorizontalLimits(float left, float right);

    /**
     * @brief Set the size of the stretched area
     *
     * @param size The size of the stretched area
     */
    void setSize(Vector2f size);

    /**
     * @brief Get the size of the stretched area
     *
     * @return The size of the stretched area
     */
    Vector2f getSize() const {
      return m_size;
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
    RectF getLocalBounds() const;

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

    /**
     * @brief Create a buffer with the current geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current geometry
     */
    VertexBuffer commitGeometry() const;

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    void updatePositions();
    void updateTexCoords();

  private:
    const Texture *m_texture;
    RectF m_textureRect;

    float m_top;
    float m_bottom;
    float m_left;
    float m_right;

    Vector2f m_size;

    Vertex m_vertices[16];
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_NINE_PATCH_H
