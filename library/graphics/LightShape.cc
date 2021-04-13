/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#include <gf/LightShape.h>

#include <cassert>

#include <gf/Color.h>
#include <gf/RenderTarget.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  LightShape::LightShape(const Polygon& polygon, LightShapeVisibility visibility)
  : m_polygon(polygon)
  , m_color(Color::White)
  , m_visibility(visibility)
  , m_vertices(PrimitiveType::TriangleFan)
  {
    updateGeometry();
  }

  LightShape::LightShape(Polygon&& polygon, LightShapeVisibility visibility)
  : m_polygon(std::move(polygon))
  , m_color(Color::White)
  , m_visibility(visibility)
  , m_vertices(PrimitiveType::TriangleFan)
  {
    updateGeometry();
  }

  LightShape::LightShape(const RectF& rect, LightShapeVisibility visibility)
  : m_color(Color::White)
  , m_visibility(visibility)
  , m_vertices(PrimitiveType::TriangleFan)
  {
    m_polygon.addPoint(rect.getTopLeft());
    m_polygon.addPoint(rect.getBottomLeft());
    m_polygon.addPoint(rect.getBottomRight());
    m_polygon.addPoint(rect.getTopRight());
    updateGeometry();
  }

  LightShape::LightShape(const CircF& circ, LightShapeVisibility visibility)
  : m_color(Color::White)
  , m_visibility(visibility)
  , m_vertices(PrimitiveType::TriangleFan)
  {
    static constexpr int PointCount = 30;

    for (int i = 0; i < PointCount; ++i) {
      m_polygon.addPoint(circ.getCenter() + gf::unit(i * 2.0f * gf::Pi / PointCount) * circ.getRadius());
    }

    updateGeometry();
  }

  std::size_t LightShape::getPointCount() const {
    return m_polygon.getPointCount();
  }

  Vector2f LightShape::getPoint(std::size_t index) const {
    assert(index < m_polygon.getPointCount());
    return m_polygon.getPoint(index);
  }

  Vector2f LightShape::getPrevPoint(std::size_t index) const {
    assert(index < m_polygon.getPointCount());
    return m_polygon.getPrevPoint(index);
  }

  Vector2f LightShape::getNextPoint(std::size_t index) const {
    assert(index < m_polygon.getPointCount());
    return m_polygon.getNextPoint(index);
  }

  void LightShape::setColor(const Color4f& color) {
    m_color = color;
    updateColors();
  }

  Color4f LightShape::getColor() const {
    return m_color;
  }

  void LightShape::setVisibility(LightShapeVisibility visibility) {
    m_visibility = visibility;
  }

  LightShapeVisibility LightShape::getVisibility() const {
    return m_visibility;
  }

  RectF LightShape::getLocalBounds() const {
    return m_bounds;
  }

  void LightShape::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, m_bounds);
  }

  void LightShape::draw(RenderTarget& target, const RenderStates& states) {
    RenderStates localStates = states;
    localStates.transform *= getTransform();
    target.draw(m_vertices, localStates);
  }

  void LightShape::updateGeometry() {
    std::size_t count = getPointCount();
    assert(count >= 3);

    m_vertices.resize(count + 2);

    Vector2f min = gf::vec(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector2f max = gf::vec(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    for (std::size_t i = 0; i < count; ++i) {
      auto point = getPoint(i);
      m_vertices[i + 1].position = point;
      min = gf::min(min, point);
      max = gf::max(max, point);
    }

    m_vertices[count + 1].position = m_vertices[1].position;
    m_vertices[0].position = (min + max) / 2;

    m_bounds = m_vertices.getBounds();

    updateColors();
  }

  void LightShape::updateColors() {
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
      m_vertices[i].color = m_color;
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
