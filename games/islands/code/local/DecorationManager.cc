#include "DecorationManager.h"

#include <cassert>

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/SpriteBatch.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace bi {
  static constexpr float SpriteSize = 256.0f;

  DecorationManager::DecorationManager(Type type)
  : gf::Entity(type == Below ? 5 : 15)
  , m_type(type)
  , m_texture(gTextureAtlas().getTexture())
  {

  }

  void DecorationManager::addDecoration(const gf::Vector2f position) {
    Decoration decoration;
    decoration.position = position;
    decoration.angle = gRandom().computeUniformFloat(0.0f, gf::Pi * 2);

    if (m_type == Above) {
      static constexpr unsigned DecorationCount = 4;
      unsigned kind = gRandom().computeUniformInteger(1u, DecorationCount);

      switch (kind) {
      case 1:
        decoration.rect = gTextureAtlas().getTextureRect("palm_tree_1");
        decoration.size = 160.0f;
        break;

      case 2:
        decoration.rect = gTextureAtlas().getTextureRect("palm_tree_2");
        decoration.size = 160.0f;
        break;

      case 3:
        decoration.rect = gTextureAtlas().getTextureRect("palm_tree_3");
        decoration.size = 160.0f;
        break;

      case 4:
        decoration.rect = gTextureAtlas().getTextureRect("house");
        decoration.size = 160.0f;
        break;

      default:
        assert(false);
        break;
      }
    } else {
      static constexpr unsigned DecorationCount = 4;
      unsigned kind = gRandom().computeUniformInteger(1u, DecorationCount);

      switch (kind) {
      case 1:
        decoration.rect = gTextureAtlas().getTextureRect("rocks_1");
        decoration.size = 80.0f;
        break;

      case 2:
        decoration.rect = gTextureAtlas().getTextureRect("rocks_2");
        decoration.size = 80.0f;
        break;

      case 3:
        decoration.rect = gTextureAtlas().getTextureRect("campfire");
        decoration.size = 100.0f;
        break;

      case 4:
        decoration.rect = gTextureAtlas().getTextureRect("pipe");
        decoration.size = 40.0f;
        break;

      default:
        assert(false);
        break;
      }

    }

    m_decorations.push_back(decoration);
  }

  void DecorationManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::SpriteBatch batch(target);
    batch.begin();

    for (const auto& decoration: m_decorations) {
      gf::Sprite sprite;
      sprite.setTexture(m_texture);
      sprite.setTextureRect(decoration.rect);
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setPosition(decoration.position);
      sprite.setScale(decoration.size / SpriteSize);
      sprite.setRotation(decoration.angle);

      batch.draw(sprite, states);
    }

    batch.end();
  }
}
