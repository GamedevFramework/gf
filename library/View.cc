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
#include <gf/View.h>

#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include "priv/Utils.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  View::View()
  : m_center{0.0f, 0.0f}
  , m_size{1000.0f, 1000.0f}
  , m_rotation(0)
  , m_viewport{0.0f, 0.0f, 1.0f, 1.0f}
  {

  }

  View::View(const RectF& rect)
  : m_center(rect.getCenter())
  , m_size(rect.getSize())
  , m_rotation(0)
  , m_viewport{0.0f, 0.0f, 1.0f, 1.0f}
  {

  }

  View::View(Vector2f center, Vector2f size)
  : m_center(center)
  , m_size(size)
  , m_rotation(0)
  , m_viewport{0.0f, 0.0f, 1.0f, 1.0f}
  {

  }

  View::~View() {

  }

  void View::reset(const RectF& rect) {
    m_center = rect.getCenter();
    m_size = rect.getSize();
    onWorldResize(m_size);
  }

  void View::rotate(float angle) {
    m_rotation += angle;
  }

  void View::move(Vector2f offset) {
    m_center += offset;
  }

  void View::zoom(float factor) {
    m_size *= factor;
    onWorldResize(m_size);
  }

  Matrix3f View::getTransform() const {
    /* compute the view matrix
     * it's a combination of:
     * 1. a translation of -center
     * 2. a rotation of -angle
     * 3. a scaling of 2 / size
     */

    Vector2f factors = 2.0f / m_size;
    return scaling({ factors.x, -factors.y }) * rotation(-m_rotation) * translation(-m_center);
  }

  Matrix3f View::getInverseTransform() const {
    return invert(getTransform());
  }

  void View::onWorldResize(Vector2f worldSize) {
    GF_UNUSED(worldSize);

    // nothing by default
  }

  /*
   * AdaptativeView
   */

  void AdaptativeView::setInitialScreenSize(Vector2u screenSize) {
    onScreenResize(screenSize);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
