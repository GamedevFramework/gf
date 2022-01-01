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
#include <gf/Effects.h>
#include <gf/Event.h>
#include <gf/PostProcessing.h>
#include <gf/RenderWindow.h>
#include <gf/RenderTexture.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Window.h>

int main() {
  gf::Window window("30_postprocessing", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  auto framebufferSize = window.getFramebufferSize();
  gf::RenderTexture textureRenderer(framebufferSize);

  gf::DefaultEffect defaultEffect;

  gf::AntiAliasingEffect antiAliasingEffect;
  antiAliasingEffect.setFramebufferSize(framebufferSize);

  gf::EdgeEffect edgeEffect;
  edgeEffect.setFramebufferSize(framebufferSize);

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

  /*
   * post processing
   */

  gf::PostProcessing postProcessing;
  postProcessing.setTexture(textureRenderer.getTexture());


  gf::Effect *currentEffect = &defaultEffect;

  std::cout << "Gamedev Framework (gf) example #30: Post-Processing\n";
  std::cout << "This example prints some shapes with various post-processing effects.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: DefaultPostProcessingEffect (no effect)\n";
  std::cout << "\t2: AntiAliasingEffect (FXAA)\n";
  std::cout << "\t3: EdgeEffect\n";
  std::cout << "Current effect: DefaultPostProcessingEffect\n";

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

         case gf::EventType::KeyPressed:
           switch (event.key.scancode) {
            case gf::Scancode::Num1:
              std::cout << "Current effect: DefaultPostProcessingEffect\n";
              currentEffect = &defaultEffect;
              break;

            case gf::Scancode::Num2:
              std::cout << "Current effect: AntiAliasingEffect\n";
              currentEffect = &antiAliasingEffect;
              break;

            case gf::Scancode::Num3:
              std::cout << "Current effect: EdgeEffect\n";
              currentEffect = &edgeEffect;
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

    textureRenderer.setActive();
    textureRenderer.clear(gf::Color::White);
    textureRenderer.draw(rectShape);
    textureRenderer.draw(circleShape);
    textureRenderer.draw(starShape);
    textureRenderer.display();

    postProcessing.setEffect(*currentEffect);

    renderer.setActive();
    renderer.clear(gf::Color::White);
    renderer.draw(postProcessing);
    renderer.display();
  }

  return 0;
}
