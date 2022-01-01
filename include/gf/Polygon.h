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
 */
#ifndef GF_POLYGON_H
#define GF_POLYGON_H

#include <vector>

#include "CoreApi.h"
#include "Math.h"
#include "Matrix.h"
#include "PointSequence.h"
#include "SerializationFwd.h"
#include "Span.h"
#include "Vector.h"
#include "Winding.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct Transform;

  /**
   * @ingroup core_geometry
   * @brief A convex polygon
   *
   */
  class GF_CORE_API Polygon : public PointSequence {
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
    Polygon(Span<const Vector2f> points)
    : PointSequence(points)
    {
    }

    /**
     * @brief Constructor from points
     *
     * @param first Iterator to the first point
     * @param last Iterator after the last point
     */
    template<typename Iterator>
    Polygon(Iterator first, Iterator last)
    : PointSequence(first, last)
    {
    }

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
     * @brief Check if the polygon is convex
     *
     * This function should return true, otherwise you may have problems in
     * other functions which assume that the polygon is convex.
     *
     * This function does not handle self-intersecting polygons (yet).
     *
     * @returns True if the polygon is convex
     */
    bool isConvex() const;

    /**
     * @brief Compute the winding of a simple polygon
     *
     * Complexity: @f$ O(n) @f$
     *
     * @returns The winding of the simple polygon
     * @sa gf::Winding
     * @sa [Curve orientation - Wikipedia](https://en.wikipedia.org/wiki/Curve_orientation)
     */
    Winding getWinding() const;

    /**
     * @brief Test if a point is inside the polygon
     *
     * @param point The point to test
     * @returns True if the point is inside the polygon
     */
    bool contains(Vector2f point) const;

    /**
     * @brief Compute the area of the polygon
     *
     * Complexity: @f$ O(n) @f$
     *
     * @returns The area of the polygon
     */
    float getArea() const;

    /**
     * @brief Get the point before the i-th point
     *
     * @param i The index of the current point
     * @returns The point before the current point
     */
    Vector2f getPrevPoint(std::size_t i) const;

    /**
     * @brief Get the point after the i-th point
     *
     * @param i The index of the current point
     * @returns The point after the current point
     */
    Vector2f getNextPoint(std::size_t i) const;
  };

  /**
   * @relates Serializer
   * @brief Serialize a polygon
   */
  inline
  Serializer& operator|(Serializer& ar, const Polygon& polygon) {
    return ar | static_cast<const PointSequence&>(polygon);
  }

  /**
   * @relates Deserializer
   * @brief Deserialize a polygon
   */
  inline
  Deserializer& operator|(Deserializer& ar, Polygon& polygon) {
    return ar | static_cast<PointSequence&>(polygon);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_POLYGON_H
