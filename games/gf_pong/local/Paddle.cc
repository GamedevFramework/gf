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
#include "Paddle.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Ball.h"
#include "Ground.h"
#include "Messages.h"
#include "Singletons.h"

Paddle::Paddle(Location loc)
: m_position({ loc == Location::Left ? - Position : Position, 0.0f })
, m_location(loc)
, m_move(Move::Stop)
{
  gMessageManager().registerHandler<BallLocationMessage>(&Paddle::onBallLocation, this);
}

void Paddle::update(gf::Time time) {
  float dt = time.asSeconds();

  switch (m_move) {
    case Move::Up:
      if (m_position.y > - (Ground::Height - Height) / 2) {
        m_position.y -= Velocity * dt;
      }

      break;

    case Move::Down:
      if (m_position.y < (Ground::Height - Height) / 2) {
        m_position.y += Velocity * dt;
      }

      break;

    case Move::Stop:
      break;
  }
}

gf::MessageStatus Paddle::onBallLocation([[maybe_unused]] gf::Id id, gf::Message *msg) {
  auto loc = static_cast<BallLocationMessage*>(msg);

  gf::Vector2f n = loc->position - m_position;

  if (n.x * loc->velocity.x < 0) {
    if (std::abs(n.x) < Ball::Radius + Width / 2 && std::abs(n.y) < Height / 2) {
      float angle = 0.7f * n.y / Height * gf::Pi;

      if (loc->velocity.x > 0) {
        loc->velocity = gf::unit(gf::Pi - angle) * Ball::Velocity;
      } else {
        loc->velocity = gf::unit(angle) * Ball::Velocity;
      }
    }
  }

  return gf::MessageStatus::Keep;
}

void Paddle::render(gf::RenderTarget& target, const gf::RenderStates& states) {
  gf::RectangleShape shape({ Width, Height });
  shape.setAnchor(gf::Anchor::Center);
  shape.setPosition(m_position);
  shape.setColor(m_location == Location::Left ? gf::Color::Orange : gf::Color::Azure);
  target.draw(shape, states);
}
