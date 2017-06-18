/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016-2017  Lux team
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
#ifndef LUX_STARS_H
#define LUX_STARS_H

#include <array>

#include <gf/Entity.h>
#include <gf/Random.h>
#include <gf/Vector.h>

namespace lux {

  class Stars : public gf::Entity {
  public:
    static constexpr float SomeWhiteStars = 0.25f;
    static constexpr float AllWhiteStars = 1.0f;

    Stars(gf::Random& random, float velocity, float radius, float whiteFreq = SomeWhiteStars);


    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    static constexpr std::size_t Count = 50;

    struct Star {
      gf::Vector2f position;
      gf::Color4f color;
    };

    gf::Random& m_random;
    float m_velocity;
    float m_radius;
    float m_whiteFreq;
    std::array<Star, Count> m_stars;
  };

}

#endif // LUX_STARS_H
