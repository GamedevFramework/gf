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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Time.h>

#include <type_traits>

#include <gf/SerializationOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  const Time Time::Zero = Time();

  Serializer& operator|(Serializer& ar, Time time) {
    ar | time.asMicroseconds();
    return ar;
  }

  Deserializer& operator|(Deserializer& ar, Time& time) {
    int64_t amount = 0;
    ar | amount;
    time = gf::microseconds(amount);
    return ar;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
