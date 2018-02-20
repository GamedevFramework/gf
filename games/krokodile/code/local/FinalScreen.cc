/*
 * Krokodile
 * Copyright (C) 2018 Hatunruna team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "FinalScreen.h"

#include <gf/Anchor.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>
#include <gf/Unused.h>

#include "Singletons.h"

namespace kkd {

  FinalScreen::FinalScreen()
  : gf::Entity(10)
  , m_font(gResourceManager().getFont("Gravity-Regular.otf"))
  , m_merges(0)
  {
    // register message handler
    gMessageManager().registerHandler<EndOfGame>(&FinalScreen::onEndOfGame, this);
    gMessageManager().registerHandler<MergeRealized>(&FinalScreen::onMergeRealized, this);
  }

  void FinalScreen::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);
    unsigned characterSize = coords.getRelativeCharacterSize(0.1f);

    gf::Text scoreTxt("Generations : " + std::to_string(m_merges) + "\nTime : " + gf::niceNum(m_time.asSeconds(), 0.1f) + " seconds\nPress 'Space' to restart", m_font, characterSize);
    scoreTxt.setOutlineColor(gf::Color::Black);
    scoreTxt.setOutlineThickness(2.0f);
    scoreTxt.setColor(gf::Color::White);
    scoreTxt.setPosition(coords.getCenter());
    scoreTxt.setParagraphWidth(1000.0f);
    scoreTxt.setAlignment(gf::Alignment::Center);
    scoreTxt.setAnchor(gf::Anchor::Center);

    target.draw(scoreTxt, states);
  }

  void FinalScreen::reset() {
    m_merges = 0;
    m_clock.restart();
  }

  gf::MessageStatus FinalScreen::onEndOfGame(gf::Id id, gf::Message *msg) {
    assert(id == EndOfGame::type);
    gf::unused(msg);

    m_time = m_clock.getElapsedTime();

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus FinalScreen::onMergeRealized(gf::Id id, gf::Message *msg) {
    assert(id == MergeRealized::type);
    gf::unused(msg);

    ++m_merges;

    return gf::MessageStatus::Keep;
  }

}
