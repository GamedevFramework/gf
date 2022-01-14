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

#include <deque>
#include <iostream>
#include <vector>

#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Geometry.h>
#include <gf/Grid.h>
#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>
#include <gf/Window.h>

static constexpr int GridSize = 60;
static constexpr float CellSize = 10.0f;
static constexpr int Size = static_cast<int>(GridSize * CellSize);

int main() {
  gf::Window window("23_bresenham", { Size, Size }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #23: Bresenham algorithm\n";
  std::cout << "This example shows the Bresenham line drawing algorithm.\n";
  std::cout << "How to use:\n";
  std::cout << "\tMouse button: Set the first end point\n";
  std::cout << "\tMouse move: Set the second end point\n";
  std::cout << "\tEscape: Close the window\n";

  gf::Grid grid = gf::Grid::createOrthogonal({ GridSize, GridSize }, { CellSize, CellSize });
  grid.setColor(gf::Color::Azure);

  gf::Vector2i start(30, 30);
  gf::Vector2i end(40, 50);
  auto points = gf::generateLine(start, end);

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

        case gf::EventType::MouseMoved:
          end = renderer.mapPixelToCoords(event.mouseCursor.coords) / CellSize;
          points = gf::generateLine(start, end);
          break;

        case gf::EventType::MouseButtonPressed:
          start = renderer.mapPixelToCoords(event.mouseButton.coords) / CellSize;
          points = gf::generateLine(start, end);
          break;

        default:
          break;
      }
    }

    renderer.clear();

    for (auto& point : points) {
      gf::RectangleShape shape({ CellSize, CellSize });
      shape.setColor(gf::Color::Orange);
      shape.setPosition(point * CellSize);
      renderer.draw(shape);
    }

    renderer.draw(grid);

    renderer.display();
  }

  return 0;
}
