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
#include "Kreature.h"

#include <cassert>

#include <gf/Color.h>
#include <gf/Sprite.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>

#include "Messages.h"
#include "Singletons.h"

namespace kkd {

  namespace {
    constexpr int TotalAnimal = 3;

    constexpr float MaxBound = 1500.0f;
    constexpr float MinBound = - MaxBound;

    constexpr gf::RectF Bounds(MinBound, MinBound, MaxBound - MinBound, MaxBound - MinBound);

    constexpr float SprintVelocityFactor = 2.0f;
    constexpr float SprintEnergyFactor = -2.0f;

    constexpr float EnergyLevelMax = 100.0f;
    constexpr float EnergyLevelSteps = 15.0f;

    constexpr float FusionEnergyConsumption = 0.80f * EnergyLevelMax;
    constexpr int FusionCountMax = 5;

    constexpr float MinimumLifeTime = 45.0f;
    constexpr float MaximumLifeTime = 90.0f;
//     constexpr float MinimumLifeTime = 15.0f;
//     constexpr float MaximumLifeTime = 20.0f;

    constexpr float LinearVelocity = 200.0f;
    constexpr float AngularVelocity = 2.0f;

    constexpr float AiMalusVelocity = 0.80f;

    constexpr gf::Time AnimationDuration = gf::seconds(0.25f);
    constexpr gf::Time RotationDuration = gf::seconds(1.0f);
    constexpr gf::Time BirthDeathDuration = gf::seconds(2.0f);

    gf::Time getAnimationTime(gf::Vector2f position, gf::Vector2f target) {
      return gf::seconds(gf::euclideanDistance(position, target) / (LinearVelocity * AiMalusVelocity));
    }

    std::unique_ptr<Kreature> computeRandomKreature() {
      gf::Vector2f position = gRandom().computePosition(Bounds);
      gf::Vector2f target = gRandom().computePosition(Bounds);
      float angle = gRandom().computeUniformFloat(0.0f, 2 * gf::Pi);
      return std::make_unique<Kreature>(position, angle, target);
    }

    Kreature::Part computeRandomPart() {
      Kreature::Part part;
      part.species = static_cast<Kreature::Species>(gRandom().computeUniformInteger(0, TotalAnimal - 1));
      part.color = static_cast<Kreature::Color>(gRandom().computeUniformInteger(0, 4));
      return part;
    }

    gf::Color4f getKreatureColor(Kreature::Color color) {
      switch (color) {
        case Kreature::Azure:
          return gf::Color::Azure;
        case Kreature::Green:
          return gf::Color::Green;
        case Kreature::Yellow:
          return gf::Color::lighter(gf::Color::Yellow, 0.25f);
        case Kreature::Red:
          return gf::Color::lighter(gf::Color::Red, 0.25f);
        case Kreature::Magenta:
          return gf::Color::lighter(gf::Color::Magenta, 0.25f);
        default:
          break;
      }

      assert(false);
      return gf::Color::Black;
    }

    gf::RectF getTextureRectForBody(Kreature::Species species) {
      switch (species) {
        case Kreature::Krokodile:
          return gTextureAtlas().getTextureRect("body_krokodile");
        case Kreature::Elephant:
          return gTextureAtlas().getTextureRect("body_elephant");
        case Kreature::Lion:
          return gTextureAtlas().getTextureRect("body_lion");
      }

      assert(false);
      return gf::RectF();
    }

    gf::RectF getTextureRectForHead(Kreature::Species species) {
      switch (species) {
        case Kreature::Krokodile:
          return gTextureAtlas().getTextureRect("head_krokodile");
        case Kreature::Elephant:
          return gTextureAtlas().getTextureRect("head_elephant");
        case Kreature::Lion:
          return gTextureAtlas().getTextureRect("head_lion");
      }

      assert(false);
      return gf::RectF();
    }

    gf::RectF getTextureRectForTail(Kreature::Species species) {
      switch (species) {
        case Kreature::Krokodile:
          return gTextureAtlas().getTextureRect("tail_krokodile");
        case Kreature::Elephant:
          return gTextureAtlas().getTextureRect("tail_elephant");
        case Kreature::Lion:
          return gTextureAtlas().getTextureRect("tail_lion");
      }

      assert(false);
      return gf::RectF();
    }

    gf::RectF getTextureRectForAnte(Kreature::Species species) {
      switch (species) {
        case Kreature::Krokodile:
          return gTextureAtlas().getTextureRect("ante_krokodile");
        case Kreature::Elephant:
          return gTextureAtlas().getTextureRect("ante_elephant");
        case Kreature::Lion:
          return gTextureAtlas().getTextureRect("ante_lion");
      }

      assert(false);
      return gf::RectF();
    }

    gf::RectF getTextureRectForPost(Kreature::Species species) {
      switch (species) {
        case Kreature::Krokodile:
          return gTextureAtlas().getTextureRect("post_krokodile");
        case Kreature::Elephant:
          return gTextureAtlas().getTextureRect("post_elephant");
        case Kreature::Lion:
          return gTextureAtlas().getTextureRect("post_lion");
      }

      assert(false);
      return gf::RectF();
    }

    constexpr int PartHead      = 0;
    constexpr int PartAnteLeft  = 1;
    constexpr int PartAnteRight = 2;
    constexpr int PartPostLeft  = 3;
    constexpr int PartPostRight = 4;
    constexpr int PartTail      = 5;

    gf::Vector2f getJoint(Kreature::Species species, int part) {
      static constexpr float Padding = 10.0f;

      // Joint points for krokodile
      static constexpr gf::Vector2f JointsForKrokodile[] = {
        {  128.0f + Padding,   0.0f }, // head
        {   77.0f + Padding, -52.0f - Padding }, // ante left
        {   77.0f + Padding, +52.0f + Padding }, // ante right
        {  -80.0f - Padding, -68.0f - Padding }, // post left
        {  -80.0f - Padding, +68.0f + Padding }, // post right
        { -128.0f - Padding,   0.0f }, // tail
      };

      // Joint points for elephant
      static constexpr gf::Vector2f JointsForElephant[] = {
        {  116.0f + Padding,   0.0f }, // head
        {   71.0f + Padding, -63.0f - Padding }, // ante left
        {   71.0f + Padding, +63.0f + Padding }, // ante right
        {  -80.0f + Padding, -86.0f - Padding }, // post left
        {  -80.0f + Padding, +86.0f + Padding }, // post right
        { -128.0f - Padding,   0.0f }, // tail
      };

      // Joint points for lion
      static constexpr gf::Vector2f JointsForLion[] = {
        {    7.0f + Padding,   0.0f }, // head
        {  -23.0f + Padding, -31.0f - Padding }, // ante left
        {  -23.0f + Padding, +31.0f + Padding }, // ante right
        { -101.0f + Padding, -34.0f - Padding }, // post left
        { -101.0f + Padding, +34.0f + Padding }, // post right
        { -128.0f - Padding,   0.0f }, // tail
      };

      assert(PartHead <= part && part <= PartTail);

      switch (species) {
        case Kreature::Krokodile:
          return JointsForKrokodile[part];
        case Kreature::Elephant:
          return JointsForElephant[part];
        case Kreature::Lion:
          return JointsForLion[part];
      }

      assert(false);
      return gf::Vector2f(0.0f, 0.0f);
    }

    int compareColor(Kreature::Color color1, Kreature::Color color2) {
      switch (color1) {
        case Kreature::Azure:
          if (color2 == Kreature::Yellow || color2 == Kreature::Magenta) {
            return 1;
          }

          if (color2 == Kreature::Green || color2 == Kreature::Red) {
            return -1;
          }

          return 0;

        case Kreature::Green:
          if (color2 == Kreature::Azure || color2 == Kreature::Yellow) {
            return 1;
          }

          if (color2 == Kreature::Magenta || color2 == Kreature::Red) {
            return -1;
          }

          return 0;

        case Kreature::Yellow:
          if (color2 == Kreature::Red || color2 == Kreature::Magenta) {
            return 1;
          }

          if (color2 == Kreature::Azure || color2 == Kreature::Green) {
            return -1;
          }

          return 0;

        case Kreature::Red:
          if (color2 == Kreature::Azure || color2 == Kreature::Green) {
            return 1;
          }

          if (color2 == Kreature::Yellow || color2 == Kreature::Magenta) {
            return -1;
          }

          return 0;

        case Kreature::Magenta:
          if (color2 == Kreature::Red || color2 == Kreature::Green) {
            return 1;
          }

          if (color2 == Kreature::Yellow || color2 == Kreature::Azure) {
            return -1;
          }

          return 0;

        default:
          break;
      }

      assert(false);
      return 0;
    }

    Kreature::Part mergeParts(Kreature::Part p1, Kreature::Part p2) {
      constexpr float UpperFusionFactor = 0.75f;
      constexpr float LowerFusionFactor = 0.25f;
      constexpr float FumbleMutation = 0.90f;

      float rand = gRandom().computeUniformFloat(0.0f, 1.0f);

      if (rand > FumbleMutation) {
        // fumble
        return computeRandomPart();
      }

      float probability = 0.5f;

      switch (compareColor(p1.color, p2.color)) {
        case -1:
          probability = LowerFusionFactor;
          break;
        case 1:
          probability = UpperFusionFactor;
          break;
        case 0:
          // nothing to do
          break;
        default:
          assert(false);
          break;
      }

      if (gRandom().computeBernoulli(probability)) {
        return p1;
      }

      return p2;
    }

  }

  Kreature::Kreature(gf::Vector2f position, float angle, gf::Vector2f target)
  : m_position(position)
  , m_angle(angle)
  , m_state(Birth)
  , m_life(gf::seconds(gRandom().computeUniformFloat(MinimumLifeTime, MaximumLifeTime)))
  , m_energy(EnergyLevelMax)
  , m_merges(FusionCountMax)
  , m_linearMove(gf::LinearMove::None)
  , m_angularMove(gf::AngularMove::None)
  , m_animationTime(gf::seconds(gRandom().computeUniformFloat(0.01f, AnimationDuration.asSeconds() - 0.01f)))
  , m_animationFactor(1.0f)
  , m_rotationActivity(angle, gf::angle(target - position), m_angle, RotationDuration)
  , m_moveActivity(position, target, m_position, getAnimationTime(position, target))
  , m_scale(0.0f)
  , m_scaleActivity(0.0f, 1.0f, m_scale, BirthDeathDuration)
  {
    m_sequence.addActivity(m_rotationActivity);
    m_sequence.addActivity(m_moveActivity);
  }

  void Kreature::updateAsPlayer(gf::Time time, bool sprint) {
    m_state = Life;

    // update energy

    updateEnergy(time, sprint);

    if (m_energy == 0.0f) {
      sprint = false;
    }

    // update animation

    if (m_linearMove != gf::LinearMove::None || m_angularMove != gf::AngularMove::None) {
      m_animationTime += time;
    }

    if (m_animationTime >= AnimationDuration) {
      m_animationTime -= AnimationDuration;
      m_animationFactor = -m_animationFactor;
      assert(m_animationFactor == 1.0f || m_animationFactor == -1.0f);
    }

    // update position and angle

    float dt = time.asSeconds();

    m_angle += gf::angularFactor(m_angularMove) * AngularVelocity * dt;

    float sprintFactor = sprint ? SprintVelocityFactor : 1.0f;
    m_position += gf::linearFactor(m_linearMove) * LinearVelocity * sprintFactor * dt * gf::unit(m_angle);

    m_position = gf::clamp(m_position, MinBound, MaxBound);

    m_life -= time;
    m_life = std::max(m_life, gf::Time::zero());

    // send message

    KrokodileState message;
    message.position = m_position;
    message.angle = m_angle;
    message.energyRatio = m_energy / EnergyLevelMax;
    message.lifeRatio = m_life.asSeconds() / MaximumLifeTime;
    message.merges = m_merges;
    gMessageManager().sendMessage(&message);
  }

  void Kreature::updateAsOther(gf::Time time) {
    if (m_state == Birth) {
      gf::ActivityStatus status = m_scaleActivity.run(time);

      if (status == gf::ActivityStatus::Finished) {
        m_state = Life;
      }
    } else if (m_state == Life) {
      gf::ActivityStatus status = m_sequence.run(time);

      if (status == gf::ActivityStatus::Finished) {
        resetActivities();
      }

      m_life -= time;

      if (m_life < gf::Time::zero()) {
        m_life = gf::Time::zero();

        m_scaleActivity.setOrigin(1.0f);
        m_scaleActivity.setTarget(0.0f);
        m_scaleActivity.restart();

        m_state = End;
      }
    } else {
      gf::ActivityStatus status = m_scaleActivity.run(time);

      if (status == gf::ActivityStatus::Finished) {
        m_state = Death;
      }
    }

    // update animation

    m_animationTime += time;

    if (m_animationTime >= AnimationDuration) {
      m_animationTime -= AnimationDuration;
      m_animationFactor = -m_animationFactor;
      assert(m_animationFactor == 1.0f || m_animationFactor == -1.0f);
    }

    // update energy

    updateEnergy(time, false);
  }

  void Kreature::updateEnergy(gf::Time time, bool sprint) {
    float sprintFactor = sprint ? SprintEnergyFactor : 1.0f;
    m_energy += sprintFactor * EnergyLevelSteps * time.asSeconds();
    m_energy = gf::clamp(m_energy, 0.0f, EnergyLevelMax);
  }

  void Kreature::render(gf::RenderTarget &target, gf::RenderStates states) {
    if (m_state == Death) {
      return;
    }

    if (m_state == Birth || m_state == End) {
      states.transform *= gf::scaling({ m_scale, m_scale }, m_position);
    }

    float animationRotationOffset = gf::Pi / 8.0f * m_animationFactor;

    // body

    static constexpr gf::Vector2f BodySpriteSize = { 256.0f, 256.0f };
    static constexpr gf::Vector2f BodyWorldSize = { 128.0f, 128.0f };

    gf::Sprite body(gTextureAtlas().getTexture(), getTextureRectForBody(m_body.species));
    body.setScale(BodyWorldSize / BodySpriteSize);
    body.setColor(getKreatureColor(m_body.color));
    body.setPosition(m_position);
    body.setRotation(m_angle);

    gf::Matrix3f matrix = body.getTransform();
    Species base = m_body.species;

    body.setAnchor(gf::Anchor::Center);

    // head

    static constexpr gf::Vector2f HeadSpriteSize = { 256.0f, 256.0f };
    static constexpr gf::Vector2f HeadWorldSize = { 128.0f, 128.0f };

    gf::Sprite head(gTextureAtlas().getTexture(), getTextureRectForHead(m_head.species));
    head.setScale(HeadWorldSize / HeadSpriteSize);
    head.setAnchor(gf::Anchor::CenterLeft);
    head.setColor(getKreatureColor(m_head.color));
    head.setPosition(gf::transform(matrix, getJoint(base, PartHead)));
    head.setRotation(m_angle);
    head.draw(target, states);

    // ante

    static constexpr gf::Vector2f AnteSpriteSize = { 128.0f, 128.0f };
    static constexpr gf::Vector2f AnteWorldSize = { 64.0f, 64.0f };

    gf::Sprite ante(gTextureAtlas().getTexture(), getTextureRectForAnte(m_limbs.species));
    ante.setScale(AnteWorldSize / AnteSpriteSize);
    ante.setAnchor(gf::Anchor::BottomCenter);
    ante.setColor(getKreatureColor(m_limbs.color));
    ante.setPosition(gf::transform(matrix, getJoint(base, PartAnteLeft)));
    ante.setRotation(m_angle + animationRotationOffset);
    ante.draw(target, states);

    ante.scale({ 1.0f, -1.0f });
    ante.setPosition(gf::transform(matrix, getJoint(base, PartAnteRight)));
    ante.draw(target, states);

    // post

    static constexpr gf::Vector2f PostSpriteSize = { 128.0f, 128.0f };
    static constexpr gf::Vector2f PostWorldSize = { 64.0f, 64.0f };

    gf::Sprite post(gTextureAtlas().getTexture(), getTextureRectForPost(m_limbs.species));
    post.setScale(PostWorldSize / PostSpriteSize);
    post.setAnchor(gf::Anchor::BottomCenter);
    post.setColor(getKreatureColor(m_limbs.color));
    post.setPosition(gf::transform(matrix, getJoint(base, PartPostLeft)));
    post.setRotation(m_angle + animationRotationOffset);
    post.draw(target, states);

    post.scale({ 1.0f, -1.0f });
    post.setPosition(gf::transform(matrix, getJoint(base, PartPostRight)));
    post.draw(target, states);

    // tail

    static constexpr gf::Vector2f TailSpriteSize = { 256.0f, 256.0f };
    static constexpr gf::Vector2f TailWorldSize = { 128.0f, 128.0f };

    gf::Sprite tail(gTextureAtlas().getTexture(), getTextureRectForTail(m_tail.species));
    tail.setScale(TailWorldSize / TailSpriteSize);
    tail.setAnchor(gf::Anchor::CenterRight);
    tail.setColor(getKreatureColor(m_tail.color));
    tail.setPosition(gf::transform(matrix, getJoint(base, PartTail)));
    tail.setRotation(m_angle);
    tail.draw(target, states);

    // finally body

    body.draw(target, states);
  }

  float Kreature::distanceTo(const Kreature& other) {
    return gf::euclideanDistance(m_position, other.m_position);
  }

  bool Kreature::canMerge() const {
    return m_state == Life && m_energy > FusionEnergyConsumption && m_merges > 0;
  }

  bool Kreature::isKrokodile() const {
    return m_body.species == Krokodile && m_body.color == Green
        && m_head.species == Krokodile && m_head.color == Green
        && m_limbs.species == Krokodile && m_limbs.color == Green
        && m_tail.species == Krokodile && m_tail.color == Green;
  }

  std::unique_ptr<Kreature> Kreature::mergeWith(Kreature& other) {
    assert(canMerge() && other.canMerge());

    // update state of this and other

    m_energy -= FusionEnergyConsumption;
    --m_merges;

    other.m_energy -= FusionEnergyConsumption;
    --other.m_merges;

    // create child

    gf::Vector2f position = (m_position + other.m_position) / 2;
    gf::Vector2f target = gRandom().computePosition(Bounds);
    float angle = (m_angle + other.m_angle) / 2;

    auto kreature = std::make_unique<Kreature>(position, angle, target);
    kreature->m_body = mergeParts(m_body, other.m_body);
    kreature->m_head = mergeParts(m_head, other.m_head);
    kreature->m_limbs = mergeParts(m_limbs, other.m_limbs);
    kreature->m_tail = mergeParts(m_tail, other.m_tail);
    return std::move(kreature);
  }

  void Kreature::resetActivities() {
    gf::Vector2f target = gRandom().computePosition(Bounds);

    m_rotationActivity.setOrigin(m_angle);
    m_rotationActivity.setTarget(gf::angle(target - m_position));

    m_moveActivity.setOrigin(m_position);
    m_moveActivity.setTarget(target);
    m_moveActivity.setDuration(getAnimationTime(m_position, target));

    m_sequence.restart();
  }

  std::unique_ptr<Kreature> Kreature::randomKreature() {
    auto kreature = computeRandomKreature();
    kreature->m_body = computeRandomPart();
    kreature->m_head = computeRandomPart();
    kreature->m_limbs = computeRandomPart();
    kreature->m_tail = computeRandomPart();
    return std::move(kreature);
  }

  std::unique_ptr<Kreature> Kreature::krokodile() {
    auto kreature = computeRandomKreature();
    kreature->m_body = { Krokodile, Green };
    kreature->m_head = { Krokodile, Green };
    kreature->m_limbs = { Krokodile, Green };
    kreature->m_tail = { Krokodile, Green };
    return std::move(kreature);
  }

}
