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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/View.h>

#include <gf/Event.h>
#include <gf/RenderTarget.h>
#include <gf/Transform.h>
#include <gf/VectorOps.h>


namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  View::View()
  : m_center{0.0f, 0.0f}
  , m_size{1000.0f, 1000.0f}
  , m_rotation(0)
  , m_viewport(RectF::fromSize({ 1.0f, 1.0f }))
  {
  }

  View::View(const RectF& rect)
  : m_center(rect.getCenter())
  , m_size(rect.getSize())
  , m_rotation(0)
  , m_viewport(RectF::fromSize({ 1.0f, 1.0f }))
  {
  }

  View::View(Vector2f center, Vector2f size)
  : m_center(center)
  , m_size(size)
  , m_rotation(0)
  , m_viewport(RectF::fromSize({ 1.0f, 1.0f }))
  {
  }

  View::~View() = default;

  RectF View::getBounds() const {
    return RectF::fromCenterSize(m_center, m_size);
  }

  namespace {

#ifndef NDEBUG
    bool isClamped(float value) {
      return 0.0f <= value && value <= 1.0f;
    }
#endif

  }

  void View::setViewport(const RectF& viewport) {
    assert(isClamped(viewport.min.x));
    assert(isClamped(viewport.min.y));
    assert(isClamped(viewport.max.x));
    assert(isClamped(viewport.max.y));

    m_viewport = viewport;
    onViewportChange(viewport);
  }

  void View::reset(const RectF& rect) {
    m_center = rect.getCenter();
    m_size = rect.getSize();
    onSizeChange(m_size);
  }

  void View::rotate(float angle) {
    m_rotation += angle;
  }

  void View::move(Vector2f offset) {
    m_center += offset;
  }

  void View::zoom(float factor) {
    m_size *= factor;
    onSizeChange(m_size);
  }

  void View::zoom(float factor, Vector2f fixed) {
    m_center += (fixed - m_center) * (1 - factor);
    m_size *= factor;
    onSizeChange(m_size);
  }

  Matrix3f View::getTransform() const {
    /* compute the view matrix
     * it's a combination of:
     * 1. a translation of -center
     * 2. a rotation of -angle
     * 3. a scaling of 2 / size
     */

    Vector2f factors = 2.0f / m_size;
    return scaling({ factors.x, -factors.y }) * rotation(-m_rotation) * translation(-m_center);
  }

  Matrix3f View::getInverseTransform() const {
    return invert(getTransform());
  }

  void View::onSizeChange([[maybe_unused]] Vector2f size) {
    // nothing by default
  }

  void View::setViewportNoCallback(const RectF& viewport) {
    assert(isClamped(viewport.min.x));
    assert(isClamped(viewport.min.y));
    assert(isClamped(viewport.max.x));
    assert(isClamped(viewport.max.y));

    m_viewport = viewport;
  }

  void View::onViewportChange([[maybe_unused]] const RectF& viewport) {
    // nothing by default
  }


  /*
   * AdaptativeView
   */

  void AdaptativeView::setInitialFramebufferSize(Vector2i framebufferSize) {
    onFramebufferSizeChange(framebufferSize);
  }


  /*
   * ZoomingViewAdaptor
   */

  ZoomingViewAdaptor::ZoomingViewAdaptor(const RenderTarget& target, View& view)
  : m_target(target)
  , m_view(view)
  , m_mousePosition({ 0, 0 })
  , m_state(State::Stationary)
  {

  }

  namespace {

    bool isCursorOnView(Vector2i cursor, Vector2i framebufferSize, const RectF& viewport) {
      RectF visible = RectF::fromPositionSize(viewport.getPosition() * framebufferSize, viewport.getSize() * framebufferSize);
      return visible.contains(cursor);
    }

  }

  void ZoomingViewAdaptor::processEvent(const Event& event) {
    static constexpr float ZoomInFactor = 0.8f;
    static constexpr float ZoomOutFactor = 1.25f;

    switch (event.type) {
      case gf::EventType::MouseMoved:
        if (m_state == State::Moving) {
          Vector2f oldPosition = m_target.mapPixelToCoords(m_mousePosition, m_view);
          Vector2f newPosition = m_target.mapPixelToCoords(event.mouseCursor.coords, m_view);
          m_view.move(oldPosition - newPosition);
        }

        m_mousePosition = event.mouseCursor.coords;
        break;

      case gf::EventType::MouseButtonPressed:
        if (isCursorOnView(event.mouseButton.coords, m_target.getSize(), m_view.getViewport())) {
          m_state = State::Moving;
        }
        break;

      case gf::EventType::MouseButtonReleased:
        m_state = State::Stationary;
        break;

      case gf::EventType::MouseWheelScrolled:
        if (event.mouseWheel.offset.y > 0) {
          m_view.zoom(ZoomInFactor, m_target.mapPixelToCoords(m_mousePosition, m_view));
        } else {
          m_view.zoom(ZoomOutFactor, m_target.mapPixelToCoords(m_mousePosition, m_view));
        }
        break;

      default:
        break;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
