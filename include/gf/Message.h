/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#ifndef GF_MESSAGE_H
#define GF_MESSAGE_H

#include <functional>

#include "Id.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief The base class for all messages
   *
   * gf::Message is the base class for all the messages that are sent to
   * a message manager. A message must derive from this class and define a
   * static unique type.
   *
   * Here is an example for defining a new message type:
   *
   * ~~~{.cc}
   * struct HeroPosition : public gf::Message {
   *   static const Id type = "HeroPosition"_id; // compile-time definition
   *   gf::Vector2f position;
   * };
   * ~~~
   *
   * @sa gf::MessageManager, gf::MessageHandler
   */
  struct GF_API Message {
    /**
     * @brief The default (invalid) type
     *
     * It must be redefined in child classes.
     */
    static const Id type = InvalidId;
  };

  /**
   * @ingroup game
   * @brief A message status
   *
   * gf::MessageStatus indicates if a handler should be kept by the message
   * manager or can be removed so that it will not receive any more messages.
   *
   * @sa gf::MessageManager, gf::MessageHandler
   */
  enum class MessageStatus {
    Keep, /**< The handler must be kept */
    Die,  /**< The handler can be removed */
  };

  /**
   * @ingroup game
   * @brief A message handler
   *
   * gf::MessageHandler is a function that can be called when a message is
   * sent in a message handler. It can be a free function:
   *
   * ~~~{.cc}
   * gf::MessageStatus onHeroPosition(gf::Id type, gf::Message *msg) {
   *   assert(type == HeroPosition::type);
   *   auto heroPosition = static_cast<HeroPosition*>(msg);
   *
   *   // do something with heroPosition->position ...
   *
   *   return gf::MessageStatus::Keep;
   * }
   *
   * // ...
   *
   * gf::MessageHandler handler = onHeroPosition;
   * ~~~
   *
   * It can also be a member function (which is, in fact, the most probable
   * use case).
   *
   * ~~~{.cc}
   * struct Foo {
   *   gf::MessageStatus onHeroPosition(gf::Id type, gf::Message *msg) {
   *     assert(type == HeroPosition::type);
   *     auto heroPosition = static_cast<HeroPosition*>(msg);
   *
   *     // do something with heroPosition->position ...
   *
   *     return gf::MessageStatus::Keep;
   *   }
   * };
   *
   * // ...
   *
   * Foo foo;
   * gf::MessageHandler handler = std::bind(&Foo::onHeroPosition, &foo);
   * ~~~
   *
   * @sa gf::MessageManager, gf::Message, gf::Id
   */
  using MessageHandler = std::function<MessageStatus(Id, Message *)>;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MESSAGE_H
