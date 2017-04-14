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
#ifndef LUX_PARTICLES_H
#define LUX_PARTICLES_H

#include <vector>

#include <gf/Entity.h>
#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/Vector.h>
#include <gf/VertexArray.h>

namespace lux {

  class Particles : public gf::Entity {
  public:
    Particles(gf::Random& random, gf::MessageManager& messages);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

    gf::MessageStatus onDead(gf::Id id, gf::Message *msg);

  private:
    struct Particle {
      gf::Vector2f velocity;
      float lifetime;
    };

    struct ParticleSystem {
      std::vector<Particle> particles;
      gf::VertexArray vertices;
      float lifetime;
      float elapsed;
    };

    gf::Random& m_random;
    std::vector<ParticleSystem> m_particleSystems;
  };

}

#endif // LUX_PARTICLES_H
