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

  gf::Window window("07_fullscreen", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::RectF world = gf::RectF::fromPositionSize({ -1, -1 }, { 2, 2 });
  gf::RectF extendedWorld = world.grow(1);

  /*
   * views
   */
  gf::ViewContainer views;

  gf::ExtendView extendView;
  extendView.reset(world);
  views.addView(extendView);

  views.setInitialFramebufferSize(ScreenSize);

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

  gf::RectangleShape background(world);
  background.setColor(gf::Color::White);

  gf::RectangleShape extendedBackground(extendedWorld);
  extendedBackground.setColor(gf::Color::Gray());

  std::cout << "Gamedev Framework (gf) example #07: Fullscreen\n";
  std::cout << "The scene is composed of a white square with a triangle, and a grey background outside the white square;\n";
  std::cout << "How to use:\n";
  std::cout << "\tF: Change between fullscreen/windowed\n";

  renderer.clear(gf::Color::Black);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::Resized:
          std::cout << "Resized: " << event.resize.size.width << 'x' << event.resize.size.height << '\n';
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.keycode) {
            case gf::Keycode::F:
              window.toggleFullscreen();
              break;

            case gf::Keycode::Escape:
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

    renderer.setView(extendView);

    renderer.clear();
    renderer.draw(extendedBackground);
    renderer.draw(background);
    renderer.draw(triangle, 3, gf::PrimitiveType::Triangles);
    renderer.display();
  }

  return 0;
}
