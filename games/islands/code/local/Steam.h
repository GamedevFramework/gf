#ifndef BI_STEAM_H
#define BI_STEAM_H

#include <deque>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

#include "Messages.h"

namespace bi {

  class Steam : public gf::Entity {
  public:
    Steam();

    void run() {
      m_running = true;
    }

    void stop() {
      m_running = false;
    }

    virtual void update(float dt) override;
    virtual void render(gf::RenderTarget& target) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    struct Cloud {
      gf::Vector2f position;
      float angle;
      float radius;
      float alpha;
    };

    std::deque<Cloud> m_clouds;
    gf::Vector2f m_heroPosition;
    float m_heroAngle;
    float m_timer;
    bool m_running;
  };

}

#endif // BI_STEAM_H
