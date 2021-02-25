/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
  constexpr gf::Vector2f CellSize = { 64.0f, 64.0f };
  constexpr float IncreaseDecreaseFactor = 1.0f;

  gf::Window window("35_squaregrid", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ScreenView screenView;
  views.addView(screenView);
  views.setInitialFramebufferSize(ScreenSize);

  float lineWidth = 1.0f;
  gf::SquareGrid squareGrid(GridSize, CellSize, gf::Color::Black, lineWidth);
  squareGrid.setAnchor(gf::Anchor::Center);

  std::cout << "Gamedev Framework (gf) example #35: SquareGrid\n";
  std::cout << "This example prints a square grid.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Increase line width\n";
  std::cout << "\t2: Decrease line width\n";
  std::cout << "\t3: Switch to red color\n";
  std::cout << "\t4: Switch to green color\n";
  std::cout << "\t5: Switch to blue color\n";
  std::cout << "\t6: Switch to black color\n";

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
              lineWidth += IncreaseDecreaseFactor;
              squareGrid.setLineWidth(lineWidth);

              std::cout << "Increased line width to " << lineWidth << "\n";
              break;

            case gf::Scancode::Num2:
              lineWidth -= IncreaseDecreaseFactor;
              squareGrid.setLineWidth(lineWidth);

              std::cout << "Decreased line width to " << lineWidth << "\n";
              break;

            case gf::Scancode::Num3:
              std::cout << "Switched to red color\n";
              squareGrid.setColor(gf::Color::Red);
              break;

            case gf::Scancode::Num4:
              std::cout << "Switched to green color\n";
              squareGrid.setColor(gf::Color::Green);
              break;
            
            case gf::Scancode::Num5:
              std::cout << "Switched to blue color\n";
              squareGrid.setColor(gf::Color::Blue);
              break;
            
            case gf::Scancode::Num6:
              std::cout << "Switched to black color\n";
              squareGrid.setColor(gf::Color::Black);
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
    squareGrid.setPosition(coordinates.getCenter());

    renderer.clear();
    renderer.draw(squareGrid);
    renderer.display();
  }

  return 0;
}
