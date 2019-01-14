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
#include <gf/Views.h>

#include <gf/Unused.h>
#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /*
   * StretchView
   */

  void StretchView::onScreenSizeChange(Vector2u screenSize) {
    gf::unused(screenSize);
    // nothing to do
  }


  /*
   * FitView
   */

  void FitView::onScreenSizeChange(Vector2u screenSize) {
    m_localScreenSize = screenSize;
    updateView();
  }

  void FitView::onSizeChange(Vector2f size) {
    gf::unused(size);
    updateView();
  }

  void FitView::onViewportChange(const RectF& viewport) {
    m_localViewport = viewport;
    updateView();
  }

  void FitView::updateView() {
    const Vector2f worldSize = getSize();
    float worldRatio = worldSize.width / worldSize.height;

    const Vector2f viewportSize = m_localScreenSize * m_localViewport.getSize();
    float screenRatio = viewportSize.width / viewportSize.height;

    RectF viewport;

    if (screenRatio < worldRatio) {
      float ratio = screenRatio / worldRatio;

      viewport.left = 0.0f;
      viewport.width = 1.0f;

      viewport.top = (1 - ratio) / 2;
      viewport.height = ratio;
    } else {
      float ratio = worldRatio / screenRatio;

      viewport.top = 0.0f;
      viewport.height = 1.0f;

      viewport.left = (1 - ratio) / 2;
      viewport.width = ratio;
    }

    viewport.setPosition(viewport.getPosition() * m_localViewport.getSize() + m_localViewport.getPosition());
    viewport.setSize(viewport.getSize() * m_localViewport.getSize());

    setViewportNoCallback(viewport);
  }

  /*
   * FillView
   */

  void FillView::onScreenSizeChange(Vector2u screenSize) {
    m_localScreenSize = screenSize;
    updateView();
  }

  void FillView::onSizeChange(Vector2f size) {
    m_localSize = size;
    updateView();
  }

  void FillView::onViewportChange(const RectF& viewport) {
    gf::unused(viewport);
    updateView();
  }

  void FillView::updateView() {
    float worldRatio = m_localSize.width / m_localSize.height;

    const Vector2f viewportSize = m_localScreenSize * getViewport().getSize();
    float screenRatio = viewportSize.width / viewportSize.height;

    Vector2f actualSize = m_localSize;

    if (screenRatio < worldRatio) {
      float ratio = screenRatio / worldRatio;
      actualSize.width *= ratio;
    } else {
      float ratio = worldRatio / screenRatio;
      actualSize.height *= ratio;
    }

    setSizeNoCallback(actualSize);
  }


  /*
   * ExtendView
   */

  void ExtendView::onScreenSizeChange(Vector2u screenSize) {
    m_localScreenSize = screenSize;
    updateView();
  }

  void ExtendView::onSizeChange(Vector2f size) {
    m_localSize = size;
    updateView();
  }

  void ExtendView::onViewportChange(const RectF& viewport) {
    gf::unused(viewport);
    updateView();
  }

  void ExtendView::updateView() {
    float worldRatio = m_localSize.width / m_localSize.height;

    const Vector2f viewportSize = m_localScreenSize * getViewport().getSize();
    float screenRatio = viewportSize.width / viewportSize.height;

    Vector2f actualSize = m_localSize;

    if (screenRatio < worldRatio) {
      float ratio = screenRatio / worldRatio;
      actualSize.height /= ratio;
    } else {
      float ratio = worldRatio / screenRatio;
      actualSize.width /= ratio;
    }

    setSizeNoCallback(actualSize);
  }

  /*
   * LockedView
   */

  void LockedView::onScreenSizeChange(Vector2u screenSize) {
    m_localScreenSize = screenSize;
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

    const Vector2f viewportSize = m_localScreenSize * m_localViewport.getSize();

    RectF viewport;

    if (m_localSize.width > viewportSize.width) {
      viewport.left = 0.0f;
      viewport.width = 1.0f;
      actualSize.width = viewportSize.width;
    } else {
      viewport.width = m_localSize.width / viewportSize.width;
      viewport.left = (1.0f - viewport.width) / 2.0f;
    }

    if (m_localSize.height > viewportSize.height) {
      viewport.top = 0.0f;
      viewport.height = 1.0f;
      actualSize.height = viewportSize.height;
    } else {
      viewport.height = m_localSize.height / viewportSize.height;
      viewport.top = (1.0f - viewport.height) / 2.0f;
    }

    setSizeNoCallback(actualSize);

    viewport.setPosition(viewport.getPosition() * m_localViewport.getSize() + m_localViewport.getPosition());
    viewport.setSize(viewport.getSize() * m_localViewport.getSize());

    setViewportNoCallback(viewport);
  }


  /*
   * ScreenView
   */

  void ScreenView::onScreenSizeChange(Vector2u screenSize) {
    m_localScreenSize = screenSize;
    updateView();
  }

  void ScreenView::onViewportChange(const RectF& viewport) {
    gf::unused(viewport);
    updateView();
  }

  void ScreenView::updateView() {
    RectF screen({ 0.0f, 0.0f }, m_localScreenSize * getViewport().getSize());
    reset(screen);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
