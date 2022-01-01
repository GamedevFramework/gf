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
#include <iostream>

#include <gf/Coordinates.h>
#include <gf/Event.h>
#include <gf/Logo.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Window.h>

int main() {
  constexpr gf::Vector2i InitialScreenSize = { 166, 169 };

  gf::Window window("Logo", InitialScreenSize);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ScreenView screenView;
  views.addView(screenView);

  views.setInitialFramebufferSize(InitialScreenSize);

  gf::Logo logo;
  logo.setAnchor(gf::Anchor::Center);

  std::cout << "Gamedev Framework (gf) example #98: Logo\n";
  std::cout << "This example prints the logo of Gamedev Framework (gf)\n";
  std::cout << "How to use:\n";
  std::cout << "\tS: Capture the image in 'gf_logo.png'\n";

  renderer.setView(screenView);
  renderer.clear({ 1.0f, 1.0f, 1.0f, 0.0f }); // transparent white


  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.keycode) {
            case gf::Keycode::S: {
              auto image = renderer.capture();
              image.saveToFile("gf_logo.png");
              std::cout << "Logo saved!\n";
              break;
            }

            case gf::Keycode::Space:
              window.setSize({ 200, 200 });
              break;

            case gf::Keycode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }

      views.processEvent(event);
    }

    gf::Coordinates coordinates(renderer);
    logo.setPosition(coordinates.getCenter());

    renderer.clear();
    renderer.setView(screenView);
    renderer.draw(logo);
    renderer.display();
  }

  return 0;
}
