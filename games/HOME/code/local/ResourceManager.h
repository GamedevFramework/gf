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
#ifndef HOME_RESOURCEMANAGER_H
#define HOME_RESOURCEMANAGER_H

#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

namespace home {

  class ResourceManager : public gf::ResourceManager {
  public:
    ResourceManager();
    sf::SoundBuffer& getSound(const gf::Path &path);
    sf::Music& getMusic(const gf::Path &path);

  private:
    gf::ResourceCache<sf::SoundBuffer> m_sounds;
    gf::ResourceCache<sf::Music> m_musics;
  };

}

#endif // HOME_RESOURCEMANAGER_H
