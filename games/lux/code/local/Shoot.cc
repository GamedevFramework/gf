/*
 * Lux, a classical shoot 'em up
 * Copyright (C) 2016  Lux team
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
#include "Shoot.h"

#include <gf/Math.h>
#include <gf/Transform.h>

#include "Messages.h"

namespace lux {

  namespace {

    template<typename T, typename ... Args>
      std::unique_ptr<T> makeUnique(Args... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    class ConcreteShoot : public Shoot {
    public:
      ConcreteShoot(Origin origin, ShipClass shipClass)
      : m_origin(origin)
      , m_shipClass(shipClass)
      {

      }

      Origin getOrigin() const {
        return m_origin;
      }

      const ShipClass& getShipClass() const {
        return m_shipClass;
      }

    private:
      Origin m_origin;
      ShipClass m_shipClass;
    };

    class SingleShoot : public ConcreteShoot {
    public:
      SingleShoot(Origin origin, ShipClass shipClass)
      : ConcreteShoot(origin, shipClass)
      {

      }

      virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) override {
        (void) dt; // unused

        ShootMessage shoot;
        shoot.origin = getOrigin();
        shoot.position = position;
        shoot.velocity = direction;
        shoot.ship = getShipClass();

        messages.sendMessage(&shoot);
      }

    };

    static constexpr float ConeAngle = gf::Pi / 12;

    class ConeShoot : public ConcreteShoot {
    public:
      ConeShoot(Origin origin, ShipClass shipClass)
      : ConcreteShoot(origin, shipClass)
      {

      }

      virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) override {
        (void) dt;

        ShootMessage shoot;
        shoot.origin = getOrigin();
        shoot.position = position;
        shoot.ship = getShipClass();

        shoot.velocity = direction;
        messages.sendMessage(&shoot);

        gf::Matrix3f tr1 = gf::rotation(ConeAngle);
        shoot.velocity = gf::transform(tr1, direction);
        messages.sendMessage(&shoot);

        gf::Matrix3f tr2 = gf::rotation(-ConeAngle);
        shoot.velocity = gf::transform(tr2, direction);
        messages.sendMessage(&shoot);
      }

    };




    class ShootDecorator : public Shoot {
    public:
      ShootDecorator(std::unique_ptr<Shoot> decorated)
      : m_decorated(std::move(decorated))
      {
      }

    protected:
      Shoot& getDecorated() {
        return *m_decorated;
      }

    private:
      std::unique_ptr<Shoot> m_decorated;
    };


    class PeriodicShoot : public ShootDecorator {
    public:
      PeriodicShoot(std::unique_ptr<Shoot> decorated, float period)
      : ShootDecorator(std::move(decorated))
      , m_elapsedTime(0.0f)
      , m_period(period)
      {

      }

      virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) override {
        m_elapsedTime -= dt;

        if (m_elapsedTime <= 0) {
          getDecorated().shoot(dt, position, direction, messages);
          m_elapsedTime += m_period;
        }
      }

    private:
      float m_elapsedTime;
      float m_period;
    };

    class DelayedShoot : public ShootDecorator {
    public:
      DelayedShoot(std::unique_ptr<Shoot> decorated, float delay)
      : ShootDecorator(std::move(decorated))
      , m_elapsedTime(0.0f)
      , m_delay(delay)
      {

      }

      virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) override {
        m_elapsedTime += dt;

        if (m_elapsedTime >= m_delay) {
          getDecorated().shoot(dt, position, direction, messages);
        }
      }

    private:
      float m_elapsedTime;
      float m_delay;
    };

    class CountedShoot : public ShootDecorator {
    public:
      CountedShoot(std::unique_ptr<Shoot> decorated, int count)
      : ShootDecorator(std::move(decorated))
      , m_count(count)
      {

      }

      virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) override {
        if (m_count > 0) {
          getDecorated().shoot(dt, position, direction, messages);
          m_count--;
        }
      }

    private:
      int m_count;
    };

    class RegularShoot : public ShootDecorator {
    public:
      RegularShoot(std::unique_ptr<Shoot> decorated, int nbShoot, float shootInterval, float inactivePeriod)
      : ShootDecorator(std::move(decorated))
      , m_nbShoot(0)
      , m_shootPeriod(nbShoot * shootInterval)
      , m_shootInterval(shootInterval)
      , m_inactivePeriod(inactivePeriod)
      , m_elapsedTime(0.0f)
      , m_topFront(false)
      {

      }

      virtual void shoot(float dt, gf::Vector2f position, gf::Vector2f direction, gf::MessageManager& messages) override {
        m_elapsedTime += dt;

        // Update state of shoot

        if (m_topFront && m_elapsedTime >= m_shootPeriod) {
          m_elapsedTime -= m_shootPeriod;
          m_topFront = false;
        } else if (!m_topFront && m_elapsedTime >= m_inactivePeriod) {
          m_elapsedTime -= m_inactivePeriod;
          m_topFront = true;
          m_nbShoot = 0;
        }

        // Launch the shoot if needed

        if (m_topFront && m_elapsedTime >= (m_shootInterval * m_nbShoot)) {
          m_nbShoot++;
          getDecorated().shoot(dt, position, direction, messages);
        }
      }

    private:
      int m_nbShoot;
      float m_shootPeriod;
      float m_shootInterval;
      float m_inactivePeriod;
      float m_elapsedTime;
      bool m_topFront;
    };

  }

  Shoot::~Shoot() {

  }

  std::unique_ptr<Shoot> makeSimpleShoot(Origin origin, ShipClass shipClass, float delay) {
    return makeUnique<DelayedShoot>(
      makeUnique<CountedShoot>(
        makeUnique<SingleShoot>(origin, shipClass)
        , 1)
      , delay);
  }

  std::unique_ptr<Shoot> makeBurstShoot(Origin origin, ShipClass shipClass, float delay, float period, int count) {
    return makeUnique<DelayedShoot>(
      makeUnique<PeriodicShoot>(
        makeUnique<CountedShoot>(
          makeUnique<SingleShoot>(origin, shipClass)
          , count)
        , period)
      , delay);
  }

  std::unique_ptr<Shoot> makeConeShoot(Origin origin, ShipClass shipClass, float delay) {
    return makeUnique<DelayedShoot>(
      makeUnique<CountedShoot>(
        makeUnique<ConeShoot>(origin, shipClass)
        , 1)
      , delay);
  }

  std::unique_ptr<Shoot> makeContinuousSimpleShoot(Origin origin, ShipClass shipClass, float period) {
    return makeUnique<PeriodicShoot>(
      makeUnique<SingleShoot>(origin, shipClass)
      , period);
  }

  std::unique_ptr<Shoot> makeSimplePlayerShoot(Origin origin, ShipClass shipClass, int nbshoot, float shootInterval, float inactivePeriod) {
    return makeUnique<RegularShoot>(
      makeUnique<SingleShoot>(origin, shipClass)
      , nbshoot, shootInterval, inactivePeriod);
  }

}
