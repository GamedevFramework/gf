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

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/Particles.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/Window.h>

class Smoke {
public:
  static constexpr float Acceleration = 30.0f;
  static constexpr float InitialVelocity = 20.0f;

  static constexpr float InitialAlpha = 0.5f;
  static constexpr float AlphaFading = 0.1f;

  static constexpr float InitialRadius = 10.0f;
  static constexpr float RadiusIncrease = 5.0f;

  static constexpr float GenerationTime = 0.04f;

  Smoke(gf::Random& random, gf::Vector2f position)
  : m_random(random)
  , m_position(position)
  , m_timer(0)
  {

  }

  void update(float dt) {
    while (!m_clouds.empty() && m_clouds.front().alpha < 0) {
      m_clouds.pop_front();
    }

    for (auto& cloud : m_clouds) {
      cloud.velocity.y -= Acceleration * dt; // clouds go up
      cloud.position += cloud.velocity * dt;
      cloud.radius += RadiusIncrease * dt;
      cloud.alpha -= AlphaFading * dt;
    }

    m_timer -= dt;

    while (m_timer < 0) {
      Cloud cloud;
      cloud.position = m_position;
      cloud.velocity = InitialVelocity * gf::unit(m_random.computeUniformFloat(gf::Pi, 2 * gf::Pi));
      cloud.radius = InitialRadius;
      cloud.alpha = InitialAlpha;

      m_clouds.push_back(cloud);

      m_timer += GenerationTime;
    }
  }

  void render(gf::RenderTarget& target) {
    gf::Color4f color(0.5f, 0.5f, 0.5f, 1.0f);

    gf::ShapeParticles particles;

    for (const auto& cloud : m_clouds) {
      color.a = cloud.alpha;
      particles.addCircle(cloud.position, cloud.radius, color);
    }

    target.draw(particles);
  }

private:
  gf::Random& m_random;
  gf::Vector2f m_position;
  float m_timer;

  struct Cloud {
    gf::Vector2f position;
    gf::Vector2f velocity;
    float radius;
    float alpha;
  };

  std::deque<Cloud> m_clouds;

};

class Fire {
public:
  static constexpr float GenerationTime = 0.0001f;
  static constexpr float InitialLifetimeMin = 2.0f;
  static constexpr float InitialLifetimeMax = 3.0f;

  static constexpr float Frequency = 10.0f;

  static constexpr float AccelerationY = 25.0f;
  static constexpr float AccelerationX = 1.0f;
  static constexpr float InitialVelocity = 10.0f;

  static constexpr float PositionRadius = 15.0f;

  Fire(gf::Random& random, gf::Vector2f position)
  : m_random(random)
  , m_position(position)
  , m_timer(0)
  {

  }

  void update(float dt) {
    m_points.erase(std::remove_if(m_points.begin(), m_points.end(), [dt](const Point& p) {
      return p.lifetime < dt;
    }), m_points.end());

    for (auto& point : m_points) {
      point.lifetime -= dt;
      point.velocity.y -= AccelerationY * dt;
      point.velocity.x += AccelerationX * std::sin(Frequency * point.lifetime + point.phase);
      point.position += point.velocity * dt;
    }

    m_timer -= dt;

    while (m_timer < 0) {
      Point point;
      point.position = m_position + m_random.computeUniformFloat(0.0f, PositionRadius) * gf::unit(m_random.computeUniformFloat(0.0f, 2 * gf::Pi));
      point.velocity = InitialVelocity * gf::unit(m_random.computeUniformFloat(0.0f, 2 * gf::Pi));
      point.phase = m_random.computeUniformFloat(0.0f, 2 * gf::Pi);
      point.lifetime = m_random.computeUniformFloat(InitialLifetimeMin, InitialLifetimeMax);

      m_points.push_back(point);

      m_timer += GenerationTime;
    }
  }

  void render(gf::RenderTarget& target) {
    gf::PointParticles particles;

    for (const auto& point : m_points) {
      float t = (InitialLifetimeMax - point.lifetime) / InitialLifetimeMax;
      gf::Color4f color = gf::lerp(gf::Color::Yellow, gf::Color::Red, t);
      color.a = std::min(1.0f, point.lifetime);
      particles.addPoint(point.position, color);
    }

    target.draw(particles);
  }

private:
  gf::Random& m_random;
  gf::Vector2f m_position;
  float m_timer;

  struct Point {
    gf::Vector2f position;
    gf::Vector2f velocity;
    float phase;
    float lifetime;
  };

  std::vector<Point> m_points;
};

int main() {
  gf::Window window("21_particles", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #21: Particles\n";
  std::cout << "This example prints a nice fire with two different particles effects.\n";

  gf::Random random;

  Smoke smoke(random, gf::Vector2f(320.0f, 400.0f));
  Fire fire(random, gf::Vector2f(320.0f, 400.0f));

  gf::Clock clock;

  renderer.clear(gf::Color::Black);

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

    float dt = clock.restart().asSeconds();
    smoke.update(dt);
    fire.update(dt);

    renderer.clear();
    smoke.render(renderer);
    fire.render(renderer);
    renderer.display();
  }

  return 0;
}
