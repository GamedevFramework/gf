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

#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Window.h>

int main() {
  gf::Window window("12_sprite", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  bool smooth = true;

  gf::Texture texture("assets/bomb.png");
  texture.setSmooth(smooth);

  gf::Sprite sprite(texture);
  sprite.setPosition({ 160, 240 });
  sprite.setScale({ 0.8f, 0.8f });
  sprite.setAnchor(gf::Anchor::Center);

  gf::Texture textureWithMipmap("assets/bomb.png");
  textureWithMipmap.setSmooth(smooth);
  textureWithMipmap.generateMipmap();

  gf::Sprite spriteWithMipmap(textureWithMipmap);
  spriteWithMipmap.setPosition({ 480, 240 });
  spriteWithMipmap.setScale({ 0.8f, 0.8f });
  spriteWithMipmap.setAnchor(gf::Anchor::Center);

  std::cout << "Gamedev Framework (gf) example #12: Sprite\n";
  std::cout << "This example prints two sprites of a bomb.\n";
  std::cout << "The left sprite does not use mipmaps whereas the right sprite uses mipmaps.\n";
  std::cout << "How to use:\n";
  std::cout << "\tSpace: Toggle texture smoothness\n";
  std::cout << "\tPageUp/PageDown: Zoom in/out\n";
  std::cout << "Using texture smoothness: " << (smooth ? "yes" : "no") << '\n';

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::Space:
              smooth = !smooth;
              texture.setSmooth(smooth);
              textureWithMipmap.setSmooth(smooth);
              std::cout << "Using texture smoothness: " << (smooth ? "yes" : "no") << '\n';
              break;

            case gf::Scancode::PageUp:
              sprite.scale(0.8f);
              spriteWithMipmap.scale(0.8f);
              break;

            case gf::Scancode::PageDown:
              sprite.scale(1.25f);
              spriteWithMipmap.scale(1.25f);
              break;

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(sprite);
    renderer.draw(spriteWithMipmap);
    renderer.display();
  }

  return 0;
}
