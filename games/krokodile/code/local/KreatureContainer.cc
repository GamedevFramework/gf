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
#include "KreatureContainer.h"

#include "Messages.h"
#include "Singletons.h"

namespace kkd {

  namespace {
    constexpr int MaximumPopulation = 25;
    constexpr int MinimumPopulation = 15;
  }

  KreatureContainer::KreatureContainer()
  : m_texture(gTextureAtlas().getTexture())
  {
    resetKreatures();
  }

  void KreatureContainer::setPlayerLinearMove(gf::LinearMove move) {
    getPlayer().setLinearMove(move);
  }

  void KreatureContainer::setPlayerAngularMove(gf::AngularMove move) {
    getPlayer().setAngularMove(move);
  }

  void KreatureContainer::setPlayerSprint(bool sprint) {
    m_isSprinting = sprint;
  }

  void KreatureContainer::swapKreatures() {
    if (m_kreatures.size() <= 1) {
      return;
    }

    auto it = std::min_element(m_kreatures.begin() + 1, m_kreatures.end(), [this](const auto& lhs, const auto& rhs) {
      return this->getPlayer().distanceTo(*lhs) < this->getPlayer().distanceTo(*rhs);
    });

    // reset the activity for the current kreature
    getPlayer().resetActivities();

    // swap with a new one
    std::swap(m_kreatures.front(), *it);

    if (getPlayer().isKrokodile()) {
      EndOfGame message;
      gMessageManager().sendMessage(&message);
    }
  }

  void KreatureContainer::mergeKreatures() {
    constexpr float LimitLengthFusion = 150.0f;

    if (m_kreatures.size() <= 1) {
      return;
    }

    if (!getPlayer().canMerge()) {
      return;
    }

    auto it = std::min_element(m_kreatures.begin() + 1, m_kreatures.end(), [this](const auto& lhs, const auto& rhs) {
      return this->getPlayer().distanceTo(*lhs) < this->getPlayer().distanceTo(*rhs);
    });

    Kreature& other = **it;

    if (getPlayer().distanceTo(other) > LimitLengthFusion || !other.canMerge()) {
      return;
    }

    auto kreature = getPlayer().mergeWith(**it);
    m_kreatures.push_back(std::move(kreature));

    MergeRealized message;
    gMessageManager().sendMessage(&message);
  }

  void KreatureContainer::createKrokodile() {
    auto kreature = Kreature::krokodile();
    m_kreatures.push_back(std::move(kreature));
  }

  void KreatureContainer::resetKreatures() {
    m_kreatures.clear();

    for (int i = 0; i < MaximumPopulation; ++i) {
      m_kreatures.push_back(std::move(Kreature::randomKreature()));
    }
  }

  void KreatureContainer::update(gf::Time time) {
    // remove dead kreatures

    m_kreatures.erase(std::remove_if(m_kreatures.begin(), m_kreatures.end(), [](auto& kreature) {
      return kreature->isDead();
    }), m_kreatures.end());

    // update kreatures state

    std::size_t count = m_kreatures.size();

    getPlayer().updateAsPlayer(time, m_isSprinting);

    for (std::size_t i = 1; i < count; ++i) {
      m_kreatures[i]->updateAsOther(time);
    }

    // add new kreatures if necessary

    while (count < MinimumPopulation) {
      m_kreatures.push_back(std::move(Kreature::randomKreature()));
      ++count;
    }
  }

  void KreatureContainer::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    for (auto& kreature : m_kreatures) {
      kreature->render(target, states);
    }
  }

  Kreature& KreatureContainer::getPlayer() {
    assert(!m_kreatures.empty());
    return *m_kreatures.front();
  }

}

