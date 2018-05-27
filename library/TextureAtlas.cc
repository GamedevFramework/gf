/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/TextureAtlas.h>

#include <cassert>
#include <cstring>

#include <gf/Log.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

#include "vendor/pugixml/src/pugixml.hpp"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  bool TextureAtlas::loadFromFile(const Path& filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.string().c_str());

    if (!result) {
      Log::error("Could not load atlas texture '%s': %s\n", filename.string().c_str(), result.description());
      return false;
    }

    pugi::xml_node root = doc.child("TextureAtlas");

    if (!root) {
      Log::error("Atlas is not in the right format '%s'\n", filename.string().c_str());
      return false;
    }

    Path texturePath = root.attribute("imagePath").as_string("");

    if (texturePath.empty()) {
      Log::error("Image path is not set in '%s'\n", filename.string().c_str());
      return false;
    }

    setTexturePath(texturePath);

    for (pugi::xml_node sub : root.children("SubTexture")) {
      assert(sub.attribute("name"));
      std::string name = sub.attribute("name").value();

      RectU rect;

      assert(sub.attribute("x"));
      rect.left = sub.attribute("x").as_uint();
      assert(sub.attribute("y"));
      rect.top = sub.attribute("y").as_uint();
      assert(sub.attribute("width"));
      rect.width = sub.attribute("width").as_uint();
      assert(sub.attribute("height"));
      rect.height = sub.attribute("height").as_uint();

      addSubTexture(std::move(name), rect);
    }

    return true;
  }

  bool TextureAtlas::loadFromFile(const Path& filename, ResourceManager& resources) {
    gf::Path absolute = resources.getAbsolutePath(filename);
    bool loaded = loadFromFile(absolute);

    if (!loaded) {
      return false;
    }

    Path parent = absolute.parent_path();
    Texture& texture = resources.getTexture(parent / getTexturePath());
    setTexture(texture);
    return true;
  }

  void TextureAtlas::addSubTexture(std::string name, const RectU& rect) {
    m_rects.emplace(std::move(name), rect);
  }

  RectU TextureAtlas::getSubTexture(const std::string& name) const {
    auto it = m_rects.find(name);

    if (it == m_rects.end()) {
      return RectU(0, 0, 1, 1);
    }

    return it->second;
  }

  RectF TextureAtlas::getTextureRect(const std::string& name) const {
    if (m_texture == nullptr) {
      return RectF(0, 0, 1, 1);
    }

    RectU rect = getSubTexture(name);
    return m_texture->computeTextureCoords(rect);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
