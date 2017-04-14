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
#include <gf/Transformable.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Transformable::Transformable()
  : m_origin{0.0f, 0.0f}
  , m_position{0.0f, 0.0f}
  , m_rotation{0.0f}
  , m_scale{1.0f, 1.0f}
  {

  }

  void Transformable::setOrigin(Vector2f origin) {
    m_origin = origin;
  }

  void Transformable::setPosition(Vector2f position) {
    m_position = position;
  }

  void Transformable::move(Vector2f offset) {
    m_position += offset;
  }

  void Transformable::setRotation(float angle) {
    m_rotation = angle;
  }

  void Transformable::rotate(float angle) {
    m_rotation += angle;
  }

  void Transformable::setScale(Vector2f factors) {
    m_scale = factors;
  }

  void Transformable::scale(Vector2f factors) {
    m_scale *= factors;
  }

  Matrix3f Transformable::getTransform() const {
    float cos = std::cos(m_rotation);
    float sin = std::sin(m_rotation);
    float xx = m_scale.x * cos;
    float xy = - m_scale.y * sin;
    float yx = m_scale.x * sin;
    float yy = m_scale.y * cos;
    float xz = - m_origin.x * xx - m_origin.y * xy + m_position.x;
    float yz = - m_origin.x * yx - m_origin.y * yy + m_position.y;
    return Matrix3f(xx, xy, xz, yx, yy, yz, 0.0f, 0.0f, 1.0f);
//    return translation(m_position) * rotation(m_rotation) * scaling(m_scale) * translation(-m_origin);
  }

  Matrix3f Transformable::getInverseTransform() const {
    return invert(getTransform());
  }

  void Transformable::setOriginFromAnchorAndBounds(Anchor anchor, const RectF& bounds) {
    switch (anchor) {
      case Anchor::TopLeft:
        setOrigin(bounds.position);
        break;
      case Anchor::TopCenter:
        setOrigin(bounds.position + Vector2f(bounds.width / 2, 0.0f));
        break;
      case Anchor::TopRight:
        setOrigin(bounds.position + Vector2f(bounds.width, 0.0f));
        break;
      case Anchor::CenterLeft:
        setOrigin(bounds.position + Vector2f(0.0f, bounds.height / 2));
        break;
      case Anchor::Center:
        setOrigin(bounds.getCenter());
        break;
      case Anchor::CenterRight:
        setOrigin(bounds.position + Vector2f(bounds.width, bounds.height / 2));
        break;
      case Anchor::BottomLeft:
        setOrigin(bounds.position + Vector2f(0.0f, bounds.height));
        break;
      case Anchor::BottomCenter:
        setOrigin(bounds.position + Vector2f(bounds.width / 2, bounds.height));
        break;
      case Anchor::BottomRight:
        setOrigin(bounds.position + bounds.size);
        break;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
