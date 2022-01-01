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
#include <gf/Vector.h>

#include <type_traits>

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

  static_assert(sizeof(Vector2f) == 2 * sizeof(float), "Vector size mismatch");
  static_assert(sizeof(Vector3f) == 3 * sizeof(float), "Vector size mismatch");
  static_assert(sizeof(Vector4f) == 4 * sizeof(float), "Vector size mismatch");

  static_assert(sizeof(Vector2d) == 2 * sizeof(double), "Vector size mismatch");
  static_assert(sizeof(Vector3d) == 3 * sizeof(double), "Vector size mismatch");
  static_assert(sizeof(Vector4d) == 4 * sizeof(double), "Vector size mismatch");

  static_assert(sizeof(Vector2i) == 2 * sizeof(int), "Vector size mismatch");
  static_assert(sizeof(Vector3i) == 3 * sizeof(int), "Vector size mismatch");
  static_assert(sizeof(Vector4i) == 4 * sizeof(int), "Vector size mismatch");

  static_assert(sizeof(Vector2u) == 2 * sizeof(unsigned), "Vector size mismatch");
  static_assert(sizeof(Vector3u) == 3 * sizeof(unsigned), "Vector size mismatch");
  static_assert(sizeof(Vector4u) == 4 * sizeof(unsigned), "Vector size mismatch");

  static_assert(sizeof(Vector2b) == 2 * sizeof(bool), "Vector size mismatch");
  static_assert(sizeof(Vector3b) == 3 * sizeof(bool), "Vector size mismatch");
  static_assert(sizeof(Vector4b) == 4 * sizeof(bool), "Vector size mismatch");

  static_assert(sizeof(Color3u) == 3 * sizeof(uint8_t), "Vector size mismatch");
  static_assert(sizeof(Color4u) == 4 * sizeof(uint8_t), "Vector size mismatch");


  static_assert(std::is_trivial<Vector2f>::value, "Vector2f should be trivial");

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
