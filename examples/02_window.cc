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
#include <cstdlib>

#include <gf/Cursor.h>
#include <gf/Event.h>
#include <gf/Image.h>
#include <gf/Window.h>

int main() {
  gf::Window window("02_window", { 640, 480 }, ~gf::WindowHints::Resizable);

  std::cout << "Gamedev Framework (gf) example #02: Window\n";
  std::cout << "This example prints all window events and show different cursors.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1...: Show a system cursor\n";
  std::cout << "\tBackspace: Show an image cursor\n";

  gf::Image image;

  if (!image.loadFromFile("02_cursor.png")) {
    return EXIT_FAILURE;
  }

  gf::Cursor cursor;

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

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::Escape:
              window.close();
              break;

            case gf::Scancode::Num1:
              cursor.loadFromSystem(gf::Cursor::Arrow);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num2:
              cursor.loadFromSystem(gf::Cursor::ArrowWait);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num3:
              cursor.loadFromSystem(gf::Cursor::Wait);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num4:
              cursor.loadFromSystem(gf::Cursor::Text);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num5:
              cursor.loadFromSystem(gf::Cursor::Hand);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num6:
              cursor.loadFromSystem(gf::Cursor::SizeHorizontal);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num7:
              cursor.loadFromSystem(gf::Cursor::SizeVertical);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num8:
              cursor.loadFromSystem(gf::Cursor::SizeTopLeftBottomRight);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num9:
              cursor.loadFromSystem(gf::Cursor::SizeBottomLeftTopRight);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Num0:
              cursor.loadFromSystem(gf::Cursor::SizeAll);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Minus:
              cursor.loadFromSystem(gf::Cursor::Cross);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Equals:
              cursor.loadFromSystem(gf::Cursor::NotAllowed);
              window.setMouseCursor(cursor);
              break;

            case gf::Scancode::Backspace:
              cursor.loadFromImage(image, { 4u, 4u });
              window.setMouseCursor(cursor);
              break;

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
