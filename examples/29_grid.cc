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

#include <string>
#include <iostream>

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Event.h>
#include <gf/Grid.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

struct NamedGrid {
  std::string name;
  gf::Grid grid;
};

using namespace std::literals;

int main() {
  static constexpr gf::Vector2i ScreenSize(1600, 900);
  constexpr gf::Vector2i GridSize = { 7, 7 };
  constexpr float HexagonRadius = 64.0f;
  constexpr gf::Vector2f CellSize = { 128.0f, 128.0f };
  constexpr gf::Vector2f StaggeredCellSize = { 128.0f, 112.0f };

  gf::Window window("29_grid", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ScreenView screenView;
  views.addView(screenView);
  views.setInitialFramebufferSize(ScreenSize);

  NamedGrid grids[] = {
    { "Orthogonal"s, gf::Grid::createOrthogonal(GridSize, CellSize) },
    { "Staggered X Odd"s, gf::Grid::createStaggered(GridSize, StaggeredCellSize, gf::CellAxis::X, gf::CellIndex::Odd) },
    { "Staggered X Even"s, gf::Grid::createStaggered(GridSize, StaggeredCellSize, gf::CellAxis::X, gf::CellIndex::Even) },
    { "Staggered Y Odd"s, gf::Grid::createStaggered(GridSize, StaggeredCellSize, gf::CellAxis::Y, gf::CellIndex::Odd) },
    { "Staggered Y Even"s, gf::Grid::createStaggered(GridSize, StaggeredCellSize, gf::CellAxis::Y, gf::CellIndex::Even) },
    { "Hexagonal X Odd"s, gf::Grid::createHexagonal(GridSize, HexagonRadius, gf::CellAxis::X, gf::CellIndex::Odd) },
    { "Hexagonal X Even"s, gf::Grid::createHexagonal(GridSize, HexagonRadius, gf::CellAxis::X, gf::CellIndex::Even) },
    { "Hexagonal Y Odd"s, gf::Grid::createHexagonal(GridSize, HexagonRadius, gf::CellAxis::Y, gf::CellIndex::Odd) },
    { "Hexagonal Y Even"s, gf::Grid::createHexagonal(GridSize, HexagonRadius, gf::CellAxis::Y, gf::CellIndex::Even) },
  };

  std::size_t current = 0;
  std::size_t count = std::size(grids);

  std::cout << "Gamedev Framework (gf) example #29: Hexagonal\n";
  std::cout << "This example prints a hexagonal grid.\n";
  std::cout << "How to use:\n";
  std::cout << "\tPageDown: Switch to next grid\n";
  std::cout << "\tPageUp: Switch to previous grid\n";

  std::cout << "Current grid: " << grids[current].name << "\n";

  renderer.clear(gf::Color::White);
  renderer.setView(screenView);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::PageDown:
              current = (current + 1) % count;
              std::cout << "Current grid: " << grids[current].name << "\n";
              break;

            case gf::Scancode::PageUp:
              current = (current + count - 1) % count;
              std::cout << "Current grid: " << grids[current].name << "\n";
              break;

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        case gf::EventType::MouseMoved:
          grids[current].grid.hover(renderer.mapPixelToCoords(event.mouseCursor.coords));
          break;

        case gf::EventType::MouseButtonPressed:
        {
          auto coords = renderer.mapPixelToCoords(event.mouseButton.coords);
          auto localCoords = gf::transform(grids[current].grid.getInverseTransform(), coords);
          auto position = grids[current].grid.getCells().computeCoordinates(localCoords);
          std::cout << "Position: " << position.x << ',' << position.y << '\n';
          break;
        }

        default:
          break;
      }

      views.processEvent(event);
    }

    gf::Grid& grid = grids[current].grid;

    gf::Coordinates coordinates(renderer);
    grid.setPosition(coordinates.getCenter());
    grid.setAnchor(gf::Anchor::Center);
    grid.setLineWidth(1.5f);

    gf::RectangleShape shape(grid.getLocalBounds());
    shape.setPosition(grid.getPosition());
    shape.setAnchor(gf::Anchor::Center);
    shape.setColor(gf::Color::Transparent);
    shape.setOutlineColor(gf::Color::Red);
    shape.setOutlineThickness(1.0f);

    renderer.clear();
    renderer.draw(grid);
    renderer.draw(shape);
    renderer.display();
  }

  return 0;
}
