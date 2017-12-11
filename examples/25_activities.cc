/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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

class Blob {
public:
  Blob()
  : m_rotation(0.0f)
  , m_color(gf::Color::Azure)
  , m_faces(4)
  , m_moveNorth({ 100.0f, 100.0f }, { 540.0f, 100.0f }, m_position, gf::seconds(2.7f), gf::Ease::quadInOut)
  , m_moveWest({ 540.0f, 100.0f }, { 540.0f, 380.0f }, m_position, gf::seconds(1.9f), gf::Ease::backOut)
  , m_moveSouth({ 540.0f, 380.0f }, { 100.0f, 380.0f }, m_position, gf::seconds(2.7f), gf::Ease::elasticOut)
  , m_moveEast({ 100.0f, 380.0f }, { 100.0f, 100.0f }, m_position, gf::seconds(1.9f))
  , m_facesUp(4, 8, m_faces, gf::seconds(4.6), gf::Ease::circInOut)
  , m_facesDown(8, 4, m_faces, gf::seconds(4.6), gf::Ease::circInOut)
  , m_colorToOrange(gf::Color::Azure, gf::Color::Orange, m_color, gf::seconds(4.6))
  , m_colorToAzure(gf::Color::Orange, gf::Color::Azure, m_color, gf::seconds(4.6))
  , m_rotationActivity(0, 10 * gf::Pi, m_rotation, gf::seconds(9.2), gf::Ease::bounceInOut)
  , m_repeat(m_parallel)
  {
    m_moveSequence.addActivity(m_moveNorth);
    m_moveSequence.addActivity(m_moveWest);
    m_moveSequence.addActivity(m_moveSouth);
    m_moveSequence.addActivity(m_moveEast);

    m_faceSequence.addActivity(m_facesUp);
    m_faceSequence.addActivity(m_facesDown);

    m_colorSequence.addActivity(m_colorToOrange);
    m_colorSequence.addActivity(m_colorToAzure);

    m_parallel.addActivity(m_moveSequence);
    m_parallel.addActivity(m_faceSequence);
    m_parallel.addActivity(m_colorSequence);
    m_parallel.addActivity(m_rotationActivity);
  }

  void run(gf::Time time) {
    m_repeat.run(time);
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

private:
  // activities
  gf::MoveToActivity m_moveNorth;
  gf::MoveToActivity m_moveWest;
  gf::MoveToActivity m_moveSouth;
  gf::MoveToActivity m_moveEast;
  gf::SequenceActivity m_moveSequence;
  gf::ValueActivity m_facesUp;
  gf::ValueActivity m_facesDown;
  gf::SequenceActivity m_faceSequence;
  gf::ColorActivity m_colorToOrange;
  gf::ColorActivity m_colorToAzure;
  gf::SequenceActivity m_colorSequence;
  gf::ValueActivity m_rotationActivity;
  gf::ParallelActivity m_parallel;
  gf::RepeatActivity m_repeat;
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
