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
#ifndef BRFD_STORY_H
#define BRFD_STORY_H

#include <string>
#include <vector>

#include <gf/Entity.h>
#include <gf/Message.h>
#include <gf/MessageManager.h>
#include <gf/Model.h>
#include <gf/ResourceManager.h>
#include <gf/Vector.h>

namespace brfd {

  enum class Character {
    Flo,
    Roxy,
  };

  class StoryModel : public gf::Model {
  public:
    struct Caption {
      Character character;
      float timer;
      std::string content;
    };

    struct Target {
      float timer;
      gf::Vector2f point;
    };

    StoryModel(gf::MessageManager& messages);

    bool isRunning() const {
      return m_state == State::Running;
    }

    void addCaptionOnly(Caption caption);
    void addCaptionAndTarget(Caption caption, Target target);

    void start();

    bool hasCaption() const {
      return m_currentStep.caption.timer > 0;
    }

    const Caption& getCurrentCaption() const {
      return m_currentStep.caption;
    }

    bool hasTarget() const {
      return m_currentStep.hasTarget;
    }

    const Target& getCurrentTarget() const {
      return m_currentStep.target;
    }

    virtual void update(gf::Time time) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    enum class State {
      Starting,
      Running,
      Success,
      Failure,
    };

    State m_state;

    struct Step {
      Caption caption;
      bool hasTarget;
      Target target;
    };

    std::vector<Step> m_steps;
    std::size_t m_currentStepIndex;
    Step m_currentStep;

  };

  class StoryView : public gf::Entity {
  public:
    StoryView(const StoryModel& model, gf::MessageManager& messages);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::MessageStatus onHeroPosition(gf::Id id, gf::Message *msg);

  private:
    const StoryModel& m_model;
    gf::Vector2f m_hero;
  };

  class StoryHUD : public gf::Entity {
  public:
    StoryHUD(const StoryModel& model, gf::ResourceManager& resources);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const StoryModel& m_model;
    gf::Font& m_font;
    gf::Texture& m_floTexture;
    gf::Texture& m_roxyTexture;
  };

}

#endif // BRFD_STORY_H
