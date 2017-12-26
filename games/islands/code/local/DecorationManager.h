/*
 * Bygone Islands, the journey of a pirate in dangerous islands
 * Copyright (C) 2017  Hatunruna team
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
#ifndef BI_DECORATION_MANAGER_H
#define BI_DECORATION_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

namespace bi {
  class DecorationManager: public gf::Entity {
  public:
    enum Type {
      Above,
      Below,
    };

    DecorationManager(Type type);

    void addDecoration(const gf::Vector2f position);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    struct Decoration {
      gf::Vector2f position;
      gf::RectF rect;
      float size;
      float angle;
    };

  private:
    Type m_type;
    std::vector<Decoration> m_decorations;
    const gf::Texture& m_texture;
  };
}

#endif // BI_DECORATION_MANAGER_H
