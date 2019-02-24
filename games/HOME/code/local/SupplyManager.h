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
#ifndef HOME_SUPPLY_MANAGER_H
#define HOME_SUPPLY_MANAGER_H

#include <vector>

#include <SFML/Audio.hpp>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

#include "Singletons.h"

namespace home {
  enum class SupplyKind {
    Energy,
    Metal,
    Oxygen,
  };

  struct Supply {
    Supply(SupplyKind kind, int quantity, gf::Vector2f position)
    : kind(kind)
    , initialQuantity(quantity)
    , currentQuantity(quantity)
    , position(position)
    {

    }

    SupplyKind kind;
    int initialQuantity;
    int currentQuantity;
    gf::Vector2f position;
  };

  class SupplyManager : public gf::Entity {
  public:
    SupplyManager();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    void addSupply(SupplyKind type, gf::Vector2f position);

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    std::vector<Supply> m_supplies;
    gf::Vector2f m_hero;
    gf::Texture &m_texture;
    sf::Sound m_oxygenMining;
    sf::Sound m_mining;
  };

}

#endif // HOME_SUPPLY_MANAGER_H
