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

#include <gf/Activities.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Easings.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Window.h>

namespace {
  namespace ga = gf::activity;

  auto createActivity(gf::Vector2f& position, float& rotation, gf::Color4f& color, float& faces) {
    return ga::repeat(
      ga::sequence(
        ga::parallelAny(
          ga::sequence(
            ga::moveTo({ 100.0f, 100.0f }, { 540.0f, 100.0f }, position, gf::seconds(2.7f), gf::Ease::quadInOut),
            ga::moveTo({ 540.0f, 100.0f }, { 540.0f, 380.0f }, position, gf::seconds(1.9f), gf::Ease::backOut),
            ga::moveTo({ 540.0f, 380.0f }, { 100.0f, 380.0f }, position, gf::seconds(2.7f), gf::Ease::elasticOut),
            ga::moveTo({ 100.0f, 380.0f }, { 100.0f, 100.0f }, position, gf::seconds(1.9f))
          ),
          ga::sequence(
            ga::value(4, 8, faces, gf::seconds(4.6f), gf::Ease::circInOut),
            ga::value(8, 4, faces, gf::seconds(4.6f), gf::Ease::circInOut)
          ),
          ga::sequence(
            ga::color(gf::Color::Azure, gf::Color::Orange, color, gf::seconds(4.6f)),
            ga::color(gf::Color::Orange, gf::Color::Azure, color, gf::seconds(4.6f))
          ),
          ga::value(0, 10 * gf::Pi, rotation, gf::seconds(9.2f), gf::Ease::bounceInOut)
        ),
        ga::delay(gf::seconds(0.5f))
      )
    );
  }

}

class Blob {
public:
  Blob()
  : m_rotation(0.0f)
  , m_color(gf::Color::Azure)
  , m_faces(4)
  , m_activity(createActivity(m_position, m_rotation, m_color, m_faces))
  {
  }

  void run(gf::Time time) {
    m_activity.run(time);
  }

  void render(gf::RenderTarget& target) {
    gf::CircleShape object(20.0f, static_cast<unsigned>(m_faces));
    object.setColor(m_color);
    object.setAnchor(gf::Anchor::Center);
    object.setPosition(m_position);
    object.setRotation(m_rotation);
    target.draw(object);
  }

private:
  gf::Vector2f m_position;
  float m_rotation;
  gf::Color4f m_color;
  float m_faces;
  ga::AnyActivity m_activity;
};


int main() {
  gf::Window window("25_activities", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #25: activities\n";
  std::cout << "This example shows some activities.\n";

  Blob blob;

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
          switch (event.key.keycode) {
            case gf::Keycode::Escape:
              window.close();
              break;

            default:
              break;
          }

        default:
          break;
      }
    }

    blob.run(clock.restart());

    renderer.clear();
    blob.render(renderer);
    renderer.display();
  }

  return 0;
}
