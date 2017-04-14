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
#include <gf/Orientation.h>

#include <cassert>

#include <gf/Math.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Vector2f unit(Orientation orientation) {
    switch (orientation) {
      case Orientation::Center:
        return {  0.0f,  0.0f };
      case Orientation::North:
        return {  0.0f, -1.0f };
      case Orientation::NorthEast:
        return {  InvSqrt2, -InvSqrt2 };
      case Orientation::East:
        return {  1.0f,  0.0f };
      case Orientation::SouthEast:
        return {  InvSqrt2,  InvSqrt2 };
      case Orientation::South:
        return {  0.0f,  1.0f };
      case Orientation::SouthWest:
        return { -InvSqrt2,  InvSqrt2 };
      case Orientation::West:
        return { -1.0f,  0.0f };
      case Orientation::NorthWest:
        return { -InvSqrt2, -InvSqrt2 };
    }

    assert(false);
    return { 0, 0 };
  }

  Vector2i displacement(Orientation orientation) {
    switch (orientation) {
      case Orientation::Center:
        return {  0,  0 };
      case Orientation::North:
        return {  0, -1 };
      case Orientation::NorthEast:
        return {  1, -1 };
      case Orientation::East:
        return {  1,  0 };
      case Orientation::SouthEast:
        return {  1,  1 };
      case Orientation::South:
        return {  0,  1 };
      case Orientation::SouthWest:
        return { -1,  1 };
      case Orientation::West:
        return { -1,  0 };
      case Orientation::NorthWest:
        return { -1, -1 };
    }

    assert(false);
    return { 0, 0 };
  }

  float angle(Orientation orientation) {
    if (orientation == Orientation::Center) {
      return 0.0f;
    }

    return static_cast<int>(orientation) * Pi / 4;
  }

  Orientation opposite(Orientation orientation) {
    if (orientation == Orientation::Center) {
      return Orientation::Center;
    }

    int val = static_cast<int>(orientation);
    return static_cast<Orientation>((val + 4) % 8);
  }

  Orientation orthogonalCW(Orientation orientation) {
    if (orientation == Orientation::Center) {
      return Orientation::Center;
    }

    int val = static_cast<int>(orientation);
    return static_cast<Orientation>((val + 2) % 8);
  }

  Orientation orthogonalCCW(Orientation orientation) {
    if (orientation == Orientation::Center) {
      return Orientation::Center;
    }

    int val = static_cast<int>(orientation);
    return static_cast<Orientation>((val + 6) % 8);
  }

  Orientation nextCW(Orientation orientation) {
    if (orientation == Orientation::Center) {
      return Orientation::Center;
    }

    int val = static_cast<int>(orientation);
    return static_cast<Orientation>((val + 1) % 8);
  }

  Orientation nextCCW(Orientation orientation) {
    if (orientation == Orientation::Center) {
      return Orientation::Center;
    }

    int val = static_cast<int>(orientation);
    return static_cast<Orientation>((val + 7) % 8);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
