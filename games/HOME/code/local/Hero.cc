/*
 * H.O.M.E.
 * Copyright (C) 2019 Hatunruna team
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
#include "Hero.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace home {

  namespace {
    static constexpr float HeroVelocity = 200.0f;
    static constexpr float HeroVolume = 75.0f;

    gf::Orientation harvestOrientation(float angle) {
      assert(-gf::Pi <= angle && angle <= gf::Pi);

      static constexpr float Pi4 = gf::Pi / 4.0f;

      if (angle < -2 * Pi4) { return gf::Orientation::NorthWest; }
      if (angle <  0 * Pi4) { return gf::Orientation::NorthEast; }
      if (angle <  2 * Pi4) { return gf::Orientation::SouthEast; }
      return gf::Orientation::SouthWest;
    }
  }

  Hero::Hero()
  : gf::Entity(100)
  , m_position({ 45.0f * 128.0f, 42.0f * 64.0f })
  , m_positionClicked(m_position)
  , m_jetSound(gResourceManager().getSound("sounds/jet_engine.ogg"))
  , m_deathSound(gResourceManager().getSound("sounds/death.ogg"))
  , m_orientation(gf::Orientation::SouthEast)
  , m_status(Status::Waiting)
  , m_activity(Activity::Walking)
  , m_moveAndPauseTexture(gResourceManager().getTexture("images/player_animations.png"))
  , m_deathTexture(gResourceManager().getTexture("images/player_death.png"))
  , m_currentAnimation(nullptr)
  , m_crosshairTexture(gResourceManager().getTexture("crosshair.png"))
  , m_gameOverTexture(gResourceManager().getTexture("images/game_over.png"))
  , m_gameOverAlpha(0.0f)
  {
    gMessageManager().registerHandler<CursorClickedPosition>(&Hero::onMouseClicked, this);
    gMessageManager().registerHandler<HarvestSupply>(&Hero::onHarvestSupply, this);
    gMessageManager().registerHandler<GameOver>(&Hero::onGameOver, this);

    m_jetSound.setLoop(true);
    m_jetSound.setVolume(HeroVolume);

    m_deathSound.setVolume(HeroVolume);

    // load animations
    loadAnimation(m_moveWest, 0);
    loadAnimation(m_moveNorthWest, 1);
    loadAnimation(m_moveNorth, 2);
    loadAnimation(m_moveNorthEast, 3);
    loadAnimation(m_moveEast, 4);
    loadAnimation(m_moveSouthEast, 5);
    loadAnimation(m_moveSouthWest, 6);

    loadAnimation(m_pauseWest, 7);
    loadAnimation(m_pauseNorthWest, 8);
    loadAnimation(m_pauseNorth, 9);
    loadAnimation(m_pauseNorthEast, 10);
    loadAnimation(m_pauseEast, 11);
    loadAnimation(m_pauseSouthEast, 12);
    loadAnimation(m_pauseSouthWest, 13);

    loadAnimation(m_south, 14);

    loadAnimation(m_harvestNorthWest, 15);
    loadAnimation(m_harvestNorthEast, 16);
    loadAnimation(m_harvestSouthEast, 17);
    loadAnimation(m_harvestSouthWest, 18);

    loadAnimationDeath();
  }

  void Hero::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Sprite cross;
    cross.setTexture(m_crosshairTexture);
    cross.setPosition(m_positionClicked);
    cross.setScale({ 0.5f, 0.25f });
    cross.setAnchor(gf::Anchor::Center);
    target.draw(cross, states);

    gf::AnimatedSprite animated;
    animated.setAnimation(*m_currentAnimation);
    animated.setScale(0.75f);
    animated.setPosition(m_position);
    animated.setAnchor(gf::Anchor::Center);
    target.draw(animated, states);

    if (m_status == Status::Dying) {
      gf::Sprite sprite;
      sprite.setTexture(m_gameOverTexture);
      sprite.setColor(gf::Color::Opaque(m_gameOverAlpha));
      sprite.setScale(0.55f);
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setPosition(m_position);
      target.draw(sprite, states);
    }
  }

  void Hero::update(gf::Time time) {
    if (m_status != Status::Dying) {
      gf::Vector2f move = m_positionClicked - m_position;
      float length = gf::euclideanLength(move);

      if (length > time.asSeconds() * HeroVelocity) {
        if (m_activity == Activity::Mining) {
          m_orientation = harvestOrientation(gf::angle(move));
        } else {
          assert(m_activity == Activity::Walking);
          m_orientation = gf::orientation(gf::angle(move));
        }

        m_velocity = (move / length) * HeroVelocity;
        m_status = Status::Moving;
      } else {
        m_position += move;
        m_velocity = { 0.0f, 0.0f };
        m_status = Status::Waiting;
      }


      if (m_status == Status::Moving) {
        if (m_jetSound.getStatus() != sf::SoundSource::Playing) {
          m_jetSound.play();
        }
      } else {
        if (m_jetSound.getStatus() == sf::SoundSource::Playing) {
          m_jetSound.stop();
        }
      }

      // Determine the animation
      if (m_activity == Activity::Mining && m_status == Status::Waiting) {
        switch (m_orientation) {
          case gf::Orientation::NorthWest:
            m_currentAnimation = &m_harvestNorthWest;
            break;
          case gf::Orientation::NorthEast:
            m_currentAnimation = &m_harvestNorthEast;
            break;
          case gf::Orientation::SouthEast:
            m_currentAnimation = &m_harvestSouthEast;
            break;
          case gf::Orientation::SouthWest:
            m_currentAnimation = &m_harvestSouthWest;
            break;
          default:
          // assert(false);
          break;
        }
      } else {
        switch (m_orientation) {
          case gf::Orientation::NorthWest:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveNorthWest;
            } else {
              m_currentAnimation = &m_pauseNorthWest;
            }
            break;
          case gf::Orientation::North:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveNorth;
            } else {
              m_currentAnimation = &m_pauseNorth;
            }
            break;
          case gf::Orientation::NorthEast:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveNorthEast;
            } else {
              m_currentAnimation = &m_pauseNorthEast;
            }
            break;
          case gf::Orientation::East:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveEast;
            } else {
              m_currentAnimation = &m_pauseEast;
            }
            break;
          case gf::Orientation::SouthEast:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveSouthEast;
            } else {
              m_currentAnimation = &m_pauseSouthEast;
            }
            break;
          case gf::Orientation::South:
            m_currentAnimation = &m_south;
            break;
          case gf::Orientation::SouthWest:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveSouthWest;
            } else {
              m_currentAnimation = &m_pauseSouthWest;
            }
            break;
          case gf::Orientation::West:
            if (m_status == Status::Moving) {
              m_currentAnimation = &m_moveWest;
            } else {
              m_currentAnimation = &m_pauseWest;
            }
            break;
          default:
            assert(false);
            break;
        }
      }
    } else {
      m_currentAnimation = &m_death;
      m_positionClicked = m_position;
      m_velocity = { 0.0f, 0.0f };

      if (m_deathSound.getStatus() != sf::SoundSource::Playing) {
        m_deathSound.play();
      }
    }

    // Update sprite
    assert(m_currentAnimation);
    m_currentAnimation->update(time);

    if (m_position.x > 11310.0f && m_position.x < 11685.0f && m_position.y < 8865.0f && m_position.y > 8680.0f) { // TODO
      Victory msg;
      gMessageManager().sendMessage(&msg);
    }

    HeroPosition message;
    message.position = m_position;
    gMessageManager().sendMessage(&message);

    m_activity = Activity::Walking;

    if (m_status == Status::Dying) {
      m_gameOverAlpha = m_gameOverAlpha >= 1.0f ? 1.0f : m_gameOverAlpha + time.asSeconds();
    }
  }

  gf::MessageStatus Hero::onMouseClicked(gf::Id id, gf::Message *msg) {
    assert(id == CursorClickedPosition::type);
    // store mouse coordinates
    m_positionClicked = static_cast<CursorClickedPosition*>(msg)->position;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onHarvestSupply(gf::Id id, gf::Message *msg) {
    assert(id == HarvestSupply::type);
    gf::unused(id, msg);
    m_activity = Activity::Mining;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOver::type);
    gf::unused(id, msg);
    m_status = Status::Dying;
    return gf::MessageStatus::Keep;
  }

  void Hero::loadAnimation(gf::Animation &animation, int line) {
    static constexpr gf::Vector2f TextureSize = { 5632.0f, 4864.0f };
    static constexpr gf::Vector2f FrameSize = { 256.0f, 256.0f };
    static constexpr gf::Time FrameDuration = gf::seconds(1.0f / 30.0f);

    for (int i = 0; i < 22; ++i) {
      gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(i, line) *  FrameSize / TextureSize, FrameSize / TextureSize);
      animation.addFrame(m_moveAndPauseTexture, frame, FrameDuration);
    }
  }

  void Hero::loadAnimationDeath() {
    static constexpr gf::Vector2f TextureSize = { 5632.0f, 1024.0f };
    static constexpr gf::Vector2f FrameSize = { 256.0f, 256.0f };
    static constexpr gf::Time FrameDuration = gf::seconds(1.0f / 30.0f);

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 22; ++j) {
        gf::RectF frame = gf::RectF::fromPositionSize(gf::Vector2i(j, i) * FrameSize / TextureSize, FrameSize / TextureSize);

        if (i == 3 && j == 18) {
          m_death.addFrame(m_deathTexture, frame, gf::seconds(60 * 60 * 24 * 10000));
          return;
        } else {
          m_death.addFrame(m_deathTexture, frame, FrameDuration);
        }
      }
    }
  }

}
