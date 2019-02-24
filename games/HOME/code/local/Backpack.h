#ifndef HOME_BACKPACK_H
#define HOME_BACKPACK_H

#include <gf/Entity.h>
#include <gf/Shader.h>
#include <gf/Texture.h>

#include "SupplyManager.h"

namespace home {

  class Backpack : public gf::Entity {
  public:
    Backpack();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    virtual void update(gf::Time time) override;

  private:
    gf::MessageStatus onSupplyHarvested(gf::Id id, gf::Message *msg);
    gf::MessageStatus onUnloadBackpack(gf::Id id, gf::Message *msg);

  private:
    struct PackedSupply {
      SupplyKind kind;
      int quantity;
    };

    std::vector<PackedSupply> m_supplies;
    int m_totalQuantity;
    gf::Texture& m_backpackTexture;

    int m_oxygen;
    gf::Texture& m_oxygenTexture;

    gf::Shader m_shader;

    bool m_dying;
  };

}

#endif // HOME_BACKPACK_H
