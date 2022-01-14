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
#ifndef LOCAL_MESSAGES_H
#define LOCAL_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

#include "Paddle.h"

using namespace gf::literals;

struct BallLocationMessage : public gf::Message {
  static constexpr gf::Id type = "BallLocation"_id;

  gf::Vector2f position;
  gf::Vector2f velocity;
};

struct PointMessage : public gf::Message {
  static constexpr gf::Id type = "Point"_id;

  Paddle::Location location;
};

#endif // LOCAL_MESSAGES_H
