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
#include "Sea.h"

#include <gf/ColorRamp.h>
#include <gf/Noises.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Messages.h"
#include "Singletons.h"

namespace bi {
  static constexpr double Scale = 6.0;
  static constexpr double SeaLevel = 0.65;

  static constexpr unsigned DecorationCount = 200;
  static constexpr unsigned DisplayHalfRange = 100;
  static constexpr unsigned TreasureCount = 30;
  static constexpr unsigned TurretCount = 50;

  Sea::Sea()
  : m_seaVertices(gf::PrimitiveType::Triangles)
  , m_landVertices(gf::PrimitiveType::Triangles)
  , m_terrain({ Size, Size })
  , m_sea({ Size, Size })
  , m_land({ Size, Size })
  , m_hero({ 0, 0 })
  , m_heroMoved(true)
  {
    gMessageManager().registerHandler<HeroPosition>(&Sea::onHeroPosition, this);
    gMessageManager().registerHandler<GoldLooted>(&Sea::onGoldLooted, this);
  }

  static double valueWithWaterLevel(double value, double waterLevel) {
    if (value < waterLevel) {
      return value / waterLevel * 0.5;
    }

    return (value - waterLevel) / (1.0 - waterLevel) * 0.5 + 0.5;
  }

  void Sea::generate(TreasureManager& treasures, DecorationManager& decorationsAbove, DecorationManager& decorationsBelow, TurretManager &turrets) {
    // generate the elevation

    gf::SimplexNoise2D simplex(gRandom());
    gf::FractalNoise2D fractal(simplex, 1);

    for (auto row : m_terrain.getRowRange()) {
      double y = static_cast<double>(row) / m_terrain.getRows() * Scale;
      for (auto col : m_terrain.getColRange()) {
        double x = static_cast<double>(col) / m_terrain.getCols() * Scale;
        m_terrain({ col, row }) = fractal.getValue(x, y);
      }
    }

    // normalize

    float min = *std::min_element(m_terrain.begin(), m_terrain.end());
    float max = *std::max_element(m_terrain.begin(), m_terrain.end());


    for (auto& elevation : m_terrain) {
      elevation = (elevation - min) / (max - min);
      elevation = valueWithWaterLevel(elevation, SeaLevel);
      assert(0.0 <= elevation && elevation <= 1.0);
    }

    // treasures

    for (unsigned i = 0; i < TreasureCount; ++i) {
      gf::Vector2f position;
      unsigned col, row;

      do {
        position.x = gRandom().computeUniformFloat(WorldMin, WorldMax);
        position.y = gRandom().computeUniformFloat(WorldMin, WorldMax);

        col = static_cast<unsigned>(position.x / TileSize);
        row = static_cast<unsigned>(position.y / TileSize);
      } while (m_terrain({ row, col }) < 0.52f);

      treasures.addTreasure(position);
    }

    // decorations

    for (unsigned i = 0; i < DecorationCount / 2; ++i) {
      gf::Vector2f position;
      unsigned col, row;

      do {
        position.x = gRandom().computeUniformFloat(WorldMin, WorldMax);
        position.y = gRandom().computeUniformFloat(WorldMin, WorldMax);

        col = static_cast<unsigned>(position.x / TileSize);
        row = static_cast<unsigned>(position.y / TileSize);
      } while (m_terrain({ row, col }) < 0.52f);

      decorationsAbove.addDecoration(position);
    }

    for (unsigned i = 0; i < DecorationCount / 2; ++i) {
      gf::Vector2f position;
      unsigned col, row;

      do {
        position.x = gRandom().computeUniformFloat(WorldMin, WorldMax);
        position.y = gRandom().computeUniformFloat(WorldMin, WorldMax);

        col = static_cast<unsigned>(position.x / TileSize);
        row = static_cast<unsigned>(position.y / TileSize);
      } while (m_terrain({ row, col }) < 0.52f);

      decorationsBelow.addDecoration(position);
    }

    // turret

    for (unsigned i = 0; i < TurretCount; ++i) {
      gf::Vector2f position;
      unsigned col, row;

      do {
        position.x = gRandom().computeUniformFloat(WorldMin, WorldMax);
        position.y = gRandom().computeUniformFloat(WorldMin, WorldMax);

        col = static_cast<unsigned>(position.x / TileSize);
        row = static_cast<unsigned>(position.y / TileSize);
      } while (m_terrain({ row, col }) < 0.52f);

      turrets.addTurret(position);
    }

    // compute colors

    gf::ColorRamp rampSea;
    rampSea.addColorStop(0.000f, gf::Color::fromRgba32(  2,  43,  68)); // very dark blue: deep water
    rampSea.addColorStop(0.250f, gf::Color::fromRgba32(  9,  62,  92)); // dark blue: water
    rampSea.addColorStop(0.499f, gf::Color::fromRgba32( 17,  82, 112)); // blue: shallow water
    rampSea.addColorStop(0.500f, gf::Color::fromRgba32( 69, 108, 118)); // light blue: shore

    gf::ColorRamp rampLand;
    rampLand.addColorStop(0.500f, gf::Color::fromRgba32(255, 251, 121)); // sand
    rampLand.addColorStop(0.550f, gf::Color::fromRgba32(255, 251, 121)); // sand
    rampLand.addColorStop(0.551f, gf::Color::fromRgba32( 54, 205,  20)); // grass
    rampLand.addColorStop(0.700f, gf::Color::fromRgba32( 54, 205,  20)); // grass
    rampLand.addColorStop(0.701f, gf::Color::fromRgba32( 38, 143,  14)); // grass
    rampLand.addColorStop(1.000f, gf::Color::fromRgba32( 38, 143,  14)); // grass

    static constexpr gf::Vector3f Light = { -1, -1, 0 };

    for (auto row : m_terrain.getRowRange()) {
      for (auto col : m_terrain.getColRange()) {
        float elevation = m_terrain({ row, col });
        assert(0.0f <= elevation && elevation <= 1.0);

        m_sea({ row, col }) = rampSea.computeColor(elevation);

        float x = static_cast<float>(col);
        float y = static_cast<float>(row);

        // compute the normal vector

        gf::Vector3f normal(0, 0, 0);
        unsigned count = 0;

        gf::Vector3f p{x, y, elevation};

        if (col > 0 && row > 0) {
          gf::Vector3f pn{x    , y - 1, m_terrain({ row - 1, col     })};
          gf::Vector3f pw{x - 1, y    , m_terrain({ row    , col - 1 })};

          gf::Vector3f v3 = cross(p - pw, p - pn);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (col > 0 && row < m_terrain.getRows() - 1) {
          gf::Vector3f pw{x - 1, y    , m_terrain({ row    , col - 1 })};
          gf::Vector3f ps{x    , y + 1, m_terrain({ row + 1, col     })};

          gf::Vector3f v3 = cross(p - ps, p - pw);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (col < m_terrain.getCols() - 1 && row > 0) {
          gf::Vector3f pe{x + 1, y    , m_terrain({ row    , col + 1 })};
          gf::Vector3f pn{x    , y - 1, m_terrain({ row - 1, col     })};

          gf::Vector3f v3 = cross(p - pn, p - pe);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (col < m_terrain.getCols() - 1 && row < m_terrain.getRows() - 1) {
          gf::Vector3f pe{x + 1, y    , m_terrain({ row    , col + 1 })};
          gf::Vector3f ps{x    , y + 1, m_terrain({ row + 1, col     })};

          gf::Vector3f v3 = cross(p - pe, p - ps);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        normal = gf::normalize(normal / count);
        float d = gf::dot(Light, normal);
        d = gf::clamp(0.5f + 35 * d, 0.0f, 1.0f);

        gf::Color4f color = rampLand.computeColor(elevation);

        gf::Color4f lo = gf::lerp(color, gf::Color::fromRgba32(0x33, 0x11, 0x33, 0xFF), 0.7);
        gf::Color4f hi = gf::lerp(color, gf::Color::fromRgba32(0xFF, 0xFF, 0xCC, 0xFF), 0.3);

        if (d < 0.5f) {
          color = gf::lerp(lo, color, 2 * d);
        } else {
          color = gf::lerp(color, hi, 2 * d - 1);
        }

        m_land({ row, col }) = color;
      }
    }
  }

  void Sea::update(gf::Time time) {
    gf::unused(time);

    if (!m_heroMoved) {
      return;
    }

    unsigned rowMin = (m_hero.y > DisplayHalfRange) ? (m_hero.y - DisplayHalfRange) : 0;
    unsigned rowMax = (m_hero.y + DisplayHalfRange < Size) ? (m_hero.y + DisplayHalfRange) : Size - 1;
    unsigned colMin = (m_hero.x > DisplayHalfRange) ? (m_hero.x - DisplayHalfRange) : 0;
    unsigned colMax = (m_hero.x + DisplayHalfRange < Size) ? (m_hero.x + DisplayHalfRange) : Size - 1;

    m_seaVertices.clear();
    m_landVertices.clear();

    for (unsigned row = rowMin; row < rowMax; ++row) {
      assert(row < Size - 1);

      for (unsigned col = colMin; col < colMax; ++col) {
        assert(col < Size - 1);

        gf::Vertex vertices[4];

        vertices[0].position = {  col      * TileSize,  row      * TileSize };
        vertices[1].position = { (col + 1) * TileSize,  row      * TileSize };
        vertices[2].position = {  col      * TileSize, (row + 1) * TileSize };
        vertices[3].position = { (col + 1) * TileSize, (row + 1) * TileSize };

        // vertices for sea

        vertices[0].color = m_sea({ row,     col     });
        vertices[1].color = m_sea({ row,     col + 1 });
        vertices[2].color = m_sea({ row + 1, col     });
        vertices[3].color = m_sea({ row + 1, col + 1 });

        // first triangle
        m_seaVertices.append(vertices[0]);
        m_seaVertices.append(vertices[1]);
        m_seaVertices.append(vertices[2]);

        // second triangle
        m_seaVertices.append(vertices[2]);
        m_seaVertices.append(vertices[1]);
        m_seaVertices.append(vertices[3]);

        // vertices for land

        std::array<float, 4> elevations = {{ m_terrain({ row, col }), m_terrain({ row, col + 1 }), m_terrain({ row + 1, col }), m_terrain({ row + 1, col + 1 }) }};
        auto count = std::count_if(elevations.begin(), elevations.end(), [](float elevation) { return elevation < 0.5f; });
        assert(0 <= count && count <= 4);

        if (count >= 2) {
          continue;
        }

        vertices[0].color = m_land({ row,     col     });
        vertices[1].color = m_land({ row,     col + 1 });
        vertices[2].color = m_land({ row + 1, col     });
        vertices[3].color = m_land({ row + 1, col + 1 });

        if (count == 0) {
          // first triangle
          m_landVertices.append(vertices[0]);
          m_landVertices.append(vertices[1]);
          m_landVertices.append(vertices[2]);

          // second triangle
          m_landVertices.append(vertices[2]);
          m_landVertices.append(vertices[1]);
          m_landVertices.append(vertices[3]);

          continue;
        }

        assert(count == 1);

        if (elevations[0] < 0.5f) {
          m_landVertices.append(vertices[2]);
          m_landVertices.append(vertices[1]);
          m_landVertices.append(vertices[3]);
        } else if (elevations[1] < 0.5f) {
          m_landVertices.append(vertices[0]);
          m_landVertices.append(vertices[3]);
          m_landVertices.append(vertices[2]);
        } else if (elevations[2] < 0.5f) {
          m_landVertices.append(vertices[0]);
          m_landVertices.append(vertices[1]);
          m_landVertices.append(vertices[3]);
        } else if (elevations[3] < 0.5f) {
          m_landVertices.append(vertices[0]);
          m_landVertices.append(vertices[1]);
          m_landVertices.append(vertices[2]);
        }

      }
    }

    m_heroMoved = false;
  }

  void Sea::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(m_seaVertices, states);
    target.draw(m_landVertices, states);
  }

  gf::MessageStatus Sea::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    gf::unused(id);

    auto hero = static_cast<HeroPosition*>(msg);

    float x = gf::clamp(hero->position.x, WorldMin, WorldMax);
    float y = gf::clamp(hero->position.y, WorldMin, WorldMax);

    hero->position = { x, y };

    unsigned col = static_cast<unsigned>(hero->position.x / TileSize);
    unsigned row = static_cast<unsigned>(hero->position.y / TileSize);

    hero->isOnIsland = (m_terrain({ row, col }) > 0.48f);

    if (m_hero.x != col || m_hero.y != row) {
      m_heroMoved = true;
    }

    m_hero.x = col;
    m_hero.y = row;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Sea::onGoldLooted(gf::Id id, gf::Message *msg) {
    assert(id == GoldLooted::type);
    gf::unused(id);

    auto loot = static_cast<GoldLooted*>(msg);

    gf::Vector2f next;
    unsigned col, row;

    do {
      next.x = gRandom().computeUniformFloat(WorldMin, WorldMax);
      next.y = gRandom().computeUniformFloat(WorldMin, WorldMax);

      col = static_cast<unsigned>(next.x / TileSize);
      row = static_cast<unsigned>(next.y / TileSize);
    } while (m_terrain({ row, col }) < 0.52f);

    loot->next = next;

    return gf::MessageStatus::Keep;
  }

}
