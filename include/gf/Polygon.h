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
#ifndef GF_POLYGON_H
#define GF_POLYGON_H

#include <vector>

#include "ArrayRef.h"
#include "Matrix.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct Transform;

  /**
   * @ingroup core
   * @brief The direction of a polygon's rotation
   */
  enum class Winding {
    Clockwise,        ///< Same direction as a clock's hands
    Counterclockwise, ///< Opposite direction of a clock's hands
  };

  /**
   * @ingroup core
   * @brief A convex polygon
   *
   */
  class GF_API Polygon {
  public:
    /**
     * @brief Default constructor
     */
    Polygon() = default;

    /**
     * @brief Constructor from an array
     *
     * @param points The array of points
     */
    Polygon(ArrayRef<Vector2f> points);

    /**
     * @brief Constructor from points
     *
     * @param first Iterator to the first point
     * @param last Iterator after the last point
     */
    template<typename Iterator>
    Polygon(Iterator first, Iterator last)
    : m_points(first, last)
    {

    }

    /**
     * @brief Add a point to the polygon
     *
     * @param point The point to add to the polygon
     */
    void addPoint(Vector2f point);

    /**
     * @brief Get the number of points of the polygon
     *
     * @returns The number of points of the polygon
     */
    std::size_t getPointCount() const;

    /**
     * @brief Get the i-th point of the polygon
     *
     * @param index The index of the point
     */
    Vector2f getPoint(std::size_t index) const;

    /**
     * @brief Get the center of the polygon
     *
     * As the polygon is convex, the center is inside the polygon
     *
     * @returns The center of the polygon
     */
    Vector2f getCenter() const;

    /**
     * @brief Get the farthest point in a direction
     *
     * @param direction The direction to search (in world coordinates)
     * @param transform The transformation of the polygon
     * @returns The farthest point of the polygon in the given direction
     */
    Vector2f getSupport(Vector2f direction, const Transform& transform) const;

    /**
     * @brief Get the farthest point in a direction
     *
     * @param direction The direction to search
     * @returns The farthest point of the polygon in the given direction
     */
    Vector2f getSupport(Vector2f direction) const;

    /**
     * @brief Get an iterator to the first point
     *
     * @returns A pointer to the first point
     *
     * @sa end()
     */
    const Vector2f *begin() const;

    /**
     * @brief Get an iterator past the last point
     *
     * @returns A pointer past the last point
     *
     * @sa begin()
     */
    const Vector2f *end() const;

    /**
     * @brief Check if the polygon is convex
     *
     * This function should return true, otherwise you may have problems in
     * other functions which assume that the polygon is convex.
     *
     * @returns True if the polygon is convex
     */
    bool isConvex() const;

    /**
     * @brief Compute the winding of a convex polygon
     *
     * Complexity: @f$ O(n) @f$
     *
     * @returns The winding of the convex polygon
     * @sa gf::Winding, isConvex()
     * @sa [Curve orientation - Wikipedia](https://en.wikipedia.org/wiki/Curve_orientation)
     */
    Winding getWinding() const;


    /**
     * @brief Compute the area of the polygon
     *
     * Complexity: @f$ O(n) @f$
     *
     * @returns The area of the polygon
     */
    float getArea() const;

    /**
     * @brief Apply a transformation to the polygon
     *
     * @param mat The transformation matrix
     */
    void applyTransform(const Matrix3f& mat);

  private:
    std::vector<Vector2f> m_points;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_POLYGON_H
