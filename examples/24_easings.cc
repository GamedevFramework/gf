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

#include <deque>
#include <iostream>
#include <vector>

#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Event.h>
#include <gf/Easings.h>
#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>
#include <gf/Window.h>

template <class T, std::size_t N>
static
constexpr std::size_t arraySize(const T (&)[N]) noexcept
{
    return N;
}

int main() {
  gf::Window window("24_easings", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #24: easing functions\n";
  std::cout << "This example shows the different easing functions.\n";
  std::cout << "How to use:\n";
  std::cout << "\tPageUp: Next easing\n";
  std::cout << "\tPageDown: Previous easing\n";

  static constexpr gf::Easing Easings[][4] = {
    { gf::Ease::linear, gf::Ease::linear, gf::Ease::linear, gf::Ease::linear },
    { gf::Ease::smooth, gf::Ease::smooth, gf::Ease::smooth, gf::Ease::smooth },
    { gf::Ease::smoother, gf::Ease::smoother, gf::Ease::smoother, gf::Ease::smoother },
    { gf::Ease::quadIn, gf::Ease::quadOut, gf::Ease::quadInOut, gf::Ease::quadOutIn },
    { gf::Ease::cubicIn, gf::Ease::cubicOut, gf::Ease::cubicInOut, gf::Ease::cubicOutIn },
    { gf::Ease::quartIn, gf::Ease::quartOut, gf::Ease::quartInOut, gf::Ease::quartOutIn },
    { gf::Ease::quintIn, gf::Ease::quintOut, gf::Ease::quintInOut, gf::Ease::quintOutIn },
    { gf::Ease::circIn, gf::Ease::circOut, gf::Ease::circInOut, gf::Ease::circOutIn },
    { gf::Ease::sineIn, gf::Ease::sineOut, gf::Ease::sineInOut, gf::Ease::sineOutIn },
    { gf::Ease::backIn, gf::Ease::backOut, gf::Ease::backInOut, gf::Ease::backOutIn },
    { gf::Ease::bounceIn, gf::Ease::bounceOut, gf::Ease::bounceInOut, gf::Ease::bounceOutIn },
    { gf::Ease::elasticIn, gf::Ease::elasticOut, gf::Ease::elasticInOut, gf::Ease::elasticOutIn },
    { gf::Ease::expoIn, gf::Ease::expoOut, gf::Ease::expoInOut, gf::Ease::expoOutIn }
  };

  static constexpr const char *EasingNames[] = {
    "linear",
    "smooth",
    "smoother",
    "quad",
    "cubic",
    "quart",
    "quint",
    "circ",
    "sine",
    "back",
    "bounce",
    "elastic",
    "expo"
  };

  static_assert(arraySize(Easings) == arraySize(EasingNames), "Size mismatch");

  static constexpr float Y[4] = {
    60.0f, 180.0f, 300.0f, 420.0f
  };

  static constexpr float XMin = 100.0f;
  static constexpr float XMax = 540.0f;
  static constexpr float Velocity = 200.0f;

  float x = XMin;
  float velocity = Velocity;

  std::size_t easingIndex = 0;

  std::cout << "Current easing: " << EasingNames[easingIndex] << '\n';

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

            case gf::Keycode::PageUp:
              easingIndex = (easingIndex + 1) % arraySize(Easings);
              std::cout << "Current easing: " << EasingNames[easingIndex] << '\n';
              break;

            case gf::Keycode::PageDown:
              easingIndex = (easingIndex + arraySize(Easings) - 1) % arraySize(Easings);
              std::cout << "Current easing: " << EasingNames[easingIndex] << '\n';
              break;

            default:
              break;
          }

        default:
          break;
      }
    }

    float dt = clock.restart().asSeconds();
    x += velocity * dt;

    if (x > XMax) {
      x = XMax;
      velocity = -velocity;
    }

    if (x < XMin) {
      x = XMin;
      velocity = -velocity;
    }

    renderer.clear();

    for (int i = 0; i < 4; ++i) {
      gf::Line line({ XMin, Y[i] }, { XMax, Y[i] });
      line.setColor(gf::Color::Gray(0.7f));
      line.setWidth(1.0f);
      renderer.draw(line);

      gf::CircleShape endPoint(3.0f);
      endPoint.setColor(gf::Color::Gray(0.7f));
      endPoint.setAnchor(gf::Anchor::Center);

      endPoint.setPosition({ XMin, Y[i] });
      renderer.draw(endPoint);
      endPoint.setPosition({ XMax, Y[i] });
      renderer.draw(endPoint);

      float xEased = 0.0f;

      if (velocity > 0) {
        xEased = XMin + Easings[easingIndex][i]((x - XMin)/(XMax - XMin)) * (XMax - XMin);
      } else {
        xEased = XMax - Easings[easingIndex][i]((XMax - x)/(XMax - XMin)) * (XMax - XMin);
      }

      gf::CircleShape object(10.0f);
      object.setColor(gf::Color::Azure);
      object.setAnchor(gf::Anchor::Center);
      object.setPosition({ xEased, Y[i] });
      renderer.draw(object);

    }

    renderer.display();
  }

  return 0;
}
