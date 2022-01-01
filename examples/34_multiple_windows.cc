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

#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window0("window0", {640, 480}, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer0(window0);

  gf::Window window1("window1", {640, 480}, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer1(window1);

  while (window0.isOpen() || window1.isOpen()) {
    if(window0.isOpen()) {
      gf::Event event;

      while (window0.pollEvent(event)) {
        switch (event.type) {
          case gf::EventType::Closed:
            assert(event.window.windowId == window0.getWindowId());
            std::cout << "Close window #0\n";
            window0.close();
            break;
          case gf::EventType::KeyPressed:
            if (event.key.scancode == gf::Scancode::Escape) {
              assert(event.key.windowId == window0.getWindowId());
              std::cout << "Close window #0\n";
              window0.close();
            }
            break;
          default:
            break;
        }
      }

      renderer0.setActive();
      renderer0.clear(gf::Color::Red);
      renderer0.display();
    }

    if(window1.isOpen()) {
      gf::Event event;

      while (window1.pollEvent(event)) {
        switch (event.type) {
          case gf::EventType::Closed:
            assert(event.window.windowId == window1.getWindowId());
            std::cout << "Close window #1\n";
            window1.close();
            break;
          case gf::EventType::KeyPressed:
            if (event.key.scancode == gf::Scancode::Escape) {
              assert(event.key.windowId == window1.getWindowId());
              std::cout << "Close window #1\n";
              window1.close();
            }
            break;
          default:
            break;
        }
      }

      renderer1.setActive();
      renderer1.clear(gf::Color::Blue);
      renderer1.display();
    }
  }

  return 0;
}
