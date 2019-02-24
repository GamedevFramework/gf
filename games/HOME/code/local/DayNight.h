#ifndef HOME_DAY_NIGHT_H
#define HOME_DAY_NIGHT_H

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

#include "Timekeeper.h"

namespace home {

  class DayNight : public gf::Entity {
  public:
    DayNight(const Timekeeper& timekeeper);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    const Timekeeper& m_timekeeper;
    gf::Texture &m_texture;
    gf::Vector2f m_position;
    gf::Color4f m_color;
  };

}

#endif // HOME_DAY_NIGHT_H
