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
#include "Ground.h"

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Ball.h"
#include "Messages.h"
#include "Singletons.h"

Ground::Ground() {
  gMessageManager().registerHandler<BallLocationMessage>(&Ground::onBallLocation, this);
}

void Ground::render(gf::RenderTarget& target, const gf::RenderStates& states) {
  gf::RectangleShape shape({ Width, Height });
  shape.setPosition({ 0.0f, 0.0f });
  shape.setAnchor(gf::Anchor::Center);
  shape.setColor(gf::Color::Black);
  target.draw(shape, states);
}

gf::MessageStatus Ground::onBallLocation([[maybe_unused]] gf::Id id, gf::Message *msg) {
  auto loc = static_cast<BallLocationMessage*>(msg);

  static constexpr float YLimit = Height / 2 - Ball::Radius;

  if (loc->position.y > YLimit) {
    loc->velocity.y = -loc->velocity.y;
    loc->position.y = YLimit;
  }

  if (loc->position.y < -YLimit) {
    loc->velocity.y = -loc->velocity.y;
    loc->position.y = -YLimit;
  }

  static constexpr float XLimit = Width / 2 - Ball::Radius;

  if (loc->position.x > XLimit) {
    PointMessage point;
    point.location = Paddle::Location::Right;
    gMessageManager().sendMessage(&point);

    loc->velocity.x = -loc->velocity.x;
    loc->position = gf::Vector2f(0, 0);
  }

  if (loc->position.x < -XLimit) {
    PointMessage point;
    point.location = Paddle::Location::Left;
    gMessageManager().sendMessage(&point);

    loc->velocity.x = -loc->velocity.x;
    loc->position = gf::Vector2f(0, 0);
  }

  return gf::MessageStatus::Keep;
}
