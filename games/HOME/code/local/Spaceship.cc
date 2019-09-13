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
#include "Spaceship.h"

#include <gf/Rect.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "Messages.h"

namespace home {

  namespace {

    constexpr gf::RectF HitBox = gf::RectF::fromPositionSize({5318.0f, 2350.0f}, {350.0f, 200.0f});

    constexpr int OxygenHarvestSpeed = 6'000;
    constexpr int OxygenQuantity = 1'000'000;
    constexpr int UnloadSpeed = 250'000;
    constexpr int ExpectedMetal = 8'000'000;
    constexpr int ExpectedEnergy = 9'000'000;
  }

  Spaceship::Spaceship()
  : gf::Entity(300)
  , m_oxygen(OxygenQuantity)
  , m_energy(0)
  , m_metal(0)
  {
    gMessageManager().registerHandler<HeroPosition>(&Spaceship::onHeroPosition, this);
  }

  void Spaceship::update(gf::Time time) {
    if (HitBox.contains(m_hero)) {
      // refill oxygen

      int oxygen = OxygenHarvestSpeed * time.asSeconds();

      if (oxygen > m_oxygen) {
        oxygen = m_oxygen;
      }

      m_oxygen -= oxygen;

      HarvestSupply message;
      message.kind = SupplyKind::Oxygen;
      message.quantity = oxygen;
      gMessageManager().sendMessage(&message);

      m_oxygen += message.quantity;

      // unload

      UnloadBackpack unload;
      unload.quantity = UnloadSpeed * time.asSeconds();
      gMessageManager().sendMessage(&unload);

      if (unload.quantity == 0) {
        return;
      }

      switch (unload.kind) {
        case SupplyKind::Metal:
          m_metal += unload.quantity;
          break;
        case SupplyKind::Energy:
          m_energy += unload.quantity;
          break;
        default:
          assert(false);
          break;
      }

      if (m_metal > ExpectedMetal) {
        m_metal = ExpectedMetal;
      }

      if (m_energy > ExpectedEnergy) {
        m_energy = ExpectedEnergy;
      }

      if (m_metal == ExpectedMetal && m_energy == ExpectedEnergy) {
        Victory message;
        gMessageManager().sendMessage(&message);
      }
    }
  }

  void Spaceship::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Vector2f position = HitBox.getPositionFromAnchor(gf::Anchor::TopCenter);
    position.x -= 40.0f;

    static constexpr gf::Vector2f Size = { 200.0f, 20.0f };
    static constexpr gf::Vector2f Offset = { 10.0f, 0.0f };
    static constexpr float Thickness = 5.0f;

    gf::Color4f metalColor = gf::Color::Gray(0.5f);

    gf::RectangleShape metal;
    metal.setSize(Size);
    metal.setPosition(position + Offset);
    metal.setColor(gf::Color::darker(metalColor) * gf::Color::Opaque(0.3f));
    metal.setOutlineColor(gf::Color::darker(metalColor));
    metal.setOutlineThickness(Thickness);
    metal.setAnchor(gf::Anchor::CenterLeft);
    target.draw(metal, states);

    float metalRatio = static_cast<float>(m_metal) / ExpectedMetal;
    gf::Vector2f metalSize = { metalRatio * Size.width, Size.height };
    metal.setSize(metalSize);
    metal.setColor(metalColor);
    metal.setOutlineThickness(0);
    metal.setAnchor(gf::Anchor::CenterLeft);
    target.draw(metal, states);

    gf::Color4f energyColor = gf::Color::Yellow;

    gf::RectangleShape energy;
    energy.setSize(Size);
    energy.setPosition(position - Offset);
    energy.setColor(gf::Color::darker(energyColor) * gf::Color::Opaque(0.3f));
    energy.setOutlineColor(gf::Color::darker(energyColor));
    energy.setOutlineThickness(Thickness);
    energy.setAnchor(gf::Anchor::CenterRight);
    target.draw(energy, states);

    float energyRatio = static_cast<float>(m_energy) / ExpectedEnergy;
    gf::Vector2f energySize = { energyRatio * Size.width, Size.height };
    energy.setSize(energySize);
    energy.setColor(energyColor);
    energy.setOutlineThickness(0);
    energy.setAnchor(gf::Anchor::CenterRight);
    target.draw(energy, states);
  }

  gf::MessageStatus Spaceship::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    m_hero = static_cast<HeroPosition*>(msg)->position;
    return gf::MessageStatus::Keep;
  }

}
