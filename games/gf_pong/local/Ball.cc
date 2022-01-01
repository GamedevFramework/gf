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
#include "Ball.h"

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Messages.h"
#include "Singletons.h"

void Ball::update(gf::Time time) {
  float dt = time.asSeconds();

  m_position += m_velocity * dt;

  BallLocationMessage loc;
  loc.position = m_position;
  loc.velocity = m_velocity;

  gMessageManager().sendMessage(&loc);

  m_position = loc.position;
  m_velocity = loc.velocity;
}

void Ball::render(gf::RenderTarget& target, const gf::RenderStates& states) {
  gf::CircleShape shape(Radius);
  shape.setAnchor(gf::Anchor::Center);
  shape.setPosition(m_position);
  shape.setColor(gf::Color::White);
  target.draw(shape, states);
}
