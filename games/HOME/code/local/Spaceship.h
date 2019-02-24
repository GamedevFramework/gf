#ifndef HOME_SPACESHIP_H
#define HOME_SPACESHIP_H

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/Vector.h>

namespace home {

  class Spaceship : public gf::Entity {
  public:
    Spaceship();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    virtual void update(gf::Time time) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    int m_oxygen;
    int m_energy;
    int m_metal;
    gf::Vector2f m_hero;
  };

}

#endif // HOME_SPACESHIP_H
