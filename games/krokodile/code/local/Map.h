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
#ifndef KKD_MAP_H
#define KKD_MAP_H

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/TileLayer.h>

namespace kkd {

  class Map : public gf::Entity {
  public:
    Map();

    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    static constexpr int Size = 75;

  private:
    gf::Texture& m_texture;
    gf::TileLayer m_layer;
  };

}

#endif // KKD_MAP_H
