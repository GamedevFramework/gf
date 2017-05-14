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
#ifndef GF_POLYLINE_H
#define GF_POLYLINE_H

#include <vector>

#include "ArrayRef.h"
#include "Matrix.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A polyline
   *
   * A polyline is a set of consecutive segments. It is defined by the points
   * ending the segments. A polyline can be open (chain) or closed (loop).
   */
  class GF_API Polyline {
  public:
    /**
     * @brief The type of polyline
     */
    enum Type {
      Chain,  ///< The polyline is open
      Loop,   ///< The polyline is closed
    };

    /**
     * @brief Default constructor
     *
     * @param type The type of polyline (default: chain)
     */
    Polyline(Type type = Chain)
    : m_type(type)
    {

    }

    /**
     * @brief Constructor from an array
     *
     * @param points The array of points
     * @param type The type of polyline (default: chain)
     */
    Polyline(ArrayRef<Vector2f> points, Type type = Chain)
    : m_points(points.begin(), points.end())
    , m_type(type)
    {

    }

    /**
     * @brief Constructor from points
     *
     * @param first Iterator to the first point
     * @param last Iterator after the last point
     * @param type The type of polyline (default: chain)
     */
    template<typename Iterator>
    Polyline(Iterator first, Iterator last, Type type = Chain)
    : m_points(first, last)
    , m_type(type)
    {

    }

    /**
     * @brief Add a point to the polyline
     *
     * @param point The point to add to the polyline
     */
    void addPoint(Vector2f point);

    /**
     * @brief Get the number of points of the polyline
     *
     * @returns The number of points of the polyline
     */
    std::size_t getPointCount() const;

    /**
     * @brief Get the i-th point of the polyline
     *
     * @param index The index of the point
     */
    Vector2f getPoint(std::size_t index) const;

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
     * @brief Check if there is a point before the i-th point
     *
     * @param i The index of the current point
     * @returns True if there is a point before the current point
     *
     * @sa getPrevPoint()
     */
    bool hasPrevPoint(std::size_t i) const;

    /**
     * @brief Get the point before the i-th point
     *
     * @param i The index of the current point
     * @returns The point before the current point
     *
     * @sa hasPrevPoint()
     */
    Vector2f getPrevPoint(std::size_t i) const;

    /**
     * @brief Check if there is a point after the i-th point
     *
     * @param i The index of the current point
     * @returns True if there is a point after the current point
     *
     * @sa getNextPoint()
     */
    bool hasNextPoint(std::size_t i) const;

    /**
     * @brief Get the point after the i-th point
     *
     * @param i The index of the current point
     * @returns The point after the current point
     *
     * @sa hasNextPoint()
     */
    Vector2f getNextPoint(std::size_t i) const;

    /**
     * @brief Set the type of the polyline
     *
     * @param type The new type of the polyline
     */
    void setType(Type type) {
      m_type = type;
    }

    /**
     * @brief Check is the polyline is a loop
     *
     * @returns True if the polyline is a loop
     *
     * @sa isChain(), setType()
     */
    bool isLoop() const {
      return m_type == Loop;
    }

    /**
     * @brief Check is the polyline is a chain
     *
     * @returns True if the polyline is a chain
     *
     * @sa isLoop(), setType()
     */
    bool isChain() const {
      return m_type == Chain;
    }

  private:
    std::vector<Vector2f> m_points;
    Type m_type;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_POLYLINE_H
