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
#ifndef LUX_BULLETS_H
#define LUX_BULLETS_H

#include <gf/Entity.h>
#include <gf/MessageManager.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

#include "Properties.h"

namespace lux {

  class Bullets : public gf::Entity {
  public:
    Bullets(gf::MessageManager& messages, gf::ResourceManager &resources);

    void addBullet(Origin origin, ShipClass shipClass, gf::Vector2f position, gf::Vector2f velocity);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onLocation(gf::Id id, gf::Message *msg);
    gf::MessageStatus onShoot(gf::Id id, gf::Message *msg);

  private:
    struct Bullet {
      Origin origin;
      gf::Vector2f position;
      gf::Vector2f velocity;
      gf::Texture *texture;
      bool active;
    };

    std::vector<Bullet> m_bullets;

    gf::Texture& m_bulletBlueTexture;
    gf::Texture& m_bulletGreenTexture;
    gf::Texture& m_bulletYellowTexture;
    gf::Texture& m_bulletPurpleTexture;
  };


}

#endif // LUX_BULLETS_H
