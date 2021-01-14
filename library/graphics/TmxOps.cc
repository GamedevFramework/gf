/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#include <gf/TmxOps.h>

#include <cassert>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  TileLayer makeTileLayer(const TmxLayers& map, const TmxTileLayer& layer, ResourceManager& resources) {
    TileLayer tiles;

    switch (map.orientation) {
      case TileOrientation::Orthogonal:
        tiles = TileLayer::createOrthogonal(map.mapSize);
        break;
      case TileOrientation::Staggered:
        tiles = TileLayer::createStaggered(map.mapSize, map.mapCellAxis, map.mapCellIndex);
        break;
      default:
        assert(false);
        break;
    }

    tiles.setTileSize(map.tileSize);

    std::map<const TmxTileset *, std::size_t> mapping;
    int k = 0;

    for (auto& cell : layer.cells) {
      int i = k % map.mapSize.width;
      int j = k / map.mapSize.width;
      assert(j < map.mapSize.height);

      int gid = cell.gid;

      if (gid != 0) {
        const TmxTileset *tileset = map.getTileSetFromGID(gid);
        assert(tileset);

        std::size_t id;

        if (auto it = mapping.find(tileset); it != mapping.end()) {
          id = it->second;
        } else {
          id = tiles.createTilesetId();

          assert(tileset->image);
          const gf::Texture& texture = resources.getTexture(tileset->image->source);

          Tileset& ts = tiles.getTileset(id);
          ts.setTexture(texture);

          ts.setTileSize(tileset->tileSize);
          ts.setOffset(tileset->offset);
          ts.setMargin(tileset->margin);
          ts.setSpacing(tileset->spacing);
        }

        gid = gid - tileset->firstGid;
        tiles.setTile({ i, j }, id, gid, cell.flip);
      }

      k++;
    }

    return tiles;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
