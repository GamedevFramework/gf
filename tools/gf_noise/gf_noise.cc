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
#include <gf/Font.h>
#include <gf/Image.h>
#include <gf/Math.h>
#include <gf/Noises.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/UI.h>
#include <gf/Window.h>

#include "config.h"

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

static void generate(gf::Texture& texture, gf::Image& image, gf::Array2D<double>& array, gf::Noise2D& noise, double scale = 1.0) {
  generateArrayFromNoise(array, noise, scale);
  generateImageFromArray(image, array);
  texture.update(image);
}

enum class NoiseFunction : std::size_t {
  Gradient      = 0,
  Simplex       = 1,
  OpenSimplex   = 2,
  Worley        = 3,
};

enum class StepFunction : std::size_t {
  Linear  = 0,
  Cubic   = 1,
  Quintic = 2,
  Cosine  = 3,
};

static gf::Step<double> getStepFunction(StepFunction func) {
  switch (func) {
    case StepFunction::Linear:
      return gf::linearStep;
    case StepFunction::Cubic:
      return gf::cubicStep;
    case StepFunction::Quintic:
      return gf::quinticStep;
    case StepFunction::Cosine:
      return gf::cosineStep;
  }

  assert(false);
  return gf::linearStep;
}

enum class DistanceFunction : std::size_t {
  Euclidean = 0,
  Manhattan = 1,
  Chebyshev = 2,
};

static gf::Distance2<double> getDistanceFunction(DistanceFunction func) {
  switch (func) {
    case DistanceFunction::Euclidean:
      return gf::squareDistance;
    case DistanceFunction::Manhattan:
      return gf::manhattanDistance;
    case DistanceFunction::Chebyshev:
      return gf::chebyshevDistance;
  }

  assert(false);
  return gf::squareDistance;
}

enum class CombinationFunction : std::size_t {
  F1    = 0,
  F2    = 1,
  F2F1  = 2,
};

static std::vector<double> getCombinationVector(CombinationFunction func) {
  switch (func) {
    case CombinationFunction::F1:
      return { 1.0 };
    case CombinationFunction::F2:
      return { 0.0, 1.0 };
    case CombinationFunction::F2F1:
      return { -1.0, 1.0 };
  }

  assert(false);
  return { 1.0 };
}


int main() {
  gf::Random random;

  constexpr unsigned Size = 512;
  constexpr unsigned ExtraSize = 200;

  gf::Array2D<double> array({ Size, Size });
  gf::Image image;
  image.create({ Size, Size });

  gf::WindowHints hints;
  hints.resizable = false;

  gf::Window window("gf noise", { Size + ExtraSize, Size }, hints);
  gf::RenderWindow renderer(window);

  gf::Texture texture;
  texture.loadFromImage(image);

  gf::Sprite sprite(texture);

  gf::Font font;
  gf::Path fontPath(GF_DATADIR);

  if (!font.loadFromFile(fontPath / "DroidSans.ttf")) {
    return EXIT_FAILURE;
  }

  gf::DefaultUIRenderer uiRenderer(font);
  gf::UILayout layout;
  gf::UI ui(uiRenderer, layout);

  float scrollArea = 0.0f;

  // noise states

  double scale = 10.0;

  std::vector<std::string> noiseChoices = { "Gradient", "Simplex", "OpenSimplex", "Worley" }; // keep in line with NoiseFunction
  std::size_t noiseChoice = 0;

  std::vector<std::string> stepChoices = { "Linear", "Cubic", "Quintic", "Cosine" }; // keep in line with StepFunction
  std::size_t stepChoice = 2;

  std::vector<std::string> distanceChoices = { "Euclidean", "Manhattan", "Chebyshev" }; // keep in line with DistanceFunction
  std::size_t distanceChoice = 0;

  float pointCount = 20;

  std::vector<std::string> combinationChoices = { "F1", "F2", "F2F1" };
  std::size_t combinationChoice = 2;

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        default:
          break;
      }

      ui.update(event);
    }

    ui.clear();

    ui.beginScrollArea("Noise", gf::RectF(Size, 0, ExtraSize, Size), &scrollArea);

    ui.separatorLine();

    if (ui.cycle(noiseChoices, noiseChoice)) {
      noiseChoice = (noiseChoice + 1) % noiseChoices.size();
    }

    NoiseFunction noiseFunction = static_cast<NoiseFunction>(noiseChoice);

    switch (noiseFunction) {
      case NoiseFunction::Gradient:
        ui.separator();

        ui.label("Step function:");
        if (ui.cycle(stepChoices, stepChoice)) {
          stepChoice = (stepChoice + 1) % stepChoices.size();
        }
        break;

      case NoiseFunction::Worley:
        ui.separator();

        ui.slider("Point count", &pointCount, 5, 40, 1);

        ui.label("Distance function:");
        if (ui.cycle(distanceChoices, distanceChoice)) {
          distanceChoice = (distanceChoice + 1) % distanceChoices.size();
        }

        ui.label("Combination:");
        if (ui.cycle(combinationChoices, combinationChoice)) {
          combinationChoice = (combinationChoice + 1) % combinationChoices.size();
        }

      default:
        break;
    }



    ui.separatorLine();

    if (ui.button("Generate")) {
      switch (noiseFunction) {
        case NoiseFunction::Gradient: {
          StepFunction stepFunction = static_cast<StepFunction>(stepChoice);
          gf::Step<double> step = getStepFunction(stepFunction);

          gf::GradientNoise2D noise(random, step);
          generate(texture, image, array, noise, scale);
          break;
        }

        case NoiseFunction::Simplex: {
          gf::SimplexNoise2D noise(random);
          generate(texture, image, array, noise, scale);
          break;
        }

        case NoiseFunction::OpenSimplex: {
          gf::OpenSimplexNoise2D noise(random);
          generate(texture, image, array, noise, scale);
          break;
        }

        case NoiseFunction::Worley: {
          DistanceFunction distanceFunction = static_cast<DistanceFunction>(distanceChoice);
          gf::Distance2<double> distance = getDistanceFunction(distanceFunction);

          CombinationFunction combinationFunction = static_cast<CombinationFunction>(combinationChoice);
          std::vector<double> combination = getCombinationVector(combinationFunction);

          gf::WorleyNoise2D noise(random, pointCount, distance, combination);
          generate(texture, image, array, noise, 1.0);
          break;
        }
      }
    }

    ui.endScrollArea();

    renderer.clear();
    renderer.draw(sprite);
    renderer.draw(ui);
    renderer.display();
  }

  return 0;
}
