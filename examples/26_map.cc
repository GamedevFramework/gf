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
#include <gf/Map.h>
#include <gf/Math.h>
#include <gf/Particles.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>
#include <gf/Window.h>

static constexpr int GridSize = 60;
static constexpr float CellSize = 10.0f;
static constexpr int Size = static_cast<int>(GridSize * CellSize);

static const char *ExampleMap[GridSize] = {
  "############################################################",
  "#                           #            #                 #",
  "#    ##         ###         #            #                 #",
  "#   ###         ###                                        #",
  "#         ##   ###          #            #                 #",
  "#         #     ##     ######            #                 #",
  "#        ##     ##    ########### ########### ######## #####",
  "###     #####        ########         #          #         #",
  "####               ##########         #          #         #",
  "####                #####   #         #                    #",
  "#####    ####                                    #         #",
  "##       ######             #         #          #         #",
  "#            ####           #         #          #         #",
  "#      #########      ##    ############# ############ #####",
  "#         ###         ##    #               #              #",
  "#        #  ##      ####    #               #              #",
  "#      ##  ###     #####    #       #       #              #",
  "#     ###   ##              #       #       #              #",
  "#     ####                  #       #       #      ##      #",
  "#     ####                          #              ##      #",
  "#                           #       #       #              #",
  "#               ##          #               #              #",
  "#              ###          #               #              #",
  "#              ##           ####################### ########",
  "#              ###                                         #",
  "#              ##                                          #",
  "#                                                          #",
  "#               ##########################                 #",
  "#               #                        #                 #",
  "#               #                        #                 #",
  "#               #                        #                 #",
  "#                                        #                 #",
  "#                                        #                 #",
  "#                                        #                 #",
  "#                                        #                 #",
  "#                                        #                 #",
  "#                                        #                 #",
  "#                                        #                 #",
  "#               #                        #                 #",
  "#               #                        #                 #",
  "#               #                        #                 #",
  "#               ##########################                 #",
  "#                                                          #",
  "#                                                          #",
  "#                                                          #",
  "#                           ###################      ### ###",
  "#                       #####          #    # ########## # #",
  "#                             # #### ###### #            # #",
  "############################### #         # ######## ##### #",
  "#    #                          ###########                #",
  "# ## # ######## # ###############           ### ####### # ##",
  "# #### ##       #               # ######### #         # ####",
  "#      ## ### # # ############# #         # # # ## ## #    #",
  "########### ### # #           # # ## ###### ###### ## #### #",
  "#               # # ########### #  # #      #         #  # #",
  "# ####### ######### #           #### # ###### ####### # ## #",
  "# #                 # ##### ###      #        #     # #    #",
  "# ################### #       ######## ########## ### #### #",
  "#                     # #####       #        #             #",
  "############################################################"
};

enum class Mode {
  FoV,
  Route,
};

int main() {
  static constexpr int ExampleMaxRadius = 12;
  static constexpr std::size_t DiagonalCostsCount = 3;
  static constexpr float DiagonalCosts[DiagonalCostsCount] = { 0.0f, 1.0f, gf::Sqrt2 };

  gf::Window window("26_map", { Size, Size }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #26: Map\n";
  std::cout << "This example shows field of vision and route finding in a square grid.\n";
  std::cout << "How to use:\n";
  std::cout << "\tM: change mode between field of vision and route finding\n";
  std::cout << "\tEscape: Close the window\n";
  std::cout << "How to use (Mode: FoV):\n";
  std::cout << "\tMouse move: Set the origin of the field of vision\n";
  std::cout << "\tR: Toggle max radius betwen 0 (no limit) and " << ExampleMaxRadius << '\n';
  std::cout << "\tC: Clear the explored cells\n";
  std::cout << "How to use (Mode: Route):\n";
  std::cout << "\tMouse button: Set the first end point\n";
  std::cout << "\tMouse move: Set the second end point\n";
  std::cout << "\tD: Toggle diagonal cost between 0 (no diagonal), 1 and sqrt(2)\n";
  std::cout << "\tR: Toggle route algorithm between Dijkstra and A*\n";
  std::cout << '\n';

  gf::SquareMap map({ GridSize, GridSize });
  gf::Grid grid = gf::Grid::createOrthogonal({ GridSize, GridSize }, { CellSize, CellSize });
  grid.setColor(gf::Color::Azure);

  // build a map

  for (auto position : map.getRange()) {
    if (ExampleMap[position.y][position.x] == ' ') {
      map.setEmpty(position);
    }
  }

  Mode mode = Mode::FoV;

  gf::Vector2i light(1, 1);
  int maxRadius = 0;

  std::size_t diagonalCostIndex = 2;
  gf::Route route = gf::Route::Dijkstra;
  gf::Vector2i start(1, 1);
  gf::Vector2i end(1, 1);
  std::vector<gf::Vector2i> points;

  gf::Vector2i position;

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.keycode) {
            case gf::Keycode::Escape:
              window.close();
              break;

            case gf::Keycode::M:
              if (mode == Mode::FoV) {
                mode = Mode::Route;
                std::cout << "Mode: Route\n";

                if (route == gf::Route::Dijkstra) {
                  std::cout << "\tRoute: Dijkstra\n";
                } else {
                  std::cout << "\tRoute: AStar\n";
                }

                std::cout << "\tDiagonal cost: " << DiagonalCosts[diagonalCostIndex] << '\n';
              } else {
                mode = Mode::FoV;
                std::cout << "Mode: FoV\n";
                std::cout << "\tMax radius: " << maxRadius << '\n';
              }
              break;

            case gf::Keycode::D:
              if (mode == Mode::Route) {
                diagonalCostIndex = (diagonalCostIndex + 1) % DiagonalCostsCount;
                std::cout << "Diagonal cost: " << DiagonalCosts[diagonalCostIndex] << '\n';
                points = map.computeRoute(start, end, DiagonalCosts[diagonalCostIndex], route);
              }
              break;

            case gf::Keycode::R:
              if (mode == Mode::Route) {
                if (route == gf::Route::Dijkstra) {
                  route = gf::Route::AStar;
                  std::cout << "Route: A*\n";
                } else {
                  route = gf::Route::Dijkstra;
                  std::cout << "Route: Dijkstra\n";
                }

                points = map.computeRoute(start, end, DiagonalCosts[diagonalCostIndex], route);
              } else {
                if (maxRadius == 0) {
                  maxRadius = ExampleMaxRadius;
                } else {
                  maxRadius = 0;
                }

                map.clearFieldOfVision();
                map.computeFieldOfVision(light, maxRadius);
                std::cout << "Max radius: " << maxRadius << '\n';
              }
              break;

            case gf::Keycode::C:
              if (mode == Mode::FoV) {
                map.clearExplored();
              }
              break;

            default:
              break;
          }
          break;

        case gf::EventType::MouseMoved:
          position = renderer.mapPixelToCoords(event.mouseCursor.coords) / CellSize;

          if (mode == Mode::Route) {
            if (position != end && map.isWalkable(position)) {
              end = position;
              points = map.computeRoute(start, end, DiagonalCosts[diagonalCostIndex], route);
            }
          } else {
            assert(mode == Mode::FoV);
            if (position != light && map.isTransparent(position)) {
              light = position;
              map.clearFieldOfVision();
              map.computeFieldOfVision(light, maxRadius);
            }
          }
          break;

        case gf::EventType::MouseButtonPressed:
          if (mode == Mode::Route) {
            position = renderer.mapPixelToCoords(event.mouseButton.coords) / CellSize;

            if (position != start && map.isWalkable(position)) {
              start = position;
              points = map.computeRoute(start, end, DiagonalCosts[diagonalCostIndex], route);
            }
          }
          break;

        default:
          break;
      }
    }

    renderer.clear();

    gf::ShapeParticles particles;

    for (auto point : map.getRange()) {
      if (!map.isWalkable(point)) {
        particles.addRectangle(point * CellSize, { CellSize, CellSize }, gf::Color::Black);
      }
    }

    if (mode == Mode::Route) {
      for (auto& point : points) {
        particles.addRectangle(point * CellSize, { CellSize, CellSize }, gf::Color::Orange);
      }
    } else {
      assert(mode == Mode::FoV);

      for (auto point : map.getRange()) {
        if (map.isInFieldOfVision(point)) {
          particles.addRectangle(point * CellSize, { CellSize, CellSize },
            map.isWalkable(point) ? gf::Color::Yellow : gf::Color::Gray());
        } else if (map.isExplored(point)) {
          particles.addRectangle(point * CellSize, { CellSize, CellSize },
            map.isWalkable(point) ? gf::Color::lighter(gf::Color::Yellow, 0.7f) : gf::Color::Gray(0.7f));
        }
      }

      particles.addRectangle(light * CellSize, { CellSize, CellSize }, gf::Color::Orange);
    }

    renderer.draw(particles);
    renderer.draw(grid);

    renderer.display();
  }

  return 0;
}
