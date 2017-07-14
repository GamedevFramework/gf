#ifndef BI_HERO_H
#define BI_HERO_H

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"
#include "Steam.h"

namespace bi {
  class Hero: public gf::Entity {
  public:
    Hero(Steam& steam, const gf::Vector2f postion);

    void moveForward() {
      m_move = Move::Forward;
    }

    void moveBackward() {
      m_move = Move::Backward;
    }

    void moveStop() {
      m_move = Move::None;
    }

    void turnLeft() {
      m_turn = Turn::Left;
    }

    void turnRight() {
      m_turn = Turn::Right;
    }

    void turnNone() {
      m_turn = Turn::None;
    }

    void sendInitialPosition() const;
    bool isOnIsland() const;
    bool scanAvailable() const;

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStopScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onGameOver(gf::Id id, gf::Message *msg);

  private:
    enum class Move {
      Forward,
      Backward,
      None,
    };

    enum class Turn {
      Left,
      Right,
      None,
    };

    Steam& m_steam;

    Move m_move;
    Turn m_turn;

    gf::Vector2f m_position;
    float m_angle;

    const gf::Texture &m_texture;
    gf::RectF m_tricorn;
    gf::RectF m_boat;

    float m_timeElapsed;
    bool m_alternateStep;
    bool m_isOnIsland;
    bool m_isFrozen;

    bool m_isGameOver;
  };
}

#endif // BI_HERO_H
