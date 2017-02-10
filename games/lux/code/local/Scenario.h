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
#ifndef LUX_SCENARIO_H
#define LUX_SCENARIO_H

#include <gf/MessageManager.h>
#include <gf/ResourceManager.h>

#include "Enemy.h"

namespace lux {

  class Scenario : public gf::Entity {
  public:
    Scenario(EnemyManager& enemies, gf::MessageManager& messages, gf::ResourceManager& resources);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

    gf::MessageStatus onScore(gf::Id id, gf::Message *msg);
    gf::MessageStatus onDead(gf::Id id, gf::Message *msg);

    struct WaveShip {
      ShipClass ship;
      gf::Vector2f position;
      gf::Vector2f velocity;
      float start;
    };

    struct Wave {
      float duration;
      std::vector<WaveShip> ships;
    };

  private:
    struct Score {
      unsigned idPlayer;
      unsigned long score;
    };

  private:
    EnemyManager& m_enemies;
    gf::MessageManager& m_messages;

    std::vector<Wave> m_waves;

    float m_elapsedTime;
    unsigned m_currentPlayer;
    std::size_t m_currentWave;
    std::size_t m_currentShip;

    gf::Font& m_font;
    unsigned long m_currentScore;
    bool m_win;

    bool m_updateScore;
    std::vector<Score> m_highScores;
  };

}

#endif // LUX_SCENARIO_H
