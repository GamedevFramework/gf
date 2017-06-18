/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef LOCAL_BOARD_H
#define LOCAL_BOARD_H

#include <gf/Direction.h>
#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/Random.h>
#include <gf/Rect.h>

#include "Grid.h"

class Board : public gf::Entity {
public:
  Board(gf::Font& font, gf::Random& random);

  bool move(gf::Direction dir);

  static gf::RectF getView();

  virtual void update(gf::Time time) override;

  virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

private:
  void addRandomTile();

private:
  gf::Font& m_font;
  gf::Random& m_random;
  Grid m_grid;
};

#endif // LOCAL_BOARD_H
