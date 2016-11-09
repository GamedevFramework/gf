/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016  Lux team
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
#ifndef LUX_SHOOT_H
#define LUX_SHOOT_H

#include <memory>

#include <gf/MessageManager.h>
#include <gf/Vector.h>

#include "Properties.h"

namespace lux {

  class Shoot {
  public:
    virtual ~Shoot();
    virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) = 0;
  };

  std::unique_ptr<Shoot> makeSimpleShoot(Origin origin, ShipClass shipClass, float delay);
  std::unique_ptr<Shoot> makeBurstShoot(Origin origin, ShipClass shipClass, float delay, float period, int count);
  std::unique_ptr<Shoot> makeConeShoot(Origin origin, ShipClass shipClass, float delay);
  std::unique_ptr<Shoot> makeContinuousSimpleShoot(Origin origin, ShipClass shipClass, float period);
  std::unique_ptr<Shoot> makeSimplePlayerShoot(Origin origin, ShipClass shipClass, int nbshoot, float shootInterval, float inactivePeriod);

}

#endif // LUX_SHOOT_H
