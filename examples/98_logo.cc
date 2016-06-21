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
#include <iostream>

#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Window.h>

int main() {
  gf::Window window("Logo", { 200, 200 });
  gf::RenderWindow renderer(window);

  gf::Font font;
  if (!font.loadFromFile("16_DejaVuSans.ttf")) {
    return EXIT_FAILURE;
  }

  gf::Text text;
  text.setColor(gf::Color::Orange);
  text.setOutlineThickness(12.0f);
  text.setOutlineColor(gf::Color::Azure);
  text.setString("gf");
  text.setCharacterSize(150);
  text.setFont(font);
  text.setPosition({ 100, 100 });
  text.setAnchor(gf::Anchor::Center);

  renderer.clear({ 1.0f, 1.0f, 1.0f, 0.0f }); // transparent white

  std::cout << "Gamedev Framework (gf) example #98: Logo\n";
  std::cout << "This example prints the logo of Gamedev Framework (gf)\n";
  std::cout << "How to use:\n";
  std::cout << "\tS: Capture the image in 'gf_logo.png'\n";

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          if (event.key.keycode == gf::Keycode::S) {
            auto image = renderer.capture();
            image.saveToFile("gf_logo.png");
            std::cout << "Logo saved!\n";
          }
          break;

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(text);
    renderer.display();
  }

  return 0;
}
