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
#include "SupplyManager.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>

#include "Messages.h"
#include "Singletons.h"

namespace home {
  static constexpr int HarvestSpeed = 100'000;
  static constexpr int HarvestQuantityMin = 700'000;
  static constexpr int HarvestQuantityMax = 1'300'000;

  static constexpr int OxygenHarvestSpeed = 6'000;
  static constexpr int OxygenQuantityMin = 30'000;
  static constexpr int OxygenQuantityMax = 70'000;

  static constexpr float SupplyDistance = 64.0f;
  static constexpr float SupplyMiningVolume = 75.0f;

  static constexpr gf::Vector2i TileSize = { 128, 128 };

  SupplyManager::SupplyManager()
  : gf::Entity(10)
  , m_hero({0.0f, 0.0f})
  , m_texture(gResourceManager().getTexture("map/ResourceSet.png"))
  , m_oxygenMining(gResourceManager().getSound("sounds/o2_filling.ogg"))
  , m_mining(gResourceManager().getSound("sounds/mining.ogg"))
  {
    gMessageManager().registerHandler<HeroPosition>(&SupplyManager::onHeroPosition, this);

    m_oxygenMining.setLoop(true);
    m_oxygenMining.setVolume(SupplyMiningVolume);

    m_mining.setLoop(true);
    m_mining.setVolume(SupplyMiningVolume);
  }

  void SupplyManager::update(gf::Time time) {
    bool isMining = false;

    for (auto &supply: m_supplies) {
      if (gf::squareDistance(m_hero, supply.position) > gf::square(SupplyDistance)) {
        continue;
      }

      isMining = true;
      int quantity = 0;

      switch (supply.kind) {
        case SupplyKind::Oxygen:
          quantity = OxygenHarvestSpeed * time.asSeconds();

          if (m_oxygenMining.getStatus() != sf::SoundSource::Playing) {
            m_oxygenMining.play();
          }
          break;

        default:
          quantity = HarvestSpeed * time.asSeconds();

          if (m_mining.getStatus() != sf::SoundSource::Playing) {
            m_mining.play();
          }
          break;
      }

      if (quantity > supply.currentQuantity) {
        quantity = supply.currentQuantity;
      }

      supply.currentQuantity -= quantity;

      HarvestSupply message;
      message.kind = supply.kind;
      message.quantity = quantity;
      gMessageManager().sendMessage(&message);

      // Add the remainder to the supply
      supply.currentQuantity += message.quantity;
    }

    if (!isMining) {
      m_oxygenMining.stop();
      m_mining.stop();
    }

    // Remove empty supplies
    m_supplies.erase(std::remove_if(m_supplies.begin(), m_supplies.end(), [](const Supply &supply) {
      return supply.currentQuantity <= 0;
    }), m_supplies.end());
  }

  void SupplyManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    static constexpr gf::Vector2f AssetSize = { 128.0f, 128.0f };
    static constexpr gf::Vector2f TilesetSize = { 896.0f, 640.0f };
    static constexpr gf::Vector2f BarSize = { 128.0f, 10.0f } ;
    static constexpr float BarThickness = 2.0f;
    static constexpr gf::Vector2f BarOffset = { 64.0f, 80.0f };

    for (auto &supply: m_supplies) {
      gf::RectF textureRect = gf::RectF::fromPositionSize({ static_cast<int>(supply.kind) * AssetSize.width / TilesetSize.width, 0.0f }, AssetSize / TilesetSize);

      gf::Sprite sprite(m_texture, textureRect);
      sprite.setPosition(supply.position);
      sprite.setAnchor(gf::Anchor::Center);
      target.draw(sprite, states);

      // Life bar of supply
      float life = static_cast<float>(supply.currentQuantity) / static_cast<float>(supply.initialQuantity);

      if (life < 1.0f) {
        gf::Color4f color;

        switch (supply.kind) {
          case SupplyKind::Energy:
            color = gf::Color::Yellow;
            break;
          case SupplyKind::Metal:
            color = gf::Color::Gray(0.5f);
            break;
          case SupplyKind::Oxygen:
            color = gf::Color::Cyan;
            break;
        }

        gf::Vector2f position = supply.position;
        position -= BarOffset;

        gf::RectangleShape bar;
        bar.setSize(BarSize);
        bar.setColor(gf::Color::darker(color) * gf::Color::Opaque(0.3f));
        bar.setOutlineColor(gf::Color::darker(color));
        bar.setOutlineThickness(BarThickness);
        bar.setPosition(position);
        bar.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar, states);

        bar.setSize({BarSize.width * life, BarSize.height});
        bar.setColor(color);
        bar.setOutlineThickness(0.0f);
        bar.setPosition(position);
        bar.setAnchor(gf::Anchor::TopLeft);
        target.draw(bar, states);
      }
    }
  }

  void SupplyManager::addSupply(SupplyKind type, gf::Vector2f position) {
    switch (type) {
      case SupplyKind::Oxygen:
        m_supplies.push_back({ type, gRandom().computeUniformInteger(OxygenQuantityMin, OxygenQuantityMax), position });
        break;
      default:
        m_supplies.push_back({ type, gRandom().computeUniformInteger(HarvestQuantityMin, HarvestQuantityMax), position });
        break;
    }
  }

  gf::MessageStatus SupplyManager::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);
    m_hero = hero->position;
    return gf::MessageStatus::Keep;
  }

}
