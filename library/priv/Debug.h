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
#ifndef GF_DEBUG_H
#define GF_DEBUG_H

#include <iostream>

#include <gf/Matrix.h>
#include <gf/Rect.h>
#include <gf/Vector.h>

#ifdef GF_DEBUG
    #define glCheck(expr) do { expr; gf::priv::glCheckError(__FILE__, __LINE__, #expr); } while (false)
#else
    #define glCheck(expr) (expr)
#endif

namespace gf {
  namespace priv {
    void glCheckError(const char* file, unsigned int line, const char* expr);
  }

  template<typename T, std::size_t N>
  std::ostream& operator<<(std::ostream& o, const Vector<T, N>& value) {
    o << '(';

    for (std::size_t i = 0; i < N; ++i) {
      o << ' ' << value.data[i];
    }

    return o << " )";
  }

  template<typename T, std::size_t ROWS, std::size_t COLS>
  std::ostream& operator<<(std::ostream& o, const Matrix<T, ROWS, COLS>& value) {
    o << "(\n";

    for (std::size_t i = 0; i < ROWS; ++i) {
      o << "\t(";

      for (std::size_t j = 0; j < COLS; ++j) {
        o << ' ' << value.grid[i][j];
      }

      o << " )\n";
    }

    return o << ")";
  }

  template<typename T>
  std::ostream& operator<<(std::ostream& o, const Rect<T>& value) {
    return o << "[ " << value.left << ", " << value.top << " ; " << value.width << ", " << value.height << " ]";
  }

}

#endif // GF_DEBUG_H
