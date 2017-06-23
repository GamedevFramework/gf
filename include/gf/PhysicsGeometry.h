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
#ifndef GF_PHYSICS_GEOMETRY_H
#define GF_PHYSICS_GEOMETRY_H

#include "Circ.h"
#include "Polygon.h"
#include "Rect.h"
#include "Vector.h"
#include "Matrix.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;

  /**
   * @ingroup game
   * @brief The geometry of a physics body
   *
   * The geometry is defined in model coordinates.
   *
   * @sa gf::PhysicsBody
   */
  class GF_API PhysicsGeometry {
  public:
    /**
     * @brief The type of geometry
     */
    enum class Type {
      Circle, ///< A circle (see gf::CircleGeometry)
      Polygon, ///< A polygon (see gf::PolygonGeometry)
    };

    /**
     * @brief Constructor
     *
     * @param type The type of the geometry
     */
    PhysicsGeometry(Type type)
    : m_type(type)
    {

    }

    /**
     * @brief Destructor
     */
    virtual ~PhysicsGeometry();

    /**
     * @brief Get the type of the geometry
     */
    Type getType() const {
      return m_type;
    }

    /**
     * @brief Compute the area of the geometry
     *
     * @returns The area in world units
     */
    virtual float getArea() const = 0;

    /**
     * @brief Get a bouding circle
     *
     * The circle may not be the [minimum bouding circle](https://en.wikipedia.org/wiki/Smallest-circle_problem).
     *
     * @returns A bounding circle
     */
    virtual CircF getBoundingCircle() const = 0;

    /**
     * @brief Render the geometry
     *
     * @param target The render target
     * @param states The render states to use for drawing
     * @param position The position of the geometry
     * @param angle The angle of the geometry
     */
    virtual void renderAt(RenderTarget& target, const RenderStates& states, Vector2f position, float angle) const = 0;

  private:
    Type m_type;
  };

  /**
   * @brief A circle physics geometry
   */
  class GF_API CircleGeometry : public PhysicsGeometry {
  public:
    /**
     * @brief Constructor
     *
     * The geometry is centered in @f$ (0,0) @f$
     *
     * @param radius The radius of the circle
     */
    CircleGeometry(float radius);

    /**
     * @brief Constructor
     *
     * @param circle The base circle
     */
    CircleGeometry(CircF circle);

    /**
     * @brief Get the circle
     *
     * @returns The current circle
     */
    const CircF& get() const;

    virtual float getArea() const override;
    virtual CircF getBoundingCircle() const override;
    virtual void renderAt(RenderTarget& target, const RenderStates& states, Vector2f position, float angle) const override;

  private:
    CircF m_circle;
  };

  /**
   * @brief A polygon physics geometry
   *
   * This geometry includes rectangles.
   */
  class GF_API PolygonGeometry : public PhysicsGeometry {
  public:
    /**
     * @brief Constructor
     *
     * @param polygon The base polygon
     */
    PolygonGeometry(Polygon polygon);

    /**
     * @brief Constructor
     *
     * The geometry is centered in @f$ (0,0) @f$
     *
     * @param size The size of the rectangle
     */
    PolygonGeometry(Vector2f size);

    /**
     * @brief Constructor
     *
     * @param rectangle The base rectangle
     */
    PolygonGeometry(RectF rectangle);

    /**
     * @brief Get the polygon
     *
     * @returns The current polygon
     */
    const Polygon& get() const;

    virtual float getArea() const override;
    virtual CircF getBoundingCircle() const override;
    virtual void renderAt(RenderTarget& target, const RenderStates& states, Vector2f position, float angle) const override;

  private:
    void computeBoundingCircle();

  private:
    Polygon m_polygon;
    CircF m_boundingCircle;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PHYSICS_GEOMETRY_H
