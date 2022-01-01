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
#include <gf/Orientation.h>

#include <cassert>
#include <limits>

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
    switch (orientation) {
      case Orientation::Center:
        return std::numeric_limits<float>::quiet_NaN();
      case Orientation::North:
        return 6 * Pi4;
      case Orientation::NorthEast:
        return 7 * Pi4;
      case Orientation::East:
        return 0 * Pi4;
      case Orientation::SouthEast:
        return 1 * Pi4;
      case Orientation::South:
        return 2 * Pi4;
      case Orientation::SouthWest:
        return 3 * Pi4;
      case Orientation::West:
        return 4 * Pi4;
      case Orientation::NorthWest:
        return 5 * Pi4;
    }

    assert(false);
    return std::numeric_limits<float>::quiet_NaN();
  }

  Orientation orientation(float angle) {
    float normalized = std::fmod(angle, 2 * Pi);

    if (angle < 0) {
      assert(normalized < 0);
      normalized += 2 * Pi;
    }

    assert(0.0f <= normalized && normalized < 2 * Pi);

    static constexpr float Pi8 = Pi / 8.0f;

    if (normalized <  1 * Pi8) { return Orientation::East; }
    if (normalized <  3 * Pi8) { return Orientation::SouthEast; }
    if (normalized <  5 * Pi8) { return Orientation::South; }
    if (normalized <  7 * Pi8) { return Orientation::SouthWest; }
    if (normalized <  9 * Pi8) { return Orientation::West; }
    if (normalized < 11 * Pi8) { return Orientation::NorthWest; }
    if (normalized < 13 * Pi8) { return Orientation::North; }
    if (normalized < 15 * Pi8) { return Orientation::NorthEast; }
    return Orientation::East;
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
