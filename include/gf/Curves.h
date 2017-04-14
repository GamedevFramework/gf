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
#ifndef GF_CURVES_H
#define GF_CURVES_H

#include <vector>

#include "Curve.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @brief A line
   *
   * A line is a straight curve. It is defined by two end points.
   */
  class GF_API Line : public Curve {
  public:
    /**
     * @brief Constructor
     *
     * @param p0 The first end point
     * @param p1 The second end point
     */
    Line(Vector2f p0, Vector2f p1);

    virtual std::size_t getPointCount() const override;

    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    Vector2f m_p0;
    Vector2f m_p1;
  };

  /**
   * @brief A quadratic Bézier curve
   *
   * A quadratic Bézier curve is a Bézier curve with two end points and one control point.
   */
  class GF_API QuadraticBezierCurve : public Curve {
  public:
    /**
     * @brief Constructor
     *
     * @param p0 The first end point
     * @param p1 The control point
     * @param p2 The second end point
     * @param pointCount The number of points composing the curve
     */
    QuadraticBezierCurve(Vector2f p0, Vector2f p1, Vector2f p2, std::size_t pointCount = 20);

    virtual std::size_t getPointCount() const override;

    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    std::size_t m_pointCount;
    Vector2f m_p0;
    Vector2f m_p1;
    Vector2f m_p2;
  };

  /**
   * @brief A cubic Bézier curve
   *
   * A cubic Bézier curve is a Bézier curve with two end points and two control points.
   */
  class GF_API CubicBezierCurve : public Curve {
  public:
    /**
     * @brief Constructor
     *
     * @param p0 The first end point
     * @param p1 The first control point
     * @param p2 The second control point
     * @param p3 The second end point
     * @param pointCount The number of points composing the curve
     */
    CubicBezierCurve(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, std::size_t pointCount = 30);

    virtual std::size_t getPointCount() const override;

    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    std::size_t m_pointCount;
    Vector2f m_p0;
    Vector2f m_p1;
    Vector2f m_p2;
    Vector2f m_p3;
  };

  /**
   * @brief A compound curve
   *
   * A compound curve is a curve composed of several continuous curves. It is
   * sometimes called a path in vector graphics software.
   */
  class GF_API CompoundCurve : public Curve {
  public:
    /**
     * @brief Constructor
     *
     * @param origin The first point of the curve
     */
    CompoundCurve(Vector2f origin = Vector2f(0, 0));

    /**
     * @brief Set the first point of the curve
     *
     * @param origin The first point of the curve
     * @return A reference on the current compound curve
     */
    CompoundCurve& setOrigin(Vector2f origin);

    /**
     * @brief Create a line from the last point to a new point
     *
     * @param p1 The new end point
     */
    CompoundCurve& lineTo(Vector2f p1);

    /**
     * @brief Create a quadratic Bézier curve from the last point to a new point
     *
     * @param p1 The control point
     * @param p2 The new end point
     * @param pointCount The number of points composing the curve
     */
    CompoundCurve& quadraticCurveTo(Vector2f p1, Vector2f p2, std::size_t pointCount = 20);

    /**
     * @brief Create a cubic Bézier curve from the last point to a new point
     *
     * @param p1 The first control point
     * @param p2 The second control point
     * @param p3 The new end point
     * @param pointCount The number of points composing the curve
     */
    CompoundCurve& cubicCurveTo(Vector2f p1, Vector2f p2, Vector2f p3, std::size_t pointCount = 30);

    /**
     * @brief Close the curve
     *
     * This create a line between the last point and the first point of the curve.
     */
    void close();

    /**
     * @brief Reset the curve to a new origin
     *
     * The curve is open again.
     *
     * @param origin The first point of the curve
     */
    CompoundCurve& clear(Vector2f origin = Vector2f(0, 0));

    virtual std::size_t getPointCount() const override;

    virtual Vector2f getPoint(std::size_t index) const override;

  private:
    std::vector<Vector2f> m_points;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CURVES_H
