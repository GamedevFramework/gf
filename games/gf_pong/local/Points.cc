/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
#include "Points.h"

#include <cassert>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Messages.h"
#include "Singletons.h"

Points::Points()
: gf::Entity(1)
, m_leftPlayerPoints(0)
, m_rightPlayerPoints(0)
{
  gMessageManager().registerHandler<PointMessage>(&Points::onPoint, this);
}


static const char digits[10][5][4] = {
  {
    " ##",
    "# #",
    "# #",
    "# #",
    "## ",
  },
  {
    " # ",
    " # ",
    " # ",
    " # ",
    " # ",
  },
  {
    "## ",
    "  #",
    " # ",
    "#  ",
    "###",
  },
  {
    "###",
    "  #",
    " ##",
    "  #",
    "###",
  },
  {
    "#  ",
    "#  ",
    "# #",
    "###",
    "  #",
  },
  {
    "###",
    "#  ",
    "###",
    "  #",
    "## ",
  },
  {
    "#  ",
    "#  ",
    "###",
    "# #",
    "###",
  },
  {
    "###",
    "  #",
    "  #",
    "  #",
    "  #",
  },
  {
    "###",
    "# #",
    "###",
    "# #",
    "###",
  },
  {
    "###",
    "# #",
    "###",
    "  #",
    "  #",
  },
};

static void displayDigit(int d, gf::Vector2f position, const gf::Color4f& color, gf::RenderTarget& target, const gf::RenderStates& states) {
  assert(0 <= d && d <= 9);

  static constexpr float Width = 1.3f;
  static constexpr float Height = 2.0f;

  gf::RectangleShape shape({ Width, Height });
  shape.setAnchor(gf::Anchor::Center);
  shape.setColor(color);

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (digits[d][i][j] == '#') {
        shape.setPosition({ position.x + (j - 1) * Width, position.y + (i - 2) * Height });
        target.draw(shape, states);
      }
    }
  }
}

void Points::render(gf::RenderTarget& target, const gf::RenderStates& states) {
  static constexpr float HiX = 85.0f;
  static constexpr float LoX = 90.0f;
  static constexpr float Y = 40.0f;

  displayDigit((m_leftPlayerPoints / 10) % 10, { -LoX, -Y }, gf::Color::Orange, target, states);
  displayDigit(m_leftPlayerPoints % 10, { - HiX, -Y }, gf::Color::Orange, target, states);

  displayDigit((m_rightPlayerPoints / 10) % 10, { HiX, -Y }, gf::Color::Azure, target, states);
  displayDigit(m_rightPlayerPoints % 10, { LoX, -Y }, gf::Color::Azure, target, states);
}


gf::MessageStatus Points::onPoint([[maybe_unused]] gf::Id id, gf::Message *msg) {
  assert(id == PointMessage::type);

  auto point = static_cast<PointMessage*>(msg);

  if (point->location == Paddle::Location::Right) {
    m_leftPlayerPoints++;
  } else {
    m_rightPlayerPoints++;
  }

  return gf::MessageStatus::Keep;
}
