#include "Victory.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Singletons.h"

namespace home {

  VictoryScreen::VictoryScreen()
  : m_texture(gResourceManager().getTexture("images/victory.jpg"))
  {

  }

  void VictoryScreen::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);

    gf::Vector2f size = coords.getRelativeSize({ 1.0f, 1.0f });
    gf::Vector2f scale = size / m_texture.getSize();

    gf::Sprite victory;
    victory.setTexture(m_texture);
    victory.setPosition(coords.getCenter());
    victory.setScale(std::min(scale.x, scale.y));
    victory.setAnchor(gf::Anchor::Center);
    target.draw(victory, states);
  }

}
