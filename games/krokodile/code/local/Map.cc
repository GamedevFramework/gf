/*
 * Krokodile
 * Copyright (C) 2018 Hatunruna team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Map.h"

#include <cassert>

#include <gf/Heightmap.h>
#include <gf/Noises.h>
#include <gf/RenderTarget.h>

#include "Singletons.h"

namespace kkd {

  Map::Map()
  : m_texture(gResourceManager().getTexture("map.png"))
  , m_layer({ Size, Size })
  {
    m_layer.setTexture(m_texture);
    m_layer.setTileSize({ 64u, 64u });

    gf::Heightmap heightmap({ Size, Size });
    heightmap.reset();

    gf::PerlinNoise2D noise(gRandom(), 2);
    heightmap.addNoise(noise);
    heightmap.normalize();

    for (int y = 0; y < Size; ++y) {
      for (int x = 0; x < Size; ++x) {
        double value = heightmap.getValue({ x, y });
        assert(0.0 <= value && value <= 1.0);
        int tile = static_cast<int>(value * 3.999999);
        assert(0 <= tile && tile <= 3);
        m_layer.setTile({ x, y }, tile);
      }
    }

    m_layer.setOrigin({ 64.f * Size / 2, 64.f * Size / 2 });
  }

  void Map::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    target.draw(m_layer, states);
  }

}
