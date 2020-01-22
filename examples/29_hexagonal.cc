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
#include <gf/Coordinates.h>
#include <gf/Event.h>
#include <gf/Grid.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);
  constexpr gf::Vector2i GridSize = { 7, 7 };
  constexpr float HexagonRadius = 32.0f;

  gf::Window window("29_hexagonal", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ScreenView screenView;
  views.addView(screenView);
  views.setInitialScreenSize(ScreenSize);

  gf::HexagonGrid gridPointy(gf::MapCellAxis::X, GridSize, HexagonRadius, gf::Color::Black);
  gf::HexagonGrid gridFlat(gf::MapCellAxis::Y, GridSize, HexagonRadius, gf::Color::Black);

  gf::HexagonGrid *currentGrid = &gridPointy;

  std::cout << "Gamedev Framework (gf) example #29: Hexagonal\n";
  std::cout << "This example prints a hexagonal grid.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Switch to Pointy grid\n";
  std::cout << "\t2: Switch to Flat grid\n";
  std::cout << "Current view: Pointy grid\n";

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
            case gf::Scancode::Num1:
              std::cout << "Current view: Pointy grid\n";
              currentGrid = &gridPointy;
              break;

            case gf::Scancode::Num2:
              std::cout << "Current view: Flat grid\n";
              currentGrid = &gridFlat;
              break;

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }

      views.processEvent(event);
    }

    gf::Coordinates coordinates(renderer);
    currentGrid->setPosition(coordinates.getCenter());
    currentGrid->setAnchor(gf::Anchor::Center);

    renderer.clear();
    renderer.draw(*currentGrid);
    renderer.display();
  }

  return 0;
}
