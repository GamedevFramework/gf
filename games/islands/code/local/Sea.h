#ifndef BI_SEA_H
#define BI_SEA_H

#include <gf/Array2D.h>
#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Random.h>
#include <gf/VertexArray.h>

#include "DecorationManager.h"
#include "TreasureManager.h"
#include "TurretManager.h"

namespace bi {

  class Sea : public gf::Entity {
  public:
    static constexpr unsigned Size = 1500;
    static constexpr unsigned Edge = 80;
    static constexpr float TileSize = 8.0f;
    static constexpr float WorldMin = Edge * TileSize;
    static constexpr float WorldMax = (Size - Edge) * TileSize;

    Sea();

    void generate(TreasureManager& treasures, DecorationManager& decorationsAbove, DecorationManager& decorationsBelow, TurretManager &turrets);

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onGoldLooted(gf::Id id, gf::Message *msg);

  private:
    struct Point {
      float elevation;
      gf::Color4f color;
    };

    gf::VertexArray m_vertices;
    gf::Array2D<Point> m_sea;
    gf::Vector2u m_hero;
  };

}

#endif // BI_SEA_H
