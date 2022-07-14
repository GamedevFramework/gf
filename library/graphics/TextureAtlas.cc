/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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

#include <pugixml.hpp>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  TextureAtlas::TextureAtlas(const Path& filename)
  : m_texture(nullptr)
  {
    loadXml(filename);
  }

  TextureAtlas::TextureAtlas(const Path& filename, ResourceManager& resources)
  : m_texture(nullptr)
  {
    gf::Path absolute = resources.getAbsolutePath(filename);
    loadXml(absolute);
    Path parent = absolute.parent_path();
    Texture& texture = resources.getTexture(parent / getTexturePath());
    setTexture(texture);
  }

  void TextureAtlas::addSubTexture(std::string name, const RectI& rect) {
    m_rects.emplace(std::move(name), rect);
  }

  RectI TextureAtlas::getSubTexture(const std::string& name) const {
    auto it = m_rects.find(name);

    if (it == m_rects.end()) {
      return RectI::fromSize({ 1, 1 });
    }

    return it->second;
  }

  RectF TextureAtlas::getTextureRect(const std::string& name) const {
    if (m_texture == nullptr) {
      return RectF::fromSize({ 1.0f, 1.0f });
    }

    RectI rect = getSubTexture(name);
    return m_texture->computeTextureCoords(rect);
  }


  void TextureAtlas::loadXml(const Path& filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.string().c_str());

    if (!result) {
      Log::error("Could not load atlas texture '%s': %s\n", filename.string().c_str(), result.description());
      throw std::runtime_error("Could not load atlas texture");
    }

    pugi::xml_node root = doc.child("TextureAtlas");

    if (!root) {
      Log::error("Atlas is not in the right format '%s'\n", filename.string().c_str());
      throw std::runtime_error("Atlas is not in the right format");
    }

    Path texturePath = root.attribute("imagePath").as_string("");

    if (texturePath.empty()) {
      Log::error("Image path is not set in '%s'\n", filename.string().c_str());
      throw std::runtime_error("Image path is not set");
    }

    setTexturePath(texturePath);

    for (pugi::xml_node sub : root.children("SubTexture")) {
      assert(sub.attribute("name"));
      std::string name = sub.attribute("name").value();

      Vector2i position;
      assert(sub.attribute("x"));
      position.x = sub.attribute("x").as_int();
      assert(sub.attribute("y"));
      position.y = sub.attribute("y").as_int();

      Vector2i size;
      assert(sub.attribute("width"));
      size.width = sub.attribute("width").as_int();
      assert(sub.attribute("height"));
      size.height = sub.attribute("height").as_int();

      addSubTexture(std::move(name), RectI::fromPositionSize(position, size));
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
