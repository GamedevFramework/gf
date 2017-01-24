/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef GF_PARTICLES_H
#define GF_PARTICLES_H

#include "Portability.h"
#include "Transformable.h"
#include "VertexArray.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A class to display a high number of small shapes
   */
  class GF_API Particles : public Transformable {
  public:
    /**
     * @brief Default constructor
     */
    Particles();

    /**
     * @brief Add a point to the particles
     *
     * @param position The position of the point
     * @param color The color of the point
     */
    void addPoint(Vector2f position, Color4f color);

    /**
     * @brief Add a circle shape to the particles
     *
     * @param position The position of the center of the circle
     * @param radius The radius of the circle
     * @param color The color of the circle
     */
    void addCircle(Vector2f position, float radius, Color4f color);

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    VertexArray m_pointVertices;
    VertexArray m_shapeVertices;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PARTICLES_H
