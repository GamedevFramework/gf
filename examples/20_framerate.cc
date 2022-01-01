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

#include <iostream>
#include <string>

#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Text.h>
#include <gf/Window.h>

struct FramerateProperties {
  bool vsync;
  bool limit;
  unsigned int framerate;
};

int main() {
  gf::Window window("20_framerate", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font("assets/DejaVuSans.ttf");

  FramerateProperties prop;
  prop.vsync = window.isVerticalSyncEnabled();
  prop.limit = false;
  prop.framerate = 60;

  gf::Text text;
  text.setCharacterSize(15);
  text.setFont(font);
  text.setPosition({ 20.0f, 20.0f });

  std::cout << "Gamedev Framework (gf) example #20: Framerate\n";
  std::cout << "This example shows how the framerate may be limited\n";
  std::cout << "How to use:\n";
  std::cout << "\tReturn: Toggle vertical synchronization\n";
  std::cout << "\tSpace: Toggle framerate limit\n";
  std::cout << "\tPageUp/PageDown: Increase/decrease the framerate limit\n";

  gf::Clock clock;

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
            case gf::Scancode::Escape:
              window.close();
              break;

            case gf::Scancode::PageUp:
              if (prop.limit) {
                ++prop.framerate;
                window.setFramerateLimit(prop.framerate);
              }
              break;

            case gf::Scancode::PageDown:
              if (prop.limit) {
                --prop.framerate;
                window.setFramerateLimit(prop.framerate);
              }
              break;

            case gf::Scancode::Space:
              prop.limit = !prop.limit;

              if (prop.limit) {
                window.setFramerateLimit(prop.framerate);
              } else {
                window.setFramerateLimit(0);
              }
              break;

            case gf::Scancode::Return:
              prop.vsync = !prop.vsync;
              window.setVerticalSyncEnabled(prop.vsync);
              break;

            default:
              break;

          }
          break;

        default:
          break;
      }
    }

    float dt = clock.restart().asSeconds();

    {
      std::string str = "Vertical sync: ";
      str += prop.vsync ? "active" : "inactive";
      str += "\nFramerate limit: ";

      if (!prop.limit) {
        str += "inactive";
      } else {
        str += std::to_string(prop.framerate);
      }

      str += "\nFramerate: ";
      str += std::to_string(1.0f / dt);

      text.setString(std::move(str));

      text.setAnchor(gf::Anchor::TopLeft);
    }

    renderer.clear();
    renderer.draw(text);
    renderer.display();
  }

  return 0;
}
