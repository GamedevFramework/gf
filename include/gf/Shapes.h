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
#ifndef GF_SHAPES_H
#define GF_SHAPES_H

#include <cstddef>
#include <vector>

#include "Circ.h"
#include "Polygon.h"
#include "Portability.h"
#include "Rect.h"
#include "Shape.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Specialized shape representing a rectangle
   *
   * This class inherits all the functions of gf::Transformable
   * (position, rotation, scale, bounds, ...) as well as the
   * functions of gf::Shape (outline, color, texture, ...).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::RectangleShape rectangle;
   * rectangle.setSize({ 100.0f, 50.0f });
   * rectangle.setOutlineColor(gf::Color::Red);
   * rectangle.setOutlineThickness(5);
   * rectangle.setPosition({ 10.0f, 20.0f });
   * ...
   * window.draw(rectangle);
   * ~~~
   *
   * @sa gf::Shape
   */
  class GF_API RectangleShape : public Shape {
  public:
    /**
     * @brief Default constructor
     *
     * @param size Size of the rectangle
     */
    RectangleShape(Vector2f size = Vector2f{ 0.0f, 0.0f });

    /**
     * @brief Constructor with a rectangle
     *
     * The shape takes the size of the rectangle and the position of the
     * rectangle.
     *
     * @param rect A rectangle with a size and a position
     */
    explicit RectangleShape(const RectF& rect);

    /**
     * @brief Set the size of the rectangle
     *
     * @param size New size of the rectangle
     *
     * @sa getSize()
     */
    void setSize(Vector2f size);

    /**
     * @brief Get the size of the rectangle
     *
     * @return Size of the rectangle
     *
     * @sa setSize()
     */
    Vector2f getSize() const {
      return m_size;
    }

    virtual std::size_t getPointCount() const override;
    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    Vector2f m_size;
  };

  /**
   * @ingroup graphics
   * @brief Specialized shape representing a circle
   *
   * This class inherits all the functions of gf::Transformable
   * (position, rotation, scale, bounds, ...) as well as the
   * functions of gf::Shape (outline, color, texture, ...).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::CircleShape circle;
   * circle.setRadius(150);
   * circle.setOutlineColor(gf::Color::Red);
   * circle.setOutlineThickness(5);
   * circle.setPosition({ 10.0f, 20.0f });
   * ...
   * window.draw(circle);
   * ~~~
   *
   * Since the graphics card can't draw perfect circles, we have to
   * fake them with multiple triangles connected to each other. The
   * "points count" property of gf::CircleShape defines how many of these
   * triangles to use, and therefore defines the quality of the circle.
   *
   * The number of points can also be used for another purpose; with
   * small numbers you can create any regular polygon shape:
   * equilateral triangle, square, pentagon, hexagon, ...
   *
   * @sa gf::Shape
   */
  class GF_API CircleShape : public Shape {
  public:
    /**
     * @brief Default constructor
     *
     * @param radius Radius of the circle
     * @param pointCount Number of points composing the circle
     */
    CircleShape(float radius = 0, std::size_t pointCount = 30);

    /**
     * @brief Constructor with a circle
     *
     * The shape takes the radius of the circle and the position of the
     * circle.
     *
     * @param circ A circle with a radius and a position
     * @param pointCount Number of points composing the circle
     */
    explicit CircleShape(const CircF& circ, std::size_t pointCount = 30);

    /**
     * @brief Set the radius of the circle
     *
     * @param radius New radius of the circle
     *
     * @sa getRadius()
     */
    void setRadius(float radius);

    /**
     * @brief Get the radius of the circle
     *
     * @return Radius of the circle
     *
     * @sa setRadius()
     */
    float getRadius() const {
      return m_radius;
    }

    /**
     * @brief Set the number of points of the circle
     *
     * @param pointCount New number of points of the circle
     *
     * @sa getPointCount()
     */
    void setPointCount(std::size_t pointCount);

    virtual std::size_t getPointCount() const override;
    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    float m_radius;
    std::size_t m_pointCount;
  };

  /**
   * @ingroup graphics
   * @brief Specialized shape representing a convex polygon
   *
   * This class inherits all the functions of gf::Transformable
   * (position, rotation, scale, bounds, ...) as well as the
   * functions of gf::Shape (outline, color, texture, ...).
   *
   * It is important to keep in mind that a convex shape must
   * always be... convex, otherwise it may not be drawn correctly.
   * Moreover, the points must be defined in order; using a random
   * order would result in an incorrect shape.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::ConvexShape polygon;
   * polygon.setPointCount(3);
   * polygon.setPoint(0, {  0.0f,  0.0f });
   * polygon.setPoint(1, {  0.0f, 10.0f });
   * polygon.setPoint(2, { 25.0f,  5.0f });
   * polygon.setOutlineColor(gf::Color::Red);
   * polygon.setOutlineThickness(5);
   * polygon.setPosition({ 10.0f, 20.0f });
   * ...
   * window.draw(polygon);
   * ~~~
   *
   * @sa gf::Shape
   */
  class GF_API ConvexShape : public Shape {
  public:
    /**
     * @brief Default constructor
     *
     * @param pointCount Number of points of the polygon
     */
    ConvexShape(std::size_t pointCount);

    /**
     * @brief Constructor with a polygon
     *
     * @param polygon The polygon
     */
    ConvexShape(const Polygon& polygon);

    /**
     * @brief Set the number of points of the polygon
     *
     * `pointCount` must be greater than 2 to define a valid shape.
     *
     * @param pointCount New number of points of the polygon
     *
     * @sa getPointCount()
     */
    void setPointCount(std::size_t pointCount);

    /**
     * @brief Get the number of points of the polygon
     *
     * @return Number of points of the polygon
     *
     * @sa setPointCount(), getPoint()
     */
    void setPoint(std::size_t index, Vector2f point);

    virtual std::size_t getPointCount() const override;
    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    std::vector<Vector2f> m_points;
  };


  /**
   * @ingroup graphics
   * @brief Specialized shape representing a star
   *
   * This class inherits all the functions of gf::Transformable
   * (position, rotation, scale, bounds, ...) as well as the
   * functions of gf::Shape (outline, color, texture, ...).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::StarShape star;
   * star.setMinRadius(25.0f);
   * star.setMaxRadius(50.0f);
   * star.setBranches(5);
   * star.setOutlineColor(gf::Color::Red);
   * star.setOutlineThickness(5);
   * star.setPosition({ 10.0f, 20.0f });
   * ...
   * window.draw(star);
   * ~~~
   *
   * @sa gf::Shape
   */
  class GF_API StarShape : public Shape {
  public:
    /**
     * @brief Default constructor
     *
     * @param minRadius Minimum radius
     * @param maxRadius Maximum radius
     * @param branches Number of branches of the star
     */
    StarShape(float minRadius = 0, float maxRadius = 0, std::size_t branches = 7);

    /**
     * @brief Set the minimum radius
     *
     * @param minRadius Minimum radius
     * @sa getMinRadius()
     */
    void setMinRadius(float minRadius);

    /**
     * @brief Get the minimum radius
     *
     * @return The minimum radius
     * @sa setMinRadius()
     */
    float getMinRadius() const {
      return m_minRadius;
    }

    /**
     * @brief Set the maximum radius
     *
     * @param maxRadius Maximum radius
     * @sa getMaxRadius()
     */
    void setMaxRadius(float maxRadius);

    /**
     * @brief Get the maximum radius
     *
     * @return The maximum radius
     * @sa setMaxRadius()
     */
    float getMaxRadius() const {
      return m_maxRadius;
    }

    /**
     * @brief Set the number of branches
     *
     * @param branches Number of branches
     * @sa getBranches()
     */
    void setBranches(std::size_t branches);

    /**
     * @brief Get the number of branches
     *
     * @return The number of branches
     * @sa setBranches()
     */
    std::size_t getBranches() const {
      return m_branches;
    }

    virtual std::size_t getPointCount() const override;
    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    float m_minRadius;
    float m_maxRadius;
    std::size_t m_branches;
  };


  /**
   * @ingroup graphics
   * @brief Specialized shape representing a rounded rectangle
   *
   * This class inherits all the functions of gf::Transformable
   * (position, rotation, scale, bounds, ...) as well as the
   * functions of gf::Shape (outline, color, texture, ...).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * gf::RoundedRectangleShape rectangle;
   * rectangle.setSize({ 100.0f, 50.0f });
   * rectangle.setRadius(10);
   * rectangle.setOutlineColor(gf::Color::Red);
   * rectangle.setOutlineThickness(5);
   * rectangle.setPosition({ 10.0f, 20.0f });
   * ...
   * window.draw(rect);
   * ~~~
   *
   * @sa gf::Shape
   */
  class GF_API RoundedRectangleShape : public Shape {
  public:
    /**
     * @brief Default constructor
     *
     * @param size Size of the rectangle
     * @param radius Radius of the corner
     * @param cornerPointCount Number of points in the corner
     */
    RoundedRectangleShape(Vector2f size = Vector2f{ 0.0f, 0.0f }, float radius = 0.0f, std::size_t cornerPointCount = 8);

    /**
     * @brief Constructor with a rectangle
     *
     * The shape takes the size of the rectangle and the position of the
     * rectangle.
     *
     * @param rect A rectangle with a size and a position
     * @param radius Radius of the corner
     * @param cornerPointCount Number of points in the corner
     */
    explicit RoundedRectangleShape(const RectF& rect, float radius = 0.0f, std::size_t cornerPointCount = 8);

    /**
     * @brief Set the size of the rectangle
     *
     * @param size New size of the rectangle
     *
     * @sa getSize()
     */
    void setSize(Vector2f size);

    /**
     * @brief Get the size of the rectangle
     *
     * @return Size of the rectangle
     *
     * @sa setSize()
     */
    Vector2f getSize() const {
      return m_size;
    }

    /**
     * @brief Set the radius of the corner
     *
     * @param radius New radius of the corner
     *
     * @sa getRadius()
     */
    void setRadius(float radius);

    /**
     * @brief Get the radius of the corner
     *
     * @return Radius of the corner
     *
     * @sa setRadius()
     */
    float getRadius() const {
      return m_radius;
    }

    /**
     * @brief Set the number of points of a corner
     *
     * `cornerPointCount` must be greater than 2 to define a valid shape.
     *
     * @param cornerPointCount New number of points of a corner
     */
    void setCornerPointCount(std::size_t cornerPointCount);

    virtual std::size_t getPointCount() const override;
    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    Vector2f m_size;
    float m_radius;
    std::size_t m_cornerPointCount;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SHAPES_H
