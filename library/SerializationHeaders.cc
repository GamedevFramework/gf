/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/SerializationHeaders.h>

#include <gf/Serialization.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Serializer& operator|(Serializer& ar, SerialArrayHeader header)  {
    ar.writeArrayHeader(header.size);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, SerialArrayHeader& header) {
    ar.readArrayHeader(header.size);
    return ar;
  }


  Serializer& operator|(Serializer& ar, SerialMapHeader header) {
    ar.writeMapHeader(header.size);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, SerialMapHeader& header) {
    ar.readMapHeader(header.size);
    return ar;
  }

  Serializer& operator|(Serializer& ar, SerialUserTypeHeader header)  {
    ar.writeArrayHeader(header.fields);
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, SerialUserTypeHeader& header) {
    ar.readArrayHeader(header.fields);
    return ar;
  }



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
