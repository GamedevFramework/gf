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
#ifndef KKD_KREATURE_CONTAINER_H
#define KKD_KREATURE_CONTAINER_H

#include <memory>
#include <vector>

#include <gf/Entity.h>
#include <gf/Move.h>

#include "Kreature.h"

namespace kkd {
  class KreatureContainer : public gf::Entity {
  public:
    KreatureContainer();

    void setPlayerLinearMove(gf::LinearMove move);
    void setPlayerAngularMove(gf::AngularMove move);
    void setPlayerSprint(bool sprint);

    void swapKreatures();
    void mergeKreatures();

    void createKrokodile();

    void resetKreatures();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    Kreature& getPlayer();

//     void addFoodLevel(float consumption);

  private:
    std::vector< std::unique_ptr<Kreature> > m_kreatures;
    const gf::Texture& m_texture;
    bool m_isSprinting;
  };

}

#endif // KKD_KREATURE_CONTAINER_H
