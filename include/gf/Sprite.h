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
#ifndef GF_SPRITE_H
#define GF_SPRITE_H

#include "Transformable.h"
#include "Portability.h"
#include "Vertex.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  class Texture;

  /**
   * @ingroup graphics
   * @brief A drawable representation of a texture, with its
   * own transformations, color, etc.
   *
   * gf::Sprite is a drawable class that allows to easily display
   * a texture (or a part of it) on a render target.
   *
   * It inherits all the functions from gf::Transformable:
   * position, rotation, scale, origin. It also adds sprite-specific
   * properties such as the texture to use, the part of it to display,
   * and some convenience functions to change the overall color of the
   * sprite, or to get its bounding rectangle.
   *
   * gf::Sprite works in combination with the gf::Texture class, which
   * loads and provides the pixel data of a given texture.
   *
   * The separation of gf::Sprite and gf::Texture allows more flexibility
   * and better performances: indeed a gf::Texture is a heavy resource,
   * and any operation on it is slow (often too slow for real-time
   * applications). On the other side, a gf::Sprite is a lightweight
   * object which can use the pixel data of a gf::Texture and draw
   * it with its own transformation/color/blending attributes.
   *
   * It is important to note that the gf::Sprite instance doesn't
   * copy the texture that it uses, it only keeps a reference to it.
   * Thus, a gf::Texture must not be destroyed while it is
   * used by a gf::Sprite (i.e. never write a function that
   * uses a local gf::Texture instance for creating a sprite).
   *
   * Usage example:
   * ~~~{.cc}
   * // Declare and load a texture
   * gf::Texture texture;
   * texture.loadFromFile("texture.png");
   *
   * // Create a sprite
   * gf::Sprite sprite;
   * sprite.setTexture(texture);
   * sprite.setTextureRect({ 0.1f, 0.1f, 0.5f, 0.3f });
   * sprite.setColor({ 1.0f, 1.0f, 1.0f, 0.8f });
   * sprite.setPosition({ 100.0f, 25.0f });
   *
   * // Draw it
   * renderer.draw(sprite);
   * ~~~
   *
   * @sa gf::Texture, gf::Transformable, gf::SpriteBatch
   */
  class GF_API Sprite : public Transformable {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty sprite with no source texture.
     */
    Sprite();

    /**
     * @brief Construct the sprite from a source texture
     *
     * @param texture Source texture
     *
     * @sa setTexture()
     */
    Sprite(const Texture& texture);

    /**
     * @brief Construct the sprite from a sub-rectangle of a source texture
     *
     * @param texture Source texture
     * @param textureRect Sub-rectangle of the texture to assign to the sprite
     *
     * @sa setTexture(), setTextureRect()
     */
    Sprite(const Texture& texture, const RectF& textureRect);

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
     * @sa getTexture(), setTextureRect()
     */
    void setTexture(const Texture& texture, bool resetRect = false);

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
     * @brief Set the global color of the sprite
     *
     * This color is modulated (multiplied) with the sprite's
     * texture. It can be used to colorize the sprite, or change
     * its global opacity.
     *
     * By default, the sprite's color is opaque white.
     *
     * @param color New color of the sprite
     *
     * @sa getColor()
     */
    void setColor(const Color4f& color);

    /**
     * @brief Get the global color of the sprite
     *
     * @return Global color of the sprite
     *
     * @sa setColor()
     */
    const Color4f& getColor() const;


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
    Vertex m_vertices[4];
    RectF m_bounds;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPRITE_H
