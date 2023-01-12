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
#include <gf/Flags.h>

#include "doc_utils.h"

/// [flags_def]
enum class AnimalProperties {
  HasClaws     = 0x01,
  CanFly       = 0x02,
  EatsFish     = 0x04,
  IsEndangered = 0x08,
};

namespace gf {
  template<>
  struct EnableBitmaskOperators<AnimalProperties> {
    static constexpr bool value = true;
  };
}
/// [flags_def]

void dummyFlagsUsage() {

  /// [flags_usage]
  using gf::operator|;

  gf::Flags<AnimalProperties> seahawk = AnimalProperties::CanFly | AnimalProperties::EatsFish;
  seahawk |= AnimalProperties::IsEndangered;

  bool b = seahawk.test(AnimalProperties::HasClaws); // false
  /// [flags_usage]

  unused(b);

  /// [flags_all_none]
  gf::Flags<AnimalProperties> unicorn(gf::All);

  gf::Flags<AnimalProperties> rat(gf::None);
  /// [flags_all_none]

  unused(unicorn, rat);
}
