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

#include <chrono>
#include <iostream>
#include <numeric>

#include <gf/Clock.h>
#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/SpriteBatch.h>
#include <gf/VectorOps.h>
#include <gf/Window.h>

int main() {
  gf::Window window("14_spritebatch", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Texture texture("assets/spritesheet.png");
  texture.setSmooth();

  gf::Sprite cars[8];

  for (std::size_t k = 0; k < 8; ++k) {
    gf::Sprite& sprite = cars[k];

    std::size_t i = k % 4;
    std::size_t j = k / 4;

    sprite.setTexture(texture, gf::RectF::fromPositionSize({ i * 0.25f, j * 0.5f }, { 0.25f, 0.5f }));
    sprite.setScale(0.5f);
    sprite.setOrigin({ 128.0f, 128.0f });
  }

  gf::Clock clock;
  float offset = 0.0f;

  constexpr gf::Vector2f center = { 320.0f, 240.0f };
  constexpr float majorRadius = 280.0f;
  constexpr float minorRadius = 200.0f;

  std::size_t carsCount = 8;
  bool useBatch = false;
  bool useTime = false;

  gf::SpriteBatch batch(renderer);

  std::vector<std::chrono::duration<double, std::micro>> times;

  std::cout << "Gamedev Framework (gf) example #14: Sprite Batch\n";
  std::cout << "How to use:\n";
  std::cout << "\tReturn: Change between direct rendering/batching\n";
  std::cout << "\tSpace: Start/Stop the benchmark\n";
  std::cout << "\tPageUp/PageDown: Increase/Decrease the number of cars (min: 2)\n";
  std::cout << "Using direct rendering with " <<  carsCount << " cars...\n";

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
            case gf::Scancode::Return:
              useBatch = !useBatch;

              if (useBatch) {
                std::cout << "Using batching with " <<  carsCount << " cars...\n";
              } else {
                std::cout << "Using direct rendering with " <<  carsCount << " cars...\n";
              }

              break;

            case gf::Scancode::Space:
              useTime = !useTime;

              if (useTime) {
                std::cout << "Starting benchmark...\n";
              } else {
                std::cout << "Stopping benchmark...\n";
                std::cout << "\tFrame count: " << times.size() << '\n';
                std::cout << "\tAverage time: " << std::accumulate(times.begin(), times.end(), std::chrono::duration<double, std::micro>(0)).count() / times.size() << " µs\n";
                times.clear();
              }
              break;

            case gf::Scancode::PageUp:
              if (!useTime) {
                carsCount *= 2;
                std::cout << "Setting number of cars to " << carsCount << '\n';
              } else {
                std::cout << "Please stop benchmark before changing the number of cars.\n";
              }
              break;

            case gf::Scancode::PageDown:
              if (!useTime) {
                if (carsCount > 2) {
                  carsCount /= 2;
                  std::cout << "Setting number of cars to " << carsCount << '\n';
                } else {
                  std::cout << "Minimum number of cars is 2.\n";
                }
              } else {
                std::cout << "Please stop benchmark before changing the number of cars.\n";
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

    gf::Time time = clock.restart();
    offset += time.asSeconds() * gf::Pi / 4;

    renderer.clear();

    auto start = std::chrono::high_resolution_clock::now();

    if (useBatch) {
      batch.begin();
    }

    for (std::size_t k = 0; k < carsCount; ++k) {
      gf::Sprite& sprite = cars[k % 8];

      float angle = k * 2 * gf::Pi / carsCount + offset;
      gf::Vector2f pos = { majorRadius * std::sin(angle), minorRadius * std::cos(angle) };
      sprite.setPosition(center + pos);

      sprite.setRotation(gf::Pi / 2 - angle);

      if (useBatch) {
        batch.draw(sprite);
      } else {
        renderer.draw(sprite);
      }
    }

    if (useBatch) {
      batch.end();
    }

    auto end = std::chrono::high_resolution_clock::now();

    if (useTime) {
      times.push_back(end - start);
    }

    renderer.display();
  }



  return 0;
}
