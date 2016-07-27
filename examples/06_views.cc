/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <gf/Shapes.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

int main() {
  gf::Vector2z screenSize = { 640, 480 };
  gf::Window window("06_views", screenSize);
  gf::RenderWindow renderer(window);

  gf::RectF world(-1, -1, 2, 2);
  gf::RectF extendedWorld = world.extend(1);

  /*
   * views
   */
  gf::ViewContainer views;

  gf::StretchView stretchView;
  stretchView.reset(world);
  views.addView(stretchView);

  gf::FitView fitView;
  fitView.reset(world);
  views.addView(fitView);

  gf::FillView fillView;
  fillView.reset(world);
  views.addView(fillView);

  gf::ExtendView extendView;
  extendView.reset(world);
  views.addView(extendView);

  gf::ScreenView screenView;
  views.addView(screenView);

  views.onScreenResize(screenSize);

  gf::AdaptativeView *currentView = &stretchView;

  /*
   * some things to draw
   */

  gf::Vertex triangle[3];
  triangle[0].position = { 0.0f, 0.5f };
  triangle[0].color = gf::Color::Red;
  triangle[1].position = { 0.5f, -0.5f };
  triangle[1].color = gf::Color::Green;
  triangle[2].position = { -0.5f, -0.5f };
  triangle[2].color = gf::Color::Yellow;

  gf::RectangleShape background(world.size);
  background.setPosition(world.position);
  background.setColor(gf::Color::White);

  gf::RectangleShape extendedBackground(extendedWorld.size);
  extendedBackground.setPosition(extendedWorld.position);
  extendedBackground.setColor(gf::Color::Grey());

  gf::RectangleShape hud({ 64.0f, 64.0f });
  hud.setPosition({ 10.0f, 10.0f });
  hud.setColor(gf::Color::Red);

  std::cout << "Gamedev Framework (gf) example #06: Views\n";
  std::cout << "The scene is composed of:\n";
  std::cout << "\t- a white square with a triangle, and a grey background outside the white square;\n";
  std::cout << "\t- a red square that should stay at the top left of the window (like a HUD)\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Switch to StretchView\n";
  std::cout << "\t2: Switch to FitView\n";
  std::cout << "\t3: Switch to FillView\n";
  std::cout << "\t4: Switch to ExtendView\n";
  std::cout << "Current view: StretchView\n";

  renderer.clear(gf::Color::Black);

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
              std::cout << "Current view: StretchView\n";
              currentView = &stretchView;
              break;

            case gf::Scancode::Num2:
              std::cout << "Current view: FitView\n";
              currentView = &fitView;
              break;

            case gf::Scancode::Num3:
              std::cout << "Current view: FillView\n";
              currentView = &fillView;
              break;

            case gf::Scancode::Num4:
              std::cout << "Current view: ExtendView\n";
              currentView = &extendView;
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }

      views.update(event);
    }

    renderer.clear();

    renderer.setView(*currentView);
    renderer.draw(extendedBackground);
    renderer.draw(background);
    renderer.draw(triangle, 3, gf::PrimitiveType::Triangles);

    renderer.setView(screenView);
    renderer.draw(hud);

    renderer.display();
  }

  return 0;
}
