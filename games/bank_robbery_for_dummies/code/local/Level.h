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
#ifndef BRFD_LEVEL_H
#define BRFD_LEVEL_H

#include <memory>
#include <vector>

#include <gf/Entity.h>
#include <gf/PhysicsModel.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/TileLayer.h>

#include "Car.h"

namespace brfd {

  class Level : public gf::Entity {
  public:
    static constexpr unsigned StreetCount = 15;
    static constexpr unsigned Size = StreetCount * 3;

    Level(gf::ResourceManager& resources);

    void generateLevel(gf::Random& random, gf::PhysicsModel& physics);

    gf::Vector2f getStartingPosition() const {
      return m_home;
    }

    float getStartingAngle() const {
      return m_homeStartingAngle;
    }

    gf::Vector2f getPartnerPosition() const {
      return m_partner;
    }

    gf::Vector2f getClothingStorePosition() const {
      return m_clothingStore;
    }

    gf::Vector2f getGunStorePosition() const {
      return m_gunStore;
    }

    gf::Vector2f getRocketStorePosition() const {
      return m_rocketStore;
    }

    gf::Vector2f getBankPosition() const {
      return m_bank;
    }

    gf::Vector2f getHomePosition() const {
      return m_home;
    }

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::TileLayer m_layer;

    gf::Vector2f m_partner = { 0.0f, 0.0f };
    gf::Vector2f m_clothingStore = { 0.0f, 0.0f };
    gf::Vector2f m_gunStore = { 0.0f, 0.0f };
    gf::Vector2f m_rocketStore = { 0.0f, 0.0f };
    gf::Vector2f m_bank = { 0.0f, 0.0f };
    gf::Vector2f m_home = { 0.0f, 0.0f };
    float m_homeStartingAngle = 0.0f;

    gf::Texture& m_carTexture;
    gf::PolygonGeometry m_carGeometry;
    std::vector<StaticCar> m_cars;

    gf::PolygonGeometry m_buildingGeometry;
    std::vector<gf::PhysicsBody> m_buildings;

    gf::PolygonGeometry m_occupiedRoadGeometry;
    std::vector<gf::PhysicsBody> m_occupiedRoads;
  };

}

#endif // BRFD_LEVEL_H
