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
#ifndef HOME_HERO_H
#define HOME_HERO_H

#include <gf/AnimatedSprite.h>
#include <gf/Animation.h>
#include <gf/Orientation.h>
#include <gf/Entity.h>

#include <SFML/Audio.hpp>

#include "Messages.h"

namespace home {
  class Hero : public gf::Entity {
    public :
      struct Dynamics {
        gf::Vector2f position;
        gf::Vector2f velocity;
      };

      Hero();

      void setDynamics(const Dynamics& dynamics) {
        m_position = dynamics.position;
        m_velocity = dynamics.velocity;
      }

      Dynamics getDynamics() const {
        return { m_position, m_velocity };
      }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
      virtual void update(gf::Time time) override;

  private:
      // Mouse click handler
      gf::MessageStatus onMouseClicked(gf::Id id, gf::Message *msg);
      gf::MessageStatus onHarvestSupply(gf::Id id, gf::Message *msg);
      gf::MessageStatus onGameOver(gf::Id id, gf::Message *msg);

    private:
      void loadAnimation(gf::Animation &animation, int line);
      void loadAnimationDeath();

    private :
      gf::Vector2f m_position; // Current position of the player
      gf::Vector2f m_positionClicked; // Where the mouse was last clicked

      gf::Vector2f m_velocity;

      sf::Sound m_jetSound;
      sf::Sound m_deathSound;

      gf::Orientation m_orientation;

      enum class Status {
        Waiting,
        Moving,
        Dying,
      };

      Status m_status;

      enum class Activity {
        Walking,
        Mining,
      };

      Activity m_activity;

//       bool m_moving;
//       bool m_overSupply;
//       bool m_shipFound;
//       bool m_dead;

      gf::Texture &m_moveAndPauseTexture;
      gf::Texture &m_deathTexture;

      gf::Animation *m_currentAnimation;

      gf::Animation m_moveWest;
      gf::Animation m_moveNorthWest;
      gf::Animation m_moveNorth;
      gf::Animation m_moveNorthEast;
      gf::Animation m_moveEast;
      gf::Animation m_moveSouthEast;
      gf::Animation m_moveSouthWest;

      gf::Animation m_pauseWest;
      gf::Animation m_pauseNorthWest;
      gf::Animation m_pauseNorth;
      gf::Animation m_pauseNorthEast;
      gf::Animation m_pauseEast;
      gf::Animation m_pauseSouthEast;
      gf::Animation m_pauseSouthWest;

      gf::Animation m_south;

      gf::Animation m_harvestNorthWest;
      gf::Animation m_harvestNorthEast;
      gf::Animation m_harvestSouthEast;
      gf::Animation m_harvestSouthWest;

      gf::Animation m_death;

      gf::Texture &m_crosshairTexture;

      gf::Texture &m_gameOverTexture;
      float m_gameOverAlpha;
  };
}

#endif // HOME_HERO_H
