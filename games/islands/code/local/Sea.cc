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
  : m_vertices(gf::PrimitiveType::Triangles)
  , m_sea({ Size, Size })
  , m_hero({ 0, 0 })
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

    for (auto row : m_sea.getRowRange()) {
      double y = static_cast<double>(row) / m_sea.getRows() * Scale;
      for (auto col : m_sea.getColRange()) {
        double x = static_cast<double>(col) / m_sea.getCols() * Scale;
        m_sea({ col, row }).elevation = fractal.getValue(x, y);
      }
    }

    // normalize

    auto comparator = [](const Point& lhs, const Point& rhs) {
      return lhs.elevation < rhs.elevation;
    };

    Point minPoint = *std::min_element(m_sea.begin(), m_sea.end(), comparator);
    float min = minPoint.elevation;

    Point maxPoint = *std::max_element(m_sea.begin(), m_sea.end(), comparator);
    float max = maxPoint.elevation;


    for (auto& val : m_sea) {
      val.elevation = (val.elevation - min) / (max - min);
      val.elevation = valueWithWaterLevel(val.elevation, SeaLevel);
      assert(0.0 <= val.elevation && val.elevation <= 1.0);
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
      } while (m_sea({ row, col }).elevation < 0.52f);

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
      } while (m_sea({ row, col }).elevation < 0.52f);

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
      } while (m_sea({ row, col }).elevation < 0.52f);

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
      } while (m_sea({ row, col }).elevation < 0.52f);

      turrets.addTurret(position);
    }

    // compute colors

    gf::ColorRamp ramp;
    ramp.addColorStop(0.000f, gf::Color::fromRgba32(  2,  43,  68)); // very dark blue: deep water
    ramp.addColorStop(0.250f, gf::Color::fromRgba32(  9,  62,  92)); // dark blue: water
    ramp.addColorStop(0.499f, gf::Color::fromRgba32( 17,  82, 112)); // blue: shallow water
    ramp.addColorStop(0.500f, gf::Color::fromRgba32( 69, 108, 118)); // light blue: shore
    ramp.addColorStop(0.501f, gf::Color::fromRgba32(255, 251, 121)); // sand
    ramp.addColorStop(0.550f, gf::Color::fromRgba32(255, 251, 121)); // sand
    ramp.addColorStop(0.551f, gf::Color::fromRgba32( 54, 205,  20)); // grass
    ramp.addColorStop(0.700f, gf::Color::fromRgba32( 54, 205,  20)); // grass
    ramp.addColorStop(0.701f, gf::Color::fromRgba32( 38, 143,  14)); // grass

    ramp.addColorStop(1.000f, gf::Color::fromRgba32( 38, 143,  14)); // grass

    static constexpr gf::Vector3f Light = { -1, -1, 0 };

    for (auto row : m_sea.getRowRange()) {
      for (auto col : m_sea.getColRange()) {
        float elevation = m_sea({ row, col }).elevation;
        assert(0.0f <= elevation && elevation <= 1.0);

        if (elevation < 0.5f) {
          m_sea({ row, col }).color = ramp.computeColor(elevation);
          continue;
        }

        float x = col;
        float y = row;

        // compute the normal vector

        gf::Vector3f normal(0, 0, 0);
        unsigned count = 0;

        gf::Vector3f p{x, y, elevation};

        if (col > 0 && row > 0) {
          gf::Vector3f pn{x    , y - 1, m_sea({ row - 1, col     }).elevation};
          gf::Vector3f pw{x - 1, y    , m_sea({ row    , col - 1 }).elevation};

          gf::Vector3f v3 = cross(p - pw, p - pn);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (col > 0 && row < m_sea.getRows() - 1) {
          gf::Vector3f pw{x - 1, y    , m_sea({ row    , col - 1 }).elevation};
          gf::Vector3f ps{x    , y + 1, m_sea({ row + 1, col     }).elevation};

          gf::Vector3f v3 = cross(p - ps, p - pw);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (col < m_sea.getCols() - 1 && row > 0) {
          gf::Vector3f pe{x + 1, y    , m_sea({ row    , col + 1 }).elevation};
          gf::Vector3f pn{x    , y - 1, m_sea({ row - 1, col     }).elevation};

          gf::Vector3f v3 = cross(p - pn, p - pe);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        if (col < m_sea.getCols() - 1 && row < m_sea.getRows() - 1) {
          gf::Vector3f pe{x + 1, y    , m_sea({ row    , col + 1 }).elevation};
          gf::Vector3f ps{x    , y + 1, m_sea({ row + 1, col     }).elevation};

          gf::Vector3f v3 = cross(p - pe, p - ps);
          assert(v3.z > 0);

          normal += v3;
          count += 1;
        }

        normal = gf::normalize(normal / count);
        float d = gf::dot(Light, normal);
        d = gf::clamp(0.5f + 35 * d, 0.0f, 1.0f);

        gf::Color4f color = ramp.computeColor(elevation);

        gf::Color4f lo = gf::lerp(color, gf::Color::fromRgba32(0x33, 0x11, 0x33, 0xFF), 0.7);
        gf::Color4f hi = gf::lerp(color, gf::Color::fromRgba32(0xFF, 0xFF, 0xCC, 0xFF), 0.3);

        if (d < 0.5f) {
          m_sea({ row, col }).color = gf::lerp(lo, color, 2 * d);
        } else {
          m_sea({ row, col }).color = gf::lerp(color, hi, 2 * d - 1);
        }
      }
    }
  }

  void Sea::update(gf::Time time) {
    gf::unused(time);

    unsigned rowMin = (m_hero.y > DisplayHalfRange) ? (m_hero.y - DisplayHalfRange) : 0;
    unsigned rowMax = (m_hero.y + DisplayHalfRange < Size) ? (m_hero.y + DisplayHalfRange) : Size - 1;
    unsigned colMin = (m_hero.x > DisplayHalfRange) ? (m_hero.x - DisplayHalfRange) : 0;
    unsigned colMax = (m_hero.x + DisplayHalfRange < Size) ? (m_hero.x + DisplayHalfRange) : Size - 1;

    m_vertices.clear();

    for (unsigned row = rowMin; row < rowMax; ++row) {
      assert(row < Size - 1);

      for (unsigned col = colMin; col < colMax; ++col) {
        assert(col < Size - 1);

        gf::Vertex vertices[4];

        vertices[0].position = {  col      * TileSize,  row      * TileSize };
        vertices[1].position = { (col + 1) * TileSize,  row      * TileSize };
        vertices[2].position = {  col      * TileSize, (row + 1) * TileSize };
        vertices[3].position = { (col + 1) * TileSize, (row + 1) * TileSize };

        vertices[0].color = m_sea({ row,     col     }).color;
        vertices[1].color = m_sea({ row,     col + 1 }).color;
        vertices[2].color = m_sea({ row + 1, col     }).color;
        vertices[3].color = m_sea({ row + 1, col + 1 }).color;

        // first triangle
        m_vertices.append(vertices[0]);
        m_vertices.append(vertices[1]);
        m_vertices.append(vertices[2]);

        // second triangle
        m_vertices.append(vertices[2]);
        m_vertices.append(vertices[1]);
        m_vertices.append(vertices[3]);

      }
    }
  }

  void Sea::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(m_vertices, states);
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

    hero->isOnIsland = (m_sea({ row, col }).elevation > 0.48f);

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
    } while (m_sea({ row, col }).elevation < 0.52f);

    loot->next = next;

    return gf::MessageStatus::Keep;
  }

}
