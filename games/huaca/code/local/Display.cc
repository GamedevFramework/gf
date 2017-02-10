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
#include "Display.h"

#include <gf/Coordinates.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "Singletons.h"
#include "Messages.h"

namespace huaca {

  static gf::Texture *loadTexture(const gf::Path& path) {
    gf::Texture& texture = gResourceManager().getTexture(path);
    texture.setSmooth();
    return &texture;
  }

  Display::Display()
  {
    // init

    clear();

    // load textures

    m_runes[0].texture = loadTexture("img/rune0_yellow.png");
    m_runes[1].texture = loadTexture("img/rune1_red.png");
    m_runes[2].texture = loadTexture("img/rune2_green.png");
    m_runes[3].texture = loadTexture("img/rune3_purple.png");

    m_keys[0].texture = loadTexture("img/key_iron.png");
    m_keys[1].texture = loadTexture("img/key_bronze.png");
    m_keys[2].texture = loadTexture("img/key_silver.png");
    m_keys[3].texture = loadTexture("img/key_gold.png");

    m_portals[0].texture = m_portals[1].texture = loadTexture("img/portal.png");

    // messages

    gMessageManager().registerHandler<KeyLootedMessage>(&Display::onKeyLooted, this);
    gMessageManager().registerHandler<RunePressedMessage>(&Display::onRunePressed, this);
    gMessageManager().registerHandler<PortalDroppedMessage>(&Display::onPortalDropped, this);
    gMessageManager().registerHandler<SequenceFailedMessage>(&Display::onSequenceFailed, this);

    gMessageManager().registerHandler<NewLevelMessage>(&Display::onNewLevel, this);
    gMessageManager().registerHandler<ResetLevelMessage>(&Display::onResetLevel, this);
  }

  void Display::clear() {
    for (auto& rune : m_runes) {
      rune.active = false;
    }

    for (auto& key : m_keys) {
      key.active = false;
    }

    for (auto& portal : m_portals) {
      portal.active = true;
    }
  }

  static constexpr float Padding = 20.0f;

  static constexpr float KeySize = 30.0f;
  static constexpr float KeyTextureSize = 64.0f;
  static constexpr float KeySpace = 10.0f;

  static constexpr float RuneSize = 50.0f;
  static constexpr float RuneTextureSize = 64.0f;
  static constexpr float RuneSpace = 10.0f;

  static constexpr float PortalTextureSize = 64.0f;
  static constexpr float PortalSize = 40.0f;
  static constexpr float PortalSpace = 10.0f;

  static constexpr float Transparency = 0.125f;


  void Display::render(gf::RenderTarget& target) {
    gf::Coordinates coordinates(target);

    gf::Vector2f position(Padding, Padding);

    for (const auto& key : m_keys) {
      gf::Sprite sprite;
      sprite.setTexture(*key.texture);
      sprite.setPosition(position);
      sprite.setScale(KeySize / KeyTextureSize);
      sprite.setColor({ 1.0f, 1.0f, 1.0f, (key.active ? 1.0f : Transparency) });
      target.draw(sprite);

      position.x += KeySize + KeySpace;
    }

    position = coordinates.getAbsolutePoint({ Padding, RuneSize + RuneSpace + RuneSize + Padding }, gf::Anchor::BottomLeft);

    for (unsigned i = 0; i < 2; ++i) {
      for (unsigned j = 0; j < 2; ++j) {
        const auto& rune = m_runes[i * 2 + j];

        gf::Sprite sprite;
        sprite.setTexture(*rune.texture);
        sprite.setPosition(position + RuneSize * gf::Vector2f(i, j));
        sprite.setScale(RuneSize / RuneTextureSize);
        sprite.setColor({ 1.0f, 1.0f, 1.0f, (rune.active ? 1.0f : Transparency) });
        target.draw(sprite);
      }
    }

    position = coordinates.getAbsolutePoint({ PortalSize + PortalSpace + PortalSize + Padding, Padding }, gf::Anchor::TopRight);

    for (const auto& portal : m_portals) {
      gf::Sprite sprite;
      sprite.setTexture(*portal.texture);
      sprite.setPosition(position);
      sprite.setScale({ PortalSize / PortalTextureSize });
      sprite.setColor({ 1.0f, 1.0f, 1.0f, (portal.active ? 1.0f : Transparency) });
      target.draw(sprite);

      position.x += PortalSize + PortalSpace;
    }

  }


  gf::MessageStatus Display::onKeyLooted(gf::Id id, gf::Message *msg) {
    (void) id; // not used

    auto keyLooted = static_cast<KeyLootedMessage*>(msg);
    m_keys[keyLooted->number].active = true;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Display::onRunePressed(gf::Id id, gf::Message *msg) {
    (void) id; // not used

    auto runePressed = static_cast<RunePressedMessage*>(msg);
    m_runes[runePressed->number].active = true;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Display::onPortalDropped(gf::Id id, gf::Message *msg) {
    (void) id; // not used
    (void) msg; // not used

    if (m_portals[0].active) {
      m_portals[0].active = false;
    } else if (m_portals[1].active) {
      m_portals[1].active = false;
    }

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Display::onSequenceFailed(gf::Id id, gf::Message *msg) {
    (void) id; // not used
    (void) msg; // not used

    for (auto& rune : m_runes) {
      rune.active = false;
    }

    return gf::MessageStatus::Keep;
  }


  gf::MessageStatus Display::onResetLevel(gf::Id id, gf::Message *msg) {
    (void) id; // not used
    (void) msg; // not used

    clear();
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Display::onNewLevel(gf::Id id, gf::Message *msg) {
    (void) id; // not used
    (void) msg; // not used

    clear();
    return gf::MessageStatus::Keep;
  }



}
