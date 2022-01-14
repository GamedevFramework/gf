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
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <chrono>

#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/TextureAtlas.h>
#include <gf/Window.h>

int main() {
  gf::Window window("15_atlas", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::TextureAtlas atlas("assets/bricks.xml");

  gf::Texture texture("assets" / atlas.getTexturePath());
  texture.setSmooth();
  atlas.setTexture(texture);

  gf::Sprite brickRed06;
  brickRed06.setTexture(atlas.getTexture(), atlas.getTextureRect("brickRed06.png"));
  brickRed06.setPosition({ 200.0f, 200.0f });

  gf::Sprite brickBlack01;
  brickBlack01.setTexture(atlas.getTexture(), atlas.getTextureRect("brickBlack01.png"));
  brickBlack01.setPosition({ 200.0f, 176.0f });

  std::cout << "Gamedev Framework (gf) example #15: Texture atlas\n";
  std::cout << "This example prints two sprites that come from a texture atlas.\n";

  renderer.clear(gf::Color::White);

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
    renderer.draw(brickRed06);
    renderer.draw(brickBlack01);
    renderer.display();
  }

  return 0;
}
