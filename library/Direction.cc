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
#include <gf/Direction.h>

#include <cassert>

#include <gf/Math.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Vector2f unit(Direction direction) {
    switch (direction) {
      case Direction::Center:
        return {  0.0f,  0.0f };
      case Direction::Up:
        return {  0.0f, -1.0f };
      case Direction::Right:
        return {  1.0f,  0.0f };
      case Direction::Down:
        return {  0.0f,  1.0f };
      case Direction::Left:
        return { -1.0f,  0.0f };
    }

    assert(false);
    return { 0.0f, 0.0f };
  }

  Vector2i displacement(Direction direction) {
    switch (direction) {
      case Direction::Center:
        return {  0,  0 };
      case Direction::Up:
        return {  0, -1 };
      case Direction::Right:
        return {  1,  0 };
      case Direction::Down:
        return {  0,  1 };
      case Direction::Left:
        return { -1,  0 };
    }

    assert(false);
    return { 0, 0 };
  }

  float angle(Direction direction) {
    if (direction == Direction::Center) {
      return 0.0f;
    }

    return static_cast<int>(direction) * Pi / 2;
  }

  Direction opposite(Direction direction) {
    if (direction == Direction::Center) {
      return Direction::Center;
    }

    int val = static_cast<int>(direction);
    return static_cast<Direction>((val + 2) % 4);
  }

  Direction orthogonalCW(Direction direction) {
    if (direction == Direction::Center) {
      return Direction::Center;
    }

    int val = static_cast<int>(direction);
    return static_cast<Direction>((val + 1) % 4);
  }

  Direction orthogonalCCW(Direction direction) {
    if (direction == Direction::Center) {
      return Direction::Center;
    }

    int val = static_cast<int>(direction);
    return static_cast<Direction>((val + 3) % 4);
  }

  Direction nextCW(Direction direction) {
    return orthogonalCW(direction);
  }

  Direction nextCCW(Direction direction) {
    return orthogonalCCW(direction);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
