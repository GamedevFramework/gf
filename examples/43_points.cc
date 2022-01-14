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
#include <chrono>
#include <iostream>

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Event.h>
#include <gf/Geometry.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Triangulation.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/Window.h>


int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);
  static constexpr gf::RectF World = gf::RectF::fromPositionSize({ -500.0f, -500.0f }, { 1000.0f, 1000.0f });

  gf::Window window("43_points", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ExtendView view(World);
  view.setInitialFramebufferSize(ScreenSize);

  renderer.setView(view);

  gf::Random random;

  static constexpr std::size_t PointsCount = 30;
  std::vector<gf::Vector2f> points;

  for (std::size_t i = 0; i < PointsCount; ++i) {
    gf::Vector2f p;
    p.x = random.computeUniformFloat(-500.0f, 500.0f);
    p.y = random.computeUniformFloat(-500.0f, 500.0f);
    points.push_back(p);
  }

  auto hull = gf::convexHull(points);
  auto delaunay = gf::triangulation(points);

  enum { Hull, Delaunay } mode = Hull;

  std::cout << "Gamedev Framework (gf) example #43: Points\n";
  std::cout << "How to use:\n";
  std::cout << "\tSpace: generate new points\n";

  gf::Clock clock;
  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::Space:
              points.clear();

              for (std::size_t i = 0; i < PointsCount; ++i) {
                gf::Vector2f p;
                p.x = random.computeUniformFloat(-500.0f, 500.0f);
                p.y = random.computeUniformFloat(-500.0f, 500.0f);
                points.push_back(p);
              }

              hull = gf::convexHull(points);
              delaunay = gf::triangulation(points);
              break;

            case gf::Scancode::Return:
              if (mode == Hull) {
                mode = Delaunay;
              } else {
                mode = Hull;
              }
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
    }


    renderer.clear();

    if (mode == Hull && !hull.isEmpty()) {
      gf::CompoundCurve curve(hull.getPoint(0));

      bool first = true;

      for (auto p : hull) {
        if (first) {
          first = false;
          continue;
        }

        curve.lineTo(p);
      }

      curve.close();
      curve.setColor(gf::Color::Azure);
      curve.setWidth(2.0f);
      renderer.draw(curve);
    }

    if (mode == Delaunay && !delaunay.empty()) {
      for (auto& triangle : delaunay) {
        for (std::size_t i = 0; i < 3; ++i) {
          gf::Line line(triangle[i], triangle[(i + 1) % 3]);
          line.setColor(gf::Color::Azure);
          line.setWidth(2.0f);
          renderer.draw(line);
        }
      }
    }

    for (auto p : points) {
      gf::CircleShape circle(5.0f);
      circle.setColor(gf::Color::Orange);
      circle.setPosition(p);
      circle.setAnchor(gf::Anchor::Center);
      renderer.draw(circle);
    }

    renderer.display();
  }

  return 0;
}
