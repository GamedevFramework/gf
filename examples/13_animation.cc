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

#include <gf/AnimatedSprite.h>
#include <gf/Animation.h>
#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window("13_animation", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Texture texture("assets/skeleton.png");
  texture.setSmooth();

  gf::Animation animation;
  animation.addTileset(texture, gf::vec(4, 2), gf::milliseconds(100), 8);

  gf::AnimatedSprite animatedSprite;
  animatedSprite.setAnimation(animation);
  animatedSprite.setPosition({ 320, 240 });
  animatedSprite.setOrigin({ 224 / 2.0f, 364 / 2.0f });

  gf::Clock clock;

  std::cout << "Gamedev Framework (gf) example #13: Animation\n";
  std::cout << "This example prints an animated sprite of a walking skeleton.\n";
  std::cout << "The animation is composed of 8 frames.\n";

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

    gf::Time time = clock.restart();
    animatedSprite.update(time);

    renderer.clear();
    renderer.draw(animatedSprite);
    renderer.display();
  }

  return 0;
}
