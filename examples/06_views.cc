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
#include <gf/Shapes.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);

  gf::Window window("06_views", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::RectF world = gf::RectF::fromPositionSize({ -100, -100 }, { 200, 200 });
  gf::RectF extendedWorld = world.grow(100);

  /*
   * views
   */
  gf::ViewContainer views;

  gf::StretchView stretchView(world);
  views.addView(stretchView);

  gf::FitView fitView(world);
  views.addView(fitView);

  gf::FillView fillView(world);
  views.addView(fillView);

  gf::ExtendView extendView(world);
  views.addView(extendView);

  gf::LockedView lockedView(world);
  views.addView(lockedView);

  gf::ScreenView screenView;
  views.addView(screenView);

  views.setInitialFramebufferSize(ScreenSize);

  gf::AdaptativeView *currentView = &stretchView;

  /*
   * viewports
   */

  gf::RectF maxiViewport = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f });
  gf::RectF miniViewport = gf::RectF::fromPositionSize({ 0.75f, 0.15f }, { 0.20f, 0.40f });

  /*
   * some things to draw
   */

  gf::Vertex triangle[3];
  triangle[0].position = { 0.0f, 50.0f };
  triangle[0].color = gf::Color::Red;
  triangle[1].position = { 50.0f, -50.0f };
  triangle[1].color = gf::Color::Green;
  triangle[2].position = { -50.0f, -50.0f };
  triangle[2].color = gf::Color::Yellow;

  gf::RectangleShape background(world);
  background.setColor(gf::Color::White);

  gf::RectangleShape extendedBackground(extendedWorld);
  extendedBackground.setColor(gf::Color::Gray());

  gf::RectangleShape hud({ 64.0f, 64.0f });
  hud.setPosition({ 10.0f, 10.0f });
  hud.setColor(gf::Color::Red);

  gf::RectangleShape frame(miniViewport.getSize() * ScreenSize);
  frame.setPosition(miniViewport.getPosition() * ScreenSize);
  frame.setColor(gf::Color::Transparent);
  frame.setOutlineColor(gf::Color::Red);
  frame.setOutlineThickness(2.0f);

  std::cout << "Gamedev Framework (gf) example #06: Views\n";
  std::cout << "The scene is composed of:\n";
  std::cout << "\t- a white square with a triangle, and a grey background outside the white square;\n";
  std::cout << "\t- a red square that should stay at the top left of the window (like a HUD)\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Switch to StretchView\n";
  std::cout << "\t2: Switch to FitView\n";
  std::cout << "\t3: Switch to FillView\n";
  std::cout << "\t4: Switch to ExtendView\n";
  std::cout << "\t5: Switch to LockedView\n";
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

            case gf::Scancode::Num5:
              std::cout << "Current view: LockedView\n";
              currentView = &lockedView;
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

    renderer.clear();

    // draw with maxi viewport

    currentView->setViewport(maxiViewport);
    renderer.setView(*currentView);

    renderer.draw(extendedBackground);
    renderer.draw(background);
    renderer.draw(triangle, 3, gf::PrimitiveType::Triangles);

    // draw with mini viewport

    currentView->setViewport(miniViewport);
    renderer.setView(*currentView);

    renderer.draw(extendedBackground);
    renderer.draw(background);
    renderer.draw(triangle, 3, gf::PrimitiveType::Triangles);

    // draw hud and frame

    gf::RectI viewport = renderer.getViewport(*currentView);
    frame.setPosition(viewport.getPosition());
    frame.setSize(viewport.getSize());

    renderer.setView(screenView);
    renderer.draw(frame);
    renderer.draw(hud);

    renderer.display();
  }

  return 0;
}
