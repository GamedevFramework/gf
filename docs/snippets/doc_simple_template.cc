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

/// [simple]
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  // initialization

  /// [init]
  gf::Window window("My great game", { 1024, 768 });
  gf::RenderWindow renderer(window);
  /// [init]

  // game loop

  /// [clock]
  gf::Clock clock;
  /// [clock]
  /// [clear]
  renderer.clear(gf::Color::White);
  /// [clear]

  while (window.isOpen()) {
    /// [input]
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

      case gf::EventType::KeyPressed:

        break;

      default:
        break;
      }
    }
    /// [input]

    /// [update]
    // 2. update

    float dt = clock.restart().asSeconds();
    /// [update]

    /// [draw]
    // 3. draw

    renderer.clear();

    // draw everything

    renderer.display();
    /// [draw]
  }

  return 0;
}
/// [simple]
