/*
 * Bygone Islands, the journey of a pirate in dangerous islands
 * Copyright (C) 2017  Hatunruna team
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
#include "TreasureManager.h"

#include <cassert>
#include <algorithm>

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/SpriteBatch.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float Limit = 60.0f;
  static constexpr float ViewLimit = 250.0f;
  static constexpr float SpriteSize = 256.0f;

  TreasureManager::TreasureManager()
  : gf::Entity(5)
  , m_heroPosition({ 0.0f, 0.0f })
  , m_texture(gTextureAtlas().getTexture())
  {
    gMessageManager().registerHandler<HeroPosition>(&TreasureManager::onHeroPosition, this);
    gMessageManager().registerHandler<StartScan>(&TreasureManager::onStartScan, this);
  }

  void TreasureManager::addTreasure(const gf::Vector2f position) {
    // Choose a random treasure
    static constexpr unsigned TreasureCount = 4;
    unsigned kind = gRandom().computeUniformInteger(1u, TreasureCount);

    Treasure treasure;
    treasure.position = position;
    treasure.alpha = 0.0f;
    treasure.found = false;

    switch (kind) {
    case 1:
      treasure.rect = gTextureAtlas().getTextureRect("bones");
      treasure.size = 40.0f;
      treasure.value = 0;
      break;

    case 2: // necklace
      treasure.rect = gTextureAtlas().getTextureRect("relics_1");
      treasure.size = 40.0f;
      treasure.value = 100;
      break;

    case 3: // gold
      treasure.rect = gTextureAtlas().getTextureRect("relics_2");
      treasure.size = 80.0f;
      treasure.value = 200;
      break;

    case 4:
      treasure.rect = gTextureAtlas().getTextureRect("chest");
      treasure.size = 80.0f;
      treasure.value = 500;
      break;

    default:
      assert(false);
      break;
    }

    m_treasures.push_back(treasure);
  }

  void TreasureManager::update(gf::Time time) {
    gf::unused(time);

    for (auto &treasure: m_treasures) {
      float distance = gf::euclideanDistance(treasure.position, m_heroPosition);
      treasure.alpha = gf::clamp((ViewLimit - distance) / (ViewLimit - Limit), 0.0f, 1.0f);

      if (distance <= Limit) {
        treasure.found = true;

        // Sent event
        GoldLooted message;
        message.value = treasure.value;
        gMessageManager().sendMessage(&message);

        addTreasure(message.next);
      }
    }

    auto trash = std::partition(m_treasures.begin(), m_treasures.end(), [](const Treasure& treasure) {
      return !(treasure.found);
    });

    m_treasures.erase(trash, m_treasures.end());
  }

  void TreasureManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::SpriteBatch batch(target);
    batch.begin();

    for (const auto &treasure: m_treasures) {
      gf::Sprite sprite;
      sprite.setTexture(m_texture);
      sprite.setTextureRect(treasure.rect);
      sprite.setPosition(treasure.position);
      sprite.setScale(treasure.size / SpriteSize);
      sprite.setColor(gf::Color::Opaque(treasure.alpha));
      sprite.setAnchor(gf::Anchor::Center);

      batch.draw(sprite, states);
    }

    batch.end();
  }

  gf::Vector2f TreasureManager::getNearestTreasure() const {
    auto treasure = std::min_element(std::begin(m_treasures), std::end(m_treasures), [this](const Treasure &a, const Treasure &b){
      float distanceA = gf::squareDistance(m_heroPosition, a.position);
      float distanceB = gf::squareDistance(m_heroPosition, b.position);

      return distanceA < distanceB;
    });

    return treasure->position;
  }

  gf::MessageStatus TreasureManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    gf::unused(id);

    auto hero = static_cast<HeroPosition*>(msg);
    m_heroPosition = hero->position;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus TreasureManager::onStartScan(gf::Id id, gf::Message *msg) {
    assert(id == StartScan::type);
    gf::unused(id, msg);

    NearestTreasure message;
    message.position = getNearestTreasure();
    gMessageManager().sendMessage(&message);

    return gf::MessageStatus::Keep;
  }
}
