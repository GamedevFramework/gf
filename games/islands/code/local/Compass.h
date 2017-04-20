#ifndef BI_COMPASS_H
#define BI_COMPASS_H

#include <deque>

#include <gf/Entity.h>
#include <gf/Texture.h>

#include "Messages.h"

namespace bi {
  class Compass: public gf::Entity {
  public:
    Compass();

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);
    gf::MessageStatus onStartScan(gf::Id id, gf::Message *msg);
    gf::MessageStatus onNearestTreasure(gf::Id id, gf::Message *msg);

  private:
    gf::Vector2f m_position;
    float m_angle;

    float m_timeElapsed;
    bool m_displayed;

    const gf::Texture &m_texture;
    gf::RectF m_compass;
    gf::RectF m_pointer;

    float m_timeUntilNextRadar;

    struct Radar {
      float radius;
      float lifetime;
    };

    std::deque<Radar> m_radars;
  };

}

#endif // BI_COMPASS_H
