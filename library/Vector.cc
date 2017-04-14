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
#include <gf/Vector.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

// MSVC does not like extern template
#ifndef _MSC_VER
  template struct Vector<float, 2>;
  template struct Vector<float, 3>;
  template struct Vector<float, 4>;

  template struct Vector<double, 2>;
  template struct Vector<double, 3>;
  template struct Vector<double, 4>;

  template struct Vector<int, 2>;
  template struct Vector<int, 3>;
  template struct Vector<int, 4>;

  template struct Vector<unsigned, 2>;
  template struct Vector<unsigned, 3>;
  template struct Vector<unsigned, 4>;

  template struct Vector<bool, 2>;
  template struct Vector<bool, 3>;
  template struct Vector<bool, 4>;

  template struct Vector<uint8_t, 3>;
  template struct Vector<uint8_t, 4>;
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
