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
#include <cstdlib>
#include <iostream>

#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Texture.h>
#include <gf/Window.h>

int main() {
  gf::Window window("bug23", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Texture texture;

  if (!texture.loadFromFile("gf_logo.png")) {
    return EXIT_FAILURE;
  }

  gf::RectangleShape tile;

  tile.setSize({ 75.0f, 75.0f });
  tile.setOutlineColor(gf::Color::Chartreuse);
  tile.setOutlineThickness(2);
  tile.setPosition({ 320, 240 });
  tile.setAnchor(gf::Anchor::Center);
  tile.setTexture(texture);

  renderer.clear(gf::Color::White);

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

    renderer.clear();
    renderer.draw(tile);
    renderer.display();
  }

  return 0;
}


