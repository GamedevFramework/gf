/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <gf/Polyline.h>

#include <cassert>

#include <gf/Geometry.h>
#include <gf/SerializationOps.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void Polyline::addPoint(Vector2f point) {
    m_points.push_back(point);
  }

  std::size_t Polyline::getPointCount() const {
    return m_points.size();
  }

  Vector2f Polyline::getPoint(std::size_t index) const {
    assert(index < m_points.size());
    return m_points[index];
  }

  const Vector2f *Polyline::begin() const {
    return &m_points[0];
  }

  const Vector2f *Polyline::end() const {
    return &m_points[0] + m_points.size();
  }

  bool Polyline::hasPrevPoint(std::size_t i) const {
    assert(i < m_points.size());
    return (m_type == Loop) || (i > 0);
  }

  Vector2f Polyline::getPrevPoint(std::size_t i) const {
    assert(i < m_points.size());

    if (i > 0) {
      return m_points[i - 1];
    }

    assert(m_type == Loop);
    return m_points.back();
  }

  bool Polyline::hasNextPoint(std::size_t i) const {
    assert(i < m_points.size());
    return (m_type == Loop) || (i < m_points.size() - 1);
  }

  Vector2f Polyline::getNextPoint(std::size_t i) const {
    assert(i < m_points.size());

    if (i < m_points.size() - 1) {
      return m_points[i + 1];
    }

    assert(m_type == Loop);
    return m_points.front();
  }

  void Polyline::simplify(float distance) {
    m_points = gf::simplifyPoints(m_points, distance);
  }


  Serializer& operator|(Serializer& ar, const Polyline& polyline) {
    ar | polyline.getType();

    uint64_t size = polyline.getPointCount();
    ar | size;

    for (uint64_t i = 0; i < size; ++i) {
      Vector2f point = polyline.getPoint(i);
      ar | point;
    }

    return ar;
  }

  Deserializer& operator|(Deserializer& ar, Polyline& polyline) {
    Polyline::Type type;
    ar | type;

    polyline = Polyline(type);

    uint64_t size;
    ar | size;

    for (uint64_t i = 0; i < size; ++i) {
      Vector2f point;
      ar | point;
      polyline.addPoint(point);
    }

    return ar;
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
