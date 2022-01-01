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

#include <gf/Event.h>
#include <gf/NinePatch.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window("19_ninepatch", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Texture texture("assets/patch.png");
  texture.setSmooth();

  gf::NinePatch patch(texture);
  patch.setLimits(0.4f, 0.6f, 0.3f, 0.5f);

  std::cout << "Gamedev Framework (gf) example #19: Nine Patch\n";
  std::cout << "This example prints a nine patch in three formats.\n";

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

    patch.setSize({ 300.0f, 100.0f });
    patch.setPosition({ 20.0f, 20.0f });
    renderer.draw(patch);

    patch.setSize({ 100.0f, 300.0f });
    patch.setPosition({ 400.0f, 20.0f });
    renderer.draw(patch);

    patch.setSize({ 300.0f, 250.0f });
    patch.setPosition({ 20.0f, 200.0f });
    renderer.draw(patch);

    renderer.display();
  }

  return 0;
}
