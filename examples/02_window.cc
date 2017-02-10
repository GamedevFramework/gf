/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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

#include <gf/Event.h>
#include <gf/Window.h>

int main() {
  gf::Window window("02_window", { 640, 480 });

  std::cout << "Gamedev Framework (gf) example #02: Window\n";
  std::cout << "This example prints all window events.\n";

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          std::cout << "Closed\n";
          window.close();
          break;

        case gf::EventType::Resized:
          std::cout << "Resized: " << event.size.width << 'x' << event.size.height << '\n';
          break;

        case gf::EventType::FocusGained:
          std::cout << "FocusGained\n";
          break;

        case gf::EventType::FocusLost:
          std::cout << "FocusLost\n";
          break;

        default:
          break;
      }
    }

    window.display();
  }

  return 0;
}
