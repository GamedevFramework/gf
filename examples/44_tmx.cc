/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/Unused.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Window.h>

gf::TileLayer::Type getTypeFromOrientation(gf::TmxOrientation orientation) {
  switch (orientation) {
    case gf::TmxOrientation::Orthogonal:
      return gf::TileLayer::Orthogonal;
    case gf::TmxOrientation::Staggered:
      return gf::TileLayer::Staggered;
    default:
      break;
  }

  assert(false);
  return gf::TileLayer::Orthogonal;
}

struct LayersMaker : public gf::TmxVisitor {

  virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
    if (!layer.visible) {
      return;
    }

    std::cout << "Parsing layer '" << layer.name << "'\n";

    gf::TileLayer tiles(map.mapSize, getTypeFromOrientation(map.orientation));
    tiles.setBlockSize(map.tileSize);

    unsigned k = 0;

    for (auto& cell : layer.cells) {
      unsigned i = k % map.mapSize.width;
      unsigned j = k / map.mapSize.width;
      assert(j < map.mapSize.height);

      unsigned gid = cell.gid;

      if (gid != 0) {
        auto tileset = map.getTileSetFromGID(gid);
        assert(tileset);
        tiles.setTileSize(tileset->tileSize);

        gid = gid - tileset->firstGid;
        tiles.setTile({ i, j }, gid, cell.flip);

        if (!tiles.hasTexture()) {
          assert(tileset->image);
          const gf::Texture& texture = resources.getTexture(tileset->image->source);
          tiles.setTexture(texture);
        } else {
          assert(&resources.getTexture(tileset->image->source) == &tiles.getTexture());
        }

      }

      k++;
    }

    layers.push_back(std::move(tiles));
  }

  virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
    gf::unused(map);

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
  static constexpr gf::Vector2u ScreenSize(640, 480);

  gf::Window window("44_tmx", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ExtendView view(gf::RectF({ 0.0f, 0.0f }, { 640.0f, 480.0f }));
  views.addView(view);

  views.setInitialScreenSize(ScreenSize);

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

  //

  std::cout << "Gamedev Framework (gf) example #44: TMX loading\n";
  std::cout << "This example shows a TMX file with two tile layers.\n";
  std::cout << "How to use:\n";
  std::cout << "\tReturn: Display next map\n";
  std::cout << "\tMouse: Scroll to zoom, press to move\n";

  renderer.clear(gf::Color::White);

  gf::TileLayer::Type type = gf::TileLayer::Orthogonal;

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
              if (type == gf::TileLayer::Orthogonal) {
                type = gf::TileLayer::Staggered;
              } else {
                type = gf::TileLayer::Orthogonal;
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

    switch (type) {
      case gf::TileLayer::Orthogonal:
        for (auto& layer : orthogonalMaker.layers) {
          renderer.draw(layer);
        }
        break;

      case gf::TileLayer::Staggered:
        for (auto& layer : staggeredMaker.layers) {
          renderer.draw(layer);
        }
        break;
    }

    renderer.display();
  }

  return 0;
}
