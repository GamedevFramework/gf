/*
 * H.O.M.E.
 * Copyright (C) 2019 Hatunruna team
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
#ifndef HOME_MAP_H
#define HOME_MAP_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/TileLayer.h>
#include <gf/Tmx.h>

#include "Messages.h"
#include "SupplyManager.h"

namespace home {

  struct MapGraphicsData {
    MapGraphicsData(const gf::TmxLayers& layers, SupplyManager& supplies);

    std::vector<gf::TileLayer> tiles;
    std::vector<gf::Sprite> sprites;
  };

  class Map : public gf::Entity {
  public :
    enum Type {
      Below,
      Above,
    };

    Map(Type type, MapGraphicsData& data);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    Type m_type;
    MapGraphicsData& m_data;
    gf::Vector2f m_hero;
  };
}

#endif // HOME_MAP_H
