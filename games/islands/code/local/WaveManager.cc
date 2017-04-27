#include "WaveManager.h"

#include <gf/ColorRamp.h>
#include <gf/Particles.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>

#include "Messages.h"
#include "Sea.h"
#include "Singletons.h"

namespace bi {

  static constexpr float WaveLifetime = 30.0f;
  static constexpr float WaveWidth = 1000.0f;
  static constexpr float WaveVelocity = 90.0f;
  static constexpr float WaveDeathDistance = 100.0f;

  static constexpr float BubblePerSecond = 2000.0f;
  static constexpr float BubbleAngle = 3 * gf::Pi / 2;
  static constexpr float BubbleLifetime = 3.0f;
  static constexpr float BubbleVelocity = 50.0f;
  static constexpr float BubbleInitialRadius = 6.0f;
  static constexpr float BubbleRadiusIncrease = 10.0f;
  static constexpr float BubbleAdvance = 20.0f;

  WaveManager::WaveManager()
  : gf::Entity(15)
  , m_elapsed(0)
  {
    gMessageManager().registerHandler<HeroPosition>(&WaveManager::onHeroPosition, this);

    m_p0 = { 6000.0f - WaveWidth / 2, 5500.0f };
    m_p1 = { 6000.0f + WaveWidth / 2, 5500.0f };
  }

  void WaveManager::update(float dt) {
    while (!m_bubbles.empty() && m_bubbles.front().lifetime < 0) {
      m_bubbles.pop_front();
    }

    m_elapsed += dt;

    if (m_elapsed > WaveLifetime) {
      m_elapsed = 0;

      m_p0.x = gRandom().computeUniformFloat(Sea::WorldMin, Sea::WorldMax);
      m_p0.y = gRandom().computeUniformFloat(Sea::WorldMin, Sea::WorldMax);

      float angle = gRandom().computeUniformFloat(0.0f, 2 * gf::Pi);
      m_p1 = m_p0 + gf::unit(angle) * WaveWidth;

      // go towards center of the map

      gf::Vector2f center = gf::Vector2f({ Sea::TileSize, Sea::TileSize }) * Sea::Size / 2;

      if (gf::dot(center - m_p0, gf::perp(m_p1 - m_p0)) < 0) {
        std::swap(m_p0, m_p1);
      }
    }

    for (auto& bubble : m_bubbles) {
      bubble.position += BubbleVelocity * dt * gf::unit(bubble.angle);
      bubble.radius += BubbleRadiusIncrease * dt;
      bubble.lifetime -= dt;
    }

    gf::Vector2f normal = gf::normalize(gf::perp(m_p1 - m_p0));
    float normalAngle = gf::angle(normal);

    m_p0 += WaveVelocity * dt * normal;
    m_p1 += WaveVelocity * dt * normal;

    unsigned bubbleCount = static_cast<unsigned>(dt * BubblePerSecond);

    for (unsigned i = 0; i < bubbleCount; ++i) {
      float t = gRandom().computeUniformFloat(0.0f, 1.0f);

      Bubble bubble;
      bubble.position = gf::lerp(m_p0, m_p1, t) + gRandom().computeUniformFloat(0.0f, 1.0f) * BubbleAdvance * normal;
      bubble.angle = gRandom().computeUniformFloat(normalAngle - BubbleAngle, normalAngle + BubbleAngle) + gf::Pi;
      bubble.radius = BubbleInitialRadius;
      bubble.lifetime = BubbleLifetime;

      m_bubbles.push_back(bubble);
    }

    // check if the hero is under the wave

    float distanceFromWave = gf::dot(-normal, (m_hero - m_p0));

    if (distanceFromWave <= 0 || distanceFromWave >  WaveDeathDistance) {
      return;
    }

    // http://geomalgorithms.com/a02-_lines.html#Distance-to-Ray-or-Segment

    float c1 = gf::dot((m_hero - m_p0), (m_p1 - m_p0));

    if (c1 < 0) {
      return;
    }

    float c2 = gf::squareDistance(m_p0, m_p1);

    if (c1 > c2) {
      return;
    }

    GameOver msg;
    gMessageManager().sendMessage(&msg);
  }

  void WaveManager::render(gf::RenderTarget& target) {
    gf::Color4f deepBlue = gf::Color::fromRgba32(2, 43, 68, 0);

    gf::ShapeParticles particles;

    for (const auto& bubble : m_bubbles) {
      gf::Color4f color = gf::lerp(deepBlue, gf::Color::White, bubble.lifetime / BubbleLifetime);
      particles.addCircle(bubble.position, bubble.radius, color);
    }

    target.draw(particles);
  }

  gf::MessageStatus WaveManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    gf::unused(id);

    auto hero = static_cast<HeroPosition*>(msg);
    m_hero = hero->position;
    return gf::MessageStatus::Keep;
  }

}
