#ifndef HOME_VICTORY_H
#define HOME_VICTORY_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace home {

  class VictoryScreen : public gf::Entity {
  public:
    VictoryScreen();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const gf::Texture& m_texture;
  };

}

#endif // HOME_VICTORY_H
