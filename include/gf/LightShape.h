/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#ifndef GF_LIGHT_SHAPE_H
#define GF_LIGHT_SHAPE_H

#include "Circ.h"
#include "GraphicsApi.h"
#include "Polygon.h"
#include "Rect.h"
#include "Transformable.h"
#include "Vector.h"
#include "VertexArray.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum LightShapeVisibility {
    Apparent,
    Opaque,
  };

  class GF_GRAPHICS_API LightShape : public Transformable {
  public:
    LightShape(const Polygon& polygon, LightShapeVisibility visibility = LightShapeVisibility::Apparent);

    LightShape(Polygon&& polygon, LightShapeVisibility visibility = LightShapeVisibility::Apparent);

    LightShape(const RectF& rect, LightShapeVisibility visibility = LightShapeVisibility::Apparent);

    LightShape(const CircF& circ, LightShapeVisibility visibility = LightShapeVisibility::Apparent);

    std::size_t getPointCount() const;

    Vector2f getPoint(std::size_t index) const;

    Vector2f getPrevPoint(std::size_t index) const;

    Vector2f getNextPoint(std::size_t index) const;

    void setColor(const Color4f& color);

    Color4f getColor() const;

    void setVisibility(LightShapeVisibility visibility);

    LightShapeVisibility getVisibility() const;

    void setActive(bool active = true) {
      m_active = active;
    }

    bool isActive() const {
      return m_active;
    }

    RectF getLocalBounds() const;

    void setAnchor(Anchor anchor);

    void draw(RenderTarget& target, const RenderStates& states) override;

  private:
    void updateGeometry();
    void updateColors();

  private:
    Polygon m_polygon;
    Color4f m_color;
    LightShapeVisibility m_visibility;
    bool m_active = true;

    VertexArray m_vertices;
    RectF m_bounds;
  };



#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_LIGHT_SHAPE_H
