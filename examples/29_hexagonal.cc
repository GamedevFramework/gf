/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Grid.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Window.h>

int main() {
  constexpr gf::Vector2i GridSize = { 5 , 10 };
  constexpr float GridCellSize = 64.0f;

  gf::HexagonGrid grid(GridSize, GridCellSize, gf::Color::Black);

  gf::RectangleShape border;
  border.setSize(grid.getLocalBounds().getSize());
  border.setPosition({ 0.0f, 0.0f });
  border.setColor(gf::Color::Gray(0.9f));

  gf::Window window("29_hexagonal", grid.getLocalBounds().getSize(), ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << grid.getLocalBounds().getSize().x << ", " << grid.getLocalBounds().getSize().y << std::endl;

  std::cout << "Gamedev Framework (gf) example #29: Hexagonal\n";
  std::cout << "This example prints a hexagonal grid.\n";

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

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(border);
    renderer.draw(grid);
    renderer.display();
  }

  return 0;
}
