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
#ifndef GF_CURVE_H
#define GF_CURVE_H

#include <cstddef>

#include "Portability.h"
#include "Rect.h"
#include "Transformable.h"
#include "Vector.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A curve is a one dimension object
   *
   * A curve is similar to a shape but for one dimension objects like lines,
   * [Bézier curves](https://en.wikipedia.org/wiki/B%C3%A9zier_curve), or
   * compound curves.
   *
   * A curve can be simple or outlined. A simple curve has no outline and is
   * generally thin. An outline curve is more complex and can have an outline
   * and has no limit on the width. By default, curves are outlined.
   *
   * @sa gf::Line, gf::QuadraticBezierCurve, gf::CubicBezierCurve, gf::CompoundCurve
   */
  class GF_API Curve : public Transformable {
  public:
    /**
     * @brief The type of the curve
     */
    enum Type {
      Simple,   ///< A simple curve with no outline
      Outlined, ///< An outlined curve
    };

    /**
     * @brief Default constructor
     */
    Curve();

    /**
     * @brief Set the type of the curve
     *
     * By default, a curve is outlined.
     *
     * @param type The type of the curve
     */
    void setType(Type type);

    /**
     * @brief Return the type of the curve
     *
     * @return The current type of the curve
     */
    Type getType() const noexcept {
      return m_type;
    }

    /**
     * @brief Set the fill color of the curve
     *
     * This color is modulated (multiplied) with the curve's
     * texture if any. It can be used to colorize the curve,
     * or change its global opacity.
     *
     * By default, the curve's fill color is opaque white.
     *
     * @param color New color of the curve
     *
     * @sa getColor(), setOutlineColor()
     */
    void setColor(const Color4f& color);

    /**
     * @brief Get the fill color of the curve
     *
     * @return Fill color of the curve
     *
     * @sa setColor()
     */
    const Color4f& getColor() const {
      return m_color;
    }

    /**
     * @brief Set the width of the curve
     *
     * @param width The width of the curve
     */
    void setWidth(float width);

    /**
     * @brief Get the width of the curve
     *
     * @return The current width of the curve
     */
    float getWidth() const {
      return m_width;
    }

    /**
     * @brief Set the outline color of the curve
     *
     * By default, the curve's outline color is opaque white.
     *
     * @param color New outline color of the curve
     *
     * @sa getOutlineColor(), setColor()
     */
    void setOutlineColor(const Color4f& color);

    /**
     * @brief Get the outline color of the curve
     *
     * @return Outline color of the curve
     *
     * @sa setOutlineColor()
     */
    const Color4f& getOutlineColor() const {
      return m_outlineColor;
    }

    /**
     * @brief Set the thickness of the curve's outline
     *
     * By default, the outline thickness is @f$ 0 @f$.
     *
     * @param thickness New outline thickness
     *
     * @sa getOutlineThickness()
     */
    void setOutlineThickness(float thickness);

    /**
     * @brief Get the outline thickness of the curve
     *
     * @return Outline thickness of the curve
     *
     * @sa setOutlineThickness()
     */
    float getOutlineThickness() const {
      return m_outlineThickness;
    }

    /**
     * @brief Get the total number of points of the curve
     *
     * @return Number of points of the curve
     * @sa getPoint()
     */
    virtual std::size_t getPointCount() const = 0;

    /**
     * @brief Get a point of the curve
     *
     * The returned point is in local coordinates, that is,
     * the curve's transforms (position, rotation, scale) are
     * not taken into account.
     *
     * The result is undefined if `index` is out of the valid range.
     *
     * @param index Index of the point to get, in range @f$ [0, n-1] @f$
     * where @f$ n @f$ is the number of points of the curve.
     *
     * @return index-th point of the curve
     * @sa getPointCount()
     */
    virtual Vector2f getPoint(std::size_t index) const = 0;

    /**
     * @brief Get the local bounding rectangle of the entity
     *
     * The returned rectangle is in local coordinates, which means
     * that it ignores the transformations (translation, rotation,
     * scale, ...) that are applied to the entity.
     * In other words, this function returns the bounds of the
     * entity in the entity's coordinate system.
     *
     * @return Local bounding rectangle of the entity
     */
    RectF getLocalBounds() const;

    /**
     * @brief Set the anchor origin of the entity
     *
     * Compute the origin of the entity based on the local bounds and
     * the specified anchor. Internally, this function calls
     * `Transformable::setOrigin()`.
     *
     * @param anchor The anchor of the entity
     * @sa getLocalBounds(), Transformable::setOrigin()
     */
    void setAnchor(Anchor anchor);

    /**
     * @brief Create a buffer with the current geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current geometry
     */
    VertexBuffer commitGeometry() const;

    /**
     * @brief Create a buffer with the current outline geometry
     *
     * The geometry is uploaded in the graphics memory so that it's faster
     * to draw.
     *
     * @return A buffer with the current outline geometry
     */
    VertexBuffer commitOutlineGeometry() const;

    virtual void draw(RenderTarget& target, RenderStates states);

  protected:
    /**
     * @brief Recompute the internal geometry of the curve
     *
     * This function must be called by the derived class everytime
     * the curve's points change (i.e. the result of either
     * getPointCount() or getPoint() is different).
     */
    void updateGeometry();

    /**
     * @brief Set the curve closed
     *
     * This function must be called by derived class when the curve must be
     * closed, i.e. the last point is connected to the first point with a
     * line.
     *
     * @param closed True if the curve is closed
     */
    void setClosed(bool closed = true);

  private:
    void updateColors();
    void updateOutline();
    void updateOutlineColors();

    void computeVertices(VertexArray& vertices, float halfWidth);
  private:
    Type m_type;
    bool m_closed;

    Color4f m_color;
    float m_width;
    VertexArray m_vertices;
    RectF m_bounds;

    Color4f m_outlineColor;
    float m_outlineThickness;
    VertexArray m_outlineVertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CURVE_H
