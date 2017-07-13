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
#ifndef GF_SHAPE_H
#define GF_SHAPE_H

#include "Portability.h"
#include "Transformable.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  /**
   * @ingroup graphics
   * @brief Base class for textured shapes with outline
   *
   * gf::Shape is a drawable class that allows to define and
   * display a custom convex shape on a render target.
   * It's only an abstract base, it needs to be specialized for
   * concrete types of shapes (circle, rectangle, convex polygon,
   * star, ...).
   *
   * In addition to the attributes provided by the specialized
   * shape classes, a shape always has the following attributes:
   *
   * - a texture
   * - a texture rectangle
   * - a fill color
   * - an outline color
   * - an outline thickness
   *
   * Each feature is optional, and can be disabled easily:
   *
   * - the texture can be null
   * - the fill/outline colors can be transparent
   * - the outline thickness can be zero
   *
   * You can write your own derived shape class, there are only
   * two virtual functions to override:
   *
   * - `getPointCount()` must return the number of points of the shape
   * - `getPoint()` must return the points of the shape
   *
   * @sa gf::RectangleShape, gf::CircleShape, gf::ConvexShape
   * @sa gf::Transformable
   */
  class GF_API Shape : public Transformable {
  public:
    /**
     * @brief Default constructor
     */
    Shape();

    /**
     * @brief Change the source texture of the shape
     *
     * The texture must exist as long as the shape uses it. Indeed, the
     * shape doesn't store its own copy of the texture, but rather keeps
     * a pointer to the one that you passed to this function.
     * If the source texture is destroyed and the shape tries to
     * use it, the behavior is undefined.
     *
     * If `resetRect` is true, the texture rect property of
     * the shape is automatically adjusted to the size of the new
     * texture. If it is false, the texture rect is left unchanged.
     *
     * @param texture New texture
     * @param resetRect Should the texture rect be reset to the size of the new texture?
     *
     * @sa getTexture(), setTextureRect()
     */
    void setTexture(const Texture& texture, bool resetRect = false);

    /**
     * @brief Get the source texture of the shape
     *
     * The returned reference is const, which means that you can't
     * modify the texture when you retrieve it with this function.
     *
     * @return Reference to the shape's texture
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
     * @brief Unset the source texture of the shape
     *
     * After a call to this function, the shape has no source texture.
     *
     * @sa setTexture()
     */
    void unsetTexture();

    /**
     * @brief Set the sub-rectangle of the texture that the shape will display
     *
     * The texture rect is useful when you don't want to display
     * the whole texture, but rather a part of it.
     *
     * By default, the texture rect covers the entire texture.
     *
     * @param rect Rectangle defining the region of the texture to display
     *
     * @sa getTextureRect(), setTexture()
     */
    void setTextureRect(const RectF& rect);

    /**
     * @brief Get the sub-rectangle of the texture displayed by the shape
     *
     * @return Texture rectangle of the shape
     *
     * @sa setTextureRect()
     */
    const RectF& getTextureRect() const {
      return m_textureRect;
    }

    /**
     * @brief Set the fill color of the shape
     *
     * This color is modulated (multiplied) with the shape's
     * texture if any. It can be used to colorize the shape,
     * or change its global opacity.
     *
     * You can use gf::Color::Transparent to make the inside of
     * the shape transparent, and have the outline alone.
     *
     * By default, the shape's fill color is opaque white.
     *
     * @param color New color of the shape
     *
     * @sa getColor(), setOutlineColor()
     */
    void setColor(const Color4f& color);

    /**
     * @brief Get the fill color of the shape
     *
     * @return Fill color of the shape
     *
     * @sa setColor()
     */
    const Color4f& getColor() const {
      return m_color;
    }

    /**
     * @brief Set the outline color of the shape
     *
     * By default, the shape's outline color is opaque white.
     *
     * @param color New outline color of the shape
     *
     * @sa getOutlineColor(), setColor()
     */
    void setOutlineColor(const Color4f& color);

    /**
     * @brief Get the outline color of the shape
     *
     * @return Outline color of the shape
     *
     * @sa setOutlineColor()
     */
    const Color4f& getOutlineColor() const {
      return m_outlineColor;
    }

    /**
     * @brief Set the thickness of the shape's outline
     *
     * By default, the outline thickness is @f$ 0 @f$.
     *
     * @param thickness New outline thickness
     *
     * @sa getOutlineThickness()
     */
    void setOutlineThickness(float thickness);

    /**
     * @brief Get the outline thickness of the shape
     *
     * @return Outline thickness of the shape
     * @sa setOutlineThickness()
     */
    float getOutlineThickness() const {
      return m_outlineThickness;
    }

    /**
     * @brief Get the total number of points of the shape
     *
     * @return Number of points of the shape
     * @sa getPoint()
     */
    virtual std::size_t getPointCount() const = 0;

    /**
     * @brief Get a point of the shape
     *
     * The returned point is in local coordinates, that is,
     * the shape's transforms (position, rotation, scale) are
     * not taken into account.
     *
     * The result is undefined if `index` is out of the valid range.
     *
     * @param index Index of the point to get, in range @f$ [0, n-1] @f$
     * where @f$ n @f$ is the number of points of the shape.
     *
     * @return index-th point of the shape
     * @sa getPointCount()
     */
    virtual Vector2f getPoint(std::size_t index) const = 0;

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

    /**
     * @brief Create a buffer with the current outline geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current outline geometry
     */
    VertexBuffer commitOutlineGeometry() const;

    virtual void draw(RenderTarget& target, RenderStates states) override;

  protected:
    /**
     * @brief Recompute the internal geometry of the shape
     *
     * This function must be called by the derived class everytime
     * the shape's points change (i.e. the result of either
     * getPointCount() or getPoint() is different).
     */
    void updateGeometry();

  private:
    void updateColors();
    void updateTexCoords();
    void updateOutline();
    void updateOutlineColors();

  private:
    const Texture *m_texture;
    RectF m_textureRect;
    Color4f m_color;
    VertexArray m_vertices;
    RectF m_bounds;

    Color4f m_outlineColor;
    float m_outlineThickness;
    VertexArray m_outlineVertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SHAPE_H
