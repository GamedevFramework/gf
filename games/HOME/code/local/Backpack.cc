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
#include "Backpack.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

#include "Messages.h"
#include "Singletons.h"

static const char *VertexShader = R"(
#version 100

attribute vec2 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoords;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform mat3 u_transform;

void main(void) {
  v_texCoords = a_texCoords;
  v_color = a_color;
  vec3 worldPosition = vec3(a_position, 1);
  vec3 normalizedPosition = worldPosition * u_transform;
  gl_Position = vec4(normalizedPosition.xy, 0, 1);
}
)";

static const char *FragmentShader = R"(
#version 100
precision mediump float;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform sampler2D u_texture;
uniform float u_outlineRadius;
uniform vec4 u_outlineColor;

const float PI_4 = 0.785398163397448309616;

bool isNearSprite() {
  for (int i = 0; i < 8; ++i) {
    float angle = float(i) * PI_4;
    vec4 color = texture2D(u_texture, v_texCoords + u_outlineRadius * vec2(cos(angle), sin(angle)));

    if (color.a != 0.0) {
      return true;
    }
  }

  return false;
}

void main(void) {
  vec4 color = texture2D(u_texture, v_texCoords);
  gl_FragColor = color * v_color;

  if (color.a < 0.2) {
    if (isNearSprite()) {
      gl_FragColor = u_outlineColor;
    }
  }

}
)";


namespace home {

  namespace {

    constexpr int LimitBackpack = 4'000'000;

    constexpr int MaxOxygen = 100'000;
    constexpr float OxygenLoss = 1000.0f;
    constexpr float OxygenWarningThreshold = 0.2f;

  }

  Backpack::Backpack()
  : m_totalQuantity(0)
  , m_backpackTexture(gResourceManager().getTexture("images/inventory_icon.png"))
  , m_oxygen(MaxOxygen)
  , m_oxygenTexture(gResourceManager().getTexture("images/oxygen_icon.png"))
  , m_shader(VertexShader, FragmentShader)
  , m_dying(false)
  {
    gMessageManager().registerHandler<HarvestSupply>(&Backpack::onSupplyHarvested, this);
    gMessageManager().registerHandler<UnloadBackpack>(&Backpack::onUnloadBackpack, this);

    m_backpackTexture.setSmooth();
    m_oxygenTexture.setSmooth();

    m_shader.setUniform("u_outlineRadius", 12.0f / 300.0f);
    m_shader.setUniform("u_outlineColor", gf::Color::Red * gf::Color::Opaque(0.5f));
  }

  void Backpack::update(gf::Time time) {
    if (m_oxygen > 0) {
      m_oxygen -= time.asSeconds() * OxygenLoss;
    } else {
      if (!m_dying) {
        m_oxygen = 0;
        GameOver message;
        gMessageManager().sendMessage(&message);
        m_dying = true;
      }
    }
  }

  void Backpack::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_dying) {
      return;
    }

    gf::Coordinates coords(target);

    static constexpr gf::Vector2f Size              = { 0.01f, 0.50f};
    static constexpr gf::Vector2f Padding           = {  1.0f, 0.05f };
    static constexpr gf::Vector2f Icon              = {  1.0f, 0.08f };
    static constexpr gf::Vector2f OxygenPosition    = { 0.92f, 0.90f };
    static constexpr gf::Vector2f SuppliesPosition  = { 0.95f, 0.90f };

    gf::Vector2f size = coords.getRelativeSize(Size);
    float padding = coords.getRelativeSize(Padding).height;
    float icon = coords.getRelativeSize(Icon).height;

    gf::RenderStates localStates = states;
    localStates.shader = &m_shader;

    // oxygen

    gf::Vector2f oxygenPosition = coords.getRelativePoint(OxygenPosition);

    gf::RectangleShape oxygen;
    oxygen.setSize(size);
    oxygen.setColor(gf::Color::Black * gf::Color::Opaque(0.3f));
    oxygen.setOutlineColor(gf::Color::darker(gf::Color::Cyan));
    oxygen.setOutlineThickness(size.width / 10);
    oxygen.setPosition(oxygenPosition);
    oxygen.setAnchor(gf::Anchor::BottomLeft);
    target.draw(oxygen, states);

    float oxygenRatio = static_cast<float>(m_oxygen) / MaxOxygen;

    oxygen.setSize({ size.width, oxygenRatio * size.height });
    oxygen.setColor(gf::Color::Cyan);
    oxygen.setAnchor(gf::Anchor::BottomLeft);
    target.draw(oxygen, states);

    gf::Sprite oxygenIcon(m_oxygenTexture);
    oxygenIcon.setColor(gf::Color::Cyan);
    oxygenIcon.setPosition({ oxygenPosition.x + size.width / 2, oxygenPosition.y - size.height - padding });
    oxygenIcon.setAnchor(gf::Anchor::Center);
    oxygenIcon.scale(icon / m_oxygenTexture.getSize().height);

    if (oxygenRatio > OxygenWarningThreshold) {
      target.draw(oxygenIcon, states);
    } else {
      target.draw(oxygenIcon, localStates);
    }

    // supplies

    gf::Vector2f supplyPosition = coords.getRelativePoint(SuppliesPosition);

    gf::RectangleShape supplies;
    supplies.setSize(size);
    supplies.setColor(gf::Color::Black * gf::Color::Opaque(0.3f));
    supplies.setOutlineColor(gf::Color::Black);
    supplies.setOutlineThickness(size.width / 10);
    supplies.setPosition(supplyPosition);
    supplies.setAnchor(gf::Anchor::BottomLeft);
    target.draw(supplies, states);

    gf::Sprite backpackIcon(m_backpackTexture);
    backpackIcon.setPosition({ supplyPosition.x + size.width / 2, supplyPosition.y - size.height - padding });
    backpackIcon.setAnchor(gf::Anchor::Center);
    backpackIcon.scale(icon / m_backpackTexture.getSize().height);

    if (m_totalQuantity < LimitBackpack) {
      target.draw(backpackIcon, states);
    } else {
      target.draw(backpackIcon, localStates);
    }

    for (auto& supply : m_supplies) {
      gf::Color4f supplyColor;

      switch (supply.kind) {
        case SupplyKind::Energy:
          supplyColor = gf::Color::Yellow;
          break;
        case SupplyKind::Metal:
          supplyColor = gf::Color::Gray(0.5f);
          break;
        default:
          assert(false);
          break;
      }

      float supplyRatio = static_cast<float>(supply.quantity) / LimitBackpack;
      gf::Vector2f supplySize = { size.width, supplyRatio * size.height };

      supplies.setColor(supplyColor);
      supplies.setOutlineColor(gf::Color::darker(supplyColor));
      supplies.setSize(supplySize);
      supplies.setPosition(supplyPosition);
      supplies.setAnchor(gf::Anchor::BottomLeft);
      target.draw(supplies, states);

      supplyPosition.y -= supplySize.height;
    }

  }

  gf::MessageStatus Backpack::onSupplyHarvested(gf::Id id, gf::Message *msg) {
    assert(id == HarvestSupply::type);
    auto message = static_cast<HarvestSupply*>(msg);

    if (message->kind == SupplyKind::Oxygen) {
      int quantity = message->quantity;

      if (m_oxygen + quantity > MaxOxygen) {
        quantity = MaxOxygen - m_oxygen;
      }

      assert(quantity >= 0);

      m_oxygen += quantity;
      message->quantity -= quantity;
      return gf::MessageStatus::Keep;
    }

    if (m_totalQuantity == LimitBackpack) {
      return gf::MessageStatus::Keep;
    }

    int quantity = message->quantity;

    if (m_totalQuantity + quantity > LimitBackpack) {
      quantity = LimitBackpack - m_totalQuantity;
    }

    assert(quantity > 0);

    if (m_supplies.empty() || m_supplies.back().kind != message->kind) {
      m_supplies.push_back({ message->kind, quantity });
    } else {
      m_supplies.back().quantity += quantity;
    }

    m_totalQuantity += quantity;
    message->quantity -= quantity;
    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Backpack::onUnloadBackpack(gf::Id id, gf::Message *msg) {
    assert(id == UnloadBackpack::type);
    UnloadBackpack *message = static_cast<UnloadBackpack*>(msg);

    if (m_supplies.empty()) {
      assert(m_totalQuantity == 0);
      message->quantity = 0;
      return gf::MessageStatus::Keep;
    }

    auto& supply = m_supplies.back();

    message->kind = supply.kind;
    int quantity = supply.quantity;

    if (quantity >= message->quantity) {
      quantity = message->quantity;
    } else {
      message->quantity = quantity;
    }

    supply.quantity -= quantity;
    m_totalQuantity -= quantity;

    if (supply.quantity == 0) {
      m_supplies.pop_back();
    }

    return gf::MessageStatus::Keep;
  }

}
