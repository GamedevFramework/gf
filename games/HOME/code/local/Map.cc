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
#include "Map.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Tmx.h>
#include <gf/TmxOps.h>

#include "Singletons.h"

namespace home {

  namespace {

    class LayersMaker : public gf::TmxVisitor {
    public:
      LayersMaker(std::vector<gf::TileLayer>& layers, std::vector<gf::Sprite>& sprites, SupplyManager& supplies)
      : m_layers(layers)
      , m_sprites(sprites)
      , m_supplies(supplies)
      {

      }

      virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
        if (!layer.visible) {
          return;
        }

        gf::Log::info("Parsing layer '%s'\n", layer.name.c_str());
        assert(map.orientation == gf::TileOrientation::Staggered);

        m_layers.push_back(gf::makeTileLayer(map, layer, gResourceManager()));
      }

      virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
        gf::Log::info("Parsing object layer '%s'\n", layer.name.c_str());

        bool isResources = (layer.name == "Resources");

        for (auto& object : layer.objects) {
          if (object->kind != gf::TmxObject::Tile) {
            continue;
          }

          auto tile = static_cast<gf::TmxTileObject *>(object.get());

          auto tileset = map.getTileSetFromGID(tile->gid);
          assert(tileset);
          assert(tileset->image);

          // compute texture rect
          auto lid = tile->gid - tileset->firstGid;
          auto subTexture = tileset->getSubTexture(lid, tileset->image->size);

          if (isResources) {
            gf::Vector2f position = tile->position;
            position.x += subTexture.getWidth() / 2;
            position.y -= subTexture.getHeight() / 2;

            switch (lid) {
              case 0:
                m_supplies.addSupply(SupplyKind::Energy, position);
                break;
              case 1:
                m_supplies.addSupply(SupplyKind::Metal, position);
                break;
              case 2:
                m_supplies.addSupply(SupplyKind::Oxygen, position);
                break;
              default:
                assert(false);
                break;
            }
          } else {
            const gf::Texture& texture = gResourceManager().getTexture(tileset->image->source);
            gf::RectF textureRect = texture.computeTextureCoords(subTexture);

            gf::Sprite sprite(texture, textureRect);
            sprite.setPosition(tile->position);
            sprite.setRotation(gf::degreesToRadians(tile->rotation));
            sprite.setAnchor(gf::Anchor::BottomLeft); // see http://docs.mapeditor.org/en/stable/reference/tmx-map-format/#object

            m_sprites.push_back(std::move(sprite));
          }
        }
      }

    private:
      std::vector<gf::TileLayer>& m_layers;
      std::vector<gf::Sprite>& m_sprites;
      SupplyManager& m_supplies;
    };
  }

  MapGraphicsData::MapGraphicsData(const gf::TmxLayers& layers, SupplyManager& supplies)
  {
    LayersMaker maker(tiles, sprites, supplies);
    layers.visitLayers(maker);
  }

  Map::Map(Type type, MapGraphicsData& data)
  : gf::Entity(type == Below ? 0 : 200)
  , m_type(type)
  , m_data(data)
  {
     gMessageManager().registerHandler<HeroPosition>(&Map::onHeroPosition, this);
  }

  void Map::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    float limit = m_hero.y + 70;

    if (m_type == Below) {
      for (auto& layer : m_data.tiles) {
        target.draw(layer, states);
      }

      for (auto& sprite : m_data.sprites) {
        if (sprite.getPosition().y < limit) {
          target.draw(sprite, states);
        }
      }
    } else {
      for (auto& sprite : m_data.sprites) {
        if (sprite.getPosition().y >= limit) {
          target.draw(sprite, states);
        }
      }
    }
  }

  gf::MessageStatus Map::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    auto hero = static_cast<HeroPosition*>(msg);
    m_hero = hero->position;
    return gf::MessageStatus::Keep;
  }

}
