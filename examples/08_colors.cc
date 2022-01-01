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
#include <iostream>

#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Window.h>

int main() {
  gf::Window window("08_colors", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Color4f allColors[12] {
    gf::Color::Red,
    gf::Color::Orange,
    gf::Color::Yellow,
    gf::Color::Chartreuse,
    gf::Color::Green,
    gf::Color::Spring,
    gf::Color::Cyan,
    gf::Color::Azure,
    gf::Color::Blue,
    gf::Color::Violet,
    gf::Color::Magenta,
    gf::Color::Rose,
  };

  constexpr float SmallStartY = 400.0f;
  constexpr float SmallSize = 25.0f;
  constexpr float SmallPaddingX = (640 - SmallSize * 24) / 2.0f;

  constexpr float Size = 40.0f;
  constexpr float PaddingX = (640 - Size * 12) / 2;
  constexpr float PaddingY = (SmallStartY - Size * 9) / 2;

  std::cout << "Gamedev Framework (gf) example #08: Colors\n";
  std::cout << "This example prints primary, secondary and tertiary colors with different variations of lightness/darkness.\n";

  renderer.clear(gf::Color::Gray());

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          if (event.key.scancode == gf::Scancode::Escape) {
            window.close();
          }
          break;

        default:
          break;
      }
    }


    renderer.clear();

    float x = PaddingX;

    for (auto color : allColors) {
      float y = PaddingY;
      gf::RectangleShape rect({ Size, Size });

      auto darkerColor = gf::Color::darker(color, 1.0f);
      rect.setPosition({ x, y });
      rect.setColor(darkerColor);
      renderer.draw(rect);

      y += Size;

      darkerColor = gf::Color::darker(color, 0.75f);
      rect.setPosition({ x, y });
      rect.setColor(darkerColor);
      renderer.draw(rect);

      y += Size;

      darkerColor = gf::Color::darker(color, 0.5f);
      rect.setPosition({ x, y });
      rect.setColor(darkerColor);
      renderer.draw(rect);

      y += Size;

      darkerColor = gf::Color::darker(color, 0.25f);
      rect.setPosition({ x, y });
      rect.setColor(darkerColor);
      renderer.draw(rect);

      y += Size;

      rect.setPosition({ x, y });
      rect.setColor(color);
      renderer.draw(rect);

      y += Size;

      auto lighterColor = gf::Color::lighter(color, 0.25f);
      rect.setPosition({ x, y });
      rect.setColor(lighterColor);
      renderer.draw(rect);

      y += Size;

      lighterColor = gf::Color::lighter(color, 0.5f);
      rect.setPosition({ x, y });
      rect.setColor(lighterColor);
      renderer.draw(rect);

      y += Size;

      lighterColor = gf::Color::lighter(color, 0.75f);
      rect.setPosition({ x, y });
      rect.setColor(lighterColor);
      renderer.draw(rect);

      y += Size;

      lighterColor = gf::Color::lighter(color, 1.0f);
      rect.setPosition({ x, y });
      rect.setColor(lighterColor);
      renderer.draw(rect);

      x += Size;
    }

    x = SmallPaddingX;

    for (int i = 0; i < 12; ++i) {
      auto color1 = allColors[i];
      auto color2 = allColors[(i + 1) % 12];
      auto colorM = gf::lerp(color1, color2, 0.5f);

      gf::RectangleShape rect({ SmallSize, SmallSize });
      rect.setPosition({ x, SmallStartY });
      rect.setColor(color1);
      renderer.draw(rect);

      x += SmallSize;

      rect.setPosition({ x, SmallStartY });
      rect.setColor(colorM);
      renderer.draw(rect);

      x += SmallSize;

    }

    renderer.display();
  }

  return 0;
}
