/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <gf/WindowGeometryTracker.h>

#include <gf/Event.h>

namespace gf {
inline namespace v1 {

  WindowGeometryTracker::WindowGeometryTracker()
  : m_screenSize(0, 0)
  {

  }

  float WindowGeometryTracker::getXCentered(float width) const {
    return getXRatio(0.5f, width);
  }

  float WindowGeometryTracker::getXFromRight(float width) const {
    return m_screenSize.width - width;
  }

  float WindowGeometryTracker::getXRatio(float ratio, float width) const {
    return (m_screenSize.width - width) * ratio;
  }


  float WindowGeometryTracker::getYCentered(float height) const {
    return getYRatio(0.5f, height);
  }

  float WindowGeometryTracker::getYFromBottom(float height) const {
    return m_screenSize.height - height;
  }

  float WindowGeometryTracker::getYRatio(float ratio, float height) const {
    return (m_screenSize.height - height) * ratio;
  }

  Vector2f WindowGeometryTracker::getCornerPosition(const Vector2f& pos) const {
    return gf::select(gf::greaterThan(pos, { 0.0f, 0.0f }), pos, m_screenSize + pos);
  }

  void WindowGeometryTracker::update(const Event& event) {
    if (event.type != EventType::Resized) {
      return;
    }

    onScreenResize(event.size);
  }

  void WindowGeometryTracker::onScreenResize(Vector2u screenSize) {
    m_screenSize = screenSize;
  }

}
}


