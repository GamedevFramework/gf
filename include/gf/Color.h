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
#ifndef GF_COLOR_H
#define GF_COLOR_H

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Predefined colors
   *
   * This class does not contain anything but predefined colors. All the
   * primary, secondary and [tertiary colors](https://en.wikipedia.org/wiki/Tertiary_color)
   * are defined by their usual names.
   *
   * If you want to define a color, you should use gf::Color4f.
   *
   * @sa gf::Color4f
   */
  struct GF_API Color {
    /**
     * @brief Deleted constructor
     */
    Color() = delete;

    /**
     * @brief Black predefined color
     */
    static constexpr Color4f Black{0.0f, 0.0f, 0.0f, 1.0f};

    /**
     * @brief White predefined color
     */
    static constexpr Color4f White{1.0f, 1.0f, 1.0f, 1.0f};

    /**
     * @brief Red predefined color
     */
    static constexpr Color4f Red{1.0f, 0.0f, 0.0f, 1.0f};

    /**
     * @brief Green predefined color
     */
    static constexpr Color4f Green{0.0f, 1.0f, 0.0f, 1.0f};

    /**
     * @brief Blue predefined color
     */
    static constexpr Color4f Blue{0.0f, 0.0f, 1.0f, 1.0f};

    /**
     * @brief Cyan predefined color
     */
    static constexpr Color4f Cyan{0.0f, 1.0f, 1.0f, 1.0f};

    /**
     * @brief Magenta predefined color
     */
    static constexpr Color4f Magenta{1.0f, 0.0f, 1.0f, 1.0f};

    /**
     * @brief Yellow predefined color
     */
    static constexpr Color4f Yellow{1.0f, 1.0f, 0.0f, 1.0f};

    /**
     * @brief Transparent (black) predefined color
     */
    static constexpr Color4f Transparent{0.0f, 0.0f, 0.0f, 0.0f};

    /**
     * @brief Opaque predefined color
     *
     * @param value The opacity value
     */
    static constexpr Color4f Opaque(float value = 0.5f) {
      return { 1.0f, 1.0f, 1.0f, value };
    }

    /**
     * @brief Gray predefined color
     *
     * @param value The gray value
     */
    static constexpr Color4f Gray(float value = 0.5f) {
      return { value, value, value, 1.0f };
    }

    /**
     * @brief Orange predefined color
     */
    static constexpr Color4f Orange{1.0f, 0.5f, 0.0f, 1.0f};

    /**
     * @brief Rose predefined color
     */
    static constexpr Color4f Rose{1.0f, 0.0f, 0.5f, 1.0f};

    /**
     * @brief Chartreuse predefined color
     */
    static constexpr Color4f Chartreuse{0.5f, 1.0f, 0.0f, 1.0f};

    /**
     * @brief Spring (green) predefined color
     */
    static constexpr Color4f Spring{0.0f, 1.0f, 0.5f, 1.0f};

    /**
     * @brief Violet predefined color
     */
    static constexpr Color4f Violet{0.5f, 0.0f, 1.0f, 1.0f};

    /**
     * @brief Azure predefined color
     */
    static constexpr Color4f Azure{0.0f, 0.5f, 1.0f, 1.0f};


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
    static Color4f lighter(Color4f color, float percent = 0.5f);

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
    static Color4f darker(Color4f color, float percent = 0.5f);

    /**
     * @brief Get a color from 4 8-bit channels
     *
     * @param r The red channel
     * @param g The green channel
     * @param b The blue channel
     * @param a The alpha channel
     * @returns The corresponding color
     */
    static Color4f fromRgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    /**
     * @brief Get a color from 32-bit value
     *
     * @param color The 32-bit color
     * @returns The corresponding color
     */
    static Color4f fromRgba32(uint32_t color);

    /**
     * @brief Get a color from a 32-bit color
     *
     * @param color A 32-bit color
     * @return The corresponding color
     */
    static Color4f fromRgba32(Color4u color);

    /**
     * @brief Convert a color to a 32-bit color
     *
     * @param color A color
     * @return The corresponding 32-bit color
     */
    static Color4u toRgba32(Color4f color);

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_COLOR_H
