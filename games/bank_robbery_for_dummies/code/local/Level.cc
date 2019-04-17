/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 * Copyright (C) 2017 Julien Bernard
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
#include "Level.h"

#include <cassert>
#include <tuple>

#include <iostream>

#include <gf/Math.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/SpriteBatch.h>
#include <gf/VectorOps.h>

namespace brfd {
  static constexpr int TileSize = 256;

  // see tileset.png
  enum class Tile : int {
    Building1_NW, Building1_NE, Building2_NW, Building2_NE, Building3_NW, Building3_NE, Bank_NW, Bank_NE,
    Building1_SW, Building1_SE, Building2_SW, Building2_SE, Building3_SW, Building3_SE, Bank_SW, Bank_SE,
    GunStore_NW, GunStore_NE, Home1_NW, Home1_NE, Home2_NW, Home2_NE, Grass1, Grass2,
    GunStore_SW, GunStore_SE, Home1_SW, Home1_SE, Home2_SW, Home2_SE, Grass3, Grass4,
    Occupied_CarsV, Occupied_CarsH, Occupied_TruckH, Occupied_TruckV, Occupied_HoleH, Occupied_HoleV, Occupied_Garden1, Occupied_Garden2,
    Road_NW, Road_NE, Road_SE, Road_SW, Road_Crossroad, Road_H, Road_V,
  };

  enum BlockType {
    Street,
    Occupied,
    Building,
    Grass,
  };

  struct Block {
    BlockType type;
    Tile tile;
  };

  /*
   Example of map:

   ############
   #          #
   # $$ BB HH #
   # $$ BB HH #
   #          #
   # BB BB SS #
   # BB BB SS #
   #          #
   # BB SS BB #
   # BB SS BB #
   #          #
   ############
   */

  struct Map {
    Block data[Level::Size][Level::Size];
  };

  static bool isPositionKnown(gf::Vector2i newPosition, const std::vector<gf::Vector2i>& positions) {
    for (auto position : positions) {
      if (newPosition == position) {
        return true;
      }
    }

    return false;
  }

  struct Clear {
    gf::Vector2i dir;
    float angle;
  };

  static std::tuple<gf::Vector2f, float> getNewPosition(gf::Random& random, Map& map, std::vector<gf::Vector2i>& positions, Tile tile) {
    gf::Vector2i pos;

    do {
      pos.x = random.computeUniformInteger(0, Level::StreetCount - 2);
      pos.y = random.computeUniformInteger(0, Level::StreetCount - 2);
    } while (isPositionKnown(pos, positions));

    positions.push_back(pos);

    pos = 2 + 3 * pos;
    map.data[pos.x][pos.y].tile = tile;

    // check if the street is clear

    static const Clear ClearRoads[4] = {
      { { -1,  0 }, gf::Pi2 },
      { {  1, -1 }, gf::Pi },
      { {  0,  2 }, 0.0f },
      { {  2,  1 }, 3 * gf::Pi2 }
    };

    for (auto clear : ClearRoads) {
      gf::Vector2i otherPos = pos + clear.dir;

      if (map.data[otherPos.x][otherPos.y].type == BlockType::Street) {
        gf::Vector2f goal = (otherPos + gf::Vector2f(0.5f, 0.5f)) * TileSize;
        return std::make_tuple(goal, clear.angle);
      }
    }

    assert(false);
    return std::make_tuple(gf::Vector2f(0, 0), 0.0f);
  }

  static bool isRealStreet(gf::Vector2i pos, const Map& map) {
    return map.data[pos.x][pos.y].tile == Tile::Road_H || map.data[pos.x][pos.y].tile == Tile::Road_V;
  }

  Level::Level(gf::ResourceManager& resources)
  : m_layer({ Size, Size })
  , m_carTexture(resources.getTexture("cars.png"))
  , m_carGeometry(getCarGeometry())
  , m_buildingGeometry(gf::Vector2f(2 * TileSize, 2 * TileSize))
  , m_occupiedRoadGeometry(gf::Vector2f(TileSize, TileSize))
  {
    m_layer.setTileSize({ TileSize, TileSize });
    m_layer.setTexture(resources.getTexture("tileset.png"));
  }

  static constexpr float OccupiedRatio = 0.25f;
  static constexpr float SpecialOccupiedRatio = 0.2f;

  void Level::generateLevel(gf::Random& random, gf::PhysicsModel& physics) {
    /*
     * Step 1: the map
     */

    Map map;

    for (int i = 0; i < Size; ++i) {
      for (int j = 0; j < Size; ++j) {
        auto& block = map.data[i][j];

        if (i == 0 || j == 0 || i == Size - 1 || j == Size - 1) {
          block.type = BlockType::Grass;

          int tile = random.computeUniformInteger(1, 4);

          switch (tile) {
            case 1: block.tile = Tile::Grass1; break;
            case 2: block.tile = Tile::Grass2; break;
            case 3: block.tile = Tile::Grass3; break;
            case 4: block.tile = Tile::Grass4; break;
            default: assert(false); break;
          }
        } else if (i % 3 == 1 || j % 3 == 1) {
          block.type = BlockType::Street;
          block.tile = Tile::Road_Crossroad;
        } else {
          block.type = BlockType::Building;

          int tile = random.computeUniformInteger(1, 3);

          switch (tile) {
            case 1: block.tile = Tile::Building1_NW; break;
            case 2: block.tile = Tile::Building2_NW; break;
            case 3: block.tile = Tile::Building3_NW; break;
            default: assert(false); break;
          }
        }
      }
    }

    // set the occupied tiles

    for (int i = 1; i < StreetCount - 1; ++i) {
      for (int j = 0; j < StreetCount - 1; ++j) {
        if (random.computeBernoulli(OccupiedRatio)) {
          int x = 3 * i + 1;
          int y1 = 3 * j + 2;
          int y2 = 3 * j + 3;

          map.data[x][y1].type = BlockType::Occupied;
          map.data[x][y2].type = BlockType::Occupied;

          if (random.computeBernoulli(SpecialOccupiedRatio)) {
            int occupied = random.computeUniformInteger(1, 3);
            bool coin = random.computeBernoulli(0.5);

            switch (occupied) {
              case 1:
                map.data[x][y1].tile = Tile::Occupied_CarsV;
                map.data[x][y2].tile = coin ? Tile::Occupied_HoleV : Tile::Occupied_TruckV;
                break;
              case 2:
                map.data[x][y1].tile = Tile::Occupied_HoleV;
                map.data[x][y2].tile = coin ? Tile::Occupied_TruckV : Tile::Occupied_CarsV;
                break;
              case 3:
                map.data[x][y1].tile = Tile::Occupied_TruckV;
                map.data[x][y2].tile = coin ? Tile::Occupied_CarsV : Tile::Occupied_HoleV;
                break;
              default:
                assert(false);
                break;
            }
          } else {
            map.data[x][y1].tile = Tile::Occupied_Garden1;
            map.data[x][y2].tile = Tile::Occupied_Garden2;
          }
        }
      }
    }

    for (int i = 1; i < StreetCount - 1; ++i) {
      for (int j = 0; j < StreetCount - 1; ++j) {
        if (random.computeBernoulli(OccupiedRatio)) {
          int y = 3 * i + 1;
          int x1 = 3 * j + 2;
          int x2 = 3 * j + 3;

          map.data[x1][y].type = BlockType::Occupied;
          map.data[x2][y].type = BlockType::Occupied;

          if (random.computeBernoulli(SpecialOccupiedRatio)) {
            int occupied = random.computeUniformInteger(1, 3);
            bool coin = random.computeBernoulli(0.5);

            switch (occupied) {
              case 1:
                map.data[x1][y].tile = Tile::Occupied_TruckH;
                map.data[x2][y].tile = coin ? Tile::Occupied_CarsH : Tile::Occupied_HoleH;
                break;
              case 2:
                map.data[x1][y].tile = Tile::Occupied_CarsH;
                map.data[x2][y].tile = coin ? Tile::Occupied_HoleH : Tile::Occupied_TruckH;
                break;
              case 3:
                map.data[x1][y].tile = Tile::Occupied_HoleH;
                map.data[x2][y].tile = coin ? Tile::Occupied_TruckH : Tile::Occupied_CarsH;
                break;
              default:
                assert(false);
                break;
            }
          } else {
            map.data[x1][y].tile = Tile::Occupied_Garden1;
            map.data[x2][y].tile = Tile::Occupied_Garden2;
          }
        }
      }
    }

    // set the road tiles

    for (int i = 1; i < Size - 1; ++i) {
      for (int j = 1; j < Size - 1; j += 3) {
        auto& block = map.data[i][j];

        if (block.type == BlockType::Street) {
          if (i % 3 != 1) {
            block.tile = Tile::Road_H;
          }
        }
      }
    }

    for (int i = 1; i < Size - 1; i += 3) {
      for (int j = 1; j < Size - 1; ++j) {
        auto& block = map.data[i][j];

        if (block.type == BlockType::Street) {
          if (j % 3 != 1) {
            block.tile = Tile::Road_V;
          }
        }
      }
    }

    map.data[1][1].tile = Tile::Road_NW;
    map.data[1][Size - 2].tile = Tile::Road_SW;
    map.data[Size - 2][Size - 2].tile = Tile::Road_SE;
    map.data[Size - 2][1].tile = Tile::Road_NE;

    // compute special buildings

    std::vector<gf::Vector2i> positions;

    std::tie(m_partner, std::ignore) = getNewPosition(random, map, positions, Tile::Home2_NW);
    std::tie(m_clothingStore, std::ignore) = getNewPosition(random, map, positions, Tile::Building1_NW);
    std::tie(m_gunStore, std::ignore) = getNewPosition(random, map, positions, Tile::GunStore_NW);
    std::tie(m_rocketStore, std::ignore) = getNewPosition(random, map, positions, Tile::GunStore_NW);;
    std::tie(m_bank, std::ignore) = getNewPosition(random, map, positions, Tile::Bank_NW);
    std::tie(m_home, m_homeStartingAngle) = getNewPosition(random, map, positions, Tile::Home1_NW);

    // set special buildings tiles

    for (int i = 2; i < Size - 2; i += 3) {
      for (int j = 2; j < Size - 2; j += 3) {
        const auto& block = map.data[i][j];

        assert(block.type == BlockType::Building);

        switch (block.tile) {
          case Tile::Building1_NW:
            map.data[i + 1][j    ].tile = Tile::Building1_NE;
            map.data[i    ][j + 1].tile = Tile::Building1_SW;
            map.data[i + 1][j + 1].tile = Tile::Building1_SE;
            break;
          case Tile::Building2_NW:
            map.data[i + 1][j    ].tile = Tile::Building2_NE;
            map.data[i    ][j + 1].tile = Tile::Building2_SW;
            map.data[i + 1][j + 1].tile = Tile::Building2_SE;
            break;
          case Tile::Building3_NW:
            map.data[i + 1][j    ].tile = Tile::Building3_NE;
            map.data[i    ][j + 1].tile = Tile::Building3_SW;
            map.data[i + 1][j + 1].tile = Tile::Building3_SE;
            break;
          case Tile::Bank_NW:
            map.data[i + 1][j    ].tile = Tile::Bank_NE;
            map.data[i    ][j + 1].tile = Tile::Bank_SW;
            map.data[i + 1][j + 1].tile = Tile::Bank_SE;
            break;
          case Tile::GunStore_NW:
            map.data[i + 1][j    ].tile = Tile::GunStore_NE;
            map.data[i    ][j + 1].tile = Tile::GunStore_SW;
            map.data[i + 1][j + 1].tile = Tile::GunStore_SE;
            break;
          case Tile::Home1_NW:
            map.data[i + 1][j    ].tile = Tile::Home1_NE;
            map.data[i    ][j + 1].tile = Tile::Home1_SW;
            map.data[i + 1][j + 1].tile = Tile::Home1_SE;
            break;
          case Tile::Home2_NW:
            map.data[i + 1][j    ].tile = Tile::Home2_NE;
            map.data[i    ][j + 1].tile = Tile::Home2_SW;
            map.data[i + 1][j + 1].tile = Tile::Home2_SE;
            break;
          default:
            assert(false);
            break;
        }
      }
    }

    // set the tiles of the layer

    for (int i = 0; i < Size; ++i) {
      for (int j = 0; j < Size; ++j) {
        auto& block = map.data[i][j];
        m_layer.setTile({ i, j }, static_cast<int>(block.tile));
      }
    }

    /*
     * Step 2: the cars
     */

    static constexpr int CarCount = 150;

    positions.clear();

    for (int k = 0; k < CarCount; ++k) {
      gf::Vector2i pos;

      do {
        pos.x = random.computeUniformInteger(1, Size - 1);
        pos.y = random.computeUniformInteger(1, Size - 1);
      } while (!isRealStreet(pos, map) || isPositionKnown(pos, positions));

      positions.push_back(pos);

      gf::Vector2f carPosition;
      float carAngle;

      if (pos.x % 3 == 1) {
        if (random.computeBernoulli(0.5)) {
          carPosition.x = (pos.x + 0.875f) * TileSize;
          carPosition.y = (pos.y + 0.5f) * TileSize;
          carAngle = 1.5f * gf::Pi;
        } else {
          carPosition.x = (pos.x + 0.125f) * TileSize;
          carPosition.y = (pos.y + 0.5f) * TileSize;
          carAngle = 0.5f * gf::Pi;
        }
      } else {
        assert(pos.y % 3 == 1);

        if (random.computeBernoulli(0.5)) {
          carPosition.x = (pos.x + 0.5f) * TileSize;
          carPosition.y = (pos.y + 0.875f) * TileSize;
          carAngle = 0.0f;
        } else {
          carPosition.x = (pos.x + 0.5f) * TileSize;
          carPosition.y = (pos.y + 0.125f) * TileSize;
          carAngle = gf::Pi;
        }
      }

      int number = random.computeUniformInteger(0, 9);

      m_cars.push_back(StaticCar(number, carPosition, carAngle, m_carGeometry));
    }

    /*
     * Step 3: physics!
     */

    for (auto& car : m_cars) {
      physics.addBody(car.getBody());
    }

    for (int i = 2; i < Size - 2; i += 3) {
      for (int j = 2; j < Size - 2; j += 3) {
        gf::Vector2f position(static_cast<float>(i), static_cast<float>(j));

        gf::PhysicsBody body(m_buildingGeometry, gf::PhysicsBody::Static);
        body.setPosition(position * TileSize + TileSize);
        m_buildings.push_back(body);
      }
    }

    for (auto& building : m_buildings) {
      physics.addBody(building);
    }

    for (int i = 0; i < Size; ++i) {
      for (int j = 0; j < Size; ++j) {
        if (map.data[i][j].type == BlockType::Grass || map.data[i][j].type == BlockType::Occupied) {
          gf::Vector2f position(static_cast<float>(i), static_cast<float>(j));

          gf::PhysicsBody body(m_occupiedRoadGeometry, gf::PhysicsBody::Static);
          body.setPosition(position * TileSize + TileSize / 2);
          m_occupiedRoads.push_back(body);
        }
      }
    }

    for (auto& occupiedRoad : m_occupiedRoads) {
      physics.addBody(occupiedRoad);
    }
  }

  void Level::update(gf::Time time) {
    for (auto& car : m_cars) {
      car.update(time);
    }
  }

  void Level::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(m_layer);

    gf::SpriteBatch batch(target);
    batch.begin();

    for (const auto& car : m_cars) {
      gf::Sprite sprite(m_carTexture, car.getTextureRect());
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setPosition(car.getBody().getPosition());
      sprite.setRotation(car.getBody().getAngle());
      batch.draw(sprite, states);
    }

    batch.end();

#if 0
    // debug

    for (const auto& car : m_cars) {
      car.getBody().render(target);
    }

    for (const auto& building : m_buildings) {
      building.render(target);
    }

    for (auto& occupiedRoad : m_occupiedRoads) {
      occupiedRoad.render(target);
    }
#endif
  }


}
