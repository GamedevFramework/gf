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
#include <gf/Random.h>

#include <algorithm>
#include <functional>
#include <iterator>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  // see http://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
  // and http://www.pcg-random.org/posts/cpp-seeding-surprises.html
  static std::mt19937 getCorrectlyInitializedEngine() {
    std::mt19937::result_type data[std::mt19937::state_size];
    std::random_device source;
    std::generate(std::begin(data), std::end(data), std::ref(source));
    std::seed_seq seeds(std::begin(data), std::end(data));
    return std::mt19937(seeds);
  }

  Random::Random()
  : m_engine(getCorrectlyInitializedEngine())
  {

  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
