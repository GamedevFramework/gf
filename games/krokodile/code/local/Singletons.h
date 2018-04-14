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
#ifndef KKD_SINGLETONS_H
#define KKD_SINGLETONS_H

#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/Singleton.h>
#include <gf/TextureAtlas.h>

namespace kkd {

  extern gf::Singleton<gf::ResourceManager> gResourceManager;
  extern gf::Singleton<gf::MessageManager> gMessageManager;
  extern gf::Singleton<gf::Random> gRandom;
  extern gf::Singleton<gf::TextureAtlas> gTextureAtlas;

}

#endif // KKD_SINGLETONS_H
