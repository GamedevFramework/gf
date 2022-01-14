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

#include <iostream>

#include <gf/Clock.h>
#include <gf/Curves.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window("11_curves", { 800, 600 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  /*
   * A line
   */

  gf::Line line({ 0.0f, 0.0f }, { 50.0f, 150.0f });
  line.setColor(gf::Color::Rose);
  line.setWidth(10.0f);
  line.setOutlineColor(gf::Color::darker(gf::Color::Rose));
  line.setOutlineThickness(5.0f);
  line.setPosition({ 50.0f, 50.0f });

  /*
   * A quadratic Bezier curve
   */

  gf::QuadraticBezierCurve quadratic({ 0.0f, 0.0f }, { 200.0f, 400.0f }, { 300.0f, 0.0f });
  quadratic.setColor(gf::Color::Spring);
  quadratic.setWidth(5.0f);
  quadratic.setOutlineColor(gf::Color::darker(gf::Color::Spring));
  quadratic.setOutlineThickness(2.0f);
  quadratic.setPosition({ 200.0f, 50.0f });

  gf::Vector2f p0(0, 0);
  gf::Vector2f p1(100, -100);
  gf::Vector2f p2(300, 200);
  gf::Vector2f p3(450, 0);

  /*
   * A cubic Bezier curve
   */

  gf::CubicBezierCurve cubic(p0, p1, p2, p3);
  cubic.setColor(gf::Color::Azure);
  cubic.setWidth(15.0f);
  cubic.setOutlineColor(gf::Color::darker(gf::Color::Azure));
  cubic.setOutlineThickness(3.0f);
  cubic.setPosition({ 100.0f, 250.0f });

  /*
   * A simple compound curve with the control points of the cubic Bezier curve
   */

  gf::CompoundCurve compound;
  compound.setType(gf::Curve::Simple);
  compound.setOrigin(p0).lineTo(p1).lineTo(p2).lineTo(p3);
  compound.setColor(gf::Color::Black);
  compound.setWidth(2.0f);
  compound.setPosition(cubic.getPosition());

  /*
   * A closed compound curve
   */

  gf::CompoundCurve closed;
  closed.setOrigin({ 0.0f, 0.0f }).lineTo({ 100.0f, 100.0f }).lineTo({ 200.0f, 0.0f }).close();
  closed.setColor(gf::Color::Chartreuse);
  closed.setWidth(10.0f);
  closed.setOutlineColor(gf::Color::darker(gf::Color::Chartreuse));
  closed.setOutlineThickness(1.0f);
  closed.setPosition({ 50.0f, 300.0f });
  closed.rotate(gf::Pi / 8);


  /*
   * A spline
   */

  gf::Polyline polyline(gf::Polyline::Loop);
  polyline.addPoint({ 50.0f, 500.0f });
  polyline.addPoint({ 150.0f, 550.0f });
  polyline.addPoint({ 150.0f, 450.0f });
  polyline.addPoint({ 300.0f, 500.0f });
  polyline.addPoint({ 700.0f, 500.0f });
  polyline.addPoint({ 700.0f, 200.0f });
  polyline.addPoint({ 600.0f, 500.0f });

  gf::SplineCurve spline;
  spline.setControlPoints(polyline);
  spline.setColor(gf::Color::Orange);
  spline.setWidth(8.0f);
  spline.setOutlineColor(gf::Color::darker(gf::Color::Orange));
  spline.setOutlineThickness(2.0f);

  std::cout << "Gamedev Framework (gf) example #11: Curves\n";
  std::cout << "This example prints various curves.\n";

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
    renderer.draw(line);
    renderer.draw(quadratic);
    renderer.draw(cubic);
    renderer.draw(compound);
    renderer.draw(closed);
    renderer.draw(spline);
    renderer.display();
  }

  return 0;
}
