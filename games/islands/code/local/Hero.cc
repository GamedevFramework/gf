#include "Hero.h"

#include <gf/Math.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Unused.h>

#include "Messages.h"
#include "Singletons.h"

namespace bi {
  static constexpr float HeroAngularVelocity = 1.5f;
  static constexpr float HeroVelocity = 100.0f;

  static constexpr float BoatAngulatVelocity = 0.5f;
  static constexpr float BoatVelocity = 200.0f;

  static constexpr float SpriteSize = 256.0f;
  static constexpr float HeroSize = 64.0f;
  static constexpr float BoatSize = 128.0f;

  static constexpr float StepTime = 0.25f;
  static constexpr float StepAngle = gf::degreesToRadians(10.0f);

  Hero::Hero(Steam& steam, const gf::Vector2f postion)
  : gf::Entity(10)
  , m_steam(steam)
  , m_move(Move::None)
  , m_turn(Turn::None)
  , m_position(postion)
  , m_angle(0.0f)
  , m_texture(gTextureAtlas().getTexture())
  , m_tricorn(gTextureAtlas().getTextureRect("tricorn"))
  , m_boat(gTextureAtlas().getTextureRect("boat"))
  , m_timeElapsed(0.0f)
  , m_alternateStep(true)
  , m_isOnIsland(true)
  , m_isFrozen(false)
  , m_isGameOver(false)
  {
    gMessageManager().registerHandler<StartScan>(&Hero::onStartScan, this);
    gMessageManager().registerHandler<StopScan>(&Hero::onStopScan, this);
    gMessageManager().registerHandler<GameOver>(&Hero::onGameOver, this);
  }

  void Hero::sendInitialPosition() const {
    HeroPosition message;
    message.position = m_position;
    gMessageManager().sendMessage(&message);
  }

  bool Hero::isOnIsland() const {
    return m_isOnIsland;
  }

  bool Hero::scanAvailable() const {
    return m_isOnIsland && !m_isFrozen;
  }

  void Hero::update(float dt) {
    if (m_isGameOver) {
      return;
    }

    m_timeElapsed += dt;

    if (!m_isFrozen) {
      // Set the new angle
      float angularVelocity = m_isOnIsland ? HeroAngularVelocity : BoatAngulatVelocity;

      switch (m_turn) {
      case Turn::Right:
        m_angle += angularVelocity * dt;
        break;
      case Turn::Left:
        m_angle -= angularVelocity * dt;
        break;
      case Turn::None:
        // Nothing
        break;
      }

      // Manage the step "animation"
      while (m_timeElapsed > StepTime) {
        m_timeElapsed -= StepTime;
        m_alternateStep = !m_alternateStep;
      }

      // Set the velocity
      float velocity = m_isOnIsland ? HeroVelocity : BoatVelocity;
      float distance = 0.0f;

      switch (m_move) {
      case Move::Forward:
        distance = velocity * dt;
        break;

      case Move::Backward:
        distance = -velocity * dt;
        break;

      case Move::None:
        // Nothing
        break;
      }

      // Compute the new position
      m_position += gf::unit(m_angle) * distance;
    }

    if (!m_isOnIsland) {
      m_steam.run();
    } else {
      m_steam.stop();
    }

    // Send the position message
    HeroPosition message;
    message.position = m_position;
    message.angle = m_angle;
    gMessageManager().sendMessage(&message);

    m_isOnIsland = message.isOnIsland;
    m_position = message.position;
  }

  void Hero::render(gf::RenderTarget& target) {
    gf::Sprite sprite;
    sprite.setTexture(m_texture);

    if (m_isOnIsland) {
      // Render the step
      float angleRendered = m_angle;

      if (!m_isFrozen && m_move != Move::None) {
        if (m_alternateStep) {
          angleRendered += StepAngle;
        } else {
          angleRendered -= StepAngle;
        }
      }

      sprite.setTextureRect(m_tricorn);
      sprite.setScale(HeroSize / SpriteSize);
      sprite.setRotation(angleRendered - gf::Pi2); // Pi/2 to align the hero front face

    } else {
      sprite.setTextureRect(m_boat);
      sprite.setScale(BoatSize / SpriteSize);
      sprite.setRotation(m_angle);
    }

    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);

    target.draw(sprite);
  }

  gf::MessageStatus Hero::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    gf::unused(id, msg);
    m_isFrozen = true;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onStopScan(gf::Id id, gf::Message *msg) {
    assert(id == StopScan::type);
    gf::unused(id, msg);
    m_isFrozen = false;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOver::type);
    gf::unused(id, msg);
    m_isGameOver = true;
    return gf::MessageStatus::Keep;
  }

}
