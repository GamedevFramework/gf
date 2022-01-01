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

#include <gf/Effects.h>
#include <gf/Event.h>
#include <gf/RenderPipeline.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Window.h>

int main() {
  gf::Window window("31_pipeline", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderPipeline renderer(window);

  gf::AntiAliasingEffect antiAliasingEffect;
  antiAliasingEffect.setFramebufferSize({ 640.0f, 480.0f });

  gf::EdgeEffect edgeEffect;
  edgeEffect.setFramebufferSize({ 640.0f, 480.0f });


  /*
   * some shapes
   */

  gf::RectangleShape rectShape({ 250.0f, 100.0f });
  rectShape.setColor(gf::Color::Blue);
  rectShape.setPosition({ 400.0f, 200.0f });
  rectShape.setRotation(gf::Pi / 3);
  rectShape.setOutlineThickness(5.0f);
  rectShape.setOutlineColor(gf::Color::lighter(gf::Color::Blue));

  gf::CircleShape circleShape(100.0f);
  circleShape.setPosition({ 50.0f, 50.0f });
  circleShape.setColor(gf::Color::Red);
  circleShape.setOutlineThickness(10.0f);
  circleShape.setOutlineColor(gf::Color::darker(gf::Color::Red));

  gf::StarShape starShape(50.0f, 100.0f, 5);
  starShape.setPosition({ 400.0f, 50.0f });
  starShape.setColor(gf::Color::Yellow);

  bool withEffects = false;

  std::cout << "Gamedev Framework (gf) example #31: Pipeline\n";
  std::cout << "This example prints some shapes with several post-processing effects.\n";
  std::cout << "The effects are: EdgeEffect + AntiAliasingEffect\n";
  std::cout << "How to use:\n";
  std::cout << "\tSpace: Activate/deactivate effects\n";

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
              withEffects = !withEffects;

              if (withEffects) {
                std::cout << "With effects\n";
                renderer.addEffect(edgeEffect);
                renderer.addEffect(antiAliasingEffect);
              } else {
                std::cout << "Without effects\n";
                renderer.clearEffects();
              }

              break;

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
           }

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(rectShape);
    renderer.draw(circleShape);
    renderer.draw(starShape);
    renderer.display();
  }

  return 0;
}
