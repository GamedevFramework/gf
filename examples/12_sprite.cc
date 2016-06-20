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
#include <cstdlib>
#include <cstdio>

#include <iostream>

#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Window.h>

int main() {

  gf::WindowHints hints;
  hints.resizable = false;

  gf::Window window("12_sprite", { 640, 480 }, hints);
  gf::RenderWindow renderer(window);

  gf::Texture texture;
  bool loaded = texture.loadFromFile("12_bomb.png");

  if (!loaded) {
    return EXIT_FAILURE;
  }

  texture.setSmooth();

  gf::Sprite sprite(texture);
  sprite.setPosition({ 320, 240 });
  sprite.setScale({ 0.8, 0.8 });
  sprite.setAnchor(gf::Anchor::Center);

  std::cout << "Gamedev Framework (gf) example #12: Sprite\n";
  std::cout << "This example prints a sprite of a bomb.\n";

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        default:
          break;
      }
    }

    renderer.clear(gf::Color::White);
    renderer.draw(sprite);
    renderer.display();
  }

  return 0;
}
