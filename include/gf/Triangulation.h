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
#ifndef GF_TRIANGULATION_H
#define GF_TRIANGULATION_H

#include <vector>

#include "CoreApi.h"
#include "Span.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_geometry
   * @brief A reference to an edge (two points)
   */
  template<typename T>
  class EdgeRef {
  public:
    /**
     * @brief Constructor with two points
     *
     * The address of the points must not change while the edge is alive.
     *
     * @param p0 The first point
     * @param p1 The second point
     */
    EdgeRef(const T& p0, const T& p1)
    : m_points{ &p0, &p1 }
    {

    }

    /**
     * @brief Access the points of the edge
     *
     * @param index The index of the point (@f$ 0 @f$ or @f$ 1 @f$)
     */
    const T& operator[](std::size_t index) const {
      return *m_points[index];
    }

  protected:
    const T *m_points[2];
  };

  /**
   * @ingroup core_geometry
   * @brief A reference to a triangle (three points)
   */
  template<typename T>
  class TriangleRef {
  public:
    /**
     * @brief Constructor with three points
     *
     * The address of the points must not change while the triangle is alive.
     *
     * @param p0 The first point
     * @param p1 The second point
     * @param p2 The third point
     */
    TriangleRef(T& p0, T& p1, T& p2)
    : m_points{ &p0, &p1, &p2 }
    {

    }

    /**
     * @brief Access the points of the triangle
     *
     * @param index The index of the point (@f$ 0 @f$ or @f$ 1 @f$ or @f$ 2 @f$)
     */
    const T& operator[](std::size_t index) const {
      return *m_points[index];
    }

    /**
     * @brief Access the points of the triangle
     *
     * @param index The index of the point (@f$ 0 @f$ or @f$ 1 @f$ or @f$ 2 @f$)
     */
    T& operator[](std::size_t index) {
      return *m_points[index];
    }

  protected:
    T *m_points[3];
  };

  /**
   * @ingroup core_geometry
   * @brief Compute a Delaunay triangulation of a set of points
   *
   * @param points The set of points
   * @returns A set of triangles refering to the original points
   */
  GF_CORE_API std::vector<TriangleRef<const Vector2f>> triangulation(Span<const Vector2f> points);

//   GF_CORE_API std::vector<TriangleRef<const Vector2f>> triangulationConstrained(Span<const Vector2f> points, Span<const EdgeRef<Vector2f>> contrainedEdges);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TRIANGULATION_H
