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
#ifndef GF_SEGUE_H
#define GF_SEGUE_H

#include "Drawable.h"
#include "Easings.h"
#include "GraphicsApi.h"
#include "Rect.h"
#include "Time.h"
#include "Vertex.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class SegueEffect;
  class Texture;

  /**
   * @ingroup graphics_segues
   * @brief A transition between two scenes
   *
   * @sa [Segue - Wikipedia](https://en.wikipedia.org/wiki/Segue)
   */
  class GF_GRAPHICS_API Segue : public Drawable {
  public:
    /**
     * @brief Contructor
     */
    Segue();

    /**
     * @brief Start the segue for a specified time
     *
     * @param time The time of the segue
     */
    void start(Time time);

    /**
     * @brief Stop the segue
     */
    void stop();

    /**
     * @brief Update the segue
     *
     * @param time The time since the last update
     */
    void update(Time time);

    /**
     * @brief Tell if the segue is still active
     *
     * @return True if the segue is not finished
     */
    bool isActive() {
      return m_currentTime < m_totalTime;
    }

    /**
     * @brief Set the two texture for the segue
     *
     * @param texture0 The old scene
     * @param texture1 The new scene
     */
    void setTextures(const Texture& texture0, const Texture& texture1);

    /**
     * @brief Set the effect to apply
     *
     * @param effect The post-processing effect
     */
    void setEffect(SegueEffect& effect) {
      m_effect = &effect;
    }

    /**
     * @brief Set the easing for the segue
     *
     * @param easing The new easing
     */
    void setEasing(Easing easing) {
      m_easing = easing;
    }

    virtual void draw(RenderTarget& target, const RenderStates& states) override;

  private:
    void updatePositions();
    void updateTexCoords();

  private:
    const Texture *m_texture0;
    const Texture *m_texture1;
    RectF m_textureRect;
    SegueEffect *m_effect;
    Easing m_easing;
    Vertex m_vertices[4];

    Time m_totalTime;
    Time m_currentTime;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SEGUE_H
