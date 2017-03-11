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
#include <gf/Color.h>

#include <cassert>
#include <cmath>
#include <algorithm>
#include <tuple>

#include <gf/Math.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

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

  struct Hsv {
    float h;
    float s;
    float v;
    float a;
  };

  static Hsv convertRgbToHsv(Color4f color) {
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

  static Color4f convertHsvToRgb(Hsv hsv) {
    float hue = hsv.h / 60.0f;
    float sat = hsv.s;
    float val = hsv.v;

    int i = static_cast<int>(hue) % 6;
    assert(0 <= i && i < 6);

    float f = hue - i;

    float x = val * (1 - sat);
    float y = val * (1 - (f * sat));
    float z = val * (1 - ((1 - f) * sat));

    Color4f color;
    color.a = hsv.a;

    switch (i) {
      case 0:
        color.r = val;
        color.g = z;
        color.b = x;
        break;
      case 1:
        color.r = y;
        color.g = val;
        color.b = x;
        break;
      case 2:
        color.r = x;
        color.g = val;
        color.b = z;
        break;
      case 3:
        color.r = x;
        color.g = y;
        color.b = val;
        break;
      case 4:
        color.r = z;
        color.g = x;
        color.b = val;
        break;
      case 5:
        color.r = val;
        color.g = x;
        color.b = y;
        break;
      default:
        assert(false);
        break;
    }

    return color;
  }

  Color4f Color::lighter(Color4f color, float percent) {
    assert(0.0f <= percent && percent <= 1.0);
    Hsv hsv = convertRgbToHsv(color);
    hsv.v += hsv.v * percent;

    if (hsv.v > 1.0f) {
      hsv.s -= (hsv.v - 1.0f);

      if (hsv.s < 0) {
        hsv.s = 0.0f;
      }

      hsv.v = 1.0f;
    }

    return convertHsvToRgb(hsv);
  }

  Color4f Color::darker(Color4f color, float percent) {
    assert(0.0f <= percent && percent <= 1.0);
    Hsv hsv = convertRgbToHsv(color);
    hsv.v -= hsv.v * percent;
    return convertHsvToRgb(hsv);
  }

  Color4f Color::fromRgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
  }

  Color4f Color::fromRgba32(uint32_t c) {
    return fromRgba32(c >> 24, c >> 16, c >> 8, c);
  }

  Color4f Color::fromRgba32(Color4u color) {
    return fromRgba32(color.r, color.g, color.b, color.a);
  }

  Color4u Color::toRgba32(Color4f color) {
    return {
      static_cast<uint8_t>(color.r * 255),
      static_cast<uint8_t>(color.g * 255),
      static_cast<uint8_t>(color.b * 255),
      static_cast<uint8_t>(color.a * 255)
    };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
