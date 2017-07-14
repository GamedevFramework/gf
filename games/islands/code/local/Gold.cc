#include "Gold.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Unused.h>

#include "Singletons.h"

namespace bi {
  static constexpr float SpriteSize = 256.0f;

  Gold::Gold()
  : gf::Entity(10)
  , m_score(0)
  , m_gameOver(false)
  , m_win(false)
  , m_font(gResourceManager().getFont("blkchcry.ttf"))
  , m_texture(gTextureAtlas().getTexture())
  , m_rect(gTextureAtlas().getTextureRect("coin"))
  {
    // Event
    gMessageManager().registerHandler<GameOver>(&Gold::onGameOver, this);
    gMessageManager().registerHandler<GoldLooted>(&Gold::onGoldLooted, this);
  }

  void Gold::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);

    if (m_gameOver || m_win) {
      // Compute the margin 50% of screen
      gf::Vector2f center = coords.getCenter();

      // Compute the font size 10% of height of screen
      unsigned fontSize = coords.getRelativeCharacterSize(0.10f);

      if (m_win) {
        gf::Text text("You win!\nYour score is : " + std::to_string(m_score), m_font, fontSize);
        text.setColor(gf::Color::White);
        text.setOutlineColor(gf::Color::Black);
        text.setAnchor(gf::Anchor::Center);
        text.setPosition(center);

        target.draw(text, states);
      } else {
        gf::Text text("Game Over!\nYour score is : " + std::to_string(m_score), m_font, fontSize);
        text.setColor(gf::Color::White);
        text.setOutlineColor(gf::Color::Black);
        text.setAnchor(gf::Anchor::Center);
        text.setPosition(center);

        target.draw(text, states);
      }
    } else {
      // Compute the margin 5% of screen
      gf::Vector2f position = coords.getRelativePoint({ 0.10f, 0.05f });

      // Compute the font size 10% of height of screen
      unsigned fontSize = coords.getRelativeCharacterSize(0.07f);

      gf::Text text(std::to_string(m_score) + "/5000", m_font, fontSize);
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setAnchor(gf::Anchor::TopLeft);
      text.setPosition(position);

      target.draw(text, states);

      // Draw the coins
      position.x -= 10.0f;

      gf::Sprite sprite;
      sprite.setTexture(m_texture);
      sprite.setTextureRect(m_rect);
      sprite.setPosition(position);
      sprite.setScale(fontSize / SpriteSize);
      sprite.setAnchor(gf::Anchor::TopRight);

      target.draw(sprite, states);
    }
  }

  gf::MessageStatus Gold::onGoldLooted(gf::Id id, gf::Message *msg) {
    assert(id == GoldLooted::type);
    gf::unused(id);

    auto gold = static_cast<GoldLooted*>(msg);
    m_score += gold->value;

    if (m_score > 5000) {
      m_win = true;
    }

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus Gold::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOver::type);
    gf::unused(id, msg);
    m_gameOver = true;
    return gf::MessageStatus::Keep;
  }

}
