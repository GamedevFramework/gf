/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include "Board.h"

#include <array>
#include <algorithm>

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Text.h>
#include <gf/Unused.h>

Board::Board(gf::Font& font, gf::Random& random)
: m_font(font)
, m_random(random)
{
  addRandomTile();
  addRandomTile();
}

static bool isOnSide(gf::Vector2u pos, gf::Direction dir) {
  switch (dir) {
    case gf::Direction::Up:
      return pos.y == 0;
    case gf::Direction::Down:
      return pos.y == 3;
    case gf::Direction::Left:
      return pos.x == 0;
    case gf::Direction::Right:
      return pos.x == 3;
    default:
      return true;
  }

  return true;
}


bool Board::move(gf::Direction dir) {
  std::array<unsigned, 4> xIndices = {{ 0, 1, 2, 3 }};
  std::array<unsigned, 4> yIndices = {{ 0, 1, 2, 3 }};

  auto vec = gf::displacement(dir);

  if (vec.x == 1) {
    std::reverse(xIndices.begin(), xIndices.end());
  }

  if (vec.y == 1) {
    std::reverse(yIndices.begin(), yIndices.end());
  }

  bool moved = false;

  Grid grid = m_grid;

  for (auto x : xIndices) {
    for (auto y : yIndices) {
      gf::Vector2u curr(x, y);

      int tile = grid(curr);

      if (tile == 0) {
        continue;
      }

      while (!isOnSide(curr, dir)) {
        gf::Vector2u prev = curr;
        curr = prev + vec;

        grid(prev) = 0;

        if (grid(curr) == tile) {
          // merge
          moved = true;
          grid(curr) = -(tile * 2);
          break;
        }

        if (grid(curr) != 0) {
          grid(prev) = tile;
          break;
        }

        grid(curr) = tile;
        moved = true;
      }
    }
  }

  m_grid = grid;

  if (moved) {
    addRandomTile();
  }

  return moved;
}

void Board::update(gf::Time time) {
  gf::unused(time);

  gf::Vector2u pos;

  for (pos.y = 0; pos.y < Grid::Size; ++pos.y) {
    for (pos.x = 0; pos.x < Grid::Size; ++pos.x) {
      int tile = m_grid(pos);
      if (tile < 0) {
        m_grid(pos) = -tile;
      }
    }
  }

}

void Board::addRandomTile() {
  gf::Vector2u pos;

  do {
    pos.x = m_random.computeUniformInteger(0u, 3u);
    pos.y = m_random.computeUniformInteger(0u, 3u);
  } while (m_grid(pos) != 0);

  m_grid(pos) = m_random.computeBernoulli(0.9) ? 2 : 4;
}


static gf::Color4f getTileColor(int value) {
  switch (value) {
    case 0:
      return gf::Color::fromRgba32(0xCDC1B4FF);
    case 2:
      return gf::Color::fromRgba32(0xEEE4DAFF);
    case 4:
      return gf::Color::fromRgba32(0xEDE0C8FF);
    case 8:
      return gf::Color::fromRgba32(0xF2B179FF);
    case 16:
      return gf::Color::fromRgba32(0xF59563FF);
    case 32:
      return gf::Color::fromRgba32(0xF67C5FFF);
    case 64:
      return gf::Color::fromRgba32(0xF65E3BFF);
    case 128:
      return gf::Color::fromRgba32(0xEDCF72FF);
    case 256:
      return gf::Color::fromRgba32(0xEDCC61FF);
    case 512:
      return gf::Color::fromRgba32(0xEDC850FF);
    case 1024:
      return gf::Color::fromRgba32(0xEDC53FFF);
    case 2048:
      return gf::Color::fromRgba32(0xEDC22EFF);
    default:
      return gf::Color::fromRgba32(0x3C3A32FF);
  }

  return gf::Color::Black;
}

static gf::Color4f getTextColor(int value) {
  switch (value) {
    case 0:
      return gf::Color::Transparent;
    case 2:
    case 4:
      return gf::Color::fromRgba32(0x776E65FF);
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
    case 256:
    case 512:
    default:
      return gf::Color::fromRgba32(0xF9F6F2FF);
  }

  return gf::Color::Black;
}

static unsigned getTextSize(int value) {
  switch (value) {
    case 0:
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
      return 55;
    case 128:
    case 256:
    case 512:
      return 45;
    case 1024:
    case 2048:
      return 35;
    default:
      return 30;
  }

  return 30;
}

static constexpr float TileSize = 106.25f;
static constexpr float TileSpace = 15.0f;
static constexpr float TileCorner = 3.0f;
static constexpr float ContainerSize = Grid::Size * TileSize + (Grid::Size + 1) * TileSpace;

void Board::render(gf::RenderTarget& target, const gf::RenderStates& states) {
  gf::RoundedRectangleShape background({ ContainerSize, ContainerSize }, TileCorner);
  background.setColor(gf::Color::fromRgba32(0xBBADA0FF));
  target.draw(background, states);

  for (unsigned y = 0; y < Grid::Size; ++y) {
    for (unsigned x = 0; x < Grid::Size; ++x) {
      int tile = m_grid({ x, y });

      if (tile >= 0) {
        gf::Vector2f pos(TileSpace + x * (TileSize + TileSpace), TileSpace + y * (TileSize + TileSpace));

        gf::RoundedRectangleShape shape({ TileSize, TileSize }, TileCorner);
        shape.setPosition(pos);
        shape.setColor(getTileColor(tile));
        target.draw(shape, states);

        gf::Text text(std::to_string(tile), m_font, getTextSize(tile));
        text.setColor(getTextColor(tile));
        text.setAnchor(gf::Anchor::Center);
        text.setPosition(pos + TileSize / 2);
        target.draw(text, states);
      }
    }
  }
}

gf::RectF Board::getView() {
  return gf::RectF(0, 0, ContainerSize, ContainerSize);
}
