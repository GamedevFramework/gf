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
#ifndef LOCAL_PADDLE_H
#define LOCAL_PADDLE_H

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

class Paddle : public gf::Entity {
public:
  static constexpr float Width = 2.0f;
  static constexpr float Height = 20.0f;

  static constexpr float Position = 75.0f;
  static constexpr float Velocity = 50.0f;

  enum class Move {
    Up,
    Stop,
    Down,
  };

  enum class Location {
    Left,
    Right,
  };

  Paddle(Location loc);

  void move(Move m) {
    m_move = m;
  }

  virtual void update(gf::Time time) override;
  virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

private:
  gf::MessageStatus onBallLocation(gf::Id id, gf::Message *msg);

private:
  gf::Vector2f m_position;
  Location m_location;
  Move m_move;
};


#endif // LOCAL_PADDLE_H
