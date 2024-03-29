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
#include <gf/Matrix.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

// MSVC does not like extern template
#ifndef _MSC_VER
  template struct Matrix<float, 2, 2>;
  template struct Matrix<float, 3, 3>;
  template struct Matrix<float, 4, 4>;
  template struct Matrix<double, 2, 2>;
  template struct Matrix<double, 3, 3>;
  template struct Matrix<double, 4, 4>;
#endif

  static_assert(sizeof(Matrix2f) == 2 * 2 * sizeof(float), "Matrix size mismatch");
  static_assert(sizeof(Matrix3f) == 3 * 3 * sizeof(float), "Matrix size mismatch");
  static_assert(sizeof(Matrix4f) == 4 * 4 * sizeof(float), "Matrix size mismatch");

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
