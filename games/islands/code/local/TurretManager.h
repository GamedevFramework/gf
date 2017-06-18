#ifndef BI_TURRET_MANAGER_H
#define BI_TURRET_MANAGER_H

#include <deque>
#include <vector>

#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {
  class TurretManager: public gf::Entity {
  public:
    TurretManager();

    void addTurret(gf::Vector2f position);

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Turret {
      struct Bullet {
        gf::Vector2f position;
        gf::Vector2f velocity;
        float timeElapsed;
        bool active;
      };

      gf::Vector2f position;
      std::deque<Bullet> bullets;
      float timeElapsed;
    };

  private:
    std::vector<Turret> m_turrets;
    gf::Vector2f m_heroPosition;
    const gf::Texture &m_texture;
    gf::RectF m_rect;
  };
}

#endif // BI_TURRET_MANAGER_H
