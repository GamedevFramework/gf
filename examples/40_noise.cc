/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <cassert>
#include <algorithm>
#include <tuple>
#include <iostream>

#include <gf/Array2D.h>
#include <gf/Event.h>
#include <gf/Image.h>
#include <gf/Math.h>
#include <gf/Noises.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Window.h>

static void generateArrayFromNoise(gf::Array2D<double>& array, gf::Noise2D& noise, double scale = 1.0) {
  for (auto row : array.getRowRange()) {
    double y = static_cast<double>(row) / array.getRows() * scale;
    for (auto col : array.getColRange()) {
      double x = static_cast<double>(col) / array.getCols() * scale;
      array({ col, row }) = noise(x, y);
    }
  }
}

static void generateImageFromArray(gf::Image& image, const gf::Array2D<double>& array) {
  double min = *std::min_element(array.begin(), array.end());
  double max = *std::max_element(array.begin(), array.end());

  for (auto row : array.getRowRange()) {
    for (auto col : array.getColRange()) {
      double normalized = (array({ col, row }) - min) / (max - min);
      assert(0.0 <= normalized && normalized <= 1.0);
      uint8_t val = static_cast<uint8_t>(normalized * 255);
      image.setPixel({ col, row }, { val, val, val, 255 });
    }
  }
}

int main() {
  gf::Random random;
  gf::PerlinNoise2D perlin(random, 1.0f);

  constexpr unsigned Size = 480;

  gf::Array2D<double> array({ Size, Size });
  generateArrayFromNoise(array, perlin, 10.0f);

  gf::Image image;
  image.create({ Size, Size });
  generateImageFromArray(image, array);

  gf::WindowHints hints;
  hints.resizable = false;

  gf::Window window("40_noise", { Size, Size }, hints);
  gf::RenderWindow renderer(window);

  gf::Texture texture;
  texture.loadFromImage(image);

  gf::Sprite sprite(texture);

  std::cout << "Gamedev Framework (gf) example #40: Noise\n";
  std::cout << "This example prints various type of noise\n";
  std::cout << "How to use:\n";
  std::cout << "\t1: Perlin noise (Gradient noise + Fractal)\n";
  std::cout << "\t2: Gradient noise\n";
  std::cout << "\t3: Simplex noise + Fractal\n";
  std::cout << "\t4: Simplex noise\n";
  std::cout << "\t5: OpenSimplex noise + Fractal\n";
  std::cout << "\t6: OpenSimplex noise\n";
  std::cout << "\t7: Worley noise + Fractal\n";
  std::cout << "\t8: Worley noise\n";
  std::cout << "\tS: Capture the image in 'noise.png'\n";
  std::cout << "Current noise: Perlin noise (Gradient noise + Fractal)\n";

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
            case gf::Scancode::Num1:
              std::cout << "Current noise: Perlin noise (Gradient noise + Fractal)\n";
              {
                gf::PerlinNoise2D noise(random, 1.0f);
                generateArrayFromNoise(array, noise, 10.0f);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num2:
              std::cout << "Current noise: Gradient noise\n";
              {
                gf::GradientNoise2D noise(random, gf::cubicStep);
                generateArrayFromNoise(array, noise, 10.0f);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num3:
              std::cout << "Current noise: Simplex noise + Fractal\n";
              {
                gf::SimplexNoise2D noise(random);
                gf::FractalNoise2D fractal(noise, 1.0f);
                generateArrayFromNoise(array, fractal, 10.0f);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num4:
              std::cout << "Current noise: Simplex noise\n";
              {
                gf::SimplexNoise2D noise(random);
                generateArrayFromNoise(array, noise, 10.0f);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num5:
              std::cout << "Current noise: OpenSimplex noise + Fractal\n";
              {
                gf::OpenSimplexNoise2D noise(random);
                gf::FractalNoise2D fractal(noise, 1.0f);
                generateArrayFromNoise(array, fractal, 10.0f);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num6:
              std::cout << "Current noise: OpenSimplex noise\n";
              {
                gf::OpenSimplexNoise2D noise(random);
                generateArrayFromNoise(array, noise, 10.0f);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num7:
              std::cout << "Current noise: Worley noise + Fractal\n";
              {
                gf::WorleyNoise2D noise(random, 20, gf::euclideanDistance, { -1.0, 1.0 });
                gf::FractalNoise2D fractal(noise, 1.0f, 6);
                generateArrayFromNoise(array, fractal);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            case gf::Scancode::Num8:
              std::cout << "Current noise: Worley noise\n";
              {
                gf::WorleyNoise2D noise(random, 20, gf::euclideanDistance, { -1.0, 1.0 });
                generateArrayFromNoise(array, noise);
                generateImageFromArray(image, array);
                texture.update(image);
              }
              break;

            default:
              break;
          }

          switch (event.key.keycode) {
            case gf::Keycode::S:
              std::cout << "# Capture the image\n";
              image.saveToFile("noise.png");
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(sprite);
    renderer.display();
  }

  return 0;
}
