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
#include <gf/Views.h>

#include <iostream>

#include <gf/Unused.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void StretchView::onScreenResize(Vector2u screenSize) {
    gf::unused(screenSize);

    // nothing to do, viewport stays the same:
    // {0.0f, 0.0f, 1.0f, 1.0f}
  };


  void FitView::onScreenResize(Vector2u screenSize) {
    const Vector2f worldSize = getSize();

    float worldRatio = worldSize.width / worldSize.height;
    float screenRatio = static_cast<float>(screenSize.width) / static_cast<float>(screenSize.height);

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

    setViewport(viewport);
  };


  void FillView::onScreenResize(Vector2u screenSize) {
    float worldRatio = m_worldSize.width / m_worldSize.height;
    float screenRatio = static_cast<float>(screenSize.width) / static_cast<float>(screenSize.height);

    Vector2f actualWorldSize = m_worldSize;

    if (screenRatio < worldRatio) {
      float ratio = screenRatio / worldRatio;
      actualWorldSize.width *= ratio;
    } else {
      float ratio = worldRatio / screenRatio;
      actualWorldSize.height *= ratio;
    }

    setWorldSize(actualWorldSize);
  };

  void FillView::onWorldResize(Vector2f worldSize) {
    resizeWorld(worldSize);
  };

  void FillView::resizeWorld(Vector2f worldSize) {
    m_worldSize = worldSize;
  }

  void ExtendView::onScreenResize(Vector2u screenSize) {
    float worldRatio = m_worldSize.width / m_worldSize.height;
    float screenRatio = static_cast<float>(screenSize.width) / static_cast<float>(screenSize.height);

    Vector2f actualWorldSize = m_worldSize;

    if (screenRatio < worldRatio) {
      float ratio = screenRatio / worldRatio;
      actualWorldSize.height /= ratio;
    } else {
      float ratio = worldRatio / screenRatio;
      actualWorldSize.width /= ratio;
    }

    setWorldSize(actualWorldSize);
  };

  void ExtendView::onWorldResize(Vector2f worldSize) {
    resizeWorld(worldSize);
  }

  void ExtendView::resizeWorld(Vector2f worldSize) {
    m_worldSize = worldSize;
  }

  void ScreenView::onScreenResize(Vector2u screenSize) {
    RectF screen(0.0f, 0.0f, screenSize.width, screenSize.height);
    reset(screen);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
