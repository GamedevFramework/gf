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

#include <gf/Clock.h>
#include <gf/Collision.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/Window.h>

static constexpr float Speed = 1.0f;

int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);
  static constexpr gf::RectF World = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 15.0f, 15.0f });

  gf::Window window("41_collision", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ExtendView view(World);
  view.setInitialFramebufferSize(ScreenSize);

  renderer.setView(view);

  gf::CircF circle({ 14.0f, 0.0f }, 2.0f);

  gf::Polygon polygon1;
  polygon1.addPoint({ 4.0f, 11.0f });
  polygon1.addPoint({ 9.0f,  9.0f });
  polygon1.addPoint({ 4.0f,  5.0f });

  gf::Polygon polygon2;
  polygon2.addPoint({  5.0f, 7.0f });
  polygon2.addPoint({ 12.0f, 7.0f });
  polygon2.addPoint({ 10.0f, 2.0f });
  polygon2.addPoint({  7.0f, 3.0f });



  gf::Penetration p;
  bool isColliding = gf::collides(polygon1, polygon2, p) || gf::collides(circle, polygon2, p);

  bool isMoving = false;
  gf::Vector2f velocity(0.0f, 0.0f);

  std::cout << "Gamedev Framework (gf) example #40: Collision\n";
  std::cout << "This example shows the collision between two polygons.\n";
  std::cout << "The red arrow is the normal of the collision.\n";
  std::cout << "How to use:\n";
  std::cout << "\tUp/Down/Left/Right: move the blue polygon\n";

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
            case gf::Scancode::Up:
              isMoving = true;
              velocity = gf::Vector2f(0.0f, -Speed);
              break;

            case gf::Scancode::Down:
              isMoving = true;
              velocity = gf::Vector2f(0.0f, Speed);
              break;

            case gf::Scancode::Left:
              isMoving = true;
              velocity = gf::Vector2f(-Speed, 0.0f);
              break;

            case gf::Scancode::Right:
              isMoving = true;
              velocity = gf::Vector2f(Speed, 0.0f);
              break;

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        case gf::EventType::KeyReleased:
          switch (event.key.scancode) {
            case gf::Scancode::Up:
              isMoving = false;
              break;

            case gf::Scancode::Down:
              isMoving = false;
              break;

            case gf::Scancode::Left:
              isMoving = false;
              break;

            case gf::Scancode::Right:
              isMoving = false;
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    float dt = clock.restart().asSeconds();

    if (isMoving) {
      gf::Matrix3f mat = gf::translation(dt * velocity);
      polygon2.applyTransform(mat);
      isColliding = gf::collides(polygon1, polygon2, p) || gf::collides(circle, polygon2, p);
    }

    renderer.clear();

    {
      gf::CircleShape shape0(circle);
      shape0.setColor(gf::Color::Transparent);
      shape0.setOutlineColor(gf::Color::Yellow);
      shape0.setOutlineThickness(0.05f);
      renderer.draw(shape0);

      gf::ConvexShape shape1(polygon1);
      shape1.setColor(gf::Color::Transparent);
      shape1.setOutlineColor(gf::Color::Magenta);
      shape1.setOutlineThickness(0.05f);
      renderer.draw(shape1);

      gf::ConvexShape shape2(polygon2);
      shape2.setColor(gf::Color::Transparent);
      shape2.setOutlineColor(gf::Color::Cyan);
      shape2.setOutlineThickness(0.05f);
      renderer.draw(shape2);
    }

    if (isColliding) {
      const gf::Vector2f center(2.0f, 2.0f);

      gf::Vector2f endPoint = center + p.depth * p.normal;

      gf::Line line(center, endPoint);
      line.setColor(gf::Color::Red);
      line.setWidth(0.1f);
      renderer.draw(line);

      gf::CircleShape point(gf::CircF(center, 0.1f));
	  point.setColor(gf::Color::Red);
      renderer.draw(point);

      gf::CircleShape triangle(0.3f, 3);
      triangle.setPosition(endPoint);
      triangle.setRotation(gf::angle(p.normal) + gf::Pi2);
      triangle.setColor(gf::Color::Red);
      triangle.setAnchor(gf::Anchor::Center);
      renderer.draw(triangle);
    }

    renderer.display();
  }

  return 0;
}
