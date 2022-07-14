/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <cassert>
#include <gf/Message.h>
#include <gf/MessageManager.h>
#include <gf/Vector.h>

/// [message]
using namespace gf::literals; // necessary to use _id

struct HeroPosition : public gf::Message {
  static const gf::Id type = "HeroPosition"_id; // compile-time definition
  gf::Vector2f position;
};
/// [message]

/// [manager]
gf::MessageManager messageManager;
/// [manager]

// dummy function
static gf::Vector2f computeNewPosition([[maybe_unused]] gf::Vector2f pos, [[maybe_unused]] float dt) {
  return { 0.0f, 0.0f };
}

/// [send]
class Hero {
public:

  void update(float dt) {
    // compute new position
    m_position = computeNewPosition(m_position, dt);

    // broadcast the new position
    HeroPosition message;
    message.position = m_position;
    messageManager.sendMessage(&message);
  }

private:
  gf::Vector2f m_position;
};
/// [send]

/// [recv]
class Enemy {
public:
  Enemy() {
    // register an handler: the onHeroPosition method
    messageManager.registerHandler<HeroPosition>(&Enemy::onHeroPosition, this);
  }

private:
  gf::MessageStatus onHeroPosition([[maybe_unused]] gf::Id id, gf::Message *msg) {
    // verify that we have the right message type
    assert(id == HeroPosition::type);

    // we can now safely cast the message...
    auto heroPosition = static_cast<HeroPosition*>(msg);

    // and use its data to update the ennemy
    m_target = heroPosition->position;

    // we keep this handler for future messages
    return gf::MessageStatus::Keep;
  }

private:
  gf::Vector2f m_target;
};
/// [recv]

struct Foo : public gf::Message {
  static constexpr gf::Id type = "Foo"_id;
};

struct Bar : public gf::Message {
  static constexpr gf::Id type = "Bar"_id;
};

struct Baz : public gf::Message {
  static constexpr gf::Id type = "Baz"_id;
};

// dummy function
static void doSomethingUsefulWith([[maybe_unused]] gf::Id id, [[maybe_unused]] gf::Message *msg) {
}

/// [short]
class ShortLife {
  ShortLife() {
    m_onFoo = messageManager.registerHandler<Foo>(&ShortLife::onFoo, this);
    // register the same function for two message types: that makes two different handlers
    m_onBar = messageManager.registerHandler<Bar>(&ShortLife::onBarOrBaz, this);
    m_onBaz = messageManager.registerHandler<Baz>(&ShortLife::onBarOrBaz, this);
  }

  ~ShortLife() {
    gf::Id ids[] = { m_onFoo, m_onBar, m_onBaz };
    messageManager.removeHandlers(ids);
  }

private:
  gf::MessageStatus onFoo(gf::Id id, gf::Message *msg) {
    // do something useful
    doSomethingUsefulWith(id, msg);

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus onBarOrBaz(gf::Id id, gf::Message *msg) {
    // do something useful
    doSomethingUsefulWith(id, msg);

    return gf::MessageStatus::Keep;
  }

  gf::MessageHandlerId m_onFoo, m_onBar, m_onBaz;
};
/// [short]
