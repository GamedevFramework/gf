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
#include <gf/Views.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /*
   * StretchView
   */

  void StretchView::onFramebufferSizeChange([[maybe_unused]] Vector2i framebufferSize) {
    // nothing to do
  }


  /*
   * FitView
   */

  void FitView::onFramebufferSizeChange(Vector2i framebufferSize) {
    m_localFramebufferSize = framebufferSize;
    updateView();
  }

  void FitView::onSizeChange([[maybe_unused]] Vector2f size) {
    updateView();
  }

  void FitView::onViewportChange(const RectF& viewport) {
    m_localViewport = viewport;
    updateView();
  }

  void FitView::updateView() {
    const Vector2f worldSize = getSize();
    float worldRatio = worldSize.width / worldSize.height;

    const Vector2f viewportSize = m_localFramebufferSize * m_localViewport.getSize();
    float framebufferRatio = viewportSize.width / viewportSize.height;

    Vector2f position, size;

    if (framebufferRatio < worldRatio) {
      float ratio = framebufferRatio / worldRatio;

      position.x = 0.0f;
      size.width = 1.0f;

      position.y = (1 - ratio) / 2;
      size.height = ratio;
    } else {
      float ratio = worldRatio / framebufferRatio;

      position.y = 0.0f;
      size.height = 1.0f;

      position.x = (1 - ratio) / 2;
      size.width = ratio;
    }

    RectF viewport = RectF::fromPositionSize(position * m_localViewport.getSize() + m_localViewport.getPosition(), size * m_localViewport.getSize());
    setViewportNoCallback(viewport);
  }

  /*
   * FillView
   */

  void FillView::onFramebufferSizeChange(Vector2i framebufferSize) {
    m_localFramebufferSize = framebufferSize;
    updateView();
  }

  void FillView::onSizeChange(Vector2f size) {
    m_localSize = size;
    updateView();
  }

  void FillView::onViewportChange([[maybe_unused]] const RectF& viewport) {
    updateView();
  }

  void FillView::updateView() {
    float worldRatio = m_localSize.width / m_localSize.height;

    const Vector2f viewportSize = m_localFramebufferSize * getViewport().getSize();
    float framebufferRatio = viewportSize.width / viewportSize.height;

    Vector2f actualSize = m_localSize;

    if (framebufferRatio < worldRatio) {
      float ratio = framebufferRatio / worldRatio;
      actualSize.width *= ratio;
    } else {
      float ratio = worldRatio / framebufferRatio;
      actualSize.height *= ratio;
    }

    setSizeNoCallback(actualSize);
  }


  /*
   * ExtendView
   */

  void ExtendView::onFramebufferSizeChange(Vector2i framebufferSize) {
    m_localFramebufferSize = framebufferSize;
    updateView();
  }

  void ExtendView::onSizeChange(Vector2f size) {
    m_localSize = size;
    updateView();
  }

  void ExtendView::onViewportChange([[maybe_unused]] const RectF& viewport) {
    updateView();
  }

  void ExtendView::updateView() {
    float worldRatio = m_localSize.width / m_localSize.height;

    const Vector2f viewportSize = m_localFramebufferSize * getViewport().getSize();
    float framebufferRatio = viewportSize.width / viewportSize.height;

    Vector2f actualSize = m_localSize;

    if (framebufferRatio < worldRatio) {
      float ratio = framebufferRatio / worldRatio;
      actualSize.height /= ratio;
    } else {
      float ratio = worldRatio / framebufferRatio;
      actualSize.width /= ratio;
    }

    setSizeNoCallback(actualSize);
  }

  /*
   * LockedView
   */

  void LockedView::onFramebufferSizeChange(Vector2i framebufferSize) {
    m_localFramebufferSize = framebufferSize;
    updateView();
  }

  void LockedView::onSizeChange(Vector2f size) {
    m_localSize = size;
    updateView();
  }

  void LockedView::onViewportChange(const RectF& viewport) {
    m_localViewport = viewport;
    updateView();
  }

  void LockedView::updateView() {
    Vector2f actualSize = m_localSize;

    const Vector2f viewportSize = m_localFramebufferSize * m_localViewport.getSize();

    Vector2f position, size;

    if (m_localSize.width > viewportSize.width) {
      position.x = 0.0f;
      size.width = 1.0f;
      actualSize.width = viewportSize.width;
    } else {
      size.width = m_localSize.width / viewportSize.width;
      position.x = (1.0f - size.width) / 2.0f;
    }

    if (m_localSize.height > viewportSize.height) {
      position.y = 0.0f;
      size.height = 1.0f;
      actualSize.height = viewportSize.height;
    } else {
      size.height = m_localSize.height / viewportSize.height;
      position.y = (1.0f - size.height) / 2.0f;
    }

    setSizeNoCallback(actualSize);

    RectF viewport = RectF::fromPositionSize(position * m_localViewport.getSize() + m_localViewport.getPosition(), size * m_localViewport.getSize());
    setViewportNoCallback(viewport);
  }


  /*
   * ScreenView
   */

  void ScreenView::onFramebufferSizeChange(Vector2i framebufferSize) {
    m_localFramebufferSize = framebufferSize;
    updateView();
  }

  void ScreenView::onViewportChange([[maybe_unused]] const RectF& viewport) {
    updateView();
  }

  void ScreenView::updateView() {
    RectF framebuffer = RectF::fromSize(m_localFramebufferSize * getViewport().getSize());
    reset(framebuffer);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
