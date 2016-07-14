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
 */
#include <gf/Direction.h>

#include <cassert>

#include <gf/Math.h>

namespace gf {
inline namespace v1 {

  Vector2f Direction::getUnit() const {
    switch (m_dir) {
      case -1: // center
        return {  0.0f,  0.0f };
      case 0: // north
        return {  0.0f, -1.0f };
      case 1: // north-east
        return {  InvSqrt2, -InvSqrt2 };
      case 2: // east
        return {  1.0f,  0.0f };
      case 3: // south-east
        return {  InvSqrt2,  InvSqrt2 };
      case 4: // south
        return {  0.0f,  1.0f };
      case 5: // south-west
        return { -InvSqrt2,  InvSqrt2 };
      case 6: // west
        return { -1.0f,  0.0f };
      case 7: // north-west
        return { -InvSqrt2, -InvSqrt2 };
      default:
        break;
    }

    assert(false);
    return { 0, 0 };
  }

  Vector2i Direction::getVector() const {
    switch (m_dir) {
      case -1: // center
        return {  0,  0 };
      case 0: // north
        return {  0, -1 };
      case 1: // north-east
        return {  1, -1 };
      case 2: // east
        return {  1,  0 };
      case 3: // south-east
        return {  1,  1 };
      case 4: // south
        return {  0,  1 };
      case 5: // south-west
        return { -1,  1 };
      case 6: // west
        return { -1,  0 };
      case 7: // north-west
        return { -1, -1 };
      default:
        break;
    }

    assert(false);
    return { 0, 0 };
  }

  float Direction::getAngle() const {
    if (m_dir != -1) {
      return m_dir * Pi / 4;
    }

    return 0.0f;
  }

  Direction Direction::getOppositeDirection() const {
    if (m_dir != -1) {
      return Direction{m_dir + 4 % 8};
    }

    return Center;
  }

  Direction Direction::getOrthogonalDirectionCW() const {
    if (m_dir != -1) {
      return Direction{m_dir + 2 % 8};
    }

    return Center;
  }

  Direction Direction::getOrthogonalDirectionCCW() const {
    if (m_dir != -1) {
      return Direction{m_dir + 6 % 8};
    }

    return Center;
  }

  Direction Direction::getNextDirectionCW() const {
    if (m_dir != -1) {
      return Direction{m_dir + 1 % 8};
    }

    return Center;
  }

  Direction Direction::getNextDirectionCCW() const {
    if (m_dir != -1) {
      return Direction{m_dir + 7 % 8};
    }

    return Center;
  }

  const Direction Direction::Center = Direction{-1};
  const Direction Direction::North = Direction{0};
  const Direction Direction::NorthEast = Direction{1};
  const Direction Direction::East = Direction{2};
  const Direction Direction::SouthEast = Direction{3};
  const Direction Direction::South = Direction{4};
  const Direction Direction::SouthWest = Direction{5};
  const Direction Direction::West = Direction{6};
  const Direction Direction::NorthWest = Direction{7};

}
}
