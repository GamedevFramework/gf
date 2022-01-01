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
#ifndef GF_GEOMETRY_TYPES_H
#define GF_GEOMETRY_TYPES_H

#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_geometry
   * @brief A segment with two points
   */
  template<typename T>
  struct Segment {
    Vector<T, 2> p0;  ///< First end point of the segment
    Vector<T, 2> p1;  ///< Second end point of the segment
  };


  /**
   * @ingroup core_geometry
   * @brief A segment with float vectors
   *
   * @sa gf::Segment
   */
  using SegmentF = Segment<float>;

  /**
   * @ingroup core_geometry
   * @brief A segment with double vectors
   *
   * @sa gf::Segment
   */
  using SegmentD = Segment<double>;

  /**
   * @ingroup core_geometry
   * @brief A segment with int vectors
   *
   * @sa gf::Segment
   */
  using SegmentI = Segment<int>;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GEOMETRY_TYPES_H
