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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_BASIC_SPRITE_H
#define GF_BASIC_SPRITE_H

#include "GraphicsApi.h"
#include "Rect.h"
#include "Span.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  class Texture;
  struct Vertex;

  /**
   * @ingroup graphics_sprite
   * @brief A basic sprite
   *
   * You should use gf::Sprite instead.
   *
   * @sa gf::Sprite
   */
  class GF_GRAPHICS_API BasicSprite {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty sprite with no source texture.
     */
    BasicSprite();

    /**
     * @brief Construct the sprite from a source texture
     *
     * @param texture Source texture
     *
     * @sa setTexture()
     */
    BasicSprite(const Texture& texture);

    /**
     * @brief Construct the sprite from a sub-rectangle of a source texture
     *
     * @param texture Source texture
     * @param textureRect Sub-rectangle of the texture to assign to the sprite
     *
     * @sa setTexture(), setTextureRect()
     */
    BasicSprite(const Texture& texture, const RectF& textureRect);

    /**
     * @brief Change the source texture of the sprite
     *
     * The texture must exist as long as the sprite uses it. Indeed, the
     * sprite doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the sprite tries to
     * use it, the behavior is undefined.
     *
     * If `resetRect` is true, the texture rect property of
     * the sprite is automatically adjusted to the size of the new
     * texture. If it is false, the texture rect is left unchanged.
     *
     * @param texture New texture
     * @param resetRect Should the texture rect be reset to the size of the new texture?
     *
     * @deprecated You should use setTexture(const Texture&, const RectF&) instead
     */
    [[deprecated("You should use setTexture(const Texture&, const RectF&) instead")]]
    void setTexture(const Texture& texture, bool resetRect);

    /**
     * @brief Change the source texture of the sprite
     *
     * The texture must exist as long as the sprite uses it. Indeed, the
     * sprite doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the sprite tries to
     * use it, the behavior is undefined.
     *
     * By default, the sub-rectangle will be reset to the full size of the new texture.
     *
     * @param texture New texture
     * @param textureRect Sub-rectangle of the new texture to assign to the sprite
     *
     * @sa getTexture(), setTextureRect()
     */
    void setTexture(const Texture& texture, const RectF& textureRect = RectF::fromSize({ 1.0f, 1.0f }));

    /**
     * @brief Get the source texture of the sprite
     *
     * The returned reference is const, which means that you can't
     * modify the texture when you retrieve it with this function.
     *
     * @return Reference to the sprite's texture
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
     * @brief Unset the source texture of the sprite
     *
     * After a call to this function, the sprite has no source texture.
     *
     * @sa setTexture()
     */
    void unsetTexture();

    /**
     * @brief Set the sub-rectangle of the texture that the sprite will display
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
     * @brief Get the sub-rectangle of the texture displayed by the sprite
     *
     * @return Texture rectangle of the sprite in texture coordinates
     *
     * @sa setTextureRect()
     */
    const RectF& getTextureRect() const {
      return m_textureRect;
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


    void updateGeometry(Span<Vertex> vertices);

  private:
    const Texture *m_texture;
    RectF m_textureRect;
    RectF m_bounds;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BASIC_SPRITE_H
