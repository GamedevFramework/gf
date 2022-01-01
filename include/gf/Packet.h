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
#ifndef GF_PACKET_H
#define GF_PACKET_H

#include <cassert>
#include <cstdint>
#include <vector>

#include "Id.h"
#include "NetApi.h"
#include "Packet.h"
#include "Streams.h"
#include "Serialization.h"
#include "SerializationOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup net_sockets
   * @brief A packet of bytes
   */
  struct GF_NET_API Packet {
    Id type = InvalidId; ///< The type of the packet
    std::vector<uint8_t> bytes; ///< The bytes representing the object

    /**
     * @brief Get the type of the underlying bytes
     */
    Id getType() {
      if (type != InvalidId) {
        return type;
      }

      BufferInputStream stream(&bytes);
      Deserializer deserializer(stream);
      deserializer | type;
      return type;
    }

    /**
     * @brief Deserialize the underlying bytes into an object
     */
    template<typename T>
    T as() {
      BufferInputStream stream(&bytes);
      Deserializer deserializer(stream);

      T data;
      deserializer | type | data;
      assert(T::type == type);
      return data;
    }

    /**
     * @brief Serialize the object into bytes
     */
    template<typename T>
    void is(const T& data) {
      bytes.clear();
      type = T::type;
      gf::BufferOutputStream stream(&bytes);
      gf::Serializer serializer(stream);
      serializer | type | const_cast<T&>(data);
    }

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PACKET_H
