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
#include "Stars.h"

#include <gf/Color.h>
#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "World.h"

namespace lux {

  static float getWorldTop() {
    return WorldCenter.y - WorldSize.height / 2;
  }

  static float getWorldBottom() {
    return WorldCenter.y + WorldSize.height / 2;
  }

  static float getWorldLeft() {
    return WorldCenter.x - WorldSize.width / 2;
  }

  static float getWorldRight() {
    return WorldCenter.x + WorldSize.width / 2;
  }

  Stars::Stars(gf::Random& random, float velocity, float radius, float whiteFreq)
  : gf::Entity(-1)
  , m_random(random)
  , m_velocity(velocity)
  , m_radius(radius)
  , m_whiteFreq(whiteFreq)
  {
    for (Star& star : m_stars) {
      star.position.x = m_random.computeUniformFloat(getWorldLeft(), getWorldRight());
      star.position.y = m_random.computeUniformFloat(getWorldTop(), getWorldBottom());

      if (m_random.computeBernoulli(m_whiteFreq)) {
        star.color = gf::Color::White;
      } else {
        star.color = {
          m_random.computeUniformFloat(0.2f, 1.0f),
          0.0f,
          m_random.computeUniformFloat(0.0f, 1.0f),
          1.0f
        };
      }
    }
  }

  void Stars::update(float dt) {
    for (Star& star : m_stars) {
      if (star.position.y > getWorldBottom()) {
        star.position.x = m_random.computeUniformFloat(getWorldLeft(), getWorldRight());
        star.position.y -= WorldSize.height * 1.1;

        if (m_random.computeBernoulli(m_whiteFreq)) {
          star.color = gf::Color::White;
        } else {
          star.color = {
            m_random.computeUniformFloat(0.2f, 1.0f),
            0.0f,
            m_random.computeUniformFloat(0.0f, 1.0f),
            1.0f
          };
        }
      }

      star.position.y += m_velocity * dt;
    }
  }

  void Stars::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::CircleShape shape(m_radius);
    shape.setAnchor(gf::Anchor::Center);

    for (Star& star : m_stars) {
      shape.setPosition(star.position);
      shape.setColor(star.color);
      target.draw(shape, states);
    }
  }


}
