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
#include <gf/Vertex.h>
#include <gf/Window.h>

int main() {
  gf::Window window("05_triangle", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::View view({ 0.0f, 0.0f }, { 2.0f, 2.0f });
  renderer.setView(view);

  gf::Vertex triangle[3];
  triangle[0].position = { 0.0f, 0.5f };
  triangle[0].color = gf::Color::Red;
  triangle[1].position = { 0.5f, -0.5f };
  triangle[1].color = gf::Color::Green;
  triangle[2].position = { -0.5f, -0.5f };
  triangle[2].color = gf::Color::Yellow;

  gf::Vector2f worldPos;

  std::cout << "Gamedev Framework (gf) example #05: Triangle\n";
  std::cout << "This example prints the hello world triangle of OpenGL.\n";
  std::cout << "You can click on the screen to see the screen coordinates and world coordinates.\n";

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          if (event.key.scancode == gf::Scancode::Escape) {
            window.close();
          }
          break;

        case gf::EventType::MouseButtonPressed:
          worldPos = renderer.mapPixelToCoords(event.mouseButton.coords);
          std::cout << "Mouse:\n"
            << "\tScreen coordinates: " << event.mouseButton.coords.x << 'x' << event.mouseButton.coords.y << '\n'
            << "\tWorld coordinates: " << worldPos.x << 'x' << worldPos.y << '\n';
          break;

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(triangle, 3, gf::PrimitiveType::Triangles);
    renderer.display();
  }

  return 0;
}
