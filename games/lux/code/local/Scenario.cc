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
#include "Scenario.h"

#include <cstdarg>
#include <cstdio>

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Transform.h>
#include <gf/Unused.h>

#include "Messages.h"
#include "World.h"

namespace lux {

  static float ShipSpeed = WorldSize.height / 3;
  static float Top = WorldCenter.y - WorldSize.height / 2;
  static float HalfWidth = WorldSize.width / 2;

  static Scenario::Wave wave0 {
    5.0f,
    {
    }
  };

  // two at the same time
  static Scenario::Wave wave01 {
    5.0f,
    {
      { ShipClass::Bootes, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
    }
  };

  // alternate left/right
  static Scenario::Wave wave02 {
    6.0f,
    {
      { ShipClass::Bootes, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
      { ShipClass::Bootes, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 2.0f },
      { ShipClass::Bootes, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 3.0f },
    }
  };

  // stairs
  static Scenario::Wave wave03 {
    6.0f,
    {
      { ShipClass::Bootes, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, { -0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
      { ShipClass::Bootes, {  0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 2.0f },
      { ShipClass::Bootes, {  0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 3.0f },
    }
  };

  // cross
  static Scenario::Wave wave04 {
    4.0f,
    {
      { ShipClass::Bootes, { -0.8f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.8f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.0f },
    }
  };

  // Line
  static Scenario::Wave wave05 {
    4.0f,
    {
      { ShipClass::Bootes, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, { -0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Bootes, {  0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
    }
  };

  static Scenario::Wave wave10 {
    4.0f,
    {
      { ShipClass::Cygnus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
    }
  };

  static Scenario::Wave wave11 {
    4.0f,
    {
      { ShipClass::Cygnus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.5f },
      { ShipClass::Cygnus, {  0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.5f },
    }
  };

  static Scenario::Wave wave12 {
    5.0f,
    {
      { ShipClass::Cygnus, { -0.9f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.5f },
      { ShipClass::Cygnus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
    }

  };

  // diagonal
  static Scenario::Wave wave13 {
    4.0f,
    {
      { ShipClass::Cygnus, { -0.9f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, { -0.7f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
    }
  };

  // anti-diagonal
  static Scenario::Wave wave14 {
    4.0f,
    {
      { ShipClass::Cygnus, {  0.7f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.0f },
    }
  };

  // diagonal and anti-diagonal
  static Scenario::Wave wave15 {
    5.0f,
    {
      { ShipClass::Cygnus, { -0.9f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, { -0.7f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, {  0.7f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.5f },
      { ShipClass::Cygnus, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.5f },
    }
  };

  // V squad
  static Scenario::Wave wave16 {
    8.0f,
    {
      { ShipClass::Cygnus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Cygnus, { -0.3f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.3f },
      { ShipClass::Cygnus, {  0.3f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.3f },
      { ShipClass::Cygnus, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.6f },
      { ShipClass::Cygnus, {  0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.6f },
    }
  };

  // cross
  static Scenario::Wave wave20 {
    4.0f,
    {
      { ShipClass::Draco, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
    }
  };

  // cross
  static Scenario::Wave wave21 {
    4.0f,
    {
      { ShipClass::Draco, { -0.8f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Draco, {  0.8f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.0f },
    }
  };

  // alternate left/right
  static Scenario::Wave wave22 {
    6.0f,
    {
      { ShipClass::Draco, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Draco, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
      { ShipClass::Draco, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 2.0f },
      { ShipClass::Draco, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 3.0f },
    }
  };

  // anti-diagonal
  static Scenario::Wave wave23 {
    6.0f,
    {
      { ShipClass::Draco, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Draco, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.5f },
      { ShipClass::Draco, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 1.0f },
      { ShipClass::Draco, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 1.5f },
    }
  };

  // alternate left/right
  static Scenario::Wave wave24 {
    6.0f,
    {
      { ShipClass::Draco, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Draco, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
      { ShipClass::Draco, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 2.0f },
      { ShipClass::Draco, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 3.0f },
    }
  };

  // double stairs of death
  static Scenario::Wave wave25 {
    4.0f,
    {
      { ShipClass::Draco, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Draco, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Draco, { -0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.5f },
      { ShipClass::Draco, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.5f },
      { ShipClass::Draco, {  0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
      { ShipClass::Draco, {  0.8f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
    }
  };

  static Scenario::Wave wave30 {
    4.0f,
    {
      { ShipClass::Eridanus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
    }
  };

  // diagonal
  static Scenario::Wave wave31 {
    4.0f,
    {
      { ShipClass::Eridanus, { -0.9f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, { -0.7f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
    }
  };

  // cross
  static Scenario::Wave wave32 {
    4.0f,
    {
      { ShipClass::Eridanus, { -0.8f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.8f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.0f },
    }
  };

  // diagonal and anti-diagonal
  static Scenario::Wave wave33 {
    5.0f,
    {
      { ShipClass::Eridanus, { -0.9f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, { -0.7f * HalfWidth, Top - Enemy::Height }, { ShipSpeed, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.7f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.5f },
      { ShipClass::Eridanus, {  0.9f * HalfWidth, Top - Enemy::Height }, { -ShipSpeed, ShipSpeed }, 0.5f },
    }
  };

  // V squad
  static Scenario::Wave wave34 {
    8.0f,
    {
      { ShipClass::Eridanus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, { -0.3f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.3f },
      { ShipClass::Eridanus, {  0.3f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.3f },
      { ShipClass::Eridanus, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.6f },
      { ShipClass::Eridanus, {  0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.6f },
    }
  };

  // double stairs of death
  static Scenario::Wave wave35 {
    4.0f,
    {
      { ShipClass::Eridanus, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, { -0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.5f },
      { ShipClass::Eridanus, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.5f },
      { ShipClass::Eridanus, {  0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
      { ShipClass::Eridanus, {  0.8f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 1.0f },
    }
  };

  // super 7 squad
  static Scenario::Wave wave36 {
    4.0f,
    {
      { ShipClass::Eridanus, { -0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, { -0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, { -0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.0f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.2f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.4f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
      { ShipClass::Eridanus, {  0.6f * HalfWidth, Top - Enemy::Height }, { 0.0f, ShipSpeed }, 0.0f },
    }
  };


  Scenario::Scenario(EnemyManager& enemies, gf::MessageManager& messages, gf::ResourceManager& resources)
  : gf::Entity(2)
  , m_enemies(enemies)
  , m_messages(messages)
  , m_elapsedTime(0)
  , m_currentPlayer(1)
  , m_currentWave(0)
  , m_currentShip(0)
  , m_font(resources.getFont("SulphurPoint-Regular.otf"))
  , m_currentScore(0)
  , m_win(true)
  , m_updateScore(true)
  , m_highScores({{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}})
  {
    m_messages.registerHandler<ScoreMessage>(&Scenario::onScore, this);
    m_messages.registerHandler<DeadMessage>(&Scenario::onDead, this);

    m_waves.push_back(wave0);

    m_waves.push_back(wave01);
    m_waves.push_back(wave01);
    m_waves.push_back(wave02);
    m_waves.push_back(wave02);
    m_waves.push_back(wave03);
    m_waves.push_back(wave04);
    m_waves.push_back(wave05);

    m_waves.push_back(wave10);
    m_waves.push_back(wave11);
    m_waves.push_back(wave12);
    m_waves.push_back(wave13);
    m_waves.push_back(wave14);
    m_waves.push_back(wave15);
    m_waves.push_back(wave16);

    m_waves.push_back(wave20);
    m_waves.push_back(wave21);
    m_waves.push_back(wave22);
    m_waves.push_back(wave23);
    m_waves.push_back(wave24);
    m_waves.push_back(wave25);

    m_waves.push_back(wave30);
    m_waves.push_back(wave31);
    m_waves.push_back(wave32);
    m_waves.push_back(wave01);
    m_waves.push_back(wave33);
    m_waves.push_back(wave02);
    m_waves.push_back(wave34);
    m_waves.push_back(wave12);
    m_waves.push_back(wave35);
    m_waves.push_back(wave36);

    m_waves.push_back(wave0);
  }

  static constexpr float MenuTime = 15.0f;
  static constexpr unsigned long WinBonusScore = 5000;

  void Scenario::update(gf::Time time) {
    float dt = time.asSeconds();
    m_elapsedTime += dt;


    if (m_currentWave == m_waves.size()) { // End of game
      if (m_updateScore) {
        // If the player win, add the bonus score
        if (m_win) {
          WinGameMessage win;
          win.bonusScore = WinBonusScore;
          m_messages.sendMessage(&win);
          m_currentScore += WinBonusScore;
        }

        Score finalScore = { m_currentPlayer, m_currentScore };

        // Add the new high score
        m_highScores.push_back(finalScore);

        // Remove the lower score
        std::sort(m_highScores.begin(), m_highScores.end(), [](Score a, Score b) {
          return a.score > b.score;
        });

        m_highScores.erase(m_highScores.end() - 1);
        m_updateScore = false;
      }

      if (m_elapsedTime > MenuTime) {
        m_elapsedTime = 0.0f;
        m_currentWave = 0;
        m_currentShip = 0;
        m_win = true;
        m_currentPlayer++;

        RestartGameMessage restart;
        m_messages.sendMessage(&restart);
        m_updateScore = true;
      }

      return;
    }

    Wave& wave = m_waves[m_currentWave];

    if (m_elapsedTime > wave.duration) {
      m_elapsedTime -= wave.duration;
      m_currentWave++;
      m_currentShip = 0;
      return;
    }

    while (m_currentShip < wave.ships.size()) {
      WaveShip& waveShip = wave.ships[m_currentShip];

      if (waveShip.start < m_elapsedTime) {
        m_enemies.addEnemy(waveShip.ship, waveShip.position, waveShip.velocity);
        m_currentShip++;
      } else {
        break;
      }
    }

  }

  static std::string stringFormat(const char *fmt, ...) {
    va_list args1;
    va_start(args1, fmt);

    va_list args2;
    va_copy(args2, args1);

    int sz = std::vsnprintf(nullptr, 0, fmt, args1);
    va_end(args1);

    std::string result(sz + 1, '?');
    std::vsnprintf(&result[0], result.size(), fmt, args2);
    va_end(args2);

    result.pop_back(); // to remove the trailing '\0'
    return result;
  }

  static constexpr float Margin = 50.0f;

  void Scenario::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_currentWave != m_waves.size()) {
      return;
    }

    gf::Coordinates coordinates(target);

    std::string report;

    if (m_win) {
      report += stringFormat("Congratulations player #%u! You have finished the game!\n", m_currentPlayer);
      report += stringFormat("You get a bonus of %lu points\n", WinBonusScore);
    } else {
      report += stringFormat("Sorry player #%u, you lost!\n", m_currentPlayer);
    }

    int countdown = static_cast<int>(std::lround(MenuTime - m_elapsedTime));
    report += stringFormat("Your score is: %lu\nA new game starts in %i", m_currentScore, countdown);

    gf::Text text;
    text.setString(report);
    text.setCharacterSize(30);
    text.setColor(gf::Color::White);
    text.setParagraphWidth(800); // large enough
    text.setAlignment(gf::Alignment::Center);
    text.setFont(m_font);
    text.setAnchor(gf::Anchor::Center);

    auto bounds = text.getLocalBounds();
    gf::Vector2f position = coordinates.getRelativePoint({ 0.5f, 0.3f });
    position.y -= bounds.height / 2;

    text.setPosition(position);
    target.draw(text, states);

    // Display the high score
    report = "High scores:\n";

    for (std::size_t i = 0; i < m_highScores.size(); ++i) {
      report += stringFormat("%zu. player #%u: %lu \n", (i+1), m_highScores[i].idPlayer, m_highScores[i].score);
    }

    text.setString(report);
    text.setAlignment(gf::Alignment::None);
    text.setAnchor(gf::Anchor::TopCenter);

    position.y += bounds.height + Margin;

    text.setPosition(position);
    target.draw(text, states);
  }

  gf::MessageStatus Scenario::onScore(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    auto score = static_cast<ScoreMessage*>(msg);

    m_currentScore = score->score;

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Scenario::onDead(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    auto dead = static_cast<DeadMessage*>(msg);

    if (dead->origin != Origin::Hero) {
      return gf::MessageStatus::Keep;
    }

    // Jump to end
    m_currentWave = m_waves.size() - 1;
    m_elapsedTime = 0.0f;
    m_win = false;

    return gf::MessageStatus::Keep;
  }

}
