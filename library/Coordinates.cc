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
#include <gf/Coordinates.h>

#include <cassert>

#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Coordinates::Coordinates(Vector2u size)
  : m_size(size)
  {

  }

  Coordinates::Coordinates(RenderTarget& target)
  : m_size(target.getSize())
  {

  }

  Vector2f Coordinates::getCenter() const {
    return m_size * 0.5f;
  }

  Vector2f Coordinates::getRelativePoint(Vector2f percent) const {
    return m_size * percent;
  }

  Vector2f Coordinates::getRelativeSize(Vector2f percent) const {
    return m_size * percent;
  }

  unsigned Coordinates::getRelativeCharacterSize(float percent) const {
    return static_cast<unsigned>(m_size.height * percent);
  }

  static float getXDistance(float width, float dx, Anchor anchor) {
    switch (anchor) {
      case Anchor::TopLeft:
      case Anchor::CenterLeft:
      case Anchor::BottomLeft:
        return dx;
      case Anchor::TopCenter:
      case Anchor::Center:
      case Anchor::BottomCenter:
        return width * 0.5f;
      case Anchor::TopRight:
      case Anchor::CenterRight:
      case Anchor::BottomRight:
        return width - dx;
    }

    assert(false);
    return 0.0f;
  }

  static float getYDistance(float height, float dy, Anchor anchor) {
    switch (anchor) {
      case Anchor::TopLeft:
      case Anchor::TopCenter:
      case Anchor::TopRight:
        return dy;
      case Anchor::CenterLeft:
      case Anchor::Center:
      case Anchor::CenterRight:
        return height * 0.5f;
      case Anchor::BottomLeft:
      case Anchor::BottomCenter:
      case Anchor::BottomRight:
        return height - dy;
    }

    assert(false);
    return 0.0f;
  }

  Vector2f Coordinates::getAbsolutePoint(Vector2f distance, Anchor anchor) const {
    return { getXDistance(m_size.width, distance.x, anchor), getYDistance(m_size.height, distance.y, anchor) };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
