#include "Steam.h"

#include <gf/Particles.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float Velocity = 15.0f;

  static constexpr float InitialAlpha = 0.5f;
  static constexpr float AlphaFading = 0.1f;

  static constexpr float InitialRadius = 16.0f;
  static constexpr float RadiusIncrease = 5.0f;

  static constexpr float GenerationTime = 0.04f;

  Steam::Steam()
  : gf::Entity(20)
  , m_heroPosition(0, 0)
  , m_heroAngle(0)
  , m_timer(0)
  , m_running(false)
  {
    gMessageManager().registerHandler<HeroPosition>(&Steam::onHeroPosition, this);
  }

  void Steam::update(float dt) {
    while (!m_clouds.empty() && m_clouds.front().alpha < 0) {
      m_clouds.pop_front();
    }

    for (auto& cloud : m_clouds) {
      cloud.position += Velocity * dt * gf::unit(cloud.angle);
      cloud.radius += RadiusIncrease * dt;
      cloud.alpha -= AlphaFading * dt;
    }

    if (!m_running) {
      return;
    }

    m_timer -= dt;

    if (m_timer > 0) {
      return;
    }

    Cloud cloud;
    cloud.position = m_heroPosition - gf::unit(m_heroAngle) * 40; // magic!
    cloud.angle = gRandom().computeUniformFloat(0.0f, 2 * gf::Pi);
    cloud.radius = InitialRadius;
    cloud.alpha = InitialAlpha;

    m_clouds.push_back(cloud);

    m_timer += GenerationTime;
  }

  void Steam::render(gf::RenderTarget& target) {
    gf::Color4f color(0.5f, 0.5f, 0.5f, 1.0f);

    gf::ShapeParticles particles;

    for (const auto& cloud : m_clouds) {
      color.a = cloud.alpha;
      particles.addCircle(cloud.position, cloud.radius, color);
    }

    target.draw(particles);
  }

  gf::MessageStatus Steam::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    gf::unused(id);
    auto hero = static_cast<HeroPosition*>(msg);
    m_heroPosition = hero->position;
    m_heroAngle = hero->angle;
    return gf::MessageStatus::Keep;
  }

}
