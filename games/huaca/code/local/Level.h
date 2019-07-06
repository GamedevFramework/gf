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
#ifndef HUACA_LEVEL_H
#define HUACA_LEVEL_H

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Random.h>
#include <gf/Rect.h>
#include <gf/Texture.h>
#include <gf/TileLayer.h>
#include <gf/VertexArray.h>

namespace huaca {

  class Roof : public gf::Entity {
  public:
    Roof();
    void addRoof(gf::Vector2i coords);
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
  private:
    gf::VertexArray m_vertices;
  };


  struct Item {
    gf::Vector2i coords;
    gf::RectF bounds;
    int number;
  };

  struct Rune : public Item {
    gf::Texture *texture;
    bool isPressed;
  };

  struct Key : public Item {
    gf::Texture *texture;
    bool isLooted;
  };

  struct Door : public Item {
    gf::Texture *horTexture;
    gf::Texture *verTexture;
    bool keyFound;
    bool isOpen;
    bool isVertical;
  };

  struct Portal : public Item {
    gf::Texture *texture;
    bool isActive = false;
  };

  class Level : public gf::Entity {
  public:
    static constexpr int TilesetTileSize = 256;
    static constexpr int TileSize = 64;

    static constexpr int RuneCount = 4;
    static constexpr int KeyDoorCount = 4;
    static constexpr int PortalCount = 2;

    Level();

//     virtual void update(float dt) override;

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    void generateFirst(gf::Random& random);
    void generateNew(gf::Random& random);

    Roof getRoof() const;

    bool isFinished() const {
      return m_finished;
    }

  private:
    void generateRooms(gf::Random& random, int xMin, int xMax, int yMin, int yMax);
    void generateItems(gf::Random& random);
    void generateWorld(gf::Random& random);

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onResetLevel(gf::Id id, gf::Message *msg);
    gf::MessageStatus onPortalDropped(gf::Id id, gf::Message *msg);

    void clearSequence();

  private:
    static constexpr int MapSize = 40;

    // raw world

    enum class Tile { Wall, Ground, GroundWithItem };

    struct Cell {
      Tile tile;
    };

    Cell m_world[MapSize][MapSize];

    // physics

    std::vector<gf::RectF> m_walls;

    // items

    gf::Vector2i m_heroCoords;

    Rune m_runes[RuneCount];
    int m_runeOrder[RuneCount];
    int m_currentRune;

    Key m_keys[KeyDoorCount];
    Door m_doors[KeyDoorCount];
    Portal m_portals[PortalCount];

    int m_currentPortal;
    bool m_isOnPortal;

    // display

    gf::TileLayer m_groundLayer;
    gf::TileLayer m_wallLayer;

    // finished

    bool m_finished;
  };

}


#endif // HUACA_LEVEL_H
