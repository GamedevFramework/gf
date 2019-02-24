#ifndef HOME_TIMER_H
#define HOME_TIMER_H

#include <gf/Entity.h>
#include <gf/Font.h>

#include "Timekeeper.h"

namespace home {

  class Timer : public gf::Entity {
  public:
    Timer(const Timekeeper& timekeeper);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const Timekeeper& m_timekeeper;
    gf::Font &m_font;
  };

}

#endif // HOME_TIMER_H
