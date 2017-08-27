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
#include <cassert>
#include <algorithm>
#include <fstream>
#include <tuple>
#include <iostream>

#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/ColorRamp.h>
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/Heightmap.h>
#include <gf/Image.h>
#include <gf/Math.h>
#include <gf/Noises.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/UI.h>
#include <gf/VectorOps.h>
#include <gf/Window.h>

#include "config.h"

enum class Rendering : std::size_t {
  Grayscale = 0,
  Colored   = 1,
};

struct RenderingParams {
  Rendering rendering;
  // for colored rendering
  bool shaded;
  float waterLevel;
};

enum class Fractal : std::size_t {
  None                = 0,
  FBm                 = 1,
  Multifractal        = 2,
  HeteroTerrain       = 3,
  HybridMultifractal  = 4,
  RidgedMultifractal  = 5,
};

struct FractalParams {
  Fractal fractal;

  struct {
    float dimension;
  } f; // fBm

  struct {
    float dimension;
  } m; // Multifractal

  struct {
    float offset;
    float dimension;
  } ht; // Hetero Terrain

  struct {
    float offset;
    float dimension;
  } hm; // Hybrid Multifractal

  struct {
    float offset;
    float gain;
    float dimension;
  } rm; // Ridged Multifractal

  int octaves;
  float lacunarity;
  float persistence;
};


static void generateArrayFromNoise(gf::Heightmap& heightmap, gf::Noise2D& noise, double scale = 1.0) {
  heightmap.reset();
  heightmap.addNoise(noise, scale);
  heightmap.normalize();
}

static gf::Image generateImageFromArray(const RenderingParams& renderingParams, const gf::Heightmap& heightmap) {
  gf::Image image;

  switch (renderingParams.rendering) {
    case Rendering::Grayscale:
      image = heightmap.copyToGrayscaleImage();
      break;

    case Rendering::Colored: {
      // see: http://www.blitzbasic.com/codearcs/codearcs.php?code=2415
      gf::ColorRamp ramp;
      ramp.addColorStop(0.000f, gf::Color::fromRgba32(  2,  43,  68)); // very dark blue: deep water
      ramp.addColorStop(0.250f, gf::Color::fromRgba32(  9,  62,  92)); // dark blue: water
      ramp.addColorStop(0.490f, gf::Color::fromRgba32( 17,  82, 112)); // blue: shallow water
      ramp.addColorStop(0.500f, gf::Color::fromRgba32( 69, 108, 118)); // light blue: shore
      ramp.addColorStop(0.501f, gf::Color::fromRgba32( 42, 102,  41)); // green: grass
      ramp.addColorStop(0.750f, gf::Color::fromRgba32(115, 128,  77)); // light green: veld
      ramp.addColorStop(0.850f, gf::Color::fromRgba32(153, 143,  92)); // brown: tundra
      ramp.addColorStop(0.950f, gf::Color::fromRgba32(179, 179, 179)); // grey: rocks
      ramp.addColorStop(1.000f, gf::Color::fromRgba32(255, 255, 255)); // white: snow

      gf::Heightmap::Render renderMode = renderingParams.shaded ? gf::Heightmap::Render::Shaded : gf::Heightmap::Render::Colored;
      image = heightmap.copyToColoredImage(ramp, renderingParams.waterLevel, renderMode);
      break;
    }
  }

  return image;
}

static gf::Image generate(gf::Texture& texture, const RenderingParams& renderingParams, gf::Heightmap& heightmap, gf::Noise2D& noise, const FractalParams& fractalParams, double scale = 1.0) {

  switch (fractalParams.fractal) {
    case Fractal::None:
      generateArrayFromNoise(heightmap, noise, scale);
      break;

    case Fractal::FBm: {
      gf::FractalNoise2D fractalNoise(noise, 1, fractalParams.octaves, fractalParams.lacunarity, fractalParams.persistence, fractalParams.f.dimension);
      generateArrayFromNoise(heightmap, fractalNoise, scale);
      break;
    }

    case Fractal::Multifractal:  {
      gf::Multifractal2D fractalNoise(noise, 1, fractalParams.octaves, fractalParams.lacunarity, fractalParams.persistence, fractalParams.m.dimension);
      generateArrayFromNoise(heightmap, fractalNoise, scale);
      break;
    }

    case Fractal::HeteroTerrain: {
      gf::HeteroTerrain2D fractalNoise(noise, 1, fractalParams.ht.offset, fractalParams.octaves, fractalParams.lacunarity, fractalParams.persistence, fractalParams.ht.dimension);
      generateArrayFromNoise(heightmap, fractalNoise, scale);
      break;
    }

    case Fractal::HybridMultifractal: {
      gf::HybridMultifractal2D fractalNoise(noise, 1, fractalParams.hm.offset, fractalParams.octaves, fractalParams.lacunarity, fractalParams.persistence, fractalParams.hm.dimension);
      generateArrayFromNoise(heightmap, fractalNoise, scale);
      break;
    }

    case Fractal::RidgedMultifractal: {
      gf::RidgedMultifractal2D fractalNoise(noise, 1, fractalParams.rm.offset, fractalParams.rm.gain, fractalParams.octaves, fractalParams.lacunarity, fractalParams.persistence, fractalParams.rm.dimension);
      generateArrayFromNoise(heightmap, fractalNoise, scale);
      break;
    }
  }

  gf::Image image = generateImageFromArray(renderingParams, heightmap);
  texture.update(image);
  return image;
}

enum class NoiseFunction : std::size_t {
  Noise           = 0,
  Gradient        = 1,
  BetterGradient  = 2,
  Simplex         = 3,
  OpenSimplex     = 4,
  Worley          = 5,
  Wavelet         = 6,
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
  Natural   = 3,
};

static gf::Distance2<double> getDistanceFunction(DistanceFunction func) {
  switch (func) {
    case DistanceFunction::Euclidean:
      return gf::squareDistance;
    case DistanceFunction::Manhattan:
      return gf::manhattanDistance;
    case DistanceFunction::Chebyshev:
      return gf::chebyshevDistance;
    case DistanceFunction::Natural:
      return gf::naturalDistance;
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

  constexpr unsigned Size = 1024;
  constexpr unsigned ExtraSize = 250;

  constexpr float ComboHeightMax = 200.0f;

  gf::RectF bounds;

  gf::Heightmap heightmap({ static_cast<int>(Size), static_cast<int>(Size) });
  gf::Image image;
  image.create({ Size, Size }); // initial image

  gf::Window window("gf noise", { Size + ExtraSize, Size }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Texture texture;
  texture.loadFromImage(image);

  gf::Sprite sprite(texture);

  gf::Font font;
  gf::Path fontPath(GF_DATADIR);

  if (!font.loadFromFile(fontPath / "DroidSans.ttf")) {
    return EXIT_FAILURE;
  }

  gf::UI ui(font);

  // noise states

  std::vector<std::string> noiseChoices = { "Value", "Gradient", "Better Gradient", "Simplex", "OpenSimplex", "Worley", "Wavelet" }; // keep in line with NoiseFunction
  int noiseChoice = 1;

  std::vector<std::string> stepChoices = { "Linear", "Cubic", "Quintic", "Cosine" }; // keep in line with StepFunction
  int stepChoice = 2;

  int pointCount = 20;

  std::vector<std::string> distanceChoices = { "Euclidean", "Manhattan", "Chebyshev", "Natural" }; // keep in line with DistanceFunction
  int distanceChoice = 0;

  std::vector<std::string> combinationChoices = { "F1", "F2", "F2F1" };
  int combinationChoice = 2;

  gf::UICollapse fractalCollapsed = gf::UICollapse::Minimized;

  std::vector<std::string> fractalChoices = { "None", "fBm", "Multifractal", "Hetero Terrain", "Hybrid Multifractal", "Ridged Multifractal" }; // keep in line with Fractal
  int fractalChoice = 0;

  float scale = 1.0;

  FractalParams fractalParams;
  fractalParams.fractal = Fractal::None;

  fractalParams.f.dimension = 1.0;

  fractalParams.m.dimension = 1.0;

  fractalParams.ht.dimension = 1.0;
  fractalParams.ht.offset = 1.0;

  fractalParams.hm.dimension = 0.25;
  fractalParams.hm.offset = 0.7;

  fractalParams.rm.dimension = 1.0;
  fractalParams.rm.offset = 1.0;
  fractalParams.rm.gain = 2.0;

  fractalParams.octaves = 8;
  fractalParams.lacunarity = 2.0;
  fractalParams.persistence = 0.5;

  gf::UICollapse renderingCollapsed = gf::UICollapse::Minimized;

  std::vector<std::string> renderingChoices = { "Grayscale", "Colored" };
  int renderingChoice = 0;

  RenderingParams renderingParams;
  renderingParams.rendering = Rendering::Grayscale;
  renderingParams.shaded = false;
  renderingParams.waterLevel = 0.5;

  std::string feedback;

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

      ui.processEvent(event);
    }


    ui.begin("Noise parameters", gf::RectF(Size, 0, ExtraSize, Size), gf::UIWindow::Title | gf::UIWindow::Border);

    ui.layoutRowDynamic(20, 1);
    bounds = ui.getWidgetBounds();
    ui.combobox(noiseChoices, noiseChoice, 20, { bounds.width, ComboHeightMax });

    ui.separator(5);

    ui.layoutRowDynamic(20, 1);
    ui.propertyFloat("Scale", 0.1f, scale, 20.0f, 0.1f, 0.1f);

    NoiseFunction noiseFunction = static_cast<NoiseFunction>(noiseChoice);

    switch (noiseFunction) {
      case NoiseFunction::Noise:
        ui.label("Step function:");
        bounds = ui.getWidgetBounds();
        ui.combobox(stepChoices, stepChoice, 20, { bounds.width, ComboHeightMax });
        break;

      case NoiseFunction::Gradient:
        ui.label("Step function:");
        bounds = ui.getWidgetBounds();
        ui.combobox(stepChoices, stepChoice, 20, { bounds.width, ComboHeightMax });
        break;

      case NoiseFunction::Worley:
        ui.propertyInt("Point count", 5, pointCount, 40, 1, 1.0f);
        ui.label("Distance function:");
        bounds = ui.getWidgetBounds();
        ui.combobox(distanceChoices, distanceChoice, 20, { bounds.width, ComboHeightMax });
        ui.label("Combination:");
        bounds = ui.getWidgetBounds();
        ui.combobox(combinationChoices, combinationChoice, 20, { bounds.width, ComboHeightMax });

      default:
        break;
    }

    ui.separator(5);

    if (ui.treePush(gf::UITree::Tab, "Fractal", fractalCollapsed)) {
      ui.layoutRowDynamic(20, 1);
      bounds = ui.getWidgetBounds();
      ui.combobox(fractalChoices, fractalChoice, 20, { bounds.width, ComboHeightMax });

      fractalParams.fractal = static_cast<Fractal>(fractalChoice);

      if (fractalParams.fractal != Fractal::None) {
        switch (fractalParams.fractal) {
          case Fractal::None:
            assert(false);
            break;

          case Fractal::FBm:
            ui.propertyFloat("Dimension", 0.1f, fractalParams.f.dimension, 1.9f, 0.05f, 0.05f);
            break;

          case Fractal::Multifractal:
            ui.propertyFloat("Dimension", 0.1f, fractalParams.m.dimension, 1.9f, 0.05f, 0.05f);
            break;

          case Fractal::HeteroTerrain:
            ui.propertyFloat("Dimension", 0.1f, fractalParams.ht.dimension, 1.9f, 0.05f, 0.05f);
            ui.propertyFloat("Offset", 0.0f, fractalParams.ht.offset, 10.0f, 0.1f, 0.1f);
            break;

          case Fractal::HybridMultifractal:
            ui.propertyFloat("Dimension", 0.1f, fractalParams.hm.dimension, 1.9f, 0.05f, 0.05f);
            ui.propertyFloat("Offset", 0.0f, fractalParams.hm.offset, 10.0f, 0.1f, 0.1f);
            break;

          case Fractal::RidgedMultifractal:
            ui.propertyFloat("Dimension", 0.1f, fractalParams.rm.dimension, 1.9f, 0.05f, 0.05f);
            ui.propertyFloat("Offset", 0.0f, fractalParams.rm.offset, 10.0f, 0.1f, 0.1f);
            ui.propertyFloat("Gain", 1.0f, fractalParams.rm.gain, 3.0f, 0.1f, 0.1f);
            break;
        }

        ui.propertyInt("Octaves", 1, fractalParams.octaves, 15, 1, 1.0f);
        ui.propertyFloat("Lacunarity", 1.0f, fractalParams.lacunarity, 3.0f, 0.1f, 0.1f);
        ui.propertyFloat("Persistence", 0.1f, fractalParams.persistence, 0.9f, 0.1f, 0.1f);

      }

      ui.treePop();
    }

    ui.separator(5);

    if (ui.treePush(gf::UITree::Tab, "Rendering", renderingCollapsed)) {
      ui.layoutRowDynamic(20, 1);
      bounds = ui.getWidgetBounds();
      ui.combobox(renderingChoices, renderingChoice, 20, { bounds.width, ComboHeightMax });

      renderingParams.rendering = static_cast<Rendering>(renderingChoice);

      switch (renderingParams.rendering) {
        case Rendering::Grayscale:
          break;

        case Rendering::Colored:
          ui.propertyFloat("Water level", 0.0f, renderingParams.waterLevel, 1.0f, 0.05f, 0.05f);
          ui.checkbox("Shaded", renderingParams.shaded);
          break;
      }

      ui.treePop();
    }

    ui.separator(5);

    ui.layoutRowDynamic(20, 1);
    if (ui.buttonLabel("Generate")) {
      gf::Clock clock;

      switch (noiseFunction) {
        case NoiseFunction::Noise: {
          StepFunction stepFunction = static_cast<StepFunction>(stepChoice);
          gf::Step<double> step = getStepFunction(stepFunction);

          gf::ValueNoise2D noise(random, step);
          image = generate(texture, renderingParams, heightmap, noise, fractalParams, scale);
          break;
        }

        case NoiseFunction::Gradient: {
          StepFunction stepFunction = static_cast<StepFunction>(stepChoice);
          gf::Step<double> step = getStepFunction(stepFunction);

          gf::GradientNoise2D noise(random, step);
          image = generate(texture, renderingParams, heightmap, noise, fractalParams, scale);
          break;
        }

        case NoiseFunction::BetterGradient: {
          gf::BetterGradientNoise2D noise(random);
          image = generate(texture, renderingParams, heightmap, noise, fractalParams, scale);
          break;
        }

        case NoiseFunction::Simplex: {
          gf::SimplexNoise2D noise(random);
          image = generate(texture, renderingParams, heightmap, noise, fractalParams, scale);
          break;
        }

        case NoiseFunction::OpenSimplex: {
          gf::OpenSimplexNoise2D noise(random);
          image = generate(texture, renderingParams, heightmap, noise, fractalParams, scale);
          break;
        }

        case NoiseFunction::Worley: {
          DistanceFunction distanceFunction = static_cast<DistanceFunction>(distanceChoice);
          gf::Distance2<double> distance = getDistanceFunction(distanceFunction);

          CombinationFunction combinationFunction = static_cast<CombinationFunction>(combinationChoice);
          std::vector<double> combination = getCombinationVector(combinationFunction);

          gf::WorleyNoise2D noise(random, pointCount, distance, combination);
          image = generate(texture, renderingParams, heightmap, noise, fractalParams, scale);
          break;
        }

        case NoiseFunction::Wavelet: {
          gf::WaveletNoise3D noise(random);
          gf::Noise3DTo2DAdapter adapter(noise);
          image = generate(texture, renderingParams, heightmap, adapter, fractalParams, scale);
          break;
        }
      }

      gf::Time duration = clock.getElapsedTime();
      feedback = "Generation time: " + std::to_string(duration.asMilliseconds()) + " ms";
    }

    if (ui.buttonLabel("Save to 'noise.png'")) {
      image.saveToFile("noise.png");
    }

    if (!feedback.empty()) {
      ui.label(feedback);
    }

    ui.end();

    renderer.clear();
    renderer.draw(sprite);
    renderer.draw(ui);
    renderer.display();
  }

  return 0;
}
