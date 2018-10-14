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

#include <cassert>

#include <string>

#include <gf/Id.h>
#include <gf/Unused.h>

/// [using]
using namespace gf::literals; // necessary to use _id
/// [using]

namespace {

  std::string myInput() {
    return "";
  }

}


int main() {

  {
  /// [hash]
  gf::Id id1 = gf::hash("C string");

  std::string str("std::string");
  gf::Id id2 = gf::hash(str);
  /// [hash]

  gf::unused(id1, id2);
  }

  {
  /// [udl]
  gf::Id id1 = "C string"_id;
  gf::Id id2 = "std::string"_id;
  /// [udl]

  gf::unused(id1, id2);
  }

  {
  /// [comptime]
  static_assert("foobar"_id == 0x85944171f73967e8, "It works at compile-time!");
  /// [comptime]
  }

  /// [switch]
  std::string input = myInput();

  switch (gf::hash(input)) {
    case "Foo"_id:
      // input is "Foo"
      break;
    case "Bar"_id:
      // input is "Bar"
      break;
    case "Baz"_id:
      // input is "Baz"
      break;
  }
  /// [switch]

  return 0;
}
