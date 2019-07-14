/*
 * Krokodile
 * Copyright (C) 2018 Hatunruna team
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
#ifndef KKD_KREATURE_H
#define KKD_KREATURE_H

#include <gf/Activities.h>
#include <gf/Move.h>
#include <gf/RenderTarget.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

namespace kkd {

  class Kreature {
  public:
    enum Color : int {
      Azure   = 0,
      Green   = 1,
      Yellow  = 2,
      Red     = 3,
      Magenta = 4,
    };

    enum Species : int {
      Krokodile = 0,
      Elephant  = 1,
      Lion      = 2,
    };

    struct Part {
      Species species;
      Color color;
    };

    Kreature(gf::Vector2f position, float angle, gf::Vector2f target);

    void setLinearMove(gf::LinearMove move) {
      m_linearMove = move;
    }

    void setAngularMove(gf::AngularMove move) {
      m_angularMove = move;
    }

    bool isDead() const {
      return m_state == Death;
    }

    void updateAsPlayer(gf::Time time, bool sprint);
    void updateAsOther(gf::Time time);

    void render(gf::RenderTarget &target, gf::RenderStates states);

    float distanceTo(const Kreature& other);

    bool canMerge() const;
    std::unique_ptr<Kreature> mergeWith(Kreature& other);

    void resetActivities();

    bool isKrokodile() const;

    static std::unique_ptr<Kreature> randomKreature();
    static std::unique_ptr<Kreature> krokodile();

  private:
    void updateEnergy(gf::Time time, bool sprint);

  private:
    gf::Vector2f m_position;
    float m_angle;

    enum { Birth, Life, End, Death } m_state;
    gf::Time m_life;
    float m_energy;
    int m_merges;

    Part m_body = { };
    Part m_head = { };
    Part m_limbs = { };
    Part m_tail = { };

    gf::LinearMove m_linearMove;
    gf::AngularMove m_angularMove;

    gf::Time m_animationTime;
    float m_animationFactor;

    gf::RotateToActivity m_rotationActivity;
    gf::MoveToActivity m_moveActivity;
    gf::SequenceActivity m_sequence;

    float m_scale;
    gf::ValueActivity m_scaleActivity;
  };

}

#endif // KKD_KREATURE_H
