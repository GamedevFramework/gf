/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 * Copyright (C) 2017 Julien Bernard
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
#ifndef BRFD_CAR_H
#define BRFD_CAR_H

#include <gf/Entity.h>
#include <gf/MessageManager.h>
#include <gf/Model.h>
#include <gf/Rect.h>
#include <gf/PhysicsBody.h>
#include <gf/PhysicsModel.h>
#include <gf/Polygon.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

namespace brfd {

  gf::Polygon getCarGeometry();

  class HeroCar : public gf::Entity {
  public:
    HeroCar(gf::MessageManager& messages, gf::ResourceManager& resources, gf::PhysicsModel& physics);

    void startAt(gf::Vector2f position, float angle);

    void accelerate() {
      m_move = Move::Accelerate;
    }

    void brake() {
      m_move = Move::Brake;
    }

    void cruise() {
      m_move = Move::Cruise;
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

    virtual void update(gf::Time time) override;

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageManager& m_messages;
    gf::Texture& m_texture;

    enum class Move {
      Accelerate,
      Brake,
      Cruise,
    };

    enum class Turn {
      Left,
      Right,
      None,
    };

    Move m_move;
    Turn m_turn;

    float m_velocity;
    float m_angle;

    gf::PolygonGeometry m_geometry;
    gf::PhysicsBody m_body;
  };

  class StaticCar : public gf::Model {
  public:
    StaticCar(int number, gf::Vector2f position, float angle, const gf::PhysicsGeometry& geometry);

    const gf::RectF& getTextureRect() const {
      return m_textureRect;
    }

    const gf::PhysicsBody& getBody() const {
      return m_body;
    }

    gf::PhysicsBody& getBody() {
      return m_body;
    }

    virtual void update(gf::Time time) override;

  private:
    gf::RectF m_textureRect;
    gf::PhysicsBody m_body;
  };

}

#endif // BRFD_CAR_H
