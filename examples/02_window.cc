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
#include <cstdlib>

#include <gf/Cursor.h>
#include <gf/Event.h>
#include <gf/Image.h>
#include <gf/Window.h>

namespace {

  void setSystemCursor(gf::Window& window, gf::Cursor& cursor, gf::Cursor::Type type) {
    gf::Cursor newCursor(type);
    cursor = std::move(newCursor);
    window.setMouseCursor(cursor);
  }

}

int main() {
  gf::Window window("02_window", { 640, 480 }, ~gf::WindowHints::Resizable);

  std::cout << "Gamedev Framework (gf) example #02: Window\n";
  std::cout << "This example prints all window events and show different cursors.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1...: Show a system cursor\n";
  std::cout << "\tBackspace: Show an image cursor\n";

  gf::Image image("assets/cursor.png");
  gf::Cursor cursor;

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Quit:
          std::cout << "Quit";
          break;

        case gf::EventType::Closed:
          std::cout << "Closed: " << event.window.windowId << '\n';
          window.close();
          break;

        case gf::EventType::Resized:
          std::cout << "Resized: " << event.resize.size.width << 'x' << event.resize.size.height << ' ' << event.resize.windowId << '\n';
          break;

        case gf::EventType::FocusGained:
          std::cout << "FocusGained: " << event.window.windowId << '\n';
          break;

        case gf::EventType::FocusLost:
          std::cout << "FocusLost: " << event.window.windowId << '\n';
          break;

        case gf::EventType::Shown:
          std::cout << "Shown: " << event.window.windowId << '\n';
          break;

        case gf::EventType::Hidden:
          std::cout << "Hidden: " << event.window.windowId << '\n';
          break;

        case gf::EventType::Exposed:
          std::cout << "Exposed: " << event.window.windowId << '\n';
          break;

        case gf::EventType::Maximized:
          std::cout << "Maximized: " << event.window.windowId << '\n';
          break;

        case gf::EventType::Minimized:
          std::cout << "Minimized: " << event.window.windowId << '\n';
          break;

        case gf::EventType::Restored:
          std::cout << "Restored: " << event.window.windowId << '\n';
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::Escape:
              window.close();
              break;

            case gf::Scancode::Num1:
              setSystemCursor(window, cursor, gf::Cursor::Arrow);
              break;

            case gf::Scancode::Num2:
              setSystemCursor(window, cursor, gf::Cursor::ArrowWait);
              break;

            case gf::Scancode::Num3:
              setSystemCursor(window, cursor, gf::Cursor::Wait);
              break;

            case gf::Scancode::Num4:
              setSystemCursor(window, cursor, gf::Cursor::Text);
              break;

            case gf::Scancode::Num5:
              setSystemCursor(window, cursor, gf::Cursor::Hand);
              break;

            case gf::Scancode::Num6:
              setSystemCursor(window, cursor, gf::Cursor::SizeHorizontal);
              break;

            case gf::Scancode::Num7:
              setSystemCursor(window, cursor, gf::Cursor::SizeVertical);
              break;

            case gf::Scancode::Num8:
              setSystemCursor(window, cursor, gf::Cursor::SizeTopLeftBottomRight);
              break;

            case gf::Scancode::Num9:
              setSystemCursor(window, cursor, gf::Cursor::SizeBottomLeftTopRight);
              break;

            case gf::Scancode::Num0:
              setSystemCursor(window, cursor, gf::Cursor::SizeAll);
              break;

            case gf::Scancode::Minus:
              setSystemCursor(window, cursor, gf::Cursor::Cross);
              break;

            case gf::Scancode::Equals:
              setSystemCursor(window, cursor, gf::Cursor::NotAllowed);
              break;

            case gf::Scancode::Backspace: {
              gf::Cursor newCursor(image, { 4, 4 });
              cursor = std::move(newCursor);
              window.setMouseCursor(cursor);
              break;
            }

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    window.display();
  }

  return 0;
}
