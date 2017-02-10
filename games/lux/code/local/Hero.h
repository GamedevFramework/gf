/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016-2017  Lux team
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
#ifndef LUX_HERO_H
#define LUX_HERO_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>
#include <gf/WindowGeometryTracker.h>

#include "Ship.h"
#include "Shoot.h"

namespace lux {

  class HeroProperties : public gf::Entity {
  public:
    HeroProperties(gf::WindowGeometryTracker& tracker, gf::ResourceManager& resources);

    void setHealthPercent(float healthPercent) {
      m_healthPercent = healthPercent;
    }

    void addToScore(unsigned long score) {
      m_score += score;
    }

    unsigned long getScore() const {
      return m_score;
    }

    void resetScore() {
      m_score = 0;
    }

    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::WindowGeometryTracker& m_tracker;
    unsigned long m_score;
    float m_healthPercent;
    gf::Font& m_font;
  };

  class Hero : public Ship {
  public:
    static constexpr float Width = 60.0f;
    static constexpr float Height = 60.0f;
    static constexpr float ScaleX = Width / 256.0f;
    static constexpr float ScaleY = Height / 256.0f;

    static constexpr float Speed = 500.0f;
    static constexpr float Health = 100.0f;

    Hero(HeroProperties& prop, gf::MessageManager& messages, gf::ResourceManager& resources);

    void setPosition(gf::Vector2f position);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

    void upgradeWeapon();

    gf::MessageStatus onDead(gf::Id id, gf::Message *msg);
    gf::MessageStatus onRestartGame(gf::Id id, gf::Message *msg);
    gf::MessageStatus onWinGame(gf::Id id, gf::Message *msg);

  private:
    HeroProperties& m_prop;
    gf::MessageManager& m_messages;

    float m_elapsedTime;

    gf::Vector2f m_targetPos;
    gf::Vector2f m_position;
    bool m_inGame;
    int m_weaponLevel;
    std::unique_ptr<Shoot> m_shoot;

    gf::Texture& m_texture;
  };



}

#endif // LUX_HERO_H
