/*
 * Huaca, find the ritual to escape the temple
 * Copyright (C) 2016-2017  Hatunruna team
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

#include <iostream>
#include <numeric>

#include <gf/Collision.h>
#include <gf/Direction.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>

#include "Messages.h"
#include "Singletons.h"

namespace huaca {

  static constexpr int RoomSizeMin = 12;
  static constexpr int RoomSizePad = 5;

  static constexpr int GroundTileCount = 7;
  static constexpr int WallTileCount = 5;

  static constexpr float RuneSize = 32.0f;
  static constexpr float RuneTextureSize = 64.0f;

  static constexpr float KeySize = 15.0f;
  static constexpr float KeyTextureSize = 64.0f;

  static constexpr gf::Vector2f DoorVerticalSize = { 6.0f, 64.0f };
  static constexpr gf::Vector2f DoorVerticalTextureSize = { 35.0f, 256.0f };

  static constexpr gf::Vector2f DoorHorizontalSize = { 64.0f, 32.0f };
  static constexpr gf::Vector2f DoorHorizontalTextureSize = { 256.0f, 128.0f };

  static constexpr float PortalSize = 32.0f;
  static constexpr float PortalTextureSize = 64.0f;

  Roof::Roof()
  : gf::Entity(3)
  , m_vertices(gf::PrimitiveType::Triangles)
  {
  }

  void Roof::addRoof(gf::Vector2i coords) {
    gf::Vertex vertices[4];

    float size = Level::TileSize;

    vertices[0].position = coords * size + gf::Vector2f(0,    -size / 2);
    vertices[1].position = coords * size + gf::Vector2f(size, -size / 2);
    vertices[2].position = coords * size + gf::Vector2f(0,     size / 2);
    vertices[3].position = coords * size + gf::Vector2f(size,  size / 2);

    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = gf::Color::Black;

    // first triangle

    m_vertices.append(vertices[0]);
    m_vertices.append(vertices[1]);
    m_vertices.append(vertices[2]);

    // second triangle

    m_vertices.append(vertices[2]);
    m_vertices.append(vertices[1]);
    m_vertices.append(vertices[3]);
  }

  void Roof::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(m_vertices, states);
  }

  static gf::Texture *loadTexture(const gf::Path& path) {
    gf::Texture& texture = gResourceManager().getTexture(path);
    texture.setSmooth();
    return &texture;
  }

  Level::Level()
  : gf::Entity(1)
  , m_heroCoords(0, 0)
  , m_runeOrder{ 0, 1, 2, 3}
  , m_currentRune(0)
  , m_currentPortal(0)
  , m_isOnPortal(false)
  , m_groundLayer({ MapSize, MapSize })
  , m_wallLayer({ MapSize, MapSize })
  , m_finished(false)
  {
    // setup layers

    // 1806 = 7 * 256 + 6 * 2 + 2 * 1
    //  258 = 1 * 256 + 0 * 0 + 2 * 1
    m_groundLayer.setTileSize({ TileSize, TileSize });
    m_groundLayer.setTilesetTileSize({ TilesetTileSize, TilesetTileSize });
    m_groundLayer.setMargin({ 1, 1 });
    m_groundLayer.setSpacing({ 2, 0 });
    m_groundLayer.setTexture(gResourceManager().getTexture("img/ground.png"));

    // 1290 = 5 * 256 + 4 * 2 + 2 * 1
    //  256 = 1 * 256 + 0 * 0 + 2 * 0
    m_wallLayer.setTileSize({ TileSize, TileSize });
    m_wallLayer.setTilesetTileSize({ TilesetTileSize, TilesetTileSize });
    m_wallLayer.setMargin({ 1, 0 });
    m_wallLayer.setSpacing({ 2, 0 });
    m_wallLayer.setTexture(gResourceManager().getTexture("img/walls.png"));

    // load textures

    m_runes[0].texture = loadTexture("img/rune0_yellow.png");
    m_runes[1].texture = loadTexture("img/rune1_red.png");
    m_runes[2].texture = loadTexture("img/rune2_green.png");
    m_runes[3].texture = loadTexture("img/rune3_purple.png");

    m_keys[0].texture = loadTexture("img/key_iron.png");
    m_keys[1].texture = loadTexture("img/key_bronze.png");
    m_keys[2].texture = loadTexture("img/key_silver.png");
    m_keys[3].texture = loadTexture("img/key_gold.png");

    m_doors[0].horTexture = loadTexture("img/door_iron.png");
    m_doors[0].verTexture = loadTexture("img/door_iron_vertical.png");
    m_doors[1].horTexture = loadTexture("img/door_bronze.png");
    m_doors[1].verTexture = loadTexture("img/door_bronze_vertical.png");
    m_doors[2].horTexture = loadTexture("img/door_silver.png");
    m_doors[2].verTexture = loadTexture("img/door_silver_vertical.png");
    m_doors[3].horTexture = loadTexture("img/door_gold.png");
    m_doors[3].verTexture = loadTexture("img/door_gold_vertical.png");

    m_portals[0].texture = m_portals[1].texture = loadTexture("img/portal.png");

    // set numbers

    for (int number = 0; number < RuneCount; ++number) {
      m_runes[number].number = number;
    }

    for (int number = 0; number < KeyDoorCount; ++number) {
      m_keys[number].number = number;
      m_doors[number].number = number;
    }

    for (int number = 0; number < PortalCount; ++number) {
      m_portals[number].number = number;
    }

    // register events

    gMessageManager().registerHandler<HeroPositionMessage>(&Level::onHeroPosition, this);
    gMessageManager().registerHandler<ResetLevelMessage>(&Level::onResetLevel, this);
    gMessageManager().registerHandler<PortalDroppedMessage>(&Level::onPortalDropped, this);
  }

//   void Level::update(float dt) {
//
//   }

  void Level::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.draw(m_groundLayer, states);
    target.draw(m_wallLayer, states);

    for (const auto& rune : m_runes) {
      gf::Sprite sprite;

      sprite.setTexture(*rune.texture);
      sprite.setPosition(rune.bounds.getCenter());
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setScale(RuneSize / RuneTextureSize);

      if (rune.isPressed) {
        sprite.setColor({ 1.0f, 1.0f, 1.0f, 0.5f });
      }

      target.draw(sprite, states);
    }

    for (const auto& key : m_keys) {
      if (key.isLooted) {
        continue;
      }

      gf::Sprite sprite;
      sprite.setTexture(*key.texture);
      sprite.setPosition(key.bounds.getCenter());
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setScale(KeySize / KeyTextureSize);
      target.draw(sprite, states);
    }

    for (const auto& door : m_doors) {
      if (door.isOpen) {
        continue;
      }

      gf::Sprite sprite;

      if (door.isVertical) {
        gf::Vector2f shift(0.0f, TileSize / 2);
        sprite.setPosition(door.bounds.getCenter() - shift);
        sprite.setTexture(*door.verTexture);
        sprite.setScale(DoorVerticalSize / DoorVerticalTextureSize);
      } else {
        sprite.setPosition(door.bounds.getCenter());
        sprite.setTexture(*door.horTexture);
        sprite.setScale(DoorHorizontalSize / DoorHorizontalTextureSize);
      }

      sprite.setAnchor(gf::Anchor::Center);
      target.draw(sprite, states);
    }

    for (const auto& portal : m_portals) {
      if (!portal.isActive) {
        continue;
      }

      gf::Sprite sprite;
      sprite.setTexture(*portal.texture);
      sprite.setPosition(portal.bounds.getCenter());
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setScale(PortalSize / PortalTextureSize);
      target.draw(sprite, states);
    }
  }

  gf::MessageStatus Level::onHeroPosition(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    auto heroPosition = static_cast<HeroPositionMessage*>(msg);

    for (const gf::RectF& wall : m_walls) {
      gf::Penetration p;

      if (gf::collides(wall, heroPosition->bounds, p)) {
        gf::Vector2f delta = p.depth * p.normal;
        heroPosition->position += delta;
        heroPosition->bounds.min += delta;
        heroPosition->bounds.max += delta;
      }
    }

    for (auto& key : m_keys) {
      if (key.isLooted) {
        continue;
      }

      if (key.bounds.intersects(heroPosition->bounds)) {
        key.isLooted = true;
        KeyLootedMessage otherMessage;
        otherMessage.number = key.number;
        gMessageManager().sendMessage(&otherMessage);

        m_doors[key.number].keyFound = true;
      }
    }

    for (auto& door : m_doors) {
      if (door.isOpen) {
        continue;
      }

      gf::Penetration p;

      if (gf::collides(door.bounds, heroPosition->bounds, p)) {
        if (door.keyFound) {
          door.isOpen = true;
        } else {
          gf::Vector2f delta = p.depth * p.normal;
          heroPosition->position += delta;
          heroPosition->bounds.min += delta;
          heroPosition->bounds.max += delta;
        }
      }
    }

    for (auto& rune : m_runes) {
      if (rune.isPressed) {
        continue;
      }

      if (rune.bounds.intersects(heroPosition->bounds)) {
        if (m_runeOrder[m_currentRune] == rune.number) {
          rune.isPressed = true;
          m_currentRune++;

          RunePressedMessage otherMessage;
          otherMessage.number = rune.number;
          gMessageManager().sendMessage(&otherMessage);

          if (m_currentRune == RuneCount) {
            m_finished = true;
          }
        } else {
          clearSequence();
        }
      }
    }

    if (m_currentPortal == 2) {
      int collisions = 0;

      for (auto& portal : m_portals) {
        if (portal.bounds.intersects(heroPosition->bounds)) {
          collisions++;

          if (!m_isOnPortal) {
            gf::Vector2f newPosition = m_portals[1 - portal.number].bounds.getCenter() - gf::Vector2f(0.0f, TileSize / 3); // HACK
            gf::Vector2f delta = newPosition - heroPosition->position;
            heroPosition->position += delta;
            heroPosition->bounds.min += delta;
            heroPosition->bounds.max += delta;
            m_isOnPortal = true;
          }
        }
      }

      m_isOnPortal = (collisions > 0);
    }


    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Level::onResetLevel(gf::Id id, gf::Message *msg) {
    gf::unused(id);
    gf::unused(msg);

    for (auto& rune : m_runes) {
      rune.isPressed = false;
    }

    m_currentRune = 0;

    for (auto& key : m_keys) {
      key.isLooted = false;
    }

    for (auto& door : m_doors) {
      door.isOpen = false;
      door.keyFound = false;
    }

    for (auto& portal : m_portals) {
      portal.isActive = false;
    }

    m_currentPortal = 0;

    return gf::MessageStatus::Keep;
  }

  void Level::clearSequence() {
    for (auto& rune : m_runes) {
      rune.isPressed = false;
    }

    m_currentRune = 0;

    SequenceFailedMessage msg;
    gMessageManager().sendMessage(&msg);
  }


  gf::MessageStatus Level::onPortalDropped(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    if (m_currentPortal == 2) {
      return gf::MessageStatus::Keep;
    }

    auto portalDropped = static_cast<PortalDroppedMessage*>(msg);

    m_portals[m_currentPortal].isActive = true;
    m_portals[m_currentPortal].bounds = gf::RectF::fromPositionSize(portalDropped->position - PortalSize / 2, { PortalSize, PortalSize });
    m_currentPortal++;
    m_isOnPortal = true;

    return gf::MessageStatus::Keep;
  }


  static constexpr int FirstWidth = 29;
  static constexpr int FirstHeight = 15;

  static const char *FirstLevel[FirstHeight] = {
    "#############################",
    "#       ###       ###       #",
    "#       ###       ###       #",
    "#                           #",
    "#       ###       ###       #",
    "#       ###       ###       #",
    "#### ################### ####",
    "#### ################### ####",
    "#### ################### ####",
    "#       ###       ###       #",
    "#       ###       ###       #",
    "#                           #",
    "#       ###       ###       #",
    "#       ###       ###       #",
    "#############################",
  };

  static constexpr int Shift = 10;

  void Level::generateFirst(gf::Random& random) {
    // initialize with walls

    for (int x = 0; x < MapSize; ++x) {
      for (int y = 0; y < MapSize; ++y) {
        m_world[x][y].tile = Tile::Wall;
      }
    }

    for (int y = 0; y < FirstHeight; ++y) {
      for (int x = 0; x < FirstWidth; ++x) {
        if (FirstLevel[y][x] == ' ') {
          m_world[x + Shift][y + Shift].tile = Tile::Ground;
        }
      }
    }

    // set items

    m_keys[0].coords = { Shift +  7, Shift +  3 };
    m_keys[1].coords = { Shift + 21, Shift + 11 };
    m_keys[2].coords = { Shift + 27, Shift +  1 };
    m_keys[3].coords = { Shift +  1, Shift + 13 };

    m_doors[0].coords = { Shift +  9, Shift +  3 }; m_doors[0].isVertical = true;
    m_doors[1].coords = { Shift + 19, Shift + 11 }; m_doors[1].isVertical = true;
    m_doors[2].coords = { Shift + 24, Shift +  7 }; m_doors[2].isVertical = false;
    m_doors[3].coords = { Shift +  4, Shift +  7 }; m_doors[3].isVertical = false;

    m_runes[0].coords = { Shift +  6, Shift +  3 };
    m_runes[1].coords = { Shift + 26, Shift + 11 };
    m_runes[2].coords = { Shift + 24, Shift + 11 };
    m_runes[3].coords = { Shift +  4, Shift +  6 };

    std::iota(m_runeOrder, m_runeOrder + RuneCount, 0);

    generateWorld(random);

    // set hero

    m_heroCoords = { Shift + 3, Shift + 3 };

    // warn about the end of the generation

    NewLevelMessage msg;
    msg.heroPosition = m_heroCoords * TileSize + TileSize / 2;
    gMessageManager().sendMessage(&msg);
  }

  void Level::generateNew(gf::Random& random) {
    // initialize with walls

    for (int x = 0; x < MapSize; ++x) {
      for (int y = 0; y < MapSize; ++y) {
        m_world[x][y].tile = Tile::Wall;
      }
    }

    // generate the level

    generateRooms(random, 0, MapSize, 0, MapSize);

    for (int y = 0; y < MapSize; ++y) {
      for (int x = 0; x < MapSize; ++x) {
        if (m_world[x][y].tile == Tile::Wall) {
          std::cout << '#';
        } else {
          std::cout << ' ';
        }
      }

      std::cout << '\n';
    }

    generateItems(random);
    generateWorld(random);

    // warn about the end of the generation

    NewLevelMessage msg;
    msg.heroPosition = m_heroCoords * TileSize + TileSize / 2;
    gMessageManager().sendMessage(&msg);
  }

  Roof Level::getRoof() const {
    Roof roof;

    for (int x = 0; x < MapSize; ++x) {
      for (int y = 0; y < MapSize; ++y) {
        if (m_world[x][y].tile == Tile::Wall) {
          roof.addRoof({ x, y });
        }
      }
    }

    return roof;
  }

  void Level::generateRooms(gf::Random& random, int xMin, int xMax, int yMin, int yMax) {
    int dx = xMax - xMin;
    int dy = yMax - yMin;

    if (dx < RoomSizeMin && dy < RoomSizeMin) {
      // create a room

      assert(dx >= RoomSizePad);
      assert(dy >= RoomSizePad);

      int h = random.computeUniformInteger(3, dx - 2);
      int w = random.computeUniformInteger(3, dy - 2);

      int xStart = random.computeUniformInteger(xMin + 1, xMax - h - 1);
      int yStart = random.computeUniformInteger(yMin + 1, yMax - w - 1);

      for (int x = xStart; x < xStart + h; ++x) {
        for (int y = yStart; y < yStart + w; ++y) {
          m_world[x][y].tile = Tile::Ground;
        }
      }

      return;
    }

    // recursivity

    enum { Horizontal, Vertical } split = Horizontal;

    if (dx < RoomSizeMin) {
      split = Vertical;
    } else if (dy < RoomSizeMin) {
      split = Horizontal;
    } else {
      split = random.computeBernoulli(0.5f) ? Horizontal : Vertical;
    }

    if (split == Vertical) {
      int yMid = random.computeUniformInteger(yMin + RoomSizePad, yMax - RoomSizePad);
      generateRooms(random, xMin, xMax, yMin, yMid);
      generateRooms(random, xMin, xMax, yMid, yMax);

      // generate corridor

      int x = xMin + dx / 2;
      int y = yMid;

      while (y < yMax - 1 && m_world[x][y].tile != Tile::Ground) {
        m_world[x][y].tile = Tile::Ground;
        y++;
      }

      y = yMid - 1;

      while (y > yMin && m_world[x][y].tile != Tile::Ground) {
        m_world[x][y].tile = Tile::Ground;
        y--;
      }

    } else {
      assert(split == Horizontal);

      int xMid = random.computeUniformInteger(xMin + RoomSizePad, xMax - RoomSizePad);
      generateRooms(random, xMin, xMid, yMin, yMax);
      generateRooms(random, xMid, xMax, yMin, yMax);

      // generate corridor

      int y = yMin + dy / 2;
      int x = xMid;

      while (x < xMax - 1 && m_world[x][y].tile != Tile::Ground) {
        m_world[x][y].tile = Tile::Ground;
        x++;
      }

      x = xMid - 1;

      while (x > xMin && m_world[x][y].tile != Tile::Ground) {
        m_world[x][y].tile = Tile::Ground;
        x--;
      }
    }
  }

  void Level::generateItems(gf::Random& random) {
    int x, y;

    // runes

    for (auto& rune : m_runes) {
      do {
        x = random.computeUniformInteger(0, MapSize - 1);
        y = random.computeUniformInteger(0, MapSize - 1);
      } while (m_world[x][y].tile != Tile::Ground);

      rune.coords = { x, y };
      m_world[x][y].tile = Tile::GroundWithItem;
    }


    std::iota(m_runeOrder, m_runeOrder + RuneCount, 0);
    std::shuffle(m_runeOrder, m_runeOrder + RuneCount, random.getEngine());

    // hero

    do {
      x = random.computeUniformInteger(0, MapSize - 1);
      y = random.computeUniformInteger(0, MapSize - 1);
    } while (m_world[x][y].tile != Tile::Ground);

    m_heroCoords = { x, y };
    m_world[x][y].tile = Tile::GroundWithItem;

    // make a DFS in the world

    bool visited[MapSize][MapSize];

    for (x = 0; x < MapSize; ++x) {
      for (y = 0; y < MapSize; ++y) {
        visited[x][y] = false;
      }
    }

    visited[m_heroCoords.x][m_heroCoords.y] = true;

    std::vector<gf::Vector2i> stack;
    stack.push_back(m_heroCoords);

    std::vector<gf::Vector2i> path;

    gf::Direction directions[4] = { gf::Direction::Up, gf::Direction::Left, gf::Direction::Down, gf::Direction::Right };

    do {
      gf::Vector2i curr = stack.back();
      assert(visited[curr.x][curr.y]);
      stack.pop_back();
      path.push_back(curr);

      for (int k = 0; k < 10; ++k) {
        int i = random.computeUniformInteger(0, 3);
        int j = random.computeUniformInteger(0, 3);
        std::swap(directions[i], directions[j]);
      }

      for (auto direction : directions) {
        gf::Vector2i next = curr + gf::displacement(direction);

        if (m_world[next.x][next.y].tile != Tile::Wall && !visited[next.x][next.y]) {
          visited[next.x][next.y] = true;
          stack.push_back(next);
        }
      }
    } while (!stack.empty());

    // now defines keys and doors

    std::size_t keyIndexMin = path.size() / 4;
    std::size_t keyIndexMax = keyIndexMin * 3;

    std::size_t index;

    for (int i = 0; i < KeyDoorCount; ++i) {
      do {
        index = random.computeUniformInteger(keyIndexMin, keyIndexMax);
      } while (m_world[path[index].x][path[index].y].tile != Tile::Ground);

      m_keys[i].coords = path[index];
      m_world[path[index].x][path[index].y].tile = Tile::GroundWithItem;

      std::size_t doorRange = path.size() - index;
      std::size_t doorIndexMin = index + doorRange / 3;
      std::size_t doorIndexMax = index + 2 * doorRange / 3;

      do {
        do {
          index = random.computeUniformInteger(doorIndexMin, doorIndexMax);
        } while (m_world[path[index].x][path[index].y].tile != Tile::Ground);

        auto isCorridor = [this](gf::Vector2i coords) {
          assert(0 < coords.x && coords.x < MapSize - 1);
          assert(0 < coords.y && coords.y < MapSize - 1);
          Cell& n = m_world[coords.x][coords.y - 1];
          Cell& s = m_world[coords.x][coords.y + 1];
          Cell& w = m_world[coords.x - 1][coords.y];
          Cell& e = m_world[coords.x + 1][coords.y];

          return (n.tile == Tile::Wall && s.tile == Tile::Wall)
              || (w.tile == Tile::Wall && e.tile == Tile::Wall);
        };

        while (index < path.size() && (!isCorridor(path[index]) || m_world[path[index].x][path[index].y].tile != Tile::Ground)) {
          index++;
        }
      } while (index == path.size());

      m_doors[i].coords = path[index];
      m_world[path[index].x][path[index].y].tile = Tile::GroundWithItem;
    }
  }


  void Level::generateWorld(gf::Random& random) {
    // ground and walls

    m_groundLayer.clear();
    m_wallLayer.clear();
    m_walls.clear();

    for (int x = 0; x < MapSize; ++x) {
      for (int y = 0; y < MapSize; ++y) {
        gf::Vector2i coords(x, y);

        switch (m_world[x][y].tile) {
          case Tile::Wall:
            m_wallLayer.setTile(coords, random.computeUniformInteger(0, WallTileCount - 1));
            m_walls.push_back(gf::RectF::fromPositionSize(coords * TileSize, gf::Vector2f(TileSize, TileSize)));
            break;
          default:
            m_groundLayer.setTile(coords, gf::clamp(random.computeUniformInteger(-10, GroundTileCount - 1), 0, GroundTileCount - 1));
            break;
        }
      }
    }

    // items

    for (auto& rune : m_runes) {
      gf::Vector2f pos = rune.coords * TileSize + TileSize / 2 - RuneSize / 2;
      rune.bounds = gf::RectF::fromPositionSize(pos, { RuneSize, RuneSize });
      rune.isPressed = false;
    }

    m_currentRune = 0;

    for (auto& key : m_keys) {
      gf::Vector2f pos = key.coords * TileSize + TileSize / 2 - KeySize / 2;
      key.bounds = gf::RectF::fromPositionSize(pos, { KeySize, KeySize });
      key.isLooted = false;
    }

    for (auto& door : m_doors) {
      door.isVertical = m_world[door.coords.x][door.coords.y + 1].tile == Tile::Wall;

      gf::Vector2f pos = door.coords * TileSize;

      if (door.isVertical) {
        door.bounds = gf::RectF::fromPositionSize(pos, DoorVerticalSize);
      } else {
        door.bounds = gf::RectF::fromPositionSize(pos, DoorHorizontalSize);
      }

      door.keyFound = false;
      door.isOpen = false;
    }

    m_currentPortal = 0;

    for (auto& portal : m_portals) {
      portal.isActive = false;
    }

    m_finished = false;
  }

}

