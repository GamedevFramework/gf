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

/// [all]
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>

/// [square]
class Square {
public:
  Square(gf::Vector2f position, float size, gf::Color4f color)
  : m_position(position)
  , m_velocity(0, 0)
  , m_size(size)
  , m_color(color)
  {

  }

  void setVelocity(gf::Vector2f velocity) {
    m_velocity = velocity;
  }

  void update(float dt) {
    m_position += dt * m_velocity;
  }

  void render(gf::RenderTarget& target) {
    gf::RectangleShape shape({ m_size, m_size });
    shape.setPosition(m_position);
    shape.setColor(m_color);
    shape.setAnchor(gf::Anchor::Center);
    target.draw(shape);
  }

private:
  gf::Vector2f m_position; // center of the square
  gf::Vector2f m_velocity;
  float m_size;
  gf::Color4f m_color;
};
/// [square]

int main() {
  // initialization

  static constexpr gf::Vector2i ScreenSize(500, 500);
  gf::Window window("It moves", ScreenSize);
  gf::RenderWindow renderer(window);

  // entities

  /// [entity]
  Square entity(ScreenSize / 2, 50.0f, gf::Color::Red);
  /// [entity]

  // game loop

  gf::Clock clock;
  renderer.clear(gf::Color::White);

  /// [velocity]
  static constexpr float Speed = 100.0f;
  gf::Vector2f velocity(0, 0);
  /// [velocity]

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

      /// [events]
      case gf::EventType::KeyPressed:
        switch (event.key.keycode) {
          case gf::Keycode::Up:
              velocity.y -= Speed;
            break;
          case gf::Keycode::Down:
              velocity.y += Speed;
            break;
          case gf::Keycode::Left:
              velocity.x -= Speed;
            break;
          case gf::Keycode::Right:
              velocity.x += Speed;
            break;
          default:
            break;
        }
        break;

      case gf::EventType::KeyReleased:
        switch (event.key.keycode) {
          case gf::Keycode::Up:
              velocity.y += Speed;
            break;
          case gf::Keycode::Down:
              velocity.y -= Speed;
            break;
          case gf::Keycode::Left:
              velocity.x += Speed;
            break;
          case gf::Keycode::Right:
              velocity.x -= Speed;
            break;
          default:
            break;
        }
        break;
      /// [events]

      default:
        break;
      }
    }

    // 2. update

    /// [update]
    entity.setVelocity(velocity);

    float dt = clock.restart().asSeconds();
    entity.update(dt);
    /// [update]

    // 3. draw

    /// [render]
    renderer.clear();
    entity.render(renderer);
    renderer.display();
    /// [render]
  }

  return 0;
}
/// [all]
