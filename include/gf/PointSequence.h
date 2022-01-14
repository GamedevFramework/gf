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
#ifndef GF_POINT_SEQUENCE_H
#define GF_POINT_SEQUENCE_H

#include <vector>

#include "CoreApi.h"
#include "Math.h"
#include "Matrix.h"
#include "SerializationFwd.h"
#include "Span.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_geometry
   * @brief A sequence of points
   *
   * @sa gf::Polygon, gf::Polyline
   */
  class GF_CORE_API PointSequence {
  public:
    /**
     * @brief Default constructor
     */
    PointSequence() = default;

    /**
     * @brief Constructor from an array
     *
     * @param points The array of points
     */
    PointSequence(Span<const Vector2f> points);

    /**
     * @brief Constructor from points
     *
     * @param first Iterator to the first point
     * @param last Iterator after the last point
     */
    template<typename Iterator>
    PointSequence(Iterator first, Iterator last)
    : m_points(first, last)
    {
    }

    /**
     * @brief Check if the sequence is empty
     *
     * An empty sequence has no points.
     *
     * @returns True if the sequence is empty
     */
    bool isEmpty() const {
      return m_points.empty();
    }

    /**
     * @brief Add a point to the sequence
     *
     * @param point The point to add to the sequence
     */
    void addPoint(Vector2f point) {
      m_points.push_back(point);
    }

    /**
     * @brief Get the number of points of the sequence
     *
     * @returns The number of points of the sequence
     */
    std::size_t getPointCount() const {
      return m_points.size();
    }

    /**
     * @brief Get the i-th point of the sequence
     *
     * @param index The index of the point
     */
    Vector2f getPoint(std::size_t index) const;

    /**
     * @brief Get the center of the sequence
     *
     * As the sequence is convex, the center is inside the sequence
     *
     * @returns The center of the sequence
     */
    Vector2f getCenter() const;

    /**
     * @brief Get an iterator to the first point
     *
     * @returns A pointer to the first point
     *
     * @sa end()
     */
    const Vector2f *begin() const {
      return m_points.data();
    }

    /**
     * @brief Get an iterator to the first point
     *
     * @returns A pointer to the first point
     *
     * @sa end()
     */
    Vector2f *begin() {
      return m_points.data();
    }

    /**
     * @brief Get an iterator past the last point
     *
     * @returns A pointer past the last point
     *
     * @sa begin()
     */
    const Vector2f *end() const {
      return m_points.data() + m_points.size();
    }

    /**
     * @brief Get an iterator past the last point
     *
     * @returns A pointer past the last point
     *
     * @sa begin()
     */
    Vector2f *end() {
      return m_points.data() + m_points.size();
    }

    /**
     * @brief Get the first point of the sequence
     *
     * @returns The first point of the sequence
     */
    Vector2f getFirstPoint() const {
      return m_points.front();
    }

    /**
     * @brief Get the last point of the sequence
     *
     * @returns The last point of the sequence
     */
    Vector2f getLastPoint() const {
      return m_points.back();
    }

    /**
     * @brief Apply a transformation to the sequence
     *
     * @param mat The transformation matrix
     */
    void applyTransform(const Matrix3f& mat);

    /**
     * @brief Simplify the sequence
     *
     * @param distance The maximum authorized distance between the original points and the simplified points
     */
    void simplify(float distance = Epsilon);

    /**
     * @brief Reverse the points in the sequence
     */
    void reverse();

  protected:
    /**
     * @brief Get the raw container of points
     */
    std::vector<Vector2f>& getRawPoints() {
      return m_points;
    }

    /**
     * @brief Get the raw container of points
     */
    const std::vector<Vector2f>& getRawPoints() const {
      return m_points;
    }

  private:
    std::vector<Vector2f> m_points;
  };

  /**
   * @relates Serializer
   * @brief Serialize a sequence
   */
  GF_CORE_API Serializer& operator|(Serializer& ar, const PointSequence& sequence);

  /**
   * @relates Deserializer
   * @brief Deserialize a sequence
   */
  GF_CORE_API Deserializer& operator|(Deserializer& ar, PointSequence& sequence);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_POINT_SEQUENCE_H
