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
#ifndef HOME_BACKPACK_H
#define HOME_BACKPACK_H

#include <gf/Entity.h>
#include <gf/Shader.h>
#include <gf/Texture.h>

#include "SupplyManager.h"

namespace home {

  class Backpack : public gf::Entity {
  public:
    Backpack();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    virtual void update(gf::Time time) override;

  private:
    gf::MessageStatus onSupplyHarvested(gf::Id id, gf::Message *msg);
    gf::MessageStatus onUnloadBackpack(gf::Id id, gf::Message *msg);

  private:
    struct PackedSupply {
      SupplyKind kind;
      int quantity;
    };

    std::vector<PackedSupply> m_supplies;
    int m_totalQuantity;
    gf::Texture& m_backpackTexture;

    int m_oxygen;
    gf::Texture& m_oxygenTexture;

    gf::Shader m_shader;

    bool m_dying;
  };

}

#endif // HOME_BACKPACK_H
