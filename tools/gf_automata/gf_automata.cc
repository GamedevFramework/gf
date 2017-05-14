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
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/Log.h>
#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/UI.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/VertexArray.h>
#include <gf/Window.h>

#include "config.h"

static gf::Array2D<float> generateBase(gf::Vector2u size, gf::Random& random) {
  gf::Array2D<float> ret(size);

  for (auto& value : ret) {
    value = random.computeUniformFloat(0.0f, 1.0f);
  }

  return ret;
}

enum class State : uint8_t {
  Empty,
  Full,
};

static int number(State state) {
  switch (state) {
    case State::Empty:
      return 0;
    case State::Full:
      return 1;
  }

  assert(false);
  return 0;
}

static gf::Array2D<State> computeFirst(const gf::Array2D<float>& array, float threshold) {
  gf::Array2D<State> ret(array.getSize());

  for (auto pos : array.getPositionRange()) {
    if (array(pos) > threshold) {
      ret(pos) = State::Full;
    } else {
      ret(pos) = State::Empty;
    }
  }

  return ret;
}

enum class Mode : int {
  Diamond4    = 0,
  Square8     = 1,
  Diamond12   = 2,
  Square24    = 3,
};

int modeMax(int mode) {
  switch (mode) {
    case 0:
      return 4;
    case 1:
      return 8;
    case 2:
      return 12;
    case 3:
      return 24;
    default:
      assert(false);
      break;
  }

  assert(false);
  return 0;
}

struct Params {
  Mode mode;
  int survivalThreshold;
  int birthThreshold;
  int iterations;
};

static void computeIterations(gf::Array2D<State>& automaton, const Params& params) {
  gf::Array2D<State> result(automaton.getSize());

  for (int i = 0; i < params.iterations; ++i) {
    for (auto row : automaton.getRowRange()) {
      for (auto col : automaton.getColRange()) {
        gf::Vector2u pos(col, row);
        int count = 0;

        switch (params.mode) {
          case Mode::Diamond4:
            automaton.visit4Neighbors(pos, [&count](gf::Vector2u neighbor, State state) {
              gf::unused(neighbor);
              count += number(state);
            });
            break;
          case Mode::Square8:
            automaton.visit8Neighbors(pos, [&count](gf::Vector2u neighbor, State state) {
              gf::unused(neighbor);
              count += number(state);
            });
            break;
          case Mode::Diamond12:
            automaton.visit12Neighbors(pos, [&count](gf::Vector2u neighbor, State state) {
              gf::unused(neighbor);
              count += number(state);
            });
            break;
          case Mode::Square24:
            automaton.visit24Neighbors(pos, [&count](gf::Vector2u neighbor, State state) {
              gf::unused(neighbor);
              count += number(state);
            });
            break;
        }

        if (automaton(pos) == State::Full) {
          if (count >= params.survivalThreshold) {
            result(pos) = State::Full;
          } else {
            result(pos) = State::Empty;
          }
        } else {
          if (count >= params.birthThreshold) {
            result(pos) = State::Full;
          } else {
            result(pos) = State::Empty;
          }
        }
      }
    }

    automaton.swap(result);
  }
}


static void computeDisplay(const gf::Array2D<State>& automaton, gf::VertexArray& vertices) {
  static constexpr float CellSize = 16.0f;

  vertices.clear();

  for (auto row : automaton.getRowRange()) {
    for (auto col : automaton.getColRange()) {
      gf::Vector2u pos(col, row);


      gf::Vertex v[4];
      v[0].position = pos * CellSize;
      v[1].position = pos * CellSize + gf::Vector2f(CellSize, 0.0f);
      v[2].position = pos * CellSize + gf::Vector2f(0.0f, CellSize);
      v[3].position = pos * CellSize + gf::Vector2f(CellSize, CellSize);

      if (automaton(pos) == State::Full) {
        v[0].color = v[1].color = v[2].color = v[3].color = gf::Color::White;
      } else {
        v[0].color = v[1].color = v[2].color = v[3].color = gf::Color::Black;
      }

      vertices.append(v[0]);
      vertices.append(v[1]);
      vertices.append(v[2]);

      vertices.append(v[2]);
      vertices.append(v[1]);
      vertices.append(v[3]);
    }
  }
}

int main() {
  gf::Random random;

  static constexpr unsigned Size = 1024;
  static constexpr unsigned ExtraSize = 250;
  static constexpr float ViewportX = static_cast<float>(Size) / (Size + ExtraSize);

  static constexpr float ComboHeightMax = 200.0f;

  static constexpr float ZoomInFactor = 0.8f;
  static constexpr float ZoomOutFactor = 1.25f;

  gf::Window window("gf automata", { Size + ExtraSize, Size }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font;

  if (!font.loadFromFile(gf::Path(GF_DATADIR) / "DroidSans.ttf")) {
    return EXIT_FAILURE;
  }

  gf::UI ui(font);

  // views

  gf::ViewContainer views;

  gf::ExtendView automatonView(gf::RectF( { 0.0f, 0.0f }, { Size, Size }));
  automatonView.setViewport(gf::RectF({ 0.0f, 0.0f }, { ViewportX, 1.0f }));
  views.addView(automatonView);

  gf::ScreenView uiView;
  views.addView(uiView);

  views.setInitialScreenSize({ Size + ExtraSize, Size });

  // ui

  std::vector<std::string> modeChoices = { "Diamond-4", "Square-8", "Diamond-12", "Square-24" };
  int modeChoice = 1;
  int currentModeChoice = modeChoice;

  // state

  unsigned automataSize = 64;
  int log2AutomataSize = 6;
  float threshold = 0.4;

  Params params;
  params.mode = Mode::Square8;
  params.survivalThreshold = 4;
  params.birthThreshold = 6;
  params.iterations = 5;

  gf::Array2D<float> base = generateBase({ automataSize, automataSize }, random);
  gf::Array2D<State> automaton = computeFirst(base, threshold);
  computeIterations(automaton, params);

  gf::VertexArray vertices(gf::PrimitiveType::Triangles);
  computeDisplay(automaton, vertices);

  // zoom and move

  gf::Vector2i mousePosition;
  bool isMoving = false;

  renderer.clear(gf::Color::Gray());

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

        case gf::EventType::MouseMoved:
          if (isMoving) {
            gf::Vector2f oldPosition = renderer.mapPixelToCoords(mousePosition, automatonView);
            gf::Vector2f newPosition = renderer.mapPixelToCoords(event.mouseCursor.coords, automatonView);
            automatonView.move(oldPosition - newPosition);
          }

          mousePosition = event.mouseCursor.coords;
          break;

        case gf::EventType::MouseButtonPressed:
          if (event.mouseButton.coords.x < static_cast<int>(Size)) {
            isMoving = true;
          }
          break;

        case gf::EventType::MouseButtonReleased:
          isMoving = false;
          break;

        case gf::EventType::MouseWheelScrolled:
          if (event.mouseWheel.offset.y > 0) {
            automatonView.zoom(ZoomInFactor, renderer.mapPixelToCoords(mousePosition, automatonView));
          } else {
            automatonView.zoom(ZoomOutFactor, renderer.mapPixelToCoords(mousePosition, automatonView));
          }
          break;

        default:
          break;
      }

      ui.processEvent(event);
      views.processEvent(event);
    }

    bool parameterChanged = false;

    ui.begin("Cellular automata", gf::RectF(Size, 0, ExtraSize, Size), gf::UIWindow::Title | gf::UIWindow::Border);

    ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
    ui.label("Size");
    ui.label(std::to_string(automataSize), gf::UIAlignment::Right);
    ui.layoutRowDynamic(20, 1);
    if (ui.sliderInt(5, log2AutomataSize, 9, 1)) {
      automataSize = 1 << log2AutomataSize;
      base = generateBase({ automataSize, automataSize }, random);
      parameterChanged = true;
    }

    ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
    ui.label("Initial ratio");
    ui.label(gf::niceNum(threshold, 0.01f), gf::UIAlignment::Right);
    ui.layoutRowDynamic(20, 1);
    parameterChanged = ui.sliderFloat(0.0f, threshold, 1.0f, 0.01f) || parameterChanged;

    ui.layoutRowDynamic(20, 1);
    if (ui.buttonLabel("Generate")) {
      base = generateBase({ automataSize, automataSize }, random);
      parameterChanged = true;
    }

    ui.layoutRowDynamic(20, 1);
    ui.label("Neighborhood");
    auto bounds = ui.getWidgetBounds();
    ui.combobox(modeChoices, modeChoice, 20, { bounds.width, ComboHeightMax });

    if (currentModeChoice != modeChoice) {
      currentModeChoice = modeChoice;
      params.mode = static_cast<Mode>(modeChoice);
      params.survivalThreshold = std::min(params.survivalThreshold, modeMax(currentModeChoice));
      params.birthThreshold = std::min(params.birthThreshold, modeMax(currentModeChoice));
      parameterChanged = true;
    }

    ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
    ui.label("Survival Threshold");
    ui.label(std::to_string(params.survivalThreshold), gf::UIAlignment::Right);
    ui.layoutRowDynamic(20, 1);
    parameterChanged = ui.sliderInt(0, params.survivalThreshold, modeMax(currentModeChoice), 1) || parameterChanged;

    ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
    ui.label("Birth Threshold");
    ui.label(std::to_string(params.birthThreshold), gf::UIAlignment::Right);
    ui.layoutRowDynamic(20, 1);
    parameterChanged = ui.sliderInt(0, params.birthThreshold, modeMax(currentModeChoice), 1) || parameterChanged;

    ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
    ui.label("Number of Iterations");
    ui.label(std::to_string(params.iterations), gf::UIAlignment::Right);
    ui.layoutRowDynamic(20, 1);
    parameterChanged = ui.sliderInt(0, params.iterations, 20, 1) || parameterChanged;

    ui.end();

    if (parameterChanged) {
      automaton = computeFirst(base, threshold);
      computeIterations(automaton, params);
      computeDisplay(automaton, vertices);
    }

    renderer.clear();

    renderer.setView(automatonView);
    renderer.draw(vertices);

    renderer.setView(uiView);
    renderer.draw(ui);

    renderer.display();
  }

  return 0;
}
