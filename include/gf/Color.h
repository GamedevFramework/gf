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
#ifndef GF_COLOR_H
#define GF_COLOR_H

#include <cassert>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <type_traits>

#include "Math.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_color
   * @brief Predefined colors and utilities
   *
   * This class does not contain anything but predefined colors. All the
   * primary, secondary and [tertiary colors](https://en.wikipedia.org/wiki/Tertiary_color)
   * are defined by their usual names.
   *
   * If you want to define a color, you should use gf::Color4f.
   *
   * If you want to use the functions of this class, you can use:
   *
   * - gf::ColorF or gf::Color for the instantiation with `float`
   * - gf::ColorD for the instantiation with `double`
   *
   * @sa gf::ColorRampBase, gf::Color4f
   */
  template<typename T>
  struct ColorBase {
    static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");

  private:

    /**
     * @brief A structure to represent a color in the HSV color space
     */
    struct HSV {
      T h;
      T s;
      T v;
      T a;
    };

    /**
     * @brief Convert a RGB color to a HSV color
     */
    static constexpr HSV convertRgbToHsv(Color4<T> color) {
      T min = 0;
      T max = 1;
      std::tie(min, max) = std::minmax({ color.r, color.g, color.b });

      T hue = 0;

      if ((max - min) > std::numeric_limits<T>::epsilon()) {
        if (max == color.r) {
          hue = std::fmod(T(60) * (color.g - color.b) / (max - min) + T(360), T(360));
        } else if (max == color.g) {
          hue = T(60) * (color.b - color.r) / (max - min) + T(120);
        } else if (max == color.b) {
          hue = T(60) * (color.r - color.g) / (max - min) + T(240);
        } else {
          assert(false);
        }
      }

      T sat = (max < std::numeric_limits<T>::epsilon()) ? T(0) : (T(1) - min / max);
      T val = max;

      return { hue, sat, val, color.a };
    }

    /**
     * @brief Convert a HSV color to a RGB color
     */
    static constexpr Color4<T> convertHsvToRgb(HSV hsv) {
      T hue = hsv.h / T(60);
      T sat = hsv.s;
      T val = hsv.v;

      int i = static_cast<int>(hue) % 6;
      assert(0 <= i && i < 6);

      T f = hue - static_cast<T>(i);

      T x = val * (T(1) - sat);
      T y = val * (T(1) - (f * sat));
      T z = val * (T(1) - ((T(1) - f) * sat));

      Color4<T> color = Zero;
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

  public:

    /**
     * @brief Black predefined color
     */
    static constexpr Color4<T> Black{T(0), T(0), T(0), T(1)};

    /**
     * @brief White predefined color
     */
    static constexpr Color4<T> White{T(1), T(1), T(1), T(1)};

    /**
     * @brief Red predefined color
     */
    static constexpr Color4<T> Red{T(1), T(0), T(0), T(1)};

    /**
     * @brief Green predefined color
     */
    static constexpr Color4<T> Green{T(0), T(1), T(0), T(1)};

    /**
     * @brief Blue predefined color
     */
    static constexpr Color4<T> Blue{T(0), T(0), T(1), T(1)};

    /**
     * @brief Cyan predefined color
     */
    static constexpr Color4<T> Cyan{T(0), T(1), T(1), T(1)};

    /**
     * @brief Magenta predefined color
     */
    static constexpr Color4<T> Magenta{T(1), T(0), T(1), T(1)};

    /**
     * @brief Yellow predefined color
     */
    static constexpr Color4<T> Yellow{T(1), T(1), T(0), T(1)};

    /**
     * @brief Transparent (black) predefined color
     */
    static constexpr Color4<T> Transparent{T(0), T(0), T(0), T(0)};

    /**
     * @brief Opaque predefined color
     *
     * @param value The opacity value
     */
    static constexpr Color4<T> Opaque(T value = T(0.5)) {
      return { T(1), T(1), T(1), value };
    }

    /**
     * @brief Gray predefined color
     *
     * @param value The gray value
     */
    static constexpr Color4<T> Gray(T value = T(0.5)) {
      return { value, value, value, T(1) };
    }

    /**
     * @brief Orange predefined color
     */
    static constexpr Color4<T> Orange{T(1), T(0.5), T(0), T(1)};

    /**
     * @brief Rose predefined color
     */
    static constexpr Color4<T> Rose{T(1), T(0), T(0.5), T(1)};

    /**
     * @brief Chartreuse predefined color
     */
    static constexpr Color4<T> Chartreuse{T(0.5), T(1), T(0), T(1)};

    /**
     * @brief Spring (green) predefined color
     */
    static constexpr Color4<T> Spring{T(0), T(1), T(0.5), T(1)};

    /**
     * @brief Violet predefined color
     */
    static constexpr Color4<T> Violet{T(0.5), T(0), T(1), T(1)};

    /**
     * @brief Azure predefined color
     */
    static constexpr Color4<T> Azure{T(0), T(0.5), T(1), T(1)};


    /**
     * @brief Compute a lighter color
     *
     * This function takes a color and gives a lighter color based on a
     * percentage. If this percentage is 0, the same color is returned. If this
     * percentage is 1, the white color is returned.
     *
     * @param color The color
     * @param percent The percentage, must be in @f$ [0, 1] @f$
     */
    static constexpr Color4<T> lighter(Color4<T> color, T percent = T(0.5)) {
      assert(0.0f <= percent && percent <= 1.0f);
      HSV hsv = convertRgbToHsv(color);
      hsv.v += hsv.v * percent;

      if (hsv.v > 1) {
        hsv.s -= (hsv.v - 1);

        if (hsv.s < 0) {
          hsv.s = 0;
        }

        hsv.v = 1;
      }

      return convertHsvToRgb(hsv);
    }

    /**
     * @brief Compute a darker color
     *
     * This function takes a color and gives a darker color based on a
     * percentage. If this percentage is 0, the same color is returned. If this
     * percentage is 1, the black color is returned.
     *
     * @param color The color
     * @param percent The percentage, must be in @f$ [0, 1] @f$
     */
    static constexpr Color4<T> darker(Color4<T> color, T percent = T(0.5)) {
      assert(0.0f <= percent && percent <= 1.0f);
      HSV hsv = convertRgbToHsv(color);
      hsv.v -= hsv.v * percent;
      return convertHsvToRgb(hsv);
    }

    /**
     * @brief Get an opaque color from 3 RGB floats in @f$ [0, 1] @f$
     *
     * @param r The red channel
     * @param g The green channel
     * @param b The blue channel
     * @returns The corresponding color
     */
    static constexpr Color4<T> fromRgb(T r, T g, T b) {
      return Color4<T>(r, g, b, T(1));
    }

    /**
     * @brief Get a color from 4 8-bit channels
     *
     * @param r The red channel
     * @param g The green channel
     * @param b The blue channel
     * @param a The alpha channel
     * @returns The corresponding color
     */
    static constexpr Color4<T> fromRgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
      return { r / T(255), g / T(255), b / T(255), a / T(255) };
    }

    /**
     * @brief Get a color from 32-bit value
     *
     * @param color The 32-bit color
     * @returns The corresponding color
     */
    static constexpr Color4<T> fromRgba32(uint32_t color) {
      return fromRgba32(static_cast<uint8_t>(color >> 24), static_cast<uint8_t>(color >> 16), static_cast<uint8_t>(color >> 8), static_cast<uint8_t>(color));
    }

    /**
     * @brief Get a color from a 32-bit color
     *
     * @param color A 32-bit color
     * @return The corresponding color
     */
    static constexpr Color4<T> fromRgba32(Color4u color)  {
      return fromRgba32(color.r, color.g, color.b, color.a);
    }

    /**
     * @brief Convert a color to a 32-bit color
     *
     * @param color A color
     * @return The corresponding 32-bit color
     */
    static constexpr Color4u toRgba32(Color4<T> color) {
      return {
        static_cast<uint8_t>(color.r * 255),
        static_cast<uint8_t>(color.g * 255),
        static_cast<uint8_t>(color.b * 255),
        static_cast<uint8_t>(color.a * 255)
      };
    }
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Black;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::White;

  template<typename T>
  constexpr Color4<T> ColorBase<T>::Red;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Green;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Blue;

  template<typename T>
  constexpr Color4<T> ColorBase<T>::Cyan;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Magenta;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Yellow;

  template<typename T>
  constexpr Color4<T> ColorBase<T>::Transparent;

  template<typename T>
  constexpr Color4<T> ColorBase<T>::Orange;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Rose;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Chartreuse;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Spring;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Violet;
  template<typename T>
  constexpr Color4<T> ColorBase<T>::Azure;
#endif

  /**
   * @ingroup core_color
   * @brief Instantiation of ColoBase for `float`
   */
  using ColorF = ColorBase<float>;

  /**
   * @ingroup core_color
   * @brief Instantiation of ColoBase for `double`
   */
  using ColorD = ColorBase<double>;

  /**
   * @ingroup core_color
   * @brief Instantiation of ColorBase for `float`
   *
   * This name is used for compatibility with SFML.
   */
  using Color = ColorF;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_COLOR_H
