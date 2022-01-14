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
#include <gf/Polyline.h>

#include <cassert>

#include <gf/Geometry.h>
#include <gf/SerializationOps.h>
#include <gf/VectorOps.h>

#include <gfpriv/BasicGeometry.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  bool Polyline::hasPrevPoint(std::size_t i) const {
    assert(i < getPointCount());
    return (m_type == Loop) || (i > 0);
  }

  Vector2f Polyline::getPrevPoint(std::size_t i) const {
    assert(i < getPointCount());

    if (i > 0) {
      return getPoint(i - 1);
    }

    assert(m_type == Loop);
    return getLastPoint();
  }

  Vector2f Polyline::getPrevExtensionPoint() const {
    assert(getPointCount() >= 2);
    return 2 * getPoint(0) - getPoint(1); // == p_0 - (p_1 - p_0);
  }

  bool Polyline::hasNextPoint(std::size_t i) const {
    assert(i < getPointCount());
    return (m_type == Loop) || (i < getPointCount() - 1);
  }

  Vector2f Polyline::getNextPoint(std::size_t i) const {
    assert(i < getPointCount());

    if (i < getPointCount() - 1) {
      return getPoint(i + 1);
    }

    assert(m_type == Loop);
    return getFirstPoint();
  }

  Vector2f Polyline::getNextExtensionPoint() const {
    assert(getPointCount() >= 2);
    std::size_t sz = getPointCount();
    return 2 * getPoint(sz - 1) - getPoint(sz - 2); // = p_{n-1} - (p_{n-2} - p_{n-1})
  }

  Winding Polyline::getWinding() const {
    if (isChain()) {
      return Winding::None;
    }

    float area = gf::priv::computeSignedArea(getRawPoints());

    if (area > 0) {
      return Winding::Clockwise;
    }

    if (area < 0) {
      return Winding::Counterclockwise;
    }

    return Winding::None;
  }

  bool Polyline::contains(Vector2f point) const {
    if (isChain()) {
      return false;
    }

    return gf::priv::computeWindingNumber(point, getRawPoints()) != 0;
  }

  Serializer& operator|(Serializer& ar, const Polyline& polyline) {
    ar | polyline.getType();
    ar | static_cast<const PointSequence&>(polyline);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, Polyline& polyline) {
    Polyline::Type type;
    ar | type;

    polyline = Polyline(type);
    ar | static_cast<PointSequence&>(polyline);
    return ar;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
