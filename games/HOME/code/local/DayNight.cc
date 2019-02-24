#include "DayNight.h"

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "Messages.h"
#include "Singletons.h"

namespace home {

  DayNight::DayNight(const Timekeeper& timekeeper)
  : gf::Entity(400)
  , m_timekeeper(timekeeper)
  , m_texture(gResourceManager().getTexture("images/fov.png"))
  , m_position({ 0.0f, 0.0f })
  , m_color(gf::Color::Opaque(0.0f))
  {
    gMessageManager().registerHandler<HeroPosition>(&DayNight::onHeroPosition, this);
  }

  void DayNight::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_timekeeper.isDusk()) {
      m_color = gf::Color::Opaque(m_timekeeper.getDuskRatio());
    }

    if (m_timekeeper.isDawn()) {
      m_color = gf::Color::Opaque(1.0f - m_timekeeper.getDawnRatio());
    }

    gf::Sprite fov;
    fov.setTexture(m_texture);
    fov.setAnchor(gf::Anchor::Center);
    fov.setPosition(m_position);
    fov.setColor(m_color);
    target.draw(fov, states);
  }

  gf::MessageStatus DayNight::onHeroPosition(gf::Id id, gf::Message *msg) {
    assert(id == HeroPosition::type);
    m_position = static_cast<HeroPosition*>(msg)->position;
    return gf::MessageStatus::Keep;
  }

}
