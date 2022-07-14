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
#include <iostream>

#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/Math.h>
#include <gf/Paths.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>
#include <gf/TileLayer.h>
#include <gf/Tmx.h>
#include <gf/TmxOps.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Window.h>

struct LayersMaker : public gf::TmxVisitor {

  virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
    if (!layer.visible) {
      return;
    }

    std::cout << "Parsing layer '" << layer.name << "'\n";

    layers.emplace_back(gf::makeTileLayer(map, layer, resources));
  }

  virtual void visitObjectLayer([[maybe_unused]] const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
    if (!layer.visible) {
      return;
    }

    if (objects != nullptr) {
      return;
    }

    color = gf::Color::fromRgba32(layer.color);
    objects = &layer.objects;
  }


  gf::ResourceManager resources;
  std::vector<gf::TileLayer> layers;

  gf::Color4f color;
  const std::vector<std::unique_ptr<gf::TmxObject>> *objects = nullptr;
};

int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);

  gf::Window window("44_tmx", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ExtendView view(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 640.0f, 480.0f }));
  views.addView(view);

  views.setInitialFramebufferSize(ScreenSize);

  gf::ZoomingViewAdaptor adaptor(renderer, view);

  // orthogonal layers

  gf::TmxLayers orthogonalLayers;

  if (!orthogonalLayers.loadFromFile("assets/outdoor.tmx")) {
    return EXIT_FAILURE;
  }

  LayersMaker orthogonalMaker;
  orthogonalMaker.resources.addSearchDir(gf::Paths::getBasePath());
  orthogonalMaker.resources.addSearchDir(gf::Paths::getCurrentPath());
  orthogonalLayers.visitLayers(orthogonalMaker);

  std::cout << "Number of layers: " << orthogonalMaker.layers.size() << "\n";

  // stagerred layers

  gf::TmxLayers staggeredLayers;

  if (!staggeredLayers.loadFromFile("assets/isometric_staggered_grass_and_water.tmx")) {
    return EXIT_FAILURE;
  }

  LayersMaker staggeredMaker;
  staggeredMaker.resources.addSearchDir(gf::Paths::getBasePath());
  staggeredMaker.resources.addSearchDir(gf::Paths::getCurrentPath());
  staggeredLayers.visitLayers(staggeredMaker);

  // hexagonal layers

  gf::TmxLayers hexagonalLayers;

  if (!hexagonalLayers.loadFromFile("assets/hexagonal-mini.tmx")) {
    return EXIT_FAILURE;
  }

  LayersMaker hexagonalMaker;
  hexagonalMaker.resources.addSearchDir(gf::Paths::getBasePath());
  hexagonalMaker.resources.addSearchDir(gf::Paths::getCurrentPath());
  hexagonalLayers.visitLayers(hexagonalMaker);

  for (auto& layer : hexagonalMaker.layers) {
    layer.scale(4);
  }

  //

  std::cout << "Gamedev Framework (gf) example #44: TMX loading\n";
  std::cout << "This example shows a TMX file with two tile layers.\n";
  std::cout << "How to use:\n";
  std::cout << "\tReturn: Display next map\n";
  std::cout << "\tMouse: Scroll to zoom, press to move\n";

  renderer.clear(gf::Color::White);

  gf::CellOrientation orientation = gf::CellOrientation::Orthogonal;

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
              if (orientation == gf::CellOrientation::Orthogonal) {
                orientation = gf::CellOrientation::Staggered;
              } else if (orientation == gf::CellOrientation::Staggered) {
                orientation = gf::CellOrientation::Hexagonal;
              } else {
                orientation = gf::CellOrientation::Orthogonal;
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

      adaptor.processEvent(event);
      views.processEvent(event);
    }

    renderer.setView(view);
    renderer.clear();

    switch (orientation) {
      case gf::CellOrientation::Orthogonal:
        for (auto& layer : orthogonalMaker.layers) {
          renderer.draw(layer);
        }
        break;

      case gf::CellOrientation::Staggered:
        for (auto& layer : staggeredMaker.layers) {
          renderer.draw(layer);
        }
        break;

      case gf::CellOrientation::Hexagonal:
        for (auto& layer : hexagonalMaker.layers) {
          renderer.draw(layer);
        }
        break;

      default:
        break;
    }

    renderer.display();
  }

  return 0;
}
