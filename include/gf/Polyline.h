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
#ifndef GF_POLYLINE_H
#define GF_POLYLINE_H

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

  /**
   * @ingroup core_geometry
   * @brief A polyline
   *
   * A polyline is a set of consecutive segments. It is defined by the points
   * ending the segments. A polyline can be open (chain) or closed (loop).
   */
  class GF_CORE_API Polyline : public PointSequence {
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
    Polyline(Span<const Vector2f> points, Type type = Chain)
    : PointSequence(points)
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
    : PointSequence(first, last)
    , m_type(type)
    {

    }

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
     * @brief Get the previous extension point of the first point
     *
     * When the line is a chain, the line can be extended with an extension
     * point that is the symmetric of point at index 1 relative to the point
     * at index 0. It may be useful for the computation of splines.
     *
     * @return The previous extension point
     */
    Vector2f getPrevExtensionPoint() const;

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
     * @brief Get the next extension point of the last point
     *
     * When the line is a chain, the line can be extended with an extension
     * point that is the symmetric of point at index @f$ (n-2) @f$ relative to
     * the point at index @f$ (n-1) @f$. It may be useful for the computation
     * of splines.
     *
     * @return The next extension point
     */
    Vector2f getNextExtensionPoint() const;

    /**
     * @brief Compute the winding of a simple loop polyline
     *
     * Complexity: @f$ O(n) @f$
     *
     * @returns The winding of the simple loop polyline
     * @sa gf::Winding, isConvex()
     * @sa [Curve orientation - Wikipedia](https://en.wikipedia.org/wiki/Curve_orientation)
     */
    Winding getWinding() const;

    /**
     * @brief Test if a point is inside the polyline
     *
     * If the polyline is a chain, it returns false.
     *
     * @param point The point to test
     * @returns True if the point is inside the polyline
     */
    bool contains(Vector2f point) const;

    /**
     * @brief Set the type of the polyline
     *
     * @param type The new type of the polyline
     */
    void setType(Type type) {
      m_type = type;
    }

    /**
     * @brief Get the type of the polyline
     *
     * @returns The type of the polyline
     */
    Type getType() const {
      return m_type;
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
    Type m_type;
  };

  /**
   * @relates Serializer
   * @brief Serialize a polyline
   */
  GF_CORE_API Serializer& operator|(Serializer& ar, const Polyline& polyline);

  /**
   * @relates Deserializer
   * @brief Deserialize a polyline
   */
  GF_CORE_API Deserializer& operator|(Deserializer& ar, Polyline& polyline);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_POLYLINE_H
