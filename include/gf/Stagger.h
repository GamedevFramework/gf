/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#ifndef GF_STAGGER_H
#define GF_STAGGER_H

#include <gf/Vector.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Stagger index in a staggered or hexagonal map.
   *
   * @sa gf::StaggerAxis, gf::StaggerConverter
   */
  enum class StaggerIndex {
    Odd,  ///< A odd stagger index
    Even, ///< An even stagger index
  };

  /**
   * @ingroup game
   * @brief Stagger axis in a staggered or hexagonal map.
   *
   * @sa gf::StaggerIndex, gf::StaggerConverter
   */
  enum class StaggerAxis {
    X,  ///< The x stagger axis
    Y,  ///< The y stagger axis
  };

  /**
   * @ingroup game
   * @brief A helper for computing coordinates in a staggered map
   */
  class StaggerHelper {
  public:
    StaggerHelper(StaggerAxis axis, StaggerIndex index)
    : m_axis(axis)
    , m_index(index)
    {

    }

    /**
     * @brief Compute the center of the tile
     *
     * @param coords The coordinates of the tile in the map
     * @param size The size of the tile in the map
     * @returns The position of the center
     */
    Vector2f computeCenter(Vector2i coords, Vector2f size) const;

  private:
    StaggerAxis m_axis;
    StaggerIndex m_index;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STAGGER_H
