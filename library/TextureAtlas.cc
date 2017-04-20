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
#include <gf/TextureAtlas.h>

#include <cassert>
#include <cstring>

#include <gf/Log.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

#include "vendor/tinyxml2/tinyxml2.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  bool TextureAtlas::loadFromFile(const Path& filename) {
    tinyxml2::XMLDocument doc;
    int err = doc.LoadFile(filename.string().c_str());

    if (doc.Error() || err != tinyxml2::XML_SUCCESS) {
      Log::error("Could not load atlas texture: '%s'\n", filename.string().c_str());
      return false;
    }

    assert(err == tinyxml2::XML_SUCCESS);

    const tinyxml2::XMLElement *root = doc.RootElement();

    if (std::strcmp(root->Name(), "TextureAtlas") != 0) {
      return false;
    }

    Path texturePath = root->Attribute("imagePath");

    if (texturePath.empty()) {
      return false;
    }

    setTexturePath(texturePath);

    const tinyxml2::XMLElement *child = root->FirstChildElement();

    while (child != nullptr) {
      if (std::strcmp(child->Name(), "SubTexture") != 0) {
        m_rects.clear();
        return false;
      }

      std::string name = child->Attribute("name");
      assert(!name.empty());

      RectU rect;

      err = child->QueryUnsignedAttribute("x", &rect.left);
      assert(err == tinyxml2::XML_SUCCESS);

      err = child->QueryUnsignedAttribute("y", &rect.top);
      assert(err == tinyxml2::XML_SUCCESS);

      err = child->QueryUnsignedAttribute("width", &rect.width);
      assert(err == tinyxml2::XML_SUCCESS);

      err = child->QueryUnsignedAttribute("height", &rect.height);
      assert(err == tinyxml2::XML_SUCCESS);

      addSubTexture(name, rect);

      child = child->NextSiblingElement();
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
