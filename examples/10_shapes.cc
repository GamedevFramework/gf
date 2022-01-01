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
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Window.h>

int main() {
  gf::Window window("10_shapes", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::RectangleShape rectShape({ 250.0f, 100.0f });
  rectShape.setColor(gf::Color::Azure);
  rectShape.setPosition({ 400.0f, 200.0f });
  rectShape.setRotation(gf::Pi / 3);
  rectShape.setOutlineThickness(5.0f);
  rectShape.setOutlineColor(gf::Color::darker(gf::Color::Azure));

  gf::CircleShape circleShape(100.0f);
  circleShape.setPosition({ 50.0f, 50.0f });
  circleShape.setColor(gf::Color::Rose);
  circleShape.setOutlineThickness(10.0f);
  circleShape.setOutlineColor(gf::Color::lighter(gf::Color::Rose));

  gf::StarShape starShape(50.0f, 100.0f, 5);
  starShape.setPosition({ 400.0f, 50.0f });
  starShape.setColor(gf::Color::Chartreuse);

  gf::RoundedRectangleShape roundedRectangleShape({ 250.0f, 100.0f }, 30.0f);
  roundedRectangleShape.setPosition({ 50.0f, 350.0f });
  roundedRectangleShape.setColor(gf::Color::Orange);
  roundedRectangleShape.setOutlineThickness(5.0f);
  roundedRectangleShape.setOutlineColor(gf::Color::darker(gf::Color::Orange));

  gf::Pie pie(70.0f, 0.0f, gf::Pi / 3);
  pie.setPosition({ 250.0f, 40.0f });
  pie.setColor(gf::Color::Spring);
  pie.setOutlineColor(gf::Color::darker(gf::Color::Spring));
  pie.setOutlineThickness(3.0f);

  std::cout << "Gamedev Framework (gf) example #10: Shapes\n";
  std::cout << "This example prints various shapes.\n";

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
    renderer.draw(rectShape);
    renderer.draw(circleShape);
    renderer.draw(starShape);
    renderer.draw(roundedRectangleShape);
    renderer.draw(pie);
    renderer.display();
  }

  return 0;
}
