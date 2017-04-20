#ifndef BI_GOLD_H
#define BI_GOLD_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/Rect.h>
#include <gf/Texture.h>

#include "Messages.h"

namespace bi {
  class Gold: public gf::Entity {
  public:
    Gold();

    virtual void render(gf::RenderTarget& target) override;

    bool isGameOver() { return m_gameOver; }
    bool isWin() { return m_win; }

  private:
    gf::MessageStatus onGameOver(gf::Id type, gf::Message *msg);
    gf::MessageStatus onGoldLooted(gf::Id id, gf::Message *msg);

  private:
    unsigned m_score;
    bool m_gameOver;
    bool m_win;
    gf::Font &m_font;
    const gf::Texture &m_texture;
    gf::RectF m_rect;
  };
}

#endif // BI_GOLD_H
