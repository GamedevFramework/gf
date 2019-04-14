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
#include "ResourceManager.h"

namespace home {

  namespace {
    template<typename T>
    class ResourceLoader {
    public:
      std::unique_ptr<T> operator()(const gf::Path& filename) {
        auto ptr = std::make_unique<T>();
        bool loaded = ptr->loadFromFile(filename.string());
        return loaded ? std::move(ptr) : nullptr;
      }
    };

    class MusicLoader {
    public:
      std::unique_ptr<sf::Music> operator()(const gf::Path& filename) {
        auto ptr = std::make_unique<sf::Music>();
        bool loaded = ptr->openFromFile(filename.string());
        return loaded ? std::move(ptr) : nullptr;
      }
    };
  }

  ResourceManager::ResourceManager()
  : m_sounds(ResourceLoader<sf::SoundBuffer>())
  , m_musics(MusicLoader())
  {

  }

  sf::SoundBuffer& ResourceManager::getSound(const gf::Path &path) {
    return m_sounds.getResource(*this, path);
  }

  sf::Music& ResourceManager::getMusic(const gf::Path &path) {
    return m_musics.getResource(*this, path);
  }

}
