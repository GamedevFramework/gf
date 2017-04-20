#ifndef BI_TREASURE_MANAGER_H
#define BI_TREASURE_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {
  class TreasureManager: public gf::Entity {
  public:
    TreasureManager();

    void addTreasure(const gf::Vector2f position);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::Vector2f getNearestTreasure() const;

    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);

  private:
    struct Treasure {
      gf::Vector2f position;
      gf::RectF rect;
      float size;
      float alpha;
      unsigned value;
      bool found;
    };

  private:
    std::vector<Treasure> m_treasures;
    gf::Vector2f m_heroPosition;
    const gf::Texture& m_texture;
  };
}

#endif // BI_TREASURE_MANAGER_H
