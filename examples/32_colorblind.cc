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
  gf::Window window("32_colorblind", { 1280,  768 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  auto framebufferSize = window.getFramebufferSize();
  gf::RenderTexture textureRenderer(framebufferSize);

  gf::ColorBlindEffect effect(gf::ColorBlindEffect::Normal);

  /*
   * Wesnoth screenshot
   */

  gf::Texture texture("assets/wesnoth.png");
  texture.setSmooth();

  gf::Sprite sprite(texture);

  /*
   * post processing
   */

  gf::PostProcessing postProcessing;
  postProcessing.setTexture(textureRenderer.getTexture());
  postProcessing.setEffect(effect);

  std::cout << "Gamedev Framework (gf) example #32: Colorblind\n";
  std::cout << "This example prints a Wesnoth screenshot with a simulation of color blindness.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Normal vision\n";
  std::cout << "\t2: Protanopia\n";
  std::cout << "\t3: Protanomaly\n";
  std::cout << "\t4: Deuteranopia\n";
  std::cout << "\t5: Deuteranomaly\n";
  std::cout << "\t6: Tritanopia\n";
  std::cout << "\t7: Tritanomaly\n";
  std::cout << "\t8: Achromatopsia\n";
  std::cout << "\t9: Achromatomaly\n";

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
              std::cout << "Normal\n";
              effect.setType(gf::ColorBlindEffect::Normal);
              break;

            case gf::Scancode::Num2:
              std::cout << "Protanopia\n";
              effect.setType(gf::ColorBlindEffect::Protanopia);
              break;

            case gf::Scancode::Num3:
              std::cout << "Protanomaly\n";
              effect.setType(gf::ColorBlindEffect::Protanomaly);
              break;

            case gf::Scancode::Num4:
              std::cout << "Deuteranopia\n";
              effect.setType(gf::ColorBlindEffect::Deuteranopia);
              break;

            case gf::Scancode::Num5:
              std::cout << "Deuteranomaly\n";
              effect.setType(gf::ColorBlindEffect::Deuteranomaly);
              break;

            case gf::Scancode::Num6:
              std::cout << "Tritanopia\n";
              effect.setType(gf::ColorBlindEffect::Tritanopia);
              break;

            case gf::Scancode::Num7:
              std::cout << "Tritanomaly\n";
              effect.setType(gf::ColorBlindEffect::Tritanomaly);
              break;

            case gf::Scancode::Num8:
              std::cout << "Achromatopsia\n";
              effect.setType(gf::ColorBlindEffect::Achromatopsia);
              break;

            case gf::Scancode::Num9:
              std::cout << "Achromatomaly\n";
              effect.setType(gf::ColorBlindEffect::Achromatomaly);
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
    textureRenderer.draw(sprite);
    textureRenderer.display();

    renderer.setActive();
    renderer.clear(gf::Color::White);
    renderer.draw(postProcessing);
    renderer.display();
  }

  return 0;
}
