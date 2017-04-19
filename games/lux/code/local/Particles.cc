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
#include "Particles.h"

#include <gf/Math.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Messages.h"

namespace lux {

  Particles::Particles(gf::Random& random, gf::MessageManager& messages)
  : gf::Entity(1)
  , m_random(random)
  {
    messages.registerHandler<DeadMessage>(&Particles::onDead, this);
  }

  void Particles::update(float dt) {
    for (ParticleSystem& sys : m_particleSystems) {
      sys.elapsed += dt;

      // If it's end of animation
      if (sys.elapsed >= sys.lifetime) {
        continue;
      }

      for (std::size_t i = 0; i < sys.particles.size(); ++i) {
        Particle& p = sys.particles[i];
        p.lifetime -= dt;

        if (p.lifetime < 0.0f) {
          sys.vertices[i].color.a = 0.0f;
          continue;
        }

        sys.vertices[i].position += p.velocity * dt;

        float ratio = 0.75f + 0.25f * p.lifetime / sys.lifetime;
        sys.vertices[i].color.a = ratio;
      }
    }

    // Delete the old animations
    const auto trash = std::partition(m_particleSystems.begin(), m_particleSystems.end(), [](const ParticleSystem &sys) {
      return sys.elapsed < sys.lifetime;
    });

    m_particleSystems.erase(trash, m_particleSystems.end());
  }

  void Particles::render(gf::RenderTarget& target) {
    gf::RenderStates states;
    states.lineWidth = 10.0f;

    for (ParticleSystem& sys : m_particleSystems) {
      target.draw(sys.vertices, states);
    }
  }

  static std::tuple<gf::Color4f, gf::Color4f> colorFromShipClass(ShipClass ship) {
    switch (ship) {
      case ShipClass::Antlia:
        return std::make_tuple(gf::Color::fromRgba32(0x8C, 0xED, 0xDC), gf::Color::fromRgba32(0x5D, 0x7F, 0x83));
      case ShipClass::Bootes:
        return std::make_tuple(gf::Color::fromRgba32(0x7C, 0xBE, 0x41), gf::Color::fromRgba32(0xB6, 0x0B, 0x0B));
      case ShipClass::Cygnus:
      case ShipClass::Draco:
        return std::make_tuple(gf::Color::fromRgba32(0xFF, 0x0C, 0xFF), gf::Color::fromRgba32(0xB6, 0x0B, 0x0B));
      case ShipClass::Eridanus:
        return std::make_tuple(gf::Color::fromRgba32(0xFF, 0xFF, 0x12), gf::Color::fromRgba32(0xB6, 0x0B, 0x0B));
    }

    return std::make_tuple(gf::Color::White, gf::Color::White);
  }


  static constexpr std::size_t ParticlesCount = 800;
  static constexpr float ParticleSystemLifetime = 1.5f;

  gf::MessageStatus Particles::onDead(gf::Id id, gf::Message *msg) {
    gf::unused(id);
    auto dead = static_cast<DeadMessage*>(msg);

    ParticleSystem sys;

    sys.vertices.resize(ParticlesCount);
    sys.vertices.setPrimitiveType(gf::PrimitiveType::Points);

    std::uniform_real_distribution<float> distAngle(0, 2 * gf::Pi);
    std::uniform_real_distribution<float> distNorm(0.0f, 150.0f);
    std::normal_distribution<float> distLifetime(1.0f, 0.166f);
    std::bernoulli_distribution distColor(0.6);

    gf::Color4f major;
    gf::Color4f minor;

    std::tie(major, minor) = colorFromShipClass(dead->ship);

    for (std::size_t i = 0; i < ParticlesCount; ++i) {
      Particle p;

      p.lifetime = distLifetime(m_random.getEngine());

      float angle = distAngle(m_random.getEngine());
      float norm = distNorm(m_random.getEngine());

      p.velocity.x = norm * std::cos(angle);
      p.velocity.y = norm * std::sin(angle);

      sys.particles.push_back(p);

      sys.vertices[i].position = dead->position;
      sys.vertices[i].color = distColor(m_random.getEngine()) ? major : minor;
    }

    sys.elapsed = 0.0f;
    sys.lifetime = ParticleSystemLifetime;

    m_particleSystems.push_back(sys);

    return gf::MessageStatus::Keep;
  }

}
