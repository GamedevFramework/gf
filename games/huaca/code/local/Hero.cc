/*
 * Huaca, find the ritual to escape the temple
 * Copyright (C) 2016-2017  Hatunruna team
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

#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Level.h"
#include "Messages.h"
#include "Singletons.h"

namespace huaca {

  static constexpr float TextureSize = 256.0f;

  static constexpr gf::Time FrameTime = gf::milliseconds(50);

  static constexpr float Speed = 180.0f;

  static void loadSingleFrameAnimation(gf::Animation& animation, const gf::Path& path) {
    gf::Texture& texture = gResourceManager().getTexture(path);
    texture.setSmooth();
    animation.addFrame(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f, 1.0f }), gf::seconds(1));
  }

  static void loadMultiFrameAnimation(gf::Animation& animation, const gf::Path& path) {
    gf::Texture& texture = gResourceManager().getTexture(path);
    texture.setSmooth();

    for (int i = 0; i < 12; ++i) {
      animation.addFrame(texture, gf::RectF::fromPositionSize({ (i % 4) / 4.0f, (i / 4) / 3.0f }, { 1.0f / 4.0f, 1.0f / 3.0f }), FrameTime);
    }
  }

  Hero::Hero()
  : gf::Entity(2)
  , m_initialPosition(0, 0)
  , m_position(0, 0)
  , m_direction(gf::Direction::Right)
  , m_isRunning(false)
  , m_currentAnimation(&m_static[1])
  {
    // load animation

    loadSingleFrameAnimation(m_static[0], "img/static_up.png");
    loadSingleFrameAnimation(m_static[1], "img/static_right.png");
    loadSingleFrameAnimation(m_static[2], "img/static_down.png");
    loadSingleFrameAnimation(m_static[3], "img/static_left.png");

    loadMultiFrameAnimation(m_running[0], "img/run_up.png");
    loadMultiFrameAnimation(m_running[1], "img/run_right.png");
    loadMultiFrameAnimation(m_running[2], "img/run_down.png");
    loadMultiFrameAnimation(m_running[3], "img/run_left.png");

    // register messages

    gMessageManager().registerHandler<NewLevelMessage>(&Hero::onNewLevel, this);
    gMessageManager().registerHandler<ResetLevelMessage>(&Hero::onResetLevel, this);
  }

  void Hero::dropPortal() {
    PortalDroppedMessage msg;
    msg.position = m_position + gf::Vector2f(0.0f, Level::TileSize / 3); // HACK
    gMessageManager().sendMessage(&msg);
  }

  static gf::RectF boundsFromPosition(gf::Vector2f position) {
    return gf::RectF::fromPositionSize(
      { position.x - Level::TileSize / 4 - 2, position.y + Level::TileSize / 12 }, // some magic
      { Level::TileSize / 2, Level::TileSize / 2 }
    );
  }

  void Hero::update(gf::Time time) {
    float dt = time.asSeconds();

    // update position

    if (m_isRunning) {
      m_position += gf::displacement(m_direction) * Speed * dt;
    }

    // update animation

    int dir = static_cast<int>(m_direction);
    assert(0 <= dir && dir < 4);

    if (m_isRunning) {
      m_currentAnimation = &m_running[dir];
    } else {
      m_currentAnimation = &m_static[dir];
    }

    m_currentAnimation->update(time);

    // send message

    HeroPositionMessage msg;
    msg.position = m_position;
    msg.bounds = boundsFromPosition(m_position);
    msg.isRunning = m_isRunning;

    gMessageManager().sendMessage(&msg);

    m_position = msg.position;
  }

  void Hero::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::AnimatedSprite sprite;
    sprite.setAnimation(*m_currentAnimation);
    sprite.setScale(Level::TileSize / TextureSize);
    sprite.setPosition(m_position);
    sprite.setAnchor(gf::Anchor::Center);
    target.draw(sprite, states);
  }

  gf::MessageStatus Hero::onNewLevel(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    auto newLevel = static_cast<NewLevelMessage*>(msg);
    m_initialPosition = m_position = newLevel->heroPosition;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Hero::onResetLevel(gf::Id id, gf::Message *msg) {
    gf::unused(id);
    gf::unused(msg);

    m_position = m_initialPosition;
    return gf::MessageStatus::Keep;
  }

}
