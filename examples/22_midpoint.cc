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
#include <gf/Event.h>
#include <gf/Geometry.h>
#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/VertexArray.h>
#include <gf/Window.h>

static constexpr float Width = 1000.0f;
static constexpr float Height = 500.0f;

class Landscape {
public:
  Landscape()
  : m_color(gf::Color::White)
  , m_vertices(gf::PrimitiveType::Triangles)
  {

  }

  void setColor(gf::Color4f color) {
    m_color = color;
  }

  void setPoints(const std::vector<gf::Vector2f>& points) {
    m_vertices.clear();
    auto count = points.size();

    for (std::size_t i = 1; i < count; ++i) {
      const gf::Vector2f& p0 = points[i - 1];
      const gf::Vector2f& p1 = points[i];

      gf::Vector2f p2 = { p0.x, Height };
      gf::Vector2f p3 = { p1.x, Height };

      gf::Vertex vertices[4];

      vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = m_color;

      vertices[0].position = p0;
      vertices[1].position = p1;
      vertices[2].position = p2;
      vertices[3].position = p3;

      m_vertices.append(vertices[0]);
      m_vertices.append(vertices[1]);
      m_vertices.append(vertices[2]);

      m_vertices.append(vertices[2]);
      m_vertices.append(vertices[1]);
      m_vertices.append(vertices[3]);
    }
  }

  void render(gf::RenderTarget& target) {
    target.draw(m_vertices);
  }

private:
  gf::Color4f m_color;
  gf::VertexArray m_vertices;
};

void generateLandscape(Landscape landscapes[4], gf::Random& random) {
  auto layer0 = gf::midpointDisplacement1D({ 250.0f, Height }, { Width, 300.0f }, random, 12, 0.10f, 0.40f);
  landscapes[0].setPoints(layer0);

  auto layer1 = gf::midpointDisplacement1D({ 0.0f, 320.0f }, { Width, 420.0f }, random, 12, 0.15f, 0.45f);
  landscapes[1].setPoints(layer1);

  auto layer2 = gf::midpointDisplacement1D({ 0.0f, 230.0f }, { Width, 310.0f }, random, 9, 0.25f, 0.5f);
  landscapes[2].setPoints(layer2);

  auto layer3 = gf::midpointDisplacement1D({ 0.0f, 150.0f }, { Width, 180.0f }, random, 8, 0.40f, 0.55f);
  landscapes[3].setPoints(layer3);
}


// inspired by https://bitesofcode.wordpress.com/2016/12/23/landscape-generation-using-midpoint-displacement/

int main() {
  gf::Window window("22_midpoint", { 1000, 500 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #22: Midpoint displacement\n";
  std::cout << "This example shows a landscape generated with midpoint displacement.\n";
  std::cout << "How to use:\n";
  std::cout << "\tR: Reload the landscape\n";
  std::cout << "\tS: Save the current landscape to 'landscape.png'\n";
  std::cout << "\tEscape: Close the window\n";

  gf::View view(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { Width, Height }));
  renderer.setView(view);

  gf::Random random;

  Landscape landscapes[4];
  landscapes[0].setColor(gf::Color::fromRgba32( 68,  28,  99));
  landscapes[1].setColor(gf::Color::fromRgba32(130,  79, 138));
  landscapes[2].setColor(gf::Color::fromRgba32(158,  98, 204));
  landscapes[3].setColor(gf::Color::fromRgba32(195, 157, 224));

  generateLandscape(landscapes, random);

  gf::CircleShape sun(25.0f);
  sun.setColor(gf::Color::White);
  sun.setAnchor(gf::Anchor::Center);
  sun.setPosition({ 75.0f, 50.0f });

  renderer.clear(gf::Color::fromRgba32(240, 203, 163));

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

            case gf::Keycode::R:
              generateLandscape(landscapes, random);
              break;

            case gf::Keycode::S: {
              auto image = renderer.capture();
              image.saveToFile("landscape.png");
              std::cout << "Saved!\n";
              break;
            }

            default:
              break;
          }

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(sun);
    landscapes[3].render(renderer);
    landscapes[2].render(renderer);
    landscapes[1].render(renderer);
    landscapes[0].render(renderer);
    renderer.display();
  }

  return 0;
}
