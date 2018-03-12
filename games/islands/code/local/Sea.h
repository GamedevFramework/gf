/*
 * Bygone Islands, the journey of a pirate in dangerous islands
 * Copyright (C) 2017  Hatunruna team
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
#ifndef BI_SEA_H
#define BI_SEA_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Random.h>
#include <gf/VertexArray.h>

#include "DecorationManager.h"
#include "TreasureManager.h"
#include "TurretManager.h"

namespace bi {

  class Sea : public gf::Entity {
  public:
    static constexpr unsigned Size = 1500;
    static constexpr unsigned Edge = 80;
    static constexpr float TileSize = 8.0f;
    static constexpr float WorldMin = Edge * TileSize;
    static constexpr float WorldMax = (Size - Edge) * TileSize;

    Sea();

    void generate(TreasureManager& treasures, DecorationManager& decorationsAbove, DecorationManager& decorationsBelow, TurretManager &turrets);

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onGoldLooted(gf::Id id, gf::Message *msg);

  private:
    gf::VertexArray m_seaVertices;
    gf::VertexArray m_landVertices;
    gf::Array2D<float> m_terrain;
    gf::Array2D<gf::Color4f> m_sea;
    gf::Array2D<gf::Color4f> m_land;
    gf::Vector2u m_hero;
    bool m_heroMoved;
  };

}

#endif // BI_SEA_H
