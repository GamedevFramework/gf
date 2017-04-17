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
#ifndef GF_COORDINATES_H
#define GF_COORDINATES_H

#include "Anchor.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class RenderTarget;

  /**
   * @ingroup graphics
   * @brief Helper to compute coordinates in HUD
   */
  class GF_API Coordinates {
  public:
    /**
     * @brief Constructor with size
     *
     * @param size The size of the window
     */
    Coordinates(Vector2u size);

    /**
     * @brief Constructor with render target
     *
     * @param target The render target
     */
    Coordinates(RenderTarget& target);

    /**
     * @brief Get the center of the window
     *
     * @returns The center of the window
     */
    Vector2f getCenter() const;

    /**
     * @brief Compute a relative point
     *
     * @param percent The percentage from top-right
     */
    Vector2f getRelativePoint(Vector2f percent) const;

    /**
     * @brief Compute a relative size
     *
     * @param percent The percentage of the window
     */
    Vector2f getRelativeSize(Vector2f percent) const;

    /**
     * @brief Compute a relative character size
     *
     * @param percent The percentage of the window
     */
    unsigned getRelativeCharacterSize(float percent) const;

    /**
     * @brief Compute an absolute point
     *
     * @param distance The distance from the edge
     * @param anchor The reference anchor
     */
    Vector2f getAbsolutePoint(Vector2f distance, Anchor anchor) const;

  private:
    Vector2u m_size;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_COORDINATES_H
