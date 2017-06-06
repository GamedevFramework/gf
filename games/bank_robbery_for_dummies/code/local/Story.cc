/*
 * Bank Robbery for Dummies
 * Copyright (C) 2015 JARS
 * Copyright (C) 2017 Julien Bernard
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
#include "Story.h"

#include <cassert>

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

#include <gf/Log.h>

#include "Messages.h"

namespace brfd {

  static constexpr float ValidTargetDistance = 100.0f;

  /*
   * StoryModel
   */

  StoryModel::StoryModel(gf::MessageManager& messages)
  : m_state(State::Starting)
  , m_currentStepIndex(0)
  {
    messages.registerHandler<HeroPosition>(&StoryModel::onHeroPosition, this);
  }

  void StoryModel::addCaptionOnly(Caption caption) {
    Step step;
    step.caption = std::move(caption);
    step.hasTarget = false;
    m_steps.push_back(std::move(step));
  }

  void StoryModel::addCaptionAndTarget(Caption caption, Target target) {
    Step step;
    step.caption = std::move(caption);
    step.hasTarget = true;
    step.target = std::move(target);
    m_steps.push_back(std::move(step));
  }

  void StoryModel::start() {
    assert(!m_steps.empty());
    m_state = State::Running;
    m_currentStepIndex = 0;
    m_currentStep = m_steps[m_currentStepIndex];
  }

  void StoryModel::update(float dt) {
    m_currentStep.caption.timer -= dt;

    if (m_state != State::Running) {
      return;
    }

    if (m_currentStep.hasTarget) {
      m_currentStep.target.timer -= dt;

      if (m_currentStep.target.timer < 0) {
        m_state = State::Failure;
        m_currentStep.caption.character = Character::Flo;
        m_currentStep.caption.content = "I should really do something useful with my life!";
        m_currentStep.caption.timer = 5.0f;
        m_currentStep.hasTarget = false;
      }
    } else {
      if (m_currentStep.caption.timer < 0) {
        m_currentStepIndex++;
        assert(m_currentStepIndex  < m_steps.size());
        m_currentStep = m_steps[m_currentStepIndex];
      }
    }
  }

  gf::MessageStatus StoryModel::onHeroPosition(gf::Id id, gf::Message *msg) {
    gf::unused(id);

    if (m_state != State::Running) {
      return gf::MessageStatus::Keep;
    }

    auto heroPosition = static_cast<HeroPosition*>(msg);

    float distance = gf::euclideanDistance(heroPosition->position, m_currentStep.target.point);

    if (distance < ValidTargetDistance) {
      m_currentStepIndex++;

      if (m_currentStepIndex < m_steps.size()) {
        m_currentStep = m_steps[m_currentStepIndex];
      } else {
        m_state = State::Success;
        m_currentStep.caption.character = Character::Flo;
        m_currentStep.caption.content = "Yeah! A new carrier!";
        m_currentStep.caption.timer = 5.0f;
        m_currentStep.hasTarget = false;
      }
    }

    return gf::MessageStatus::Keep;
  }

  /*
   * StoryView
   */

  StoryView::StoryView(const StoryModel& model, gf::MessageManager& messages)
  : m_model(model)
  {
    messages.registerHandler<HeroPosition>(&StoryView::onHeroPosition, this);
  }

  static constexpr float CompassRadius = 120.0f;
  static constexpr float CompassSize = 60.0f;

  void StoryView::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (!m_model.hasTarget()) {
      return;
    }

    gf::Vector2f point = m_model.getCurrentTarget().point;

    gf::CircleShape shape(ValidTargetDistance);
    shape.setColor(gf::Color::Red * gf::Color::Opaque(0.5f));
    shape.setPosition(point);
    shape.setAnchor(gf::Anchor::Center);
    target.draw(shape, states);

    float direction = gf::angle(point - m_hero);

    gf::ConvexShape compass(3);
    compass.setPoint(0, { CompassSize, 0.0f });
    compass.setPoint(1, { 0.0f,  CompassSize / 4 });
    compass.setPoint(2, { 0.0f, -CompassSize / 4 });
    compass.setPosition(m_hero + CompassRadius * gf::unit(direction));
    compass.setColor(gf::Color::Red * gf::Color::Opaque(0.5f));
    compass.setRotation(direction);
    compass.setAnchor(gf::Anchor::Center);
    target.draw(compass, states);
  }

  gf::MessageStatus StoryView::onHeroPosition(gf::Id id, gf::Message *msg) {
    gf::unused(id);
    m_hero = static_cast<HeroPosition*>(msg)->position;
    return gf::MessageStatus::Keep;
  }

  /*
   * StoryHUD
   */

  StoryHUD::StoryHUD(const StoryModel& model, gf::ResourceManager& resources)
  : m_model(model)
  , m_font(resources.getFont("Averia-Regular.ttf"))
  , m_floTexture(resources.getTexture("flo.png"))
  , m_roxyTexture(resources.getTexture("roxy.png"))
  {

  }

  static constexpr float TextPadding = 20.0f;
  static constexpr float TextWidth = 600.0f;
  static constexpr float TextHeight = 100.0f;
  static constexpr unsigned TextSize = 32;

  void StoryHUD::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coordinates(target);

    if (m_model.hasTarget()) {
      unsigned timerInSeconds = static_cast<unsigned>(m_model.getCurrentTarget().timer);
      std::array<char, 64> buffer;
      std::snprintf(buffer.data(), buffer.size(), "%.2u", timerInSeconds);

      gf::Vector2f position = coordinates.getRelativePoint({ 0.02f, 0.02f });
      unsigned characterSize = coordinates.getRelativeCharacterSize(0.05f);

      gf::Text text;
      text.setFont(m_font);
      text.setCharacterSize(characterSize);
      text.setColor(gf::Color::White);
      text.setOutlineThickness(1.0f);
      text.setOutlineColor(gf::Color::Black);
      text.setString(buffer.data());
      text.setPosition(position);
      text.setAnchor(gf::Anchor::TopLeft);
      target.draw(text, states);
    }

    if (!m_model.hasCaption()) {
      return;
    }

    gf::Vector2f size = coordinates.getRelativeSize({ 0.6f, 0.25f  });
    gf::Vector2f position = coordinates.getRelativePoint({ 0.5f, 0.7f });
    float padding = size.height * 0.1f;
    position.x -= size.width / 2;

    gf::RectangleShape textBackground(size);
    textBackground.setColor(gf::Color::fromRgba32(0xA7, 0x13, 0x13, 0x95));
    textBackground.setPosition(position);
    target.draw(textBackground, states);

    gf::Sprite sprite;

    switch (m_model.getCurrentCaption().character) {
      case Character::Flo:
        sprite.setTexture(m_floTexture);
        break;
      case Character::Roxy:
        sprite.setTexture(m_roxyTexture);
        break;
    }

    gf::Vector2f textureSize = sprite.getTexture()->getSize();
    float scale = (size.height - 2 * padding) / textureSize.height;

    sprite.setPosition(position + padding);
    sprite.setAnchor(gf::Anchor::TopLeft);
    sprite.setScale(scale);
    target.draw(sprite, states);

    position.x += padding + textureSize.width * scale;
    float paragraphWidth = size.width - textureSize.width * scale - 3 * padding;
    unsigned characterSize = coordinates.getRelativeCharacterSize(0.05f);

    gf::Text text;
    text.setParagraphWidth(paragraphWidth);
    text.setAlignment(gf::Alignment::Justify);
    text.setCharacterSize(characterSize);
    text.setString(m_model.getCurrentCaption().content);
    text.setFont(m_font);
    text.setPosition(position + padding);
    text.setAnchor(gf::Anchor::TopLeft);
    text.setColor(gf::Color::White);
    text.setOutlineThickness(1.0f);
    text.setOutlineColor(gf::Color::Black);
    target.draw(text, states);

  }

}
