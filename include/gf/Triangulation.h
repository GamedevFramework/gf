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
#ifndef GF_TRIANGULATION_H
#define GF_TRIANGULATION_H

#include <vector>

#include "ArrayRef.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  template<typename T>
  class EdgeRef {
  public:
    EdgeRef(const T& p0, const T& p1)
    : m_points{ &p0, &p1 }
    {

    }

    const T& operator[](std::size_t index) const {
      return *m_points[index];
    }

  protected:
    const T *m_points[2];
  };

  template<typename T>
  class TriangleRef {
  public:
    TriangleRef(T& p0, T& p1, T& p2)
    : m_points{ &p0, &p1, &p2 }
    {

    }

    const T& operator[](std::size_t index) const {
      return *m_points[index];
    }

    T& operator[](std::size_t index) {
      return *m_points[index];
    }

  protected:
    T *m_points[3];
  };

  GF_API std::vector<TriangleRef<const Vector2f>> triangulation(ArrayRef<Vector2f> points);

//   GF_API std::vector<TriangleRef<const Vector2f>> triangulationConstrained(ArrayRef<Vector2f> points, ArrayRef<EdgeRef<Vector2f>> contrainedEdges);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TRIANGULATION_H
