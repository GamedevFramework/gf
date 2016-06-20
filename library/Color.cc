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
#include <gf/Color.h>

#include <cassert>
#include <cmath>
#include <algorithm>
#include <tuple>

#include <gf/Math.h>

namespace gf {
inline namespace v1 {

  constexpr Color4f Color::Black;
  constexpr Color4f Color::White;

  constexpr Color4f Color::Red;
  constexpr Color4f Color::Green;
  constexpr Color4f Color::Blue;

  constexpr Color4f Color::Cyan;
  constexpr Color4f Color::Magenta;
  constexpr Color4f Color::Yellow;

  constexpr Color4f Color::Transparent;

  constexpr Color4f Color::Orange;
  constexpr Color4f Color::Rose;
  constexpr Color4f Color::Chartreuse;
  constexpr Color4f Color::Spring;
  constexpr Color4f Color::Violet;
  constexpr Color4f Color::Azure;

  static constexpr float Epsilon = 0.00001f;

  static Color4f convertRgbToHsv(Color4f color) {
    float min, max;
    std::tie(min, max) = std::minmax({ color.r, color.g, color.b });

    float hue = 0.0f;

    if ((max - min) > Epsilon) {
      if (max == color.r) {
        hue = std::fmod(60.0f * (color.g - color.b) / (max - min) + 360.0f, 360.0f);
      } else if (max == color.g) {
        hue = 60.0f * (color.b - color.r) / (max - min) + 120.0f;
      } else if (max == color.b) {
        hue = 60.0f * (color.r - color.g) / (max - min) + 240.0f;
      } else {
        assert(false);
      }
    }

    float sat = (max < Epsilon) ? 0.0f : (1.0f - min / max);
    float val = max;

    return { hue, sat, val, color.a };
  }

  static Color4f convertHsvToRgb(Color4f color) {
    float hue = color[0] / 60.0f;
    float sat = color[1];
    float val = color[2];

    int i = static_cast<int>(hue) % 6;
    assert(0 <= i && i < 6);

    float f = hue - i;

    float x = val * (1 - sat);
    float y = val * (1 - (f * sat));
    float z = val * (1 - ((1 - f) * sat));

    Color4f res;
    res.a = color.a;

    switch (i) {
      case 0:
        res.r = val;
        res.g = z;
        res.b = x;
        break;
      case 1:
        res.r = y;
        res.g = val;
        res.b = x;
        break;
      case 2:
        res.r = x;
        res.g = val;
        res.b = z;
        break;
      case 3:
        res.r = x;
        res.g = y;
        res.b = val;
        break;
      case 4:
        res.r = z;
        res.g = x;
        res.b = val;
        break;
      case 5:
        res.r = val;
        res.g = x;
        res.b = y;
        break;
      default:
        assert(false);
        break;
    }

    return res;
  }

  Color4f Color::lighter(Color4f color, float percent) {
    assert(0.0f <= percent && percent <= 1.0);
    Color4f hsv = convertRgbToHsv(color);
    hsv[2] += hsv[2] * percent;

    if (hsv[2] > 1.0f) {
      hsv[1] -= (hsv[2] - 1.0f);

      if (hsv[1] < 0) {
        hsv[1] = 0.0f;
      }

      hsv[2] = 1.0f;
    }

    return convertHsvToRgb(hsv);
  }

  Color4f Color::darker(Color4f color, float percent) {
    assert(0.0f <= percent && percent <= 1.0);
    Color4f hsv = convertRgbToHsv(color);
    hsv[2] -= hsv[2] * percent;
    return convertHsvToRgb(hsv);
  }

}
}
