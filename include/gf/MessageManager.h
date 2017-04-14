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
#ifndef GF_MESSAGE_MANAGER_H
#define GF_MESSAGE_MANAGER_H

#include <cstdint>
#include <functional>
#include <map>
#include <type_traits>
#include <vector>

#include "ArrayRef.h"
#include "Message.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief An identifier for a message handler
   *
   * @sa gf::MessageHandler
   */
  using MessageHandlerId = uint64_t;

  /**
   * @ingroup game
   * @brief A message manager
   *
   * A message manager is responsible for passing messages synchronously
   * between game entities. It relies on a variant of the
   * [observer pattern](https://en.wikipedia.org/wiki/Observer_pattern).
   * Some entities send messages (subclasses of gf::Message identified by
   * their unique message type) to the message manager while some other
   * entities listen to messages of a defined type through a message
   * handler (gf::MessageHandler). As a consequence, there is very low
   * [coupling](https://en.wikipedia.org/wiki/Coupling_%28computer_programming%29)
   * between entities.
   *
   * Generally, you only need one message manager in a game. It is a good
   * candidate for being a singleton (thanks to gf::Singleton).
   *
   * ~~~{.cc}
   * gf::Singleton<gf::MessageManager> gMessageManager;
   *
   * int main() {
   *   gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gMessageManager);
   *
   *   gMessageManager().sendMessage(&foo);
   * }
   * ~~~
   *
   * @sa gf::Message, gf::MessageHandler, gf::Id
   */
  class GF_API MessageManager {
  public:
    /**
     * @brief Constructor
     */
    MessageManager();

    /**
     * @name Registering an handler
     * @{
     */

    /**
     * @brief Register a message handler for a type of message
     *
     * ~~~{.cc}
     * struct Foo : public gf::Message {
     *   static const gf::Id type = "Foo"_id;
     *   // other fields
     * };
     *
     * gf::MessageHandler handler = ...;
     *
     * gf::MessageManager messages;
     * messages.registerHandler(Foo::type, handler);
     * ~~~
     *
     * @param type The type of message
     * @param handler The message handler
     * @return A handler id
     */
    MessageHandlerId registerHandler(Id type, MessageHandler handler);

    /**
     * @brief Register a message handler for a type of message
     *
     * ~~~{.cc}
     * struct Foo : public gf::Message {
     *   static const gf::Id type = "Foo"_id;
     *   // other fields
     * };
     *
     * gf::MessageHandler handler = ...;
     *
     * gf::MessageManager messages;
     * messages.registerHandler<Foo>(handler);
     * ~~~
     *
     * @param handler The message handler
     * @return A handler id
     */
    template<typename E>
    MessageHandlerId registerHandler(MessageHandler handler) {
      static_assert(std::is_base_of<Message, E>::value, "E must be an Message");
      static_assert(E::type != InvalidId, "E must define its type");
      return registerHandler(E::type, handler);
    }

    /**
     * @brief Register a message handler for a type of message
     *
     * ~~~{.cc}
     * struct Foo : public gf::Message {
     *   static const gf::Id type = "Foo"_id;
     *   // other fields
     * };
     *
     * class Bar {
     *   gf::MessageStatus onFoo(gf::Id type, gf::Message *msg) {
     *     // do something useful
     *     return gf::MessageStatus::Keep;
     *   }
     * };
     *
     * Bar bar;
     *
     * gf::MessageManager messages;
     * messages.registerHandler(Foo::type, &Bar::onFoo, &bar);
     * ~~~
     *
     * @param type The type of message
     * @param pm A pointer to member function that represents the handler
     * @param obj The destination object that receives the message
     * @return A handler id
     */
    template<typename R, typename T>
    MessageHandlerId registerHandler(Id type, R T::*pm, T *obj) {
      return registerHandler(type, std::bind(pm, obj, std::placeholders::_1, std::placeholders::_2));
    }

    /**
     * @brief Register a message handler for a type of message
     *
     * ~~~{.cc}
     * struct Foo : public gf::Message {
     *   static const gf::Id type = "Foo"_id;
     *   // other fields
     * };
     *
     * class Bar {
     *   gf::MessageStatus onFoo(gf::Id type, gf::Message *msg) {
     *     // do something useful
     *     return gf::MessageStatus::Keep;
     *   }
     * };
     *
     * Bar bar;
     *
     * gf::MessageManager messages;
     * messages.registerHandler<Foo>(&Bar::onFoo, &bar);
     * ~~~
     *
     * @param pm A pointer to member function that represents the handler
     * @param obj The destination object that receives the message
     * @return A handler id
     */
    template<typename E, typename R, typename T>
    MessageHandlerId registerHandler(R T::*pm, T *obj) {
      static_assert(std::is_base_of<Message, E>::value, "E must be an Message");
      static_assert(E::type != InvalidId, "E must define its type");
      return registerHandler(E::type, std::bind(pm, obj, std::placeholders::_1, std::placeholders::_2));
    }

    /** @} */

    /**
     * @name Removing an handler
     * @{
     */

    /**
     * @brief Remove a handler
     *
     * @param id The handler id
     */
    void removeHandler(MessageHandlerId id);

    /**
     * @brief Remove a list of handlers
     *
     * @param ids The list of handler ids
     */
    void removeHandlers(ArrayRef<MessageHandlerId> ids) {
      for (auto id : ids) {
        removeHandler(id);
      }
    }

    /** @} */

    /**
     * @name Sending a message
     * @{
     */

    /**
     * @brief Send a message
     *
     * The message is sent synchronously i.e. immediately when calling the
     * function. A consequence is that the memory for the message can be
     * allocated on the stack because it only has to live during the function
     * call.
     *
     * ~~~{.cc}
     * struct Foo : public gf::Message {
     *   static const gf::Id type = "Foo"_id;
     *   // other fields
     * };
     *
     * Foo foo;
     *
     * gf::MessageManager messages;
     * messages.sendMessage(Foo::type, &foo);
     * ~~~
     *
     * @param type The message type
     * @param message A pointer to the message
     */
    void sendMessage(Id type, Message *message);

    /**
     * @brief Send a message
     *
     * The message is sent synchronously i.e. immediately when calling the
     * function. A consequence is that the memory for the message can be
     * allocated on the stack because it only has to live during the function
     * call.
     *
     * ~~~{.cc}
     * struct Foo : public gf::Message {
     *   static const gf::Id type = "Foo"_id;
     *   // other fields
     * };
     *
     * Foo foo;
     *
     * gf::MessageManager messages;
     * messages.sendMessage(&foo);
     * ~~~
     *
     * @param message A pointer to the message
     */
    template<typename E>
    void sendMessage(E *message) {
      static_assert(std::is_base_of<Message, E>::value, "E must be an Message");
      static_assert(E::type != InvalidId, "E must define its type");
      sendMessage(E::type, message);
    }

    /** @} */

  private:
    struct Handler {
      MessageHandlerId id;
      MessageHandler handler;
    };

    MessageHandlerId m_currentId;
    std::map<Id, std::vector<Handler>> m_handlers;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_MESSAGE_MANAGER_H
