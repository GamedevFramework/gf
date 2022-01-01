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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/LightTextures.h>

#include <cassert>

#include <gf/Color.h>
#include <gf/Image.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Texture LightTextures::createSimpleLight(int size) {
    gf::Image image(gf::vec(size, size));
    float half = size * 0.5f;
    gf::Vector2f center = gf::vec(half, half);

    for (int y = 0; y < size; ++y) {
      for (int x = 0; x < size; ++x) {
        gf::Vector2f position = gf::vec(x, y) + 0.5f;
        float distance = gf::euclideanDistance(position, center);

        if (distance > half) {
          image.setPixel({ x, y }, gf::Color4u(0, 0, 0, 0));
        } else {
          float alpha = 1.0f - distance / half;
          image.setPixel({ x, y }, gf::Color::toRgba32(gf::Color::White * gf::Color::Opaque(alpha)));
        }
      }
    }

    return gf::Texture(image);
  }

  Texture LightTextures::createRealisticLight(int size, float attenuation, float radius) {
    gf::Image image(gf::vec(size, size));
    float half = size * 0.5f;
    assert(radius < half);
    gf::Vector2f center = gf::vec(half, half);

    for (int y = 0; y < size; ++y) {
      for (int x = 0; x < size; ++x) {
        gf::Vector2f position = gf::vec(x, y) + 0.5f;
        float distance = gf::euclideanDistance(position, center);

        if (distance > half) {
          image.setPixel({ x, y }, gf::Color4u(0, 0, 0, 0));
        } else if (distance <= radius) {
          image.setPixel({ x, y }, gf::Color4u(255, 255, 255, 255));
        } else {
          float ratio = (distance - radius) / (half - radius);
          float alpha = 1.0f / gf::square(1.0f + attenuation * ratio);

          image.setPixel({ x, y }, gf::Color::toRgba32(gf::Color::White * gf::Color::Opaque((1.0f - ratio) * alpha)));
        }
      }
    }

    image.saveToFile("realistic.png");

    return gf::Texture(image);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
