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
#ifndef GF_PARTICLES_H
#define GF_PARTICLES_H

#include "Circ.h"
#include "Portability.h"
#include "Rect.h"
#include "Transformable.h"
#include "VertexArray.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  class Texture;

  /**
   * @ingroup graphics
   * @brief A class to display a high number of points
   */
  class GF_API PointParticles : public Transformable {
  public:
    /**
     * @brief Default constructor
     */
    PointParticles();

    /**
     * @brief Add a point to the particles
     *
     * @param position The position of the point
     * @param color The color of the point
     */
    void addPoint(Vector2f position, Color4f color);

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    VertexArray m_vertices;
  };


  /**
   * @ingroup graphics
   * @brief A class to display a high number of small simple shapes
   */
  class GF_API ShapeParticles : public Transformable {
  public:
    /**
     * @brief Default constructor
     */
    ShapeParticles();

    /**
     * @brief Add a circle shape to the particles
     *
     * @param center The center of the circle
     * @param radius The radius of the circle
     * @param color The color of the circle
     * @param pointCount The number of points composing the circle
     */
    void addCircle(Vector2f center, float radius, Color4f color, std::size_t pointCount = 15);

    /**
     * @brief Add a circle shape to the particles
     *
     * @param circ The circle
     * @param color The color of the circle
     * @param pointCount The number of points composing the circle
     */
    void addCircle(const CircF& circ, Color4f color, std::size_t pointCount = 15);

    /**
     * @brief Add a rectangle shape to the particles
     *
     * @param position The position of the top-left of the rectangle
     * @param size The size of the rectangle
     * @param color The color of the rectangle
     */
    void addRectangle(Vector2f position, Vector2f size, Color4f color);

    /**
     * @brief Add a rectangle shape to the particles
     *
     * @param rect The rectangle
     * @param color The color of the rectangle
     */
    void addRectangle(const RectF& rect, Color4f color);

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    VertexArray m_vertices;
  };

  /**
   * @ingroup graphics
   * @brief A class to display a high number of sprites
   *
   * @sa gf::SpriteBatch
   */
  class GF_API SpriteParticles : public Transformable {
  public:
    /**
     * @brief Default constructor
     */
    SpriteParticles();

    /**
     * @brief Constructor with a texture
     *
     * @param texture The source texture
     */
    SpriteParticles(const Texture& texture);

    /**
     * @brief Change the source texture
     *
     * @param texture The new texture
     */
    void setTexture(const Texture& texture);

    /**
     * @brief Add a sprite to the particles
     *
     * @param position The position of the center of the sprite
     * @param textureRect The sub-rectangle of the texture to assign to the sprite
     * @param color The color of the sprite
     */
    void addSprite(Vector2f position, const RectF& textureRect, Color4f color = Color::White);

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    const Texture *m_texture;
    VertexArray m_vertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PARTICLES_H
