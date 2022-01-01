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
#include <gf/Math.h>
#include <gf/RenderWindow.h>
#include <gf/Texture.h>
#include <gf/TileLayer.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Window.h>

static constexpr int MapWidth = 44;
static constexpr int MapHeight = 25;

static int map[MapHeight][MapWidth] = {
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
  { -1, -1, 346, 315, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 284, 253, -1, -1, },
  { -1, -1, 252, 221, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 190, 159, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, 62, 62, 62, 62, 189, 62, 62, 62, 62, 439, -1, -1, -1, -1, -1, -1, -1, -1, 346, 315, 0, 0, 0, 0, 96, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, -1, -1, -1, -1, -1, -1, 252, 221, 282, 282, 282, 282, 190, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, -1, 346, 315, 284, 253, -1, 407, 345, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 313, 62, 62, 62, 62, 62, 62, 62, 62, 62, 251, -1, -1, -1, 252, 221, 190, 159, -1, 407, 345, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, 407, 345, -1, 407, 345, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, 158, 127, 0, 96, 65, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, 346, 315, 0, 0, 0, 284, 253, -1, 346, 315, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 65, 64, 33, 282, 2, 435, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, 252, 221, 282, 282, 282, 190, 159, -1, 252, 221, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 2, 435, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, 407, 345, -1, -1, -1, 407, 345, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, 407, 345, -1, -1, -1, 407, 345, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, 345, -1, -1, },
  { -1, -1, 407, 345, 407, 345, -1, -1, -1, 158, 127, 0, 96, 65, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, 188, 345, -1, -1, },
  { -1, -1, 407, 345, 158, 127, 0, 284, 253, 64, 33, 282, 2, 435, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, 407, 345, -1, -1, },
  { -1, -1, 407, 345, 64, 33, 282, 190, 159, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, 407, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, 407, 345, -1, -1, },
  { -1, -1, 407, 345, -1, -1, -1, 407, 345, 31, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 251, 407, 345, -1, -1, },
  { -1, -1, 158, 127, 0, 0, 0, 96, 127, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 217, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 65, -1, -1, },
  { -1, -1, 64, 33, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 155, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 2, 435, -1, -1, },
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
};

static constexpr int TileSize = 128;

static constexpr float ZoomInFactor = 0.8f;
static constexpr float ZoomOutFactor = 1.25f;

int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);

  gf::Window window("18_tilelayer", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ExtendView view;
  view.setCenter({ 0.0f, 0.0f });
  view.setSize({ 480.0f, 480.0f });
  views.addView(view);

  views.setInitialFramebufferSize(ScreenSize);

  gf::ZoomingViewAdaptor adaptor(renderer, view);

  gf::Texture texture("assets/tilelayer.png");
  texture.setSmooth();

  gf::TileLayer tileLayer = gf::TileLayer::createOrthogonal({ MapWidth, MapHeight }, { TileSize, TileSize });

  std::size_t id = tileLayer.createTilesetId();
  gf::Tileset& tileset = tileLayer.getTileset(id);
  tileset.setTileSize({ TileSize, TileSize });
  tileset.setSpacing(2);
  tileset.setTexture(texture);

  tileLayer.setOrigin({ TileSize * MapWidth / 2, TileSize * MapHeight / 2 });

  for (int y = 0; y < MapHeight; ++y) {
    for (int x = 0; x < MapWidth; ++x) {
      tileLayer.setTile({ x, y }, id, map[y][x]);
    }
  }

  std::cout << "Gamedev Framework (gf) example #18: Tile Layer\n";
  std::cout << "This example shows a tile layer.\n";
  std::cout << "How to use:\n";
  std::cout << "\tUp/Down: Zoom in/out\n";
  std::cout << "\tLeft/Right: Rotate\n";
  std::cout << "\tMouse: Scroll to zoom, press to move\n";

  renderer.clear(gf::Color::darker(gf::Color::Spring));

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::Left:
              view.rotate(gf::Pi / 8);
              break;

            case gf::Scancode::Right:
              view.rotate(-gf::Pi / 8);
              break;

            case gf::Scancode::Up:
              view.zoom(ZoomInFactor);
              break;

            case gf::Scancode::Down:
              view.zoom(ZoomOutFactor);
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
    renderer.draw(tileLayer);
    renderer.display();
  }

  return 0;
}
