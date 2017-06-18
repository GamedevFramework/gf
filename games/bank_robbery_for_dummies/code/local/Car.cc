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
#include "Car.h"

#include <gf/Math.h>
#include <gf/Matrix.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Transform.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include "Messages.h"

#include <iostream>

namespace brfd {

  static constexpr float CarWidth = 128;
  static constexpr float CarHeight = 64;
  static constexpr float CarHalfWidth = CarWidth / 2;
  static constexpr float CarHalfHeight = CarHeight / 2;
  static constexpr float X = 0.8;

  static constexpr float TilesetWidth = 512;
  static constexpr float TilesetHeight = 192;

  static const gf::Vector2f Geometry[] {
    { -     CarHalfWidth,   X * CarHalfHeight },
    { - X * CarHalfWidth,       CarHalfHeight },
    {   X * CarHalfWidth,       CarHalfHeight },
    {       CarHalfWidth,   X * CarHalfHeight },
    {       CarHalfWidth, - X * CarHalfHeight },
    {   X * CarHalfWidth, -     CarHalfHeight },
    { - X * CarHalfWidth, -     CarHalfHeight },
    { -     CarHalfWidth, - X * CarHalfHeight },
  };

  gf::Polygon getCarGeometry() {
    return gf::Polygon(Geometry);
  }


  HeroCar::HeroCar(gf::MessageManager& messages, gf::ResourceManager& resources, gf::PhysicsModel& physics)
  : gf::Entity(1)
  , m_messages(messages)
  , m_texture(resources.getTexture("cars.png"))
  , m_move(Move::Cruise)
  , m_turn(Turn::None)
  , m_velocity(0)
  , m_angle(0)
  , m_geometry(getCarGeometry())
  , m_body(m_geometry)
  {
    m_texture.setSmooth();
    m_body.setLinearDamping(0.2f);
    m_body.setDensity(10.0f);
    physics.addBody(m_body);
  }

  void HeroCar::startAt(gf::Vector2f position, float angle) {
    m_body.setPosition(position);
    m_body.setAngle(angle);
  }

  static constexpr float LinearAcceleration = 800.0f;
  static constexpr float AngularSpeed = 2.0f;
  static constexpr float VelocityMax = 1500.0f;
  static constexpr float VelocityMin = -500.0f;

  void HeroCar::update(gf::Time time) {
    float dt = time.asSeconds();
    gf::Vector2f position = m_body.getPosition();

    HeroPosition msg;
    msg.position = position;
    m_messages.sendMessage(&msg);

    float absoluteVelocity = gf::euclideanLength(m_body.getLinearVelocity());
    m_velocity =  m_velocity > 0 ? absoluteVelocity : -absoluteVelocity;

    switch (m_move) {
      case Move::Accelerate:
        m_velocity += LinearAcceleration * dt;
        break;

      case Move::Brake:
        m_velocity -= LinearAcceleration * dt;
        break;

      case Move::Cruise:
        break;
    }

    m_velocity = gf::clamp(m_velocity, VelocityMin, VelocityMax);

    float factor = 0.006f * std::abs(m_velocity) * std::exp(-0.002f * std::abs(m_velocity));
    m_angle = m_body.getAngle();

    switch (m_turn) {
      case Turn::Left:
        if (m_velocity >= 0) {
          m_angle -= dt * AngularSpeed * factor;
        } else {
          m_angle += dt * AngularSpeed * factor;
        }
        break;

      case Turn::Right:
        if (m_velocity >= 0) {
          m_angle += dt * AngularSpeed * factor;
        } else {
          m_angle -= dt * AngularSpeed * factor;
        }
        break;

      case Turn::None:
        break;
    }

    m_body.setLinearVelocity(gf::unit(m_angle) * m_velocity);
    m_body.setAngle(m_angle);
  }

  static constexpr gf::Vector2f TileScale(CarWidth / TilesetWidth, CarHeight / TilesetHeight);

  void HeroCar::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Sprite sprite(m_texture, gf::RectF(TileScale * gf::Vector2u(1, 1), TileScale));
    sprite.setAnchor(gf::Anchor::Center);
    sprite.setRotation(m_body.getAngle());
    sprite.setPosition(m_body.getPosition());

    target.draw(sprite, states);

//     m_body.render(target);
  }


  StaticCar::StaticCar(int number, gf::Vector2f position, float angle, const gf::PhysicsGeometry& geometry)
  : m_body(geometry, gf::PhysicsBody::Dynamic)
  {
    m_textureRect.left = (number % 4) * CarWidth / TilesetWidth;
    m_textureRect.top = (number / 4) * CarHeight / TilesetHeight;
    m_textureRect.width = CarWidth / TilesetWidth;
    m_textureRect.height = CarHeight / TilesetHeight;

    m_body.setPosition(position);
    m_body.setAngle(angle);
    m_body.setDensity(10.0f);
    m_body.setLinearDamping(5.0f);
  }

  void StaticCar::update(gf::Time time) {
    gf::unused(time);

    float angle = m_body.getAngle();
    gf::Vector2f velocity = m_body.getLinearVelocity();
    gf::Vector2f normal = gf::unit(angle + gf::Pi2);
    m_body.setLinearVelocity(velocity - gf::dot(velocity, normal) * normal);
  }

}
